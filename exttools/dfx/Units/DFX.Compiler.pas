unit DFX.Compiler;

interface

uses
  DFX.Project, DFX.Compiler.Types, DFX.Compiler.Helper, SysUtils, Classes,
  Windows, ShellAPI, Generics.Defaults, Generics.Collections, JclSysUtils,
  System.RegularExpressionsCore, D3DCommon_JSB, D3DX11_JSB, D3DCompiler_JSB, D3D11_JSB,
  TypInfo, JCLStrings, Math;

// Compiles a project
// Not thread-safe
procedure CompileDFXProject(Project: TDFXProject; Handler: IDFXCompilerHandler);
function PosLeft(const SubStr, Text: string; Index: Integer): Integer;

implementation

//
// Purpose: Substitute $ with Item.Name
//
function SubstituteDFXItemName(const Item: TDFXNamespacedTextItem): string;
var
  insStr: string;
  i: Integer;
begin
  Result := Item.Text;

  // Replace function names
  i := 1;
  repeat
    i := Pos('$', Result, i);
    if i > 0 then
    begin
      Delete(Result, i, 1);

      // Insert space only if required
      insStr := '';
      if i > 1 then
        if IsAlphaNum(Result[i-1]) then
          insStr := ' ';

      Insert(insStr + Item.Name, Result, i);
    end;
  until i = 0;
end;

//
// Purpose: Compiles an HLSL source file
//
function CompileHLSL(const SourceName, ShaderSource, Profile, EntryPoint: AnsiString;
  var ErrorMsg: ID3DBlob): ID3DBlob;
var
  srcName: PAnsiChar;
  sm: TD3D_ShaderMacro;
  compilerFlags: Cardinal;
begin
  Result := nil;

  if Length(SourceName) > 0 then
    srcName := PAnsiChar(SourceName)
  else
    srcName := nil;

  compilerFlags := GState.Project.HLSLCompilerFlags; // or D3DCOMPILE_WARNINGS_ARE_ERRORS;
  if Profile <> 'cs_5_0' then
    compilerFlags := compilerFlags and (not D3DCOMPILE_RESOURCES_MAY_ALIAS);

  D3DCompile(PAnsiChar(ShaderSource), Length(ShaderSource), srcName, nil,
    D3D_COMPILE_STANDARD_FILE_INCLUDE, PAnsiChar(EntryPoint), PAnsiChar(Profile), compilerFlags, 0, Result, ErrorMsg);

  {*
  sm.Name := nil;
  sm.Definition := nil;
  D3DX11CompileFromMemory(PAnsiChar(ShaderSource), Length(ShaderSource),
    srcName, sm, nil, PAnsiChar(EntryPoint), PAnsiChar(Profile), compilerFlags, 0, nil,
    Result, ErrorMsg, nil);  *}
end;

function CompileHLSLAndLog(const SourceName, ShaderSource, Profile, EntryPoint: AnsiString): ID3DBlob;
var
  em: ID3DBlob;
begin
  Result := CompileHLSL(SourceName, ShaderSource, Profile, EntryPoint, em);
  if not Assigned(Result) then
    RaiseAndLog(string(PAnsiChar(em.GetBufferPointer)));
end;

//
// Purpose: Saves a compiled shader to disk
//
function SaveCSO(const OutputFile: string; CSO: ID3DBlob): Boolean;
var
  fs: TFileStream;
begin
  ForceDirectories(ExtractFilePath(OutputFile));

  fs := TFileStream.Create(OutputFile, fmCreate);
  try
    try
      fs.Write(CSO.GetBufferPointer, CSO.GetBufferSize);
    except
      Exit(False);
    end;
  finally
    fs.Free;
  end;

  Result := True;
end;

//
// Purpose: Gathers reflection data about a CSO
//
procedure GatherReflectionData(CSO: ID3DBlob; Func: TDFXFunction;
  Output: TDFXShaderReflectionData);
var
  sr: ID3D11ShaderReflection;
  sd: TD3D11_ShaderDesc;
  sibd: TD3D11_ShaderInputBindDesc;
  dfxRBD: TDFXResourceBindDesc;
  i, j, k: Integer;
  hr: HRESULT;
begin
  Output.Reset;

  hr := D3DReflect(CSO.GetBufferPointer, CSO.GetBufferSize, ID3D11ShaderReflection, sr);
  if Failed(hr) then
    RaiseAndLog(Format('Internal error. (D3DReflect: 0x%x)', [hr]));

  sr.GetDesc(sd);
  sr.GetThreadGroupSize(@Output.ThreadGroupSize.X, @Output.ThreadGroupSize.Y, @Output.ThreadGroupSize.Z);

  // Gather reflection data
  for i := 0 to sd.BoundResources - 1 do
  begin
    sr.GetResourceBindingDesc(i, sibd);

    dfxRBD.BindPoint := sibd.BindPoint;
    dfxRBD.BindCount := sibd.BindCount;
    dfxRBD.Name := string(sibd.Name);
    dfxRBD.Obj := nil;

    case sibd._Type of
      D3D_SIT_CBUFFER,
      D3D_SIT_TBUFFER:
        begin
          // Delete manual CB namespace
          j := LastPos('::', dfxRBD.Name);
          if j > 0 then
          begin
            k := PosLeft('::', dfxRBD.Name, j);
            if k = 0 then
              k := 1
            else
              Inc(k, 2);
            Delete(dfxRBD.Name, k, j - k + 2);
          end;

          dfxRBD.Obj := Func.Namespace.FindGlobalConstantBufferReference(dfxRBD.Name);
          if not Assigned(dfxRBD.Obj) then
            dfxRBD.Obj := Func.Namespace.FindConstantBufferReference(dfxRBD.Name);
          Output.ConstantBuffers.Add(dfxRBD);
        end;
      D3D_SIT_SAMPLER:
        begin
          dfxRBD.Obj := Func.Namespace.FindGlobalResourceReference(dfxRBD.Name);
          if not Assigned(dfxRBD.Obj) then
            dfxRBD.Obj := Func.Namespace.FindResourceReference(dfxRBD.Name);
          Output.SamplerStates.Add(dfxRBD);
        end;
      D3D_SIT_UAV_RWTYPED,
      D3D_SIT_UAV_RWSTRUCTURED,
      D3D_SIT_UAV_RWBYTEADDRESS,
      D3D_SIT_UAV_APPEND_STRUCTURED,
      D3D_SIT_UAV_CONSUME_STRUCTURED,
      D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
        begin
          dfxRBD.Obj := Func.Namespace.FindGlobalResourceReference(dfxRBD.Name);
          if not Assigned(dfxRBD.Obj) then
            dfxRBD.Obj := Func.Namespace.FindResourceReference(dfxRBD.Name);
          Output.UAVs.Add(dfxRBD);
        end;
      else
      begin
        dfxRBD.Obj := Func.Namespace.FindGlobalResourceReference(dfxRBD.Name);
          if not Assigned(dfxRBD.Obj) then
            dfxRBD.Obj := Func.Namespace.FindResourceReference(dfxRBD.Name);
        Output.Textures.Add(dfxRBD);
      end;
    end;
  end;

  // Sort by Bind-points
  Output.Textures.Sort;
  Output.SamplerStates.Sort;
  Output.ConstantBuffers.Sort;
  Output.UAVs.Sort;
end;

//
// Purpose: Expands a namespaced text item
//
function ExpandDFXNamespacedItem(const Item: TDFXNamespacedTextItem; const Prefix: string = ''): string;
var
  sl: TDFXNamespaceStringList;
begin
  sl := TDFXNamespaceStringList.Create(Item.Namespace);

  try
    sl.BeginNamespace;
    Result := SubstituteDFXItemName(Item);
    sl.Add(Prefix + Result);
    sl.EndNamespace;
    Result := sl.Text;
  finally
    sl.Free;
  end;
end;

//
// Purpose: Builds a relative #include "<fileName>" string
//
function InclusionString(const CurrentPath, FileName: string): string;
begin
  Result := ExtractRelativePath(CurrentPath, FileName);
  Result := '#include "' + Result + '"';
end;

//
// Purpose: interprets the output of a HLSL compilation step
//
function ParseHLSLCompilerOutput(const CompilerOutput, CurrentDir: string): TDFXHLSLCompilerOutput;
var
  i, j, k: Integer;
begin
  Result.Msg := CompilerOutput;
  Result.Line := 0;
  Result.ErrorCode := 0;

  // Determine the error code
  i := Pos('error X', CompilerOutput);
  if i > 0 then
  begin
    Inc(i, Length('error X'));
    Result.ErrorCode := StrToInt(Copy(CompilerOutput, i, Pos(':', CompilerOutput, i + 1) - i));
  end;

  // Determine the line index that caused the error
  j := Pos('(', CompilerOutput);
  if j < i then
  begin
    Result.FileName := Copy(CompilerOutput, 1, j - 1);
    Result.FileName := MakeAbsolutePath(CurrentDir, Result.FileName);

    k := Pos(',', CompilerOutput, j);
    TryStrToInt(Copy(CompilerOutput, j + 1, k - j - 1), Result.Line);
  end;
end;

//
// Purpose: extracts the filename of a #include "<filename>" line
//
function ExtractInclusionFileName(const S: string): string;
var
  i, j: Integer;
begin
  Result := '';
  i := Pos('"', S);
  if i > 0 then
  begin
    j := Pos('"', S, i + 1);
    if j > 0 then
      Result := Copy(S, i + 1, j - i - 1);
  end;
end;

//
// Purpose: Searches for the index of a filename in a #include list
//
function FindInclusionIndex(const FileName: string; const CurrentDir: string;
  InclusionList: TStringList): Integer;
var
  i: Integer;
  s: string;
begin
  for i := 0 to InclusionList.Count - 1 do
  begin
    s := MakeAbsolutePath(CurrentDir, ExtractInclusionFileName(InclusionList[i]));
    if Pos(s, FileName) > 0 then
      Exit(i);
  end;

  Result := -1;
end;

//
// Purpose: Reorder the inclusion list of a shader
//
procedure ReorderInclusions(const CurrentDir: string; const CompilerOutput: TDFXHLSLCompilerOutput;
  InclusionList: TStringList; var NumberOfMoves: Integer);
var
  errorFileIndex, tarIdx: Integer;
begin
  // Check if we should've encountered a valid #include list by now
  if NumberOfMoves >= InclusionList.Count then
  begin
    InclusionList.SaveToFile('D:\\Inclusions.txt');
    RaiseAndLog(Format('Unable to compile code. Possible causes: syntax error, circular dependency.' + sLineBreak +
                       'Compiler error: ' + sLineBreak +
                       '%s', [CompilerOutput.Msg]));
  end;

  // Find the file that caused the error
  errorFileIndex := FindInclusionIndex(CompilerOutput.FileName, CurrentDir, InclusionList);
  if CompilerOutput.ErrorCode = 3129 then
  begin
    InclusionList.Delete(errorFileIndex);
    Exit;
  end;

  tarIdx := InclusionList.Count - 1;

  if (errorFileIndex >= tarIdx) or
     (errorFileIndex < 0) then
  begin
    InclusionList.SaveToFile('D:\\Inclusions.txt');
    RaiseAndLog(CompilerOutput.Msg);
  end;

  // Move the error-#include to the bottom
  InclusionList.Move(errorFileIndex, tarIdx);

  Inc(NumberOfMoves);
end;

//
// Purpose: compiles an HLSL shader and parses the compiler output
//
function CompileHLSLAndParse(const SourceName, ShaderSource, Profile, EntryPoint: AnsiString;
  const CurrentDir: string; var Output: TDFXHLSLCompilerOutput; var CSO: ID3DBlob): Boolean;
var
  em: ID3DBlob;
begin
  CSO := CompileHLSL(SourceName, ShaderSource, Profile, EntryPoint, em);
  Result := Assigned(CSO);
  if not Result then
    Output := ParseHLSLCompilerOutput(string(PAnsiChar(em.GetBufferPointer)), CurrentDir)
  else
    Output := ParseHLSLCompilerOutput('', CurrentDir);
end;

//
// Purpose: compiles and writes out a shader in an effect
//
procedure WriteShader(const Func: TDFXShaderFunction; InclusionList: TStringList;
  Reflection: TDFXShaderReflectionData; WriteCSO: Boolean);
var
  s, expandedFunc, outputFile, currentDir, csoFile, constantsHLSLI: string;
  shaderSource, outputFileANSI: AnsiString;
  f: TDFXFunction;
  compilerOutput: TDFXHLSLCompilerOutput;
  cso, minimizedCSO, em: ID3DBlob;
  recorderCount: Integer;
  i: Integer;
begin
  InclusionList.Clear;

  if Func.Reference.Length = 0 then
    Exit;

  GState.Handler.OnStatus('Compiling shader ' + Func.Reference + '...');

  // Lookup function reference
  f := GState.Effect.FindGlobalFunctionReference(Func.Reference);
  if not Assigned(f) then
    RaiseAndLog(Format('Invalid shader function reference in %s.', [GState.Effect.Name]));

  // Everything is relative to the function
  currentDir := ExtractFilePath(f.DiskPath);

  // Write out inclusions
  for i := 0 to GState.ShaderInclusions.Count - 1 do
    if GState.ShaderInclusions[i] <> f.RelativeDiskPath then // Skip this shader function
    begin
      // Filter out files that point to other shader functions
      if Pos('functions' + PathDelim, GState.ShaderInclusions[i]) > 0 then
        if TDFXFunction(GState.ShaderInclusions.Objects[i]).ShaderProfile <> '' then
          Continue;

      InclusionList.Add(InclusionString(currentDir, GState.ShaderInclusions[i]));
    end;

  // Expand function
  if func.Attributes.Length > 0 then
    expandedFunc := ExpandDFXNamespacedItem(f, func.Attributes + sLineBreak)
  else
    expandedFunc := ExpandDFXNamespacedItem(f);

  // Make everything relative to output file
  outputFile := MakeAbsolutePath(GState.BuildDir, f.DiskPath + '.hlsl');
  outputFileANSI := AnsiString(outputFile);
  currentDir := ExtractFilePath(outputFile);

  // Number of #include-reorders
  recorderCount := 0;
  constantsHLSLI := MakeAbsolutePath(GState.BuildDir, GState.ShaderInclusions[0]);

  while True do
  begin
    // Combine inclusions and shader function
    shaderSource := AnsiString(InclusionList.Text + sLineBreak + expandedFunc);

    // And compile
    if not CompileHLSLAndParse(outputFileANSI, shaderSource, f.ShaderProfile, AnsiString(Func.Reference), currentDir, compilerOutput, cso)  then
    begin
      // If the error is not within our shader file
      if (compilerOutput.FileName <> outputFile) and
         (compilerOutput.FileName <> constantsHLSLI) then
      begin
        case compilerOutput.ErrorCode of
          // Unrecognized identifier
          // Undeclared identifier (3004)
          // groupshared unsupported
          3000,
          3004,
          3129: ReorderInclusions(currentDir, compilerOutput, InclusionList, recorderCount);
        else
          // Can't be fixed by reordering
          RaiseAndLog(compilerOutput.Msg);
        end;
      end
      else
        // There is an error in the shader function
        RaiseAndLog(compilerOutput.Msg);
    end
    else
      // Success!
      Break;
  end;

  // Assume CSO is minimized
  minimizedCSO := cso;

  // Strip optional #includes
  if GState.Project.MinimizeShaders then
  begin
    GState.Handler.OnStatus('Minimizing shader ' + Func.Reference + '...');

    // Minimize inclusions
    for i := InclusionList.Count - 1 downto 0 do
    begin
      s := InclusionList[i];

      // Remove #include
      InclusionList.Delete(i);

      // Re-combine new inclusions and shader function
      shaderSource := AnsiString(InclusionList.Text + sLineBreak + expandedFunc);

      // Try re-compiling
      cso := CompileHLSL(outputFileANSI, shaderSource, f.ShaderProfile, AnsiString(Func.Reference), em);
      if Assigned(cso) then
        minimizedCSO := cso
      else
        InclusionList.Insert(i, s); // Put #include back in
    end;

    shaderSource := AnsiString(InclusionList.Text + sLineBreak + expandedFunc);
  end;

  // Write out minimized shader
  SaveText(outputFile, string(shaderSource), GState.Project.Preamble);

  // Optionally write out compiled shader
  if (WriteCSO) and (Assigned(GState.CppTranslator)) then
  begin
    csoFile := GState.CppTranslator.UCompiledShaderFileName(GState.Project.ShaderDir + f.FilePath, f.ShaderProfile);
    if not SaveCSO(csoFile, minimizedCSO) then
      GState.Handler.OnLog(Format('[WARNING] Could not output CSO "%s".', [csoFile]));
  end;

  // Gather optional reflection data
  if Assigned(Reflection) then
    GatherReflectionData(minimizedCSO, f, Reflection);
end;

//
// Purpose: Loads CSO shaders
//
procedure CppAddLoadShaders(CppTxt: TDFXNamespaceStringList);
var
  i: Integer;
  p: TDFXEffectPass;
  f: TDFXFunction;
  csoFile: string;
begin
  CppTxt.Add(Format('void dfx::%sload_shaders()', [GState.Effect.FilePath('::')]));
  CppTxt.Add('{');
  CppTxt.Indent;

  for i := 0 to GState.Effect.Passes.Count - 1 do
  begin
    p := GState.Effect.Passes[i];
    CppTxt.Add('// ' + p.Title);
    case p.PassType of
      ptCompute:
        begin
          if p.ComputeShader.Reference.Length > 0 then
          begin
            f := GState.Effect.FindGlobalFunctionReference(p.ComputeShader.Reference);
            csoFile := GState.CppTranslator.UCppCompiledShaderFileName(f.FilePath('/'), 'cs_5_0');
            CppTxt.Add(GState.CppTranslator.ULoadShader(Format('m_pass%dCS', [i]), csoFile, 'cs_5_0'));
          end;
        end;
      ptGeometry:
        begin
          if p.VertexShader.Reference.Length > 0 then
          begin
            f := GState.Effect.FindGlobalFunctionReference(p.VertexShader.Reference);
            csoFile := GState.CppTranslator.UCppCompiledShaderFileName(f.FilePath('/'), 'vs_5_0');
            CppTxt.Add(GState.CppTranslator.ULoadShader(Format('m_pass%dVS', [i]), csoFile, 'vs_5_0'));
          end;

          if p.PixelShader.Reference.Length > 0 then
          begin
            f := GState.Effect.FindGlobalFunctionReference(p.PixelShader.Reference);
            csoFile := GState.CppTranslator.UCppCompiledShaderFileName(f.FilePath('/'), 'ps_5_0');
            CppTxt.Add(GState.CppTranslator.ULoadShader(Format('m_pass%dPS', [i]), csoFile, 'ps_5_0'));
          end;
        end;
      else
        Assert(False);
    end;
  end;

  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Disposes a shader
//
procedure CppAddDisposeShader(CppTxt: TDFXNamespaceStringList; const Shader: string);
begin
  if Shader.Length > 0 then
    CppTxt.Add(Shader);
end;

//
// Purpose: Disposes all shaders
//
procedure CppAddDisposeShaders(CppTxt: TDFXNamespaceStringList);
var
  i: Integer;
  p: TDFXEffectPass;
begin
  CppTxt.Add('');
  CppTxt.Add(Format('void dfx::%sdispose_shaders()', [GState.Effect.FilePath('::')]));
  CppTxt.Add('{');
  CppTxt.Indent;

  for i := 0 to GState.Effect.Passes.Count - 1 do
  begin
    p := GState.Effect.Passes[i];
    case p.PassType of
      ptCompute:
        begin
          if p.ComputeShader.Reference.Length > 0 then
            CppAddDisposeShader(CppTxt, GState.CppTranslator.UUnloadShader(Format('m_pass%dCS', [i])));
        end;
      ptGeometry:
        begin
          if p.VertexShader.Reference.Length > 0 then
            CppAddDisposeShader(CppTxt, GState.CppTranslator.UUnloadShader(Format('m_pass%dVS', [i])));

          if p.PixelShader.Reference.Length > 0 then
            CppAddDisposeShader(CppTxt, GState.CppTranslator.UUnloadShader(Format('m_pass%dPS', [i])));
        end;
      else
        Assert(False);
    end;
  end;

  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Initializes and disposes an effect
//
procedure CppAddInitDispose(CppTxt: TDFXNamespaceStringList);
begin
  CppTxt.Add(Format('void dfx::%sinitialize(ID3D11DeviceContext *d3dDeviceContext, void *userData)', [GState.Effect.FilePath('::')]));
  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add('m_d3dDeviceContext = d3dDeviceContext;');
  CppTxt.Add('load_shaders();');
  CppTxt.Add('on_initialize(userData);');
  CppTxt.Unindent;
  CppTxt.Add('}');
  CppTxt.Add('');

  CppTxt.Add(Format('void dfx::%sdispose()', [GState.Effect.FilePath('::')]));
  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add('dispose_shaders();');
  CppTxt.Add('on_dispose();');
  CppTxt.Unindent;
  CppTxt.Add('}');
  CppTxt.Add('');

  CppAddLoadShaders(CppTxt);
  CppAddDisposeShaders(CppTxt);
end;

//
// Purpose: Finalizes a variable name for usage in Cpp
//
function CppFinalizeVariableName(const Obj: TDFXResource): string; overload;
var
  i: Integer;
begin
  Result := 'dfx_hlsl::' + Obj.FilePath('::');
  i := LastPos('::', Result);
  if i > 0 then
    Insert('g_', Result, i + 2)
  else
    Result := 'g_' + Result;
end;

//
// Purpose: overload
//
function CppFinalizeVariableName(const Obj: TDFXConstantBuffer): string; overload;
var
  i: Integer;
begin
  Result := 'dfx_hlsl::' + Obj.FilePath('::');
  i := LastPos('::', Result);
  if i > 0 then
    Insert('g_', Result, i + 2)
  else
    Result := 'g_' + Result;
end;

//
// Purpose: Binds textures to the D3D pipeline
//
procedure CppAddBindTextures(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
var
  params: string;
  i: Integer;
  res: TDFXResource;
  varName: string;
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  for i := 0 to Group.ResVarNames.Count - 1 do
  begin
    if params.Length > 0 then
      params := params + ', ';

    res := TDFXResource(Group.ResVarNames.Objects[i]);
    varName := CppFinalizeVariableName(res);
    if res.IsTexture then
      params := params + varName + ' ? ' + GState.CppTranslator.UQuerySRV(varName) + ' : nullptr'
    else
      params := params + GState.CppTranslator.UQuerySRV(varName);
  end;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11ShaderResourceView *tmp[] = { %s };', [params]));
  CppTxt.Add(Format('%s(%d, %d, tmp);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Binds UAVs to the D3D pipeline
//
procedure CppAddBindUAVs(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
var
  params: string;
  counterParams: string;
  i: Integer;
  res: TDFXResource;
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  for i := 0 to Group.ResVarNames.Count - 1 do
  begin
    if params.Length > 0 then
    begin
      params := params + ', ';
      counterParams := counterParams + ', ';
    end;

    res := TDFXResource(Group.ResVarNames.Objects[i]);
    params := params + GState.CppTranslator.UQueryUAV(CppFinalizeVariableName(res), res.SubresourceIndex);
    counterParams := counterParams + res.UAVInitialCount;
  end;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11UnorderedAccessView *tmp[] = { %s };', [params]));
  CppTxt.Add(Format('const UINT counters[] = { %s };', [counterParams]));
  CppTxt.Add(Format('%s(%d, %d, tmp, counters);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Unbinds textures from the D3D pipeline
//
procedure CppAddUnbindTextures(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11ShaderResourceView *tmp[%d] = { nullptr };', [Group.ResVarNames.Count]));
  CppTxt.Add(Format('%s(%d, %d, tmp);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Unbinds UAVs from the D3D pipeline
//
procedure CppAddUnbindUAVs(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11UnorderedAccessView *tmp[%d] = { nullptr };', [Group.ResVarNames.Count]));
  CppTxt.Add(Format('%s(%d, %d, tmp, nullptr);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Unbinds samplers from the D3D pipeline
//
procedure CppAddUnbindSamplerStates(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11SamplerState *tmp[%d] = { nullptr };', [Group.ResVarNames.Count]));
  CppTxt.Add(Format('%s(%d, %d, tmp);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Unbinds CBs from the D3D pipeline
//
procedure CppAddUnbindConstantBuffers(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11Buffer *tmp[%d] = { nullptr };', [Group.ResVarNames.Count]));
  CppTxt.Add(Format('%s(%d, %d, tmp);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Binds sampler states to the D3D pipeline
//
procedure CppAddBindSamplerStates(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
var
  params: string;
  i: Integer;
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  for i := 0 to Group.ResVarNames.Count - 1 do
  begin
    if params.Length > 0 then
      params := params + ', ';

    params := params + GState.CppTranslator.UQuerySamplerState(CppFinalizeVariableName(TDFXResource(Group.ResVarNames.Objects[i])));
  end;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11SamplerState *tmp[] = { %s };', [params]));
  CppTxt.Add(Format('%s(%d, %d, tmp);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

//
// Purpose: Binds CBs to the D3D pipeline
//
procedure CppAddBindConstantBuffers(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Group: TDFXResourceGroup);
var
  params: string;
  i: Integer;
  obj: TDFXConstantBuffer;
begin
  if Group.ResVarNames.Count = 0 then
    Exit;

  for i := 0 to Group.ResVarNames.Count - 1 do
  begin
    if params.Length > 0 then
      params := params + ', ';

    obj := TDFXConstantBuffer(Group.ResVarNames.Objects[i]);
    params := params + GState.CppTranslator.UQueryConstantBuffer(CppFinalizeVariableName(obj));
  end;

  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add(Format('ID3D11Buffer *tmp[] = { %s };', [params]));
  CppTxt.Add(Format('%s(%d, %d, tmp);', [D3DCallStr, Group.BindPoint, Group.BindCount]));
  CppTxt.Unindent;
  CppTxt.Add('}');
end;

type
  TDFXShaderGroupBindCommand = procedure(CppTxt: TDFXNamespaceStringList;
    const D3DCallStr: string; const Group: TDFXResourceGroup);

//
// Purpose: Computes bind groups
//
procedure CppAddShaderBinding(Bindings: TList<TDFXResourceBindDesc>; CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; var Group: TDFXResourceGroup; GroupBindCmd: TDFXShaderGroupBindCommand);
var
  nextBindPoint: Integer;
  i: Integer;
  rbd: TDFXResourceBindDesc;
begin
  Group.ResVarNames.Clear;

  if Bindings.Count = 0 then
    Exit;

  Group.BindPoint := Bindings[0].BindPoint;
  Group.BindCount := Bindings[0].BindCount;
  Group.ResVarNames.AddObject(Bindings[0].Name, Bindings[0].Obj);

  nextBindPoint := Group.BindPoint + Group.BindCount;

  for i := 1 to Bindings.Count - 1 do
  begin
    rbd := Bindings[i];

    if rbd.BindPoint = nextBindPoint then
    begin
      Inc(Group.BindCount, rbd.BindCount);
      Inc(nextBindPoint, rbd.BindCount);
    end
    else
    begin
      // New group
      // Output bind command
      GroupBindCmd(CppTxt, D3DCallStr, Group);

      // And reset
      Group.ResVarNames.Clear;
      Group.BindPoint := rbd.BindPoint;
      Group.BindCount := 1;
      nextBindPoint := Group.BindPoint + rbd.BindCount;
    end;

    Group.ResVarNames.AddObject(rbd.Name, rbd.Obj);
  end;

  if Group.BindCount > 0 then
    GroupBindCmd(CppTxt, D3DCallStr, Group);
end;

//
// Purpose: Binds a pass
//
procedure CppAddPassSet(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Reflection: TDFXShaderReflectionData; BindUAVs: Boolean);
var
  group: TDFXResourceGroup;
begin
  group.ResVarNames := TStringList.Create;

  try
    // Set resources in adjacent groups
    CppAddShaderBinding(Reflection.Textures, CppTxt, D3DCallStr + 'SetShaderResources', group, CppAddBindTextures);

    if BindUAVs then
      CppAddShaderBinding(Reflection.UAVs, CppTxt, D3DCallStr + 'SetUnorderedAccessViews', group, CppAddBindUAVs);

    CppAddShaderBinding(Reflection.SamplerStates, CppTxt, D3DCallStr + 'SetSamplers', group, CppAddBindSamplerStates);
    CppAddShaderBinding(Reflection.ConstantBuffers, CppTxt, D3DCallStr + 'SetConstantBuffers', group, CppAddBindConstantBuffers);
  finally
    group.ResVarNames.Free;
  end;
end;

//
// Purpose: BeginPass function
//
procedure CppAddBeginPassFunction(CppTxt: TDFXNamespaceStringList;
  const Reflection: TDFXPassReflectionData; PassIndex: Integer);
var
  i: string;
begin
  i := IntToStr(PassIndex);

  if Assigned(Reflection.CS) then
  begin
    CppAddPassSet(CppTxt, 'm_d3dDeviceContext->CS', Reflection.CS, True);
    CppTxt.Add(Format('m_d3dDeviceContext->CSSetShader((ID3D11ComputeShader *)%s, nullptr, 0);', [GState.CppTranslator.UQueryShader('m_pass' + i + 'CS')]));
  end
  else
  begin
    if (Assigned(Reflection.VS) and (GState.Effect.Passes[PassIndex].VertexShader.Reference.Length > 0)) then
    begin
      CppAddPassSet(CppTxt, 'm_d3dDeviceContext->VS', Reflection.VS, False);
      CppTxt.Add(Format('m_d3dDeviceContext->VSSetShader((ID3D11VertexShader *)%s, nullptr, 0);', [GState.CppTranslator.UQueryShader('m_pass' + i + 'VS')]));
    end;

    if (Assigned(Reflection.PS)) and (GState.Effect.Passes[PassIndex].PixelShader.Reference.Length > 0) then
    begin
      CppAddPassSet(CppTxt, 'm_d3dDeviceContext->PS', Reflection.PS, False);
      CppTxt.Add(Format('m_d3dDeviceContext->PSSetShader((ID3D11PixelShader *)%s, nullptr, 0);', [GState.CppTranslator.UQueryShader('m_pass' + i + 'PS')]));
    end;
  end;
end;

//
// Purpose: Unbinds a pass
//
procedure CppAddPassClear(CppTxt: TDFXNamespaceStringList;
  const D3DCallStr: string; const Reflection: TDFXShaderReflectionData; UnbindUAVs: Boolean);
var
  group: TDFXResourceGroup;
begin
  group.ResVarNames := TStringList.Create;

  try
    // Set resources in adjacent groups
    CppAddShaderBinding(Reflection.Textures, CppTxt, D3DCallStr + 'SetShaderResources', group, CppAddUnbindTextures);

    if UnbindUAVs then
      CppAddShaderBinding(Reflection.UAVs, CppTxt, D3DCallStr + 'SetUnorderedAccessViews', group, CppAddUnbindUAVs);

    // CppAddShaderBinding(Reflection.SamplerStates, CppTxt, D3DCallStr + 'SetSamplers', group, CppAddUnbindSamplerStates);
    // CppAddShaderBinding(Reflection.ConstantBuffers, CppTxt, D3DCallStr + 'SetConstantBuffers', group, CppAddUnbindConstantBuffers);
  finally
    group.ResVarNames.Free;
  end;
end;

//
// Purpose: EndPass function
//
procedure CppAddEndPassFunction(CppTxt: TDFXNamespaceStringList;
  const Reflection: TDFXEffectReflectionData; PassIndex: Integer);
var
  i, pi: Integer;
  doUnbind: Boolean;
begin
  if Assigned(Reflection[PassIndex].CS) then
  begin
    CppAddPassClear(CppTxt, 'm_d3dDeviceContext->CS', Reflection[PassIndex].CS, True);
    // CppTxt.Add('m_d3dDeviceContext->CSSetShader(nullptr, nullptr, 0);');
  end
  else
  begin
    CppAddPassClear(CppTxt, 'm_d3dDeviceContext->VS', Reflection[PassIndex].VS, False);
    CppAddPassClear(CppTxt, 'm_d3dDeviceContext->PS', Reflection[PassIndex].PS, False);

    if Assigned(Reflection[PassIndex].VS) then
    begin
      {*
      pi := -1;

      doUnbind := False;

      // Unbind only if last stage or next geometry stage does not overwrite shader
      // Last pass?
      if PassIndex = Reflection.Count then
        doUnbind := True
      else
      begin
        for i := PassIndex + 1 to Reflection.Count - 1 do
        begin
          // Skip compute stages
          if Assigned(Reflection[i].CS) then
            Continue;

          if not Assigned(Reflection[i].VS) then
            doUnbind
        end;
      end;
      *}

      //if ((PassIndex < Reflection.Count - 1) and (not Assigned(Reflection[PassIndex+1].VS))) or (PassIndex = Reflection.Count - 1) then
          CppTxt.Add('m_d3dDeviceContext->VSSetShader(nullptr, nullptr, 0);');

    end;

    if Assigned(Reflection[PassIndex].PS) then
      //if ((PassIndex < Reflection.Count - 1) and (not Assigned(Reflection[PassIndex+1].PS))) or (PassIndex = Reflection.Count - 1) then
        CppTxt.Add('m_d3dDeviceContext->PSSetShader(nullptr, nullptr, 0);');
  end;
end;

//
// Purpose: Adds resource semantics as fixed indices to the effect
//
procedure CppAddResourceSemantics(CppTxt: TStringList;
  const Reflection: TDFXShaderReflectionData; const ShaderPrefix: string;
  PassIndex: Integer); overload;
var
  i: Integer;
  res: TDFXResource;
begin
  if not Assigned(Reflection) then
    Exit;

  if (Reflection.Textures.Count = 0) or
     (Reflection.SamplerStates.Count = 0) then
    Exit;

  for i := 0 to Reflection.Textures.Count - 1 do
  begin
    res := TDFXResource(Reflection.Textures[i].Obj);
    if res.Semantic.Length > 0 then
      CppTxt.Add(Format('static const uint32_t kPass%d%sSemantic%sMap = %d;', [PassIndex, ShaderPrefix, res.Semantic, Reflection.Textures[i].BindPoint]));
  end;

  for i := 0 to Reflection.SamplerStates.Count - 1 do
  begin
    res := TDFXResource(Reflection.SamplerStates[i].Obj);
    if res.Semantic.Length > 0 then
      CppTxt.Add(Format('static const uint32_t kSemantic%sMap = %d;', [res.Semantic, Reflection.SamplerStates[i].BindPoint]));
  end;

  CppTxt.Add('');
end;

//
// Purpose: Adds resource semantics
//
procedure CppAddResourceSemantics(CppTxt: TStringList;
  const Reflection: TDFXPassReflectionData; PassIndex: Integer); overload;
begin
  CppAddResourceSemantics(CppTxt, Reflection.VS, 'VS', PassIndex);
  CppAddResourceSemantics(CppTxt, Reflection.PS, 'PS', PassIndex);
  CppAddResourceSemantics(CppTxt, Reflection.CS, 'CS', PassIndex);
end;

//
// Purpose: Adds passes
//
procedure CppAddRunAndPasses(CppTxt: TDFXNamespaceStringList;
  const Reflection: TDFXEffectReflectionData);
var
  i: Integer;
begin
  CppTxt.Add(Format('void dfx::%srun(void *userData)', [GState.Effect.FilePath('::')]));
  CppTxt.Add('{');
  CppTxt.Indent;
  for i := 0 to GState.Effect.Passes.Count - 1 do
  begin
    CppTxt.Add(Format('begin_pass%d();', [i]));
    CppTxt.Add(Format('on_do_pass%d(userData);', [i]));
    CppTxt.Add(Format('end_pass%d();', [i]));

    if i < GState.Effect.Passes.Count - 1 then
      CppTxt.Add('');
  end;
  CppTxt.Unindent;
  CppTxt.Add('}');
  cppTxt.Add('');

  for i := 0 to GState.Effect.Passes.Count - 1 do
  begin
    CppTxt.Add(Format('void dfx::%sbegin_pass%d()', [GState.Effect.FilePath('::'), i]));
    CppTxt.Add('{');
    CppTxt.Indent;
    CppAddBeginPassFunction(CppTxt, Reflection[i], i);
    CppTxt.Unindent;
    CppTxt.Add('}');
    CppTxt.Add('');
    CppTxt.Add(Format('void dfx::%send_pass%d()', [GState.Effect.FilePath('::'), i]));
    CppTxt.Add('{');
    CppTxt.Indent;
    CppAddEndPassFunction(CppTxt, Reflection, i);
    CppTxt.Unindent;
    CppTxt.Add('}');
    CppTxt.Add('');
  end;
end;

//
// Purpose: Adds shader members
//
procedure CppAddShaders(CppTxt: TDFXNamespaceStringList);
var
  p: TDFXEffectPass;
  i: Integer;
  shaderClass: string;
begin
  shaderClass := GState.CppTranslator.UShaderClass;

  for i := 0 to GState.Effect.Passes.Count - 1 do
  begin
    p :=GState.Effect.Passes[i];
    CppTxt.Add('');

    case p.PassType of
      ptCompute:
        begin
          CppTxt.Add(Format('%s *m_pass%dCS;', [shaderClass, i]));
        end;
      ptGeometry:
        begin
          if p.VertexShader.Reference.Length > 0 then
            CppTxt.Add(Format('%s *m_pass%dVS;', [shaderClass, i]));

          if p.PixelShader.Reference.Length > 0 then
            CppTxt.Add(Format('%s *m_pass%dPS;', [shaderClass, i]));
        end;
      else
        Assert(False);
    end;
  end;
end;

//
// Purpose: adds shader class fwd declarations
//
procedure CppAddForwardDeclaration(CppTxt: TDFXNamespaceStringList;
  const Decl: string; Index: Integer);
var
  sl: TStringList;
  i: Integer;
begin
  sl := TStringList.Create;

  try
    StrToStrings(Decl, '::', sl);
    for i := 0 to sl.Count - 2 do
    begin
      CppTxt.Insert(Index, 'namespace ' + sl[i]);
      CppTxt.Insert(Index + 1, '{');
      Inc(Index, 2);
      CppTxt.Indent;
    end;

    CppTxt.Add(GState.CppTranslator.UForwardDeclareType(sl[sl.Count-1]));
    Inc(Index);

    for i := 0 to sl.Count - 2 do
    begin
      CppTxt.Unindent;
      CppTxt.Insert(Index, '}');
      Inc(Index);
    end;
  finally
    sl.Free;
  end;
end;

//
// Purpose: Rebases the inclusions in an inclusions list
//
procedure AddRebasedInclusions(const CppTxt, Inclusions: TStringList;
  const Path, BaseOffset: string);
var
  i: Integer;
  relativeBasePath, basePath, line: string;
begin
  basePath := Path + BaseOffset;
  basePath := MakeAbsolutePath(GetCurrentDir, basePath);
  relativeBasePath := MakeAbsolutePath(GetCurrentDir, Path);
  for i := 0 to Inclusions.Count - 1 do
  begin
    line := Inclusions[i];
    Delete(line, 1, Length('#include "'));
    Delete(line, line.Length, 1);
    line := MakeAbsolutePath(basePath, line);
    line := MakeRelativePath(relativeBasePath, line);
    CppTxt.Add('#include "' + line + '"');
  end;
end;

//
// Purpose: adds constants
//
procedure CppAddConstants(const CppTxt: TDFXNamespaceStringList; Effect: TDFXEffect;
  const Reflection: TDFXEffectReflectionData);
var
  i: Integer;
  r: TDFXShaderReflectionData;
begin
  CppTxt.Add('public:');
  CppTxt.Indent;
  CppTxt.Add('// Constants');

  for i := 0 to Effect.Passes.Count - 1 do
  begin
    r := Reflection[i].CS;

    if not Assigned(r) then
      Continue;

    if (r.ThreadGroupSize.X = 0) and
       (r.ThreadGroupSize.Y = 0) and
       (r.ThreadGroupSize.Z = 0) then
      Continue;

    CppTxt.Add(Format('const uint32_t kPass%dGroupSizeX = %d;', [i, r.ThreadGroupSize.X]));
    CppTxt.Add(Format('const uint32_t kPass%dGroupSizeY = %d;', [i, r.ThreadGroupSize.Y]));
    CppTxt.Add(Format('const uint32_t kPass%dGroupSizeZ = %d;', [i, r.ThreadGroupSize.Z]));
    CppTxt.Add('');
  end;

  CppTxt.Add('// Semantics');
  for i := 0 to Effect.Passes.Count - 1 do
    CppAddResourceSemantics(CppTxt, Reflection[i], i);

  CppTxt.Unindent;
end;

//
// Purpose: Saves an effect .h
//
procedure SaveCppEffectHeader(CppTxt: TDFXNamespaceStringList;
  Reflection: TList<TDFXPassReflectionData>);
var
  i: Integer;
begin
  cppTxt.Clear;
  cppTxt.Add('// DFX auto-generated file' + #13#10);
  cppTxt.Add('#pragma once');
  cppTxt.Add('');
  cppTxt.Add('#include <cstdint>');
  cppTxt.Add('');
  cppTxt.Add('struct ID3D11DeviceContext;');
  CppAddForwardDeclaration(CppTxt, GState.CppTranslator.UShaderClass, cppTxt.Count);
  cppTxt.Add('');
  cppTxt.Add('namespace dfx');
  cppTxt.Add('{');
  cppTxt.Indent;
  cppTxt.BeginNamespace;
  cppTxt.Add('class ' +GState.Effect.Name);
  cppTxt.Add('{');
  CppAddConstants(cppTxt,GState.Effect, Reflection);
  cppTxt.Add('public:');
  cppTxt.Indent;
  cppTxt.Add(Format('~%s() { dispose(); }', [GState.Effect.Name]));
  cppTxt.Add('void initialize(ID3D11DeviceContext *d3dDeviceContext, void *userData);');
  cppTxt.Add('void dispose();');
  cppTxt.Add('void run(void *userData);');
  cppTxt.Add('void on_resize(); // USER');
  cppTxt.Unindent;
  cppTxt.Add('protected:');
  cppTxt.Indent;
  cppTxt.Add('void on_initialize(void *userData); // USER');
  cppTxt.Add('void on_dispose(); // USER');
  cppTxt.Add('');
  cppTxt.Add('void load_shaders();');
  cppTxt.Add('void dispose_shaders();');
  cppTxt.Add('');
  for i := 0 to GState.Effect.Passes.Count - 1 do
  begin
    cppTxt.Add(Format('void begin_pass%d();', [i]));
    cppTxt.Add(Format('void on_do_pass%d(void *userData); // USER', [i]));
    cppTxt.Add(Format('void end_pass%d();', [i]));
  end;
  cppTxt.Add('');
  cppTxt.Add('ID3D11DeviceContext *m_d3dDeviceContext;');
  CppAddShaders(cppTxt);
  // CppAddResources(cppTxt,GState.Effect, State);
  cppTxt.Unindent;
  cppTxt.Add('};');
  cppTxt.EndNamespace;
  cppTxt.Unindent;
  cppTxt.Add('}');
  SaveText(GState.Effect.DiskPath + 'effect.h', cppTxt.Text,GState.Project.Preamble);
end;

//
// Purpose: Saves an effect .cpp
//
procedure SaveCppEffectSource(CppTxt: TDFXNamespaceStringList; Reflection: TDFXEffectReflectionData);
var
  s: string;
begin
  cppTxt.Clear;
  cppTxt.Add('// DFX auto-generated file' + #13#10);
  cppTxt.Add('');

  s := GState.CppTranslator.UPrecompiledHeader;
  if s.Length > 0 then
  begin
    cppTxt.Add('#include "' + s + '"');
    cppTxt.Add('');
  end;

  cppTxt.Add('#include "effect.h"');
  cppTxt.Add(GState.CppTranslator.UIncludeUserTypes);
  cppTxt.Add('#include "effect_types.h"');
  cppTxt.Add('');
  cppTxt.Add('#include <d3d11.h>');
  cppTxt.Add('');
  CppAddInitDispose(CppTxt);
  cppTxt.Add('');
  CppAddRunAndPasses(CppTxt, Reflection);
  cppTxt.Unindent;
  SaveText(GState.Effect.DiskPath + 'effect.cpp', cppTxt.Text, GState.Project.Preamble);
end;

function PosLeft(const SubStr, Text: string; Index: Integer): Integer;
var
  i, j: Integer;
begin
  if Index >= Text.Length then
    Index := Text.Length;

  Result := 0;
  i := 0;
  while True do
  begin
    j := Pos(SubStr, Text, i + 1);
    if (j > 0) and (j < Index) then
      Result := j
    else
      Exit;
    i := j;
  end;
end;

function FindNonCommentText(const Text: string; const SubStr: string): Boolean;
var
  i, p, plb, nlb, mlc, c: Integer;
  inMLC: Boolean;
begin
  i := 1;

  while i < Text.Length do
  begin
    // Find SubStr
    p := Pos(SubStr, Text, i);

    if p = 0 then
      Break;

    // Go to line beginning of p
    plb := PosLeft(sLineBreak, Text, p);
    nlb := Pos(sLineBreak, Text, p);

    // Find SL-comment
    c := PosLeft('//', Text, p);
    // Find ML-comment
    mlc := PosLeft('/*', Text, Min(p, c));
    inMLC := (mlc > 0) and (p < c);
    mlc := Pos('*/', Text, mlc);
    if inMLC then
      inMLC := (mlc > 0) and (mlc > p);

    if not inMLC then
      // SLC
      if (c > 0) and (plb < c) and (c < nlb) then
      begin
        // Keyword before comment
        if p < c then
          Exit(True);
        // else keep searching...
      end
      else
        // No comment
        Exit(True);

    i := p + 1;
  end;

  Result := False;
end;

procedure AddNamespaceExternDeclarations(cppTxt: TStringList; NS: TDFXLeafNamespace);
var
  i: Integer;
begin
  for i := 0 to NS.ConstantBuffers.Count - 1 do
    cppTxt.Add(GState.CppTranslator.UConstantBufferClass + ' *' + CppFinalizeVariableName(NS.ConstantBuffers[i]) + ' = nullptr;');

  for i := 0 to NS.Resources.Count - 1 do
    cppTxt.Add(GState.CppTranslator.UResourceTypeFromHLSL(NS.Resources[i].GetHLSLTypeString) + ' *' + CppFinalizeVariableName(NS.Resources[i]) + ' = nullptr;');
end;

procedure SaveCppNamespaceTypes(NS: TDFXLeafNamespace);
var
  cppTxt: TStringList;
  includes: TStringList;
  i, j: Integer;
begin
  includes := TStringList.Create;
  cppTxt := TStringList.Create;

  try
    // TODO:
    // Try sorting constant buffers using Name-usage heuristic
    for i := 0 to NS.Structures.Count - 2 do
      for j := i + 1 to NS.Structures.Count - 1 do
      begin
        if FindNonCommentText(NS.Structures[i].Text, NS.Structures[j].Name) then
        begin
          NS.Structures.Move(i, j);
          Break;
        end;
      end;

    for i := 0 to NS.Structures.Count - 1 do
      includes.Add('#include "structures\' + NS.Structures[i].Name + '"');

    for i := 0 to NS.ConstantBuffers.Count - 2 do
      for j := i + 1 to NS.ConstantBuffers.Count - 1 do
      begin
        if FindNonCommentText(NS.ConstantBuffers[i].Text, NS.ConstantBuffers[j].Name) then
        begin
          NS.ConstantBuffers.Move(i, j);
          Break;
        end;
      end;

    for i := 0 to NS.ConstantBuffers.Count - 1 do
      includes.Add('#include "cbuffers\' + NS.ConstantBuffers[i].Name + '"');

    for i := 0 to NS.Resources.Count - 1 do
      includes.Add('#include "resources\' + NS.Resources[i].Name + '"');

    cppTxt.Clear;
    cppTxt.Add('#pragma once');
    cppTxt.Add('');
    cppTxt.Add(InclusionString(NS.DiskPath, 'dfx_cpp_types.h'));
    cppTxt.Add(InclusionString(NS.DiskPath, 'dfx_push_cpp_macros.h'));
    cppTxt.Add('#include "dfx/constants.hlsli"');
    AddRebasedInclusions(cppTxt, includes, NS.DiskPath, '');
    //cppTxt.Add(InclusionString(NS.DiskPath, 'namespace_types_includes.h'));
    cppTxt.Add(InclusionString(NS.DiskPath, 'dfx_pop_cpp_macros.h'));
    SaveText(NS.DiskPath + 'namespace_types.h', cppTxt.Text, NS.Project.Preamble);

    cppTxt.Clear;
    cppTxt.Add('#pragma once');
    cppTxt.Add('');
    AddNamespaceExternDeclarations(cppTxt, NS);
    SaveText(NS.DiskPath + 'namespace_declarations.h', cppTxt.Text, NS.Project.Preamble);
  finally
    includes.Free;
    cppTxt.Free;
  end;
end;

procedure RebaseInclusions(Inclusions: TStringList; const CurrentPath, BasePath: string);
var
  i: Integer;
  line, outPath: string;
begin
  outPath := MakeAbsolutePath(GetCurrentDir, '.' + PathDelim + BasePath);
  for i := 0 to Inclusions.Count - 1 do
  begin
    line := Inclusions[i];
    Delete(line, 1, Length('#include "'));
    Delete(line, line.Length, 1);
    line := MakeAbsolutePath(CurrentPath, line);
    if line[1] = PathDelim then
      Delete(line, 1, 1);
    line := MakeAbsolutePath(GetCurrentDir, line);
    line := MakeRelativePath(outPath, line);
    Inclusions[i] := '#include "' + line + '"';
  end;
end;

procedure SaveCppEffectTypes(CppTxt: TStringList; Effect: TDFXEffect;
  Includes: TStringList);
begin
  cppTxt.Clear;
  cppTxt.Add('#pragma once');
  cppTxt.Add('');
  cppTxt.Add(InclusionString(Effect.DiskPath, 'dfx_cpp_types.h'));
  cppTxt.Add(InclusionString(Effect.DiskPath, 'dfx_push_cpp_macros.h'));
  cppTxt.Add('#include "dfx/constants.hlsli"');
  AddRebasedInclusions(cppTxt, Includes, Effect.DiskPath, '');
  cppTxt.Add(InclusionString(Effect.DiskPath, 'dfx_pop_cpp_macros.h'));
  SaveText(Effect.DiskPath + 'effect_types.h', cppTxt.Text, Effect.Project.Preamble);

  cppTxt.Clear;
  cppTxt.Add('#pragma once');
  cppTxt.Add('');
  AddNamespaceExternDeclarations(cppTxt, Effect);
  SaveText(Effect.DiskPath + 'effect_declarations.h', cppTxt.Text, Effect.Project.Preamble);
end;

procedure CompileDFXEffect;
var
  p: TDFXEffectPass;
  cppTxt: TDFXNamespaceStringList;
  hlslInclusionList, finalHLSLInclusions: TStringList;
  effectReflection: TDFXEffectReflectionData;
  passReflection: TDFXPassReflectionData;
  i: Integer;
begin
  if not GState.Effect.ParticipateInBuild then
    Exit;

  cppTxt := TDFXNamespaceStringList.Create(GState.Effect.Parent);
  hlslInclusionList := TStringList.Create;
  finalHLSLInclusions := TStringList.Create;
  effectReflection := TList<TDFXPassReflectionData>.Create;

  try
    if GState.Effect.Passes.Count = 0 then
    begin
     GState.Handler.OnStatus('Preparing...');
     GState.Handler.OnLog('Skipping empty effect ' +GState.Effect.Name + '.');
      Exit;
    end;

   GState.Handler.OnStatus('Compiling effect ' +GState.Effect.Name + '...');

    if not GState.Handler.OnProgress then
      RaiseAndLog('Canceling effect compilation...');

   GState.Handler.OnLog(Format('Building %s...', [GState.Effect.Name]));

    for p in GState.Effect.Passes do
    begin
      passReflection.VS := nil;
      passReflection.PS := nil;
      passReflection.CS := nil;

      case p.PassType of
        ptCompute:
          begin
            if p.ComputeShader.Reference.Length = 0 then
              RaiseAndLog(Format('No shader references specified in effect %s.', [GState.Effect.Name]));

            passReflection.CS := TDFXShaderReflectionData.Create;
            try
              // TODO: check if shader profile is vaild

              WriteShader(p.ComputeShader, hlslInclusionList, passReflection.CS, True);
              RebaseInclusions(hlslInclusionList, ExtractFilePath(GState.Effect.Parent.FindGlobalFunctionReference(p.ComputeShader.Reference).DiskPath), GState.Effect.DiskPath);
              MergeStringLists(hlslInclusionList, finalHLSLInclusions);
            except
              passReflection.CS.Free;
              raise;
            end;
          end;
        ptGeometry:
          begin
            //if (p.VertexShader.Reference.Length = 0) and (p.PixelShader.Reference.Length = 0) then
            //  RaiseAndLog(State, Format('No shader references specified in effect %s.', [State.Effect.Name]));

            passReflection.VS := TDFXShaderReflectionData.Create;
            passReflection.PS := TDFXShaderReflectionData.Create;
            try
              // TODO: check if shader profile is vaild

              if p.VertexShader.Reference.Length > 0 then
              begin
                WriteShader(p.VertexShader, hlslInclusionList, passReflection.VS, True);
                RebaseInclusions(hlslInclusionList, ExtractFilePath(GState.Effect.Parent.FindGlobalFunctionReference(p.VertexShader.Reference).DiskPath), GState.Effect.DiskPath);
                MergeStringLists(hlslInclusionList, finalHLSLInclusions);
              end;

              if p.PixelShader.Reference.Length > 0 then
              begin
                WriteShader(p.PixelShader, hlslInclusionList, passReflection.PS, True);
                RebaseInclusions(hlslInclusionList, ExtractFilePath(GState.Effect.Parent.FindGlobalFunctionReference(p.PixelShader.Reference).DiskPath),
                 GState.Effect.DiskPath);
                MergeStringLists(hlslInclusionList, finalHLSLInclusions);
              end;
            except
              passReflection.VS.Free;
              passReflection.PS.Free;
              raise;
            end;
          end;
        else
          Assert(False);
      end;

      effectReflection.Add(passReflection);
    end;

    if Assigned(GState.CppTranslator) then
    begin
     GState.Handler.OnStatus(Format('Generating C++-binding for effect %s...', [GState.Effect.Name]));
      SaveCppEffectHeader(cppTxt, effectReflection);
      SaveCppEffectSource(cppTxt, effectReflection);
      SaveCppEffectTypes(cppTxt,GState.Effect, finalHLSLInclusions);
    end;
  finally
    if Assigned(effectReflection) then
      for i := 0 to effectReflection.Count - 1 do
      begin
        effectReflection[i].VS.Free;
        effectReflection[i].PS.Free;
        effectReflection[i].CS.Free;
      end;

    cppTxt.Free;
    finalHLSLInclusions.Free;
    hlslInclusionList.Free;
    effectReflection.Free;
  end;
end;

procedure SaveCppHeaders;
var
  sl: TDFXNamespaceStringList;
  procedure AddHLSLVectorType(const CppType, HLSLType: string);
  var
    i: Integer;
    j: Integer;
  begin
    if CppType <> HLSLType then
      sl.Add(Format('typedef %s %s;', [CppType, HLSLType]));

    for i := 1 to 4 do
      sl.Add(Format('typedef %s %s%d[%d];', [CppType, HLSLType, i, i]));

    for i := 1 to 4 do
      for j := 1 to 4 do
        sl.Add(Format('typedef %s %s%dx%d[%d][%d];', [CppType, HLSLType, i, j, i, j]));
  end;

  procedure PushMacro(const Name: string; const Value: string = '');
  begin
    sl.Add('#pragma push_macro("' + Name + '")');
    sl.Add('#define ' + Name + '(...) ' + Value)
  end;

  procedure PopMacro(const Name: string);
  begin
    sl.Add('#pragma pop_macro("' + Name + '")');
  end;
begin
  sl := TDFXNamespaceStringList.Create(nil);

  try
    sl.Add('// DFX auto-generated file');
    sl.Add('#pragma once');
    sl.Add('');

    //
    // Types
    //
    sl.Add('namespace dfx_hlsl');
    sl.Add('{');
    sl.Indent;
    // Vector
    sl.Add('template<typename T0, int Count> struct vector { T0 values[Count]; };');
    // Scalars
    AddHLSLVectorType('uint32_t', 'uint');
    AddHLSLVectorType('float', 'float');
    sl.Add('typedef float4x4 matrix;');
    AddHLSLVectorType('bool', 'bool');
    AddHLSLVectorType('int', 'int');
    AddHLSLVectorType('float', 'half');
    AddHLSLVectorType('uint32_t', 'dword');
    AddHLSLVectorType('double', 'double');
    AddHLSLVectorType('float', 'min16float');
    AddHLSLVectorType('float', 'min10float');
    AddHLSLVectorType('int', 'min16int');
    AddHLSLVectorType('int', 'min12int');
    AddHLSLVectorType('uint32_t', 'min16uint');
    sl.Unindent;
    sl.Add('}');

    sl.SaveToFile(GState.BuildDir + 'dfx_cpp_types.h');
    sl.Clear;

    sl.Add('// DFX auto-generated file');
    sl.Add('#pragma once');
    sl.Add('');
    //
    // Push Macros
    //
    PushMacro('snorm');
    PushMacro('unorm');

    // PushMacro('packoffset');
    // PushMacro('register');

    PushMacro('extern');
    PushMacro('nointerpolation');
    PushMacro('precise');
    PushMacro('shared');
    PushMacro('groupshared');
    PushMacro('static');
    PushMacro('uniform');

    PushMacro('row_major');
    PushMacro('column_major');

    PushMacro('cbuffer', 'struct');
    PushMacro('tbuffer', 'struct');

    sl.SaveToFile(GState.BuildDir + 'dfx_push_cpp_macros.h');
    sl.Clear;

    sl.Add('// DFX auto-generated file');
    sl.Add('#pragma once');
    sl.Add('');
    //
    // Pop Macros
    //
    PopMacro('snorm');
    PopMacro('unorm');

    // PopMacro('packoffset');
    // PopMacro('register');

    PopMacro('extern');
    PopMacro('nointerpolation');
    PopMacro('precise');
    PopMacro('shared');
    PopMacro('groupshared');
    PopMacro('static');
    PopMacro('uniform');

    PopMacro('row_major');
    PopMacro('column_major');

    sl.SaveToFile(GState.BuildDir + 'dfx_pop_cpp_macros.h');
  finally
    sl.Free;
  end;
end;

function RemoveHLSLSemantics(const Text: string): string;
var
  i, j: Integer;
begin
  // FIXME: not safe against #define
  Result := Text;
  i := Pos(':', Result);
  while (i > 0) and (i <= Text.Length) do
  begin
    if i >= Result.Length then
      Exit;

    if Result[i+1] = ':' then
    begin
      // Namespace
      Inc(i, 2);
    end
    else
    begin
      j := Pos(';', Result, i);
      Delete(Result, i, j - i);

      // Remove spaces
      while (i > 1) and (Result[i-1] = ' ') do
      begin
        Delete(Result, i-1, 1);
        Dec(i);
      end;
    end;

    i := Pos(':', Result, i);
  end;
end;

procedure CppNamespacePrefix(SL: TDFXNamespaceStringList);
begin
  sl.Add('#ifdef __cplusplus');
  sl.Add('#pragma once');
  sl.Add('');
  sl.Add('namespace dfx_hlsl');
  sl.Add('{');
  sl.Add('#endif');
end;

procedure CppNamespaceSuffix(SL: TDFXNamespaceStringList);
begin
    sl.Add('#ifdef __cplusplus');
    sl.Add('}');
    sl.Add('#endif');
end;

procedure CompileDFXConstantBuffer(const C: TDFXConstantBuffer);
var
  sl: TDFXNamespaceStringList;
  reg: string;
begin
  sl := TDFXNamespaceStringList.Create(C.Namespace);

  try
    CppNamespacePrefix(sl);
    sl.BeginNamespace;

    sl.Add('#ifndef __cplusplus');
    // Add constant buffer namespace manually for HLSL
    sl.Add('namespace ' + C.Name);
    sl.Add('{');
    sl.Indent;

    if C.ForceRegister <> -1 then
      reg := Format(' : register(b%d)', [C.ForceRegister])
    else
      reg := '';

    case C.BufferType of
      cbtConstant: sl.Add('cbuffer ' + C.Name + reg);
      cbtTexture: sl.Add('tbuffer ' + C.Name + reg);
      else RaiseAndLog('Developer error! Unknown constant buffer type.');
    end;

    sl.Add('{');
    sl.Indent;
    sl.Add(SubstituteDFXItemName(C));
    sl.Unindent;
    sl.Add('};');

    sl.Unindent;
    sl.Add('}');
    sl.Add('#else');
    sl.Add('__declspec(align(16))');
    sl.Add('struct ' + C.Name);
    sl.Add('{');
    sl.Indent;
    sl.Add(RemoveHLSLSemantics(SubstituteDFXItemName(C)));
    sl.Unindent;
    sl.Add('};');
    sl.Add('');
    if Assigned(GState.CppTranslator) then
      sl.Add('extern ' +GState.CppTranslator.UConstantBufferClass + ' *g_' + C.Name  + ';');
    if C.ForceRegister <> -1 then
      sl.Add(Format('const uint32_t kRegister_%s = %d;', [C.Name, C.ForceRegister]));
    sl.Add('#endif');

    sl.EndNamespace;
    CppNamespaceSuffix(sl);

    SaveText(C.DiskPath, sl.Text, C.Namespace.Project.Preamble);
  finally
    sl.Free;
  end;
end;

procedure CompileDFXResource(const R: TDFXResource);
var
  sl: TDFXNamespaceStringList;
  rts, reg: string;
begin
  sl := TDFXNamespaceStringList.Create(R.Namespace);

  try
    rts := R.GetHLSLTypeString;

    CppNamespacePrefix(sl);
    sl.BeginNamespace;
    sl.Add('#ifndef __cplusplus');
    if R.ForceRegister <> -1 then
    begin
      if R.IsSamplerStateType then
        reg := Format(' : register(s%d)', [R.ForceRegister])
      else if R.GPUAccessMode = amRead then
        reg := Format(' : register(t%d)', [R.ForceRegister])
      else
        reg := Format(' : register(u%d)', [R.ForceRegister])
    end
    else
      reg := '';
    sl.Add(rts + ' ' + R.Name + reg + ';');
    sl.Add('#else');
    if Assigned(GState.CppTranslator) then
      sl.Add('extern ' +GState.CppTranslator.UResourceTypeFromHLSL(rts) + ' *g_' + R.Name + ';');
    if R.ForceRegister <> -1 then
      sl.Add(Format('const uint32_t kRegister_%s = %d;', [R.Name, R.ForceRegister]));
    sl.Add('#endif');
    sl.EndNamespace;
    CppNamespaceSuffix(sl);

    SaveText(R.DiskPath, sl.Text, R.Namespace.Project.Preamble);
  finally
    sl.Free;
  end;
end;

procedure CompileDFXStructure(const S: TDFXStructure);
var
  sl: TDFXNamespaceStringList;
begin
  sl := TDFXNamespaceStringList.Create(S.Namespace);

  try
    CppNamespacePrefix(sl);
    sl.BeginNamespace;
    sl.Add('#ifndef __cplusplus');
    sl.Add('struct ' + S.Name);
    sl.Add('{');
    sl.Indent;
    sl.Add(SubstituteDFXItemName(S));
    sl.Unindent;
    sl.Add('};');
    sl.Add('#else');
    sl.Add('struct ' + S.Name);
    sl.Add('{');
    sl.Indent;
    sl.Add(RemoveHLSLSemantics(SubstituteDFXItemName(S)));
    sl.Unindent;
    sl.Add('};');
    sl.Add('#endif');
    sl.EndNamespace;
    CppNamespaceSuffix(sl);

    SaveText(S.DiskPath, sl.Text, S.Namespace.Project.Preamble);
  finally
    sl.Free;
  end;
end;

procedure CompileDFXFunction(const F: TDFXFunction);
var
  sl: TDFXNamespaceStringList;
begin
  sl := TDFXNamespaceStringList.Create(F.Namespace);

  try
    sl.Add('#ifndef __cplusplus');
    sl.Add(ExpandDFXNamespacedItem(F));
    sl.Add('#endif');

    SaveText(F.DiskPath, sl.Text, F.Namespace.Project.Preamble);
  finally
    sl.Free;
  end;
end;

procedure CppWriteNamespaceInitializeShutdown(const Namespace: TDFXLeafNamespace);
var
  cppTxt: TDFXNamespaceStringList;
  i: Integer;
  varName: string;
  assertCmd: string;
begin
  if Namespace is TDFXEffect then
    Exit;

  cppTxt := TDFXNamespaceStringList.Create(Namespace);

  try
    // Header
    cppTxt.Add('#pragma once');
    cppTxt.Add('');
    cppTxt.Add('namespace dfx');
    cppTxt.Add('{');
    cppTxt.Indent;
    cppTxt.BeginNamespace;
    cppTxt.Add('void initialize(void *userData);');
    cppTxt.Add('void shutdown();');
    cppTxt.Add('void on_shutdown(); // USER');
    cppTxt.Add('void on_resize(); // USER');
    cppTxt.Add('void on_initialize(void *userData); // USER');
    cppTxt.EndNamespace;
    cppTxt.Unindent;
    cppTxt.Add('}');

    SaveText(Namespace.DiskPath + 'namespace.h', cppTxt.Text,GState.Project.Preamble);
    cppTxt.Clear;

    // Source
    cppTxt.Add('#include "' +GState.CppTranslator.UPrecompiledHeader  + '"');
    cppTxt.Add('');
    cppTxt.Add('#include "namespace.h"');
    cppTxt.Add(GState.CppTranslator.UIncludeUserTypes);
    cppTxt.Add('#include "namespace_types.h"');
    cppTxt.Add('');
    cppTxt.Add('using namespace dfx_hlsl;');
    cppTxt.Add('');
    cppTxt.Add(Format('void dfx::%sinitialize(void *userData)', [Namespace.FilePath('::')]));
    cppTxt.Add('{');
    cppTxt.Indent;
    cppTxt.Add('on_initialize(userData);');
    cppTxt.Add('// Validate startup');

    for i := 0 to Namespace.ConstantBuffers.Count - 1 do
    begin
      varName := CppFinalizeVariableName(Namespace.ConstantBuffers[i]);
      assertCmd := Format('(void)( (!!(%s != nullptr)) || (_wassert(_CRT_WIDE("DFX: %s was not initialized!"), _CRT_WIDE(__FILE__), __LINE__), 0) );', [varName, varName]);
      cppTxt.Add(assertCmd);
    end;

    for i := 0 to Namespace.Resources.Count - 1 do
      if not Namespace.Resources[i].IsTexture then
      begin
        varName := CppFinalizeVariableName(Namespace.Resources[i]);
        assertCmd := Format('(void)( (!!(%s != nullptr)) || (_wassert(_CRT_WIDE("DFX: %s was not initialized!"), _CRT_WIDE(__FILE__), __LINE__), 0) );', [varName, varName]);
        cppTxt.Add(assertCmd);
      end;

    cppTxt.Unindent;
    cppTxt.Add('}');
    cppTxt.Add('');
    cppTxt.Add(Format('void dfx::%sshutdown()', [Namespace.FilePath('::')]));
    cppTxt.Add('{');
    cppTxt.Indent;

    cppTxt.Add('on_shutdown();');
    {*cppTxt.Add('// Validate shutdown');

    for i := 0 to Namespace.ConstantBuffers.Count - 1 do
      cppTxt.Add('assert(!' + CppFinalizeVariableName(Namespace.ConstantBuffers[i]) + ');');

    for i := 0 to Namespace.Resources.Count - 1 do
      if not Namespace.Resources[i].IsTexture then
        cppTxt.Add('assert(!' + CppFinalizeVariableName(Namespace.Resources[i]) + ');');
    *}
    cppTxt.Unindent;
    cppTxt.Add('}');

    SaveText(Namespace.DiskPath + 'namespace.cpp', cppTxt.Text,GState.Project.Preamble);
  finally
    cppTxt.Free;
  end;
end;

procedure CompileDFXLeafNamespace(const Namespace: TDFXLeafNamespace);
var
  i: Integer;
begin
  for i := 0 to Namespace.Functions.Count - 1 do
    CompileDFXFunction(Namespace.Functions[i]);

  for i := 0 to Namespace.ConstantBuffers.Count - 1 do
    CompileDFXConstantBuffer(Namespace.ConstantBuffers[i]);

  for i := 0 to Namespace.Resources.Count - 1 do
    CompileDFXResource(Namespace.Resources[i]);

  for i := 0 to Namespace.Structures.Count - 1 do
    CompileDFXStructure(Namespace.Structures[i]);

  if Assigned(GState.CppTranslator) then
    CppWriteNamespaceInitializeShutdown(Namespace);

  if not (Namespace is TDFXEffect) then
    SaveCppNamespaceTypes(Namespace);
end;

procedure CompileDFXNamespace(const Namespace: TDFXNamespace);
var
  sns: TDFXNamespace;
  e: TDFXEffect;
begin
  if not Assigned(Namespace) then
    Exit;

  CompileDFXLeafNamespace(Namespace);

  for sns in Namespace.Subnamespaces do
    CompileDFXNamespace(sns);

  for e in Namespace.Effects do
    CompileDFXLeafNamespace(e);
end;

procedure CompileDFXEffects(const Namespace: TDFXNamespace);
var
  sns: TDFXNamespace;
  e: TDFXEffect;
begin
  if not Assigned(Namespace) then
    Exit;

  for sns in Namespace.Subnamespaces do
    CompileDFXEffects(sns);

  for e in Namespace.Effects do
  begin
    GState.Effect := e;
    CompileDFXEffect;
  end;
end;

procedure FillShaderInclusions(Namespace: TDFXLeafNamespace); overload;
var
  i: Integer;
begin
  for i := 0 to Namespace.Structures.Count - 1 do
    GState.ShaderInclusions.AddObject(Namespace.Structures[i].RelativeDiskPath, Namespace.Structures[i]);

  for i := 0 to Namespace.ConstantBuffers.Count - 1 do
    GState.ShaderInclusions.AddObject(Namespace.ConstantBuffers[i].RelativeDiskPath, Namespace.ConstantBuffers[i]);

  for i := 0 to Namespace.Resources.Count - 1 do
    GState.ShaderInclusions.AddObject(Namespace.Resources[i].RelativeDiskPath, Namespace.Resources[i]);

  for i := 0 to Namespace.Functions.Count - 1 do
    GState.ShaderInclusions.AddObject(Namespace.Functions[i].RelativeDiskPath, Namespace.Functions[i]);
end;

procedure FillShaderInclusions(Namespace: TDFXNamespace); overload;
var
  ns: TDFXNamespace;
  e: TDFXEffect;
begin
  FillShaderInclusions(TDFXLeafNamespace(Namespace));

  for e in Namespace.Effects do
    FillShaderInclusions(e);

  for ns in Namespace.Subnamespaces do
    FillShaderInclusions(ns);
end;

procedure FillShaderInclusions; overload;
begin
  GState.ShaderInclusions.AddObject('constants.hlsli', nil);
  FillShaderInclusions(GState.Project.RootNamespace);
end;

procedure CompileDFXConstants;
var
  s: string;
  c: TDFXConstant;
begin
  s := '#ifdef __cplusplus' + sLineBreak;
  s := s + '#pragma once' + sLineBreak;
  s := s + '#endif' + sLineBreak;
  for c in GState.Project.Constants do
  begin
    if s.Length > 0 then
      s := s + sLineBreak + c.Text
    else
      s := s + c.Text;
  end;

  SaveText('constants.hlsli', s, GState.Project.Preamble);
end;

procedure CppWriteNamespaceInitialize(CppTxt: TDFXNamespaceStringList;
  Namespace: TDFXNamespace);
var
  ns: TDFXNamespace;
begin
  for ns in Namespace.Subnamespaces do
  begin
    CppTxt.Add(ns.FilePath('::') + 'initialize(userData);');
    CppWriteNamespaceInitialize(CppTxt, ns);
  end;
end;

procedure CppWriteNamespaceInclude(CppTxt: TDFXNamespaceStringList;
  Namespace: TDFXNamespace);
var
  ns: TDFXNamespace;
begin
  for ns in Namespace.Subnamespaces do
  begin
    CppTxt.Add('#include "' + ns.DiskPath + 'namespace.h"');
    CppWriteNamespaceInclude(CppTxt, ns);
  end;
end;

procedure CppWriteNamespaceShutdown(CppTxt: TDFXNamespaceStringList;
  Namespace: TDFXNamespace);
var
  ns: TDFXNamespace;
begin
  for ns in Namespace.Subnamespaces do
  begin
    CppTxt.Add(ns.FilePath('::') + 'shutdown();');
    CppWriteNamespaceShutdown(CppTxt, ns);
  end;
end;

procedure CppWriteNamespaceResize(CppTxt: TDFXNamespaceStringList;
  Namespace: TDFXNamespace);
var
  ns: TDFXNamespace;
begin
  for ns in Namespace.Subnamespaces do
  begin
    CppTxt.Add(ns.FilePath('::') + 'on_resize();');
    CppWriteNamespaceResize(CppTxt, ns);
  end;
end;

procedure SaveCppStartupAndShutdownHeader(CppTxt: TDFXNamespaceStringList);
begin
  CppTxt.Add('#pragma once');
  CppTxt.Add('');
  CppTxt.Add('namespace dfx');
  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add('void initialize_all(void *userData);');
  CppTxt.Add('void shutdown_all();');
  CppTxt.Add('void resize_all();');
  CppTxt.Unindent;
  CppTxt.Add('}');

  SaveText('dfx.h', cppTxt.Text, GState.Project.Preamble);
end;

procedure SaveCppStartupAndShutdownSource(CppTxt: TDFXNamespaceStringList);
var
  pch: string;
begin
  pch := GState.CppTranslator.UPrecompiledHeader;
  if pch.Length > 0 then
  begin
    CppTxt.Add('#include "' + pch + '"');
    CppTxt.Add('');
  end;

  CppTxt.Add('#include "dfx.h"');
  CppTxt.Add('#include "namespace.h"');
  CppTxt.Add('');
  CppWriteNamespaceInclude(CppTxt, GState.Project.RootNamespace);
  CppTxt.Add('');

  CppTxt.Add('void dfx::initialize_all(void *userData)');
  CppTxt.Add('{');
  CppTxt.Indent;
  CppTxt.Add('dfx::initialize(userData);');
  CppWriteNamespaceInitialize(CppTxt, GState.Project.RootNamespace);
  CppTxt.Unindent;
  CppTxt.Add('}');

  CppTxt.Add('');
  CppTxt.Add('void dfx::shutdown_all()');
  CppTxt.Add('{');
  CppTxt.Indent;
  CppWriteNamespaceShutdown(CppTxt, GState.Project.RootNamespace);
  CppTxt.Add('dfx::shutdown();');
  CppTxt.Unindent;
  CppTxt.Add('}');

  CppTxt.Add('');
  CppTxt.Add('void dfx::resize_all()');
  CppTxt.Add('{');
  CppTxt.Indent;
  CppWriteNamespaceResize(CppTxt, GState.Project.RootNamespace);
  CppTxt.Add('dfx::on_resize();');
  CppTxt.Unindent;
  CppTxt.Add('}');

  SaveText('dfx.cpp', cppTxt.Text, GState.Project.Preamble);
end;

procedure CompileDFXShaders(NS: TDFXLeafNamespace); overload;
var
  i: Integer;
  func: TDFXShaderFunction;
  inc: TStringList;
begin
  inc := TStringList.Create;

  try
    for i := 0 to NS.Functions.Count - 1 do
    begin
      if NS.Functions[i].ShaderProfile <> '' then
      begin
        func.Reference := NS.Functions[i].FilePath('::');
        func.Attributes := '';
        GState.Effect := TDFXEffect(NS);
        GState.Handler.OnLog('Compiling ' + func.Reference);
        WriteShader(func, inc, nil, True);
        GState.Handler.OnProgress;
      end;
    end;
  finally
    inc.Free;
  end;
end;

procedure CompileDFXShaders(NS: TDFXNamespace); overload;
var
  i: Integer;
begin
  CompileDFXShaders(TDFXLeafNamespace(NS));

  for i := 0 to NS.Effects.Count - 1 do
    CompileDFXShaders(NS.Effects[i]);

  for i := 0 to NS.Subnamespaces.Count - 1 do
    CompileDFXShaders(NS.Subnamespaces[i]);
end;


procedure SaveCppStartupAndShutdown;
var
  cppTxt: TDFXNamespaceStringList;
begin
  cppTxt := TDFXNamespaceStringList.Create(nil);

  try
    SaveCppStartupAndShutdownHeader(cppTxt);
    cppTxt.Clear;
    SaveCppStartupAndShutdownSource(cppTxt);
  finally
    cppTxt.Free;
  end;
end;

procedure CompileDFXProject(Project: TDFXProject; Handler: IDFXCompilerHandler);
var
  ns: TDFXNamespace;
  startTime, duration: TTime;

  ss: TStringList;
  outp: TDFXHLSLCompilerOutput;
  cso: ID3DBlob;
begin
  GState.ShaderInclusions := TStringList.Create;

  try
    try
      // Start building
      startTime := Time;

      GState.Handler := Handler;
      GState.Project := Project;

          {*
      ss := TStringList.Create;
      ss.LoadFromFile('D:\\DFX\\output\\dfx\\effects\\fstriangle\\functions\\Inclusions.txt');
      CompileHLSLAndParse('D:\\DFX\\output\\dfx\\effects\\fstriangle\\functions\\Inclusions.txt',
        ss.Text, 'vs_5_0', 'fstriangle::vs',
        'D:\\DFX\\output\\dfx\\effects\\fstriangle\\functions\\',
        outp, cso);
      ss.Free;
      Abort;*}


      //function CompileHLSLAndParse(const SourceName, ShaderSource, Profile, EntryPoint: AnsiString;
 // const CurrentDir: string; var Output: TDFXHLSLCompilerOutput; var CSO: ID3DBlob): Boolean;


      // Create initial output directory
      GState.OutputDir := Project.ExpandedOutputDir;
      GState.BuildDir := GState.OutputDir + 'dfx' + PathDelim;

      if DirectoryExists(GState.BuildDir) then
        DeleteDirectory(GState.BuildDir);

      ForceDirectories(GState.BuildDir);
      SetCurrentDir(GState.BuildDir);

      // Compile top-down
      GState.Handler.OnLog(StringOfChar('-', 50));
      {$IFDEF DEBUG}
      GState.Handler.OnLog('DFX Build 1.0 (Debug Build)' );
      {$ELSE}
      GState.Handler.OnLog('DFX Build 1.0' );
      {$ENDIF}
      GState.Handler.OnLog(Format('Building "%s"...', [Project.Name]));
      GState.Handler.OnLog(StringOfChar('-', 50));

      GState.Handler.OnStatus('Pre-processing...');

      // Load CPP translator
      if GCPPScriptPath.Length > 0 then
        GState.CppTranslator := TDFXCppTranslator.Create
      else
        GState.CppTranslator := nil;

      // Fill in shader inclusions from ALL namespaces
      // Inclusion order is changed later using compiler feedback
      FillShaderInclusions;

      CompileDFXConstants;

      ns := Project.RootNamespace;
      CompileDFXNamespace(ns);

      // Write out CPP stuff
      if Assigned(GState.CppTranslator) then
      begin
        SaveCppHeaders;
        SaveCppStartupAndShutdown;
      end;

      // Compile all shaders
      GState.Handler.OnLog('Compiling shaders...');
      GState.Handler.OnBeginCompileShaders;
      CompileDFXShaders(ns);

      // Compile all effects
      GState.Handler.OnBeginBuildEffects;
      CompileDFXEffects(ns);

      GState.Handler.OnLog(StringOfChar('-', 50));
      GState.Handler.OnLog('Done.');
    except
      GState.Handler.OnLog(StringOfChar('-', 50));
      GState.Handler.OnLog('Compilation aborted.');
    end;
  finally
    duration := Time - startTime;
    GState.Handler.OnLog('Total build time: ' + TimeToStr(duration));
    GState.CppTranslator.Free;
    GState.ShaderInclusions.Free;
  end;
end;

end.
