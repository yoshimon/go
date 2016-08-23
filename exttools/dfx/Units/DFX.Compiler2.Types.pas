unit DFX.Compiler2.Types;

interface

uses
  DFX.Project, Lua, Classes, Generics.Collections, Generics.Defaults, SysUtils;

type
  IDFXCompilerHandler = interface
    procedure OnError(const S: string);
    procedure OnLog(const S: string);
    function OnProgress: Boolean;
    procedure OnBeginBuildEffects;
    procedure OnBeginCompileShaders;
    procedure OnStatus(const Msg: string);
  end;

  TDFXCppTranslator = class;

  TDFXCompilerState = record
    Project: TDFXProject;
    Effect: TDFXEffect;
    OutputDir, BuildDir: string;
    Handler: IDFXCompilerHandler;
    ShaderInclusions: TStringList;
    CppTranslator: TDFXCppTranslator;
  end;

  PDFXCompilerState = ^TDFXCompilerState;

  TDFXCppTranslator = class
  private
    FLua: TLua;
    L: TLuaState;

    function LuaQuery(const FuncName: AnsiString; Args: array of AnsiString): string;
  published
    // Auto-registered
    function GetCurrentNamespace(L: TLuaState): Integer;
  public
    CurrentNamespace: string;

    constructor Create;
    destructor Destroy; override;

    function IsOpen: Boolean;

    function UPrecompiledHeader: string;
    function UShaderClass: string;
    function UConstantBufferClass: string;
    function UResourceClass(const HLSLType: string): string;
    function UForwardDeclareType(const TypeName: string): string;
    function ULoadShader(const Obj, FileName, Profile: string): string;
    function UUnloadShader(const Obj: string): string;
    function UBindResource(const Shader, Obj: string; Slot, ResourceIndex: Integer): string;
    function UBindSampler(const Shader, Obj: string; Slot: Integer): string;
    function UBindConstants(const Shader, Obj: string; Slot: Integer): string;
    function UBindShader(const Shader, Obj: string): string;
    function UBindUAV(const Shader, Obj: string; Slot, ResourceIndex, InitialCount: Integer): string;
    function UCompiledShaderFileName(const FileName: string; const Profile: AnsiString): string;
    function UCppCompiledShaderFileName(const FileName: string; const Profile: AnsiString): string;
    function UIncludeUserTypes: string;
  end;

  TDFXHLSLCompilerOutput = record
    ErrorCode: Integer;
    FileName: string;
    Line: Integer;
    Msg: string;
  end;

  TDFXNamespaceStringList= class(TStringList)
  private
    FNamespace: TDFXLeafNamespace;
    FIndent: Integer;
  public
    property Namespace: TDFXLeafNamespace read FNamespace write FNamespace;

    constructor Create(const Namespace: TDFXLeafNamespace);

    function Add(const S: string): Integer; override;
    procedure Indent;
    procedure Unindent;
    procedure BeginNamespace;
    procedure EndNamespace;

    procedure SaveToFile(const FileName: string); override;
  end;

  TDFXThreadGroupSize = record
    X, Y, Z: Integer;
  end;

  TDFXResourceCategory = (rcSamplerState, rcBuffer, rcUAV);

  TDFXResourceBindDesc = record
    BindPoint, BindCount: Integer;
    Name: string;
    Obj: TObject;
  end;
  PDFXResourceBindDesc = ^TDFXResourceBindDesc;

  TDFXResourceBindDescComparer = class(TComparer<TDFXResourceBindDesc>)
  public
    function Compare(const A, B: TDFXResourceBindDesc): Integer; override;
  end;

  TDFXShaderReflectionData = class
  public
    Textures: TList<TDFXResourceBindDesc>;
    UAVs: TList<TDFXResourceBindDesc>;
    SamplerStates: TList<TDFXResourceBindDesc>;
    ConstantBuffers: TList<TDFXResourceBindDesc>;
    ThreadGroupSize: TDFXThreadGroupSize;

    constructor Create;
    destructor Destroy; override;

    procedure Reset;
  end;

  TDFXPassReflectionData = record
    VS: TDFXShaderReflectionData;
    PS: TDFXShaderReflectionData;
    CS: TDFXShaderReflectionData;
    GS: TDFXShaderReflectionData;
  end;

  TDFXEffectReflectionData = TList<TDFXPassReflectionData>;

  TDFXResourceGroup = record
    BindPoint: Integer;
    BindCount: Integer;
    ResVarNames: TStringList;
  end;

{$IFDEF CPUX86}
  const LUALIBRARY = 'lua52-32.dll';
{$ELSE}
  const LUALIBRARY = 'lua52-64.dll';
{$ENDIF}

var
  GState: TDFXCompilerState;
  GCppScriptPath: string = '';

procedure RaiseAndLog(const Error: string);
procedure LuaRaiseAndLog(const Error: string);

implementation

var
  GIndentSize: Integer = 4;

procedure RaiseAndLog(const Error: string); overload;
begin
  GState.Handler.OnError(Error);
  raise Exception.Create(Error);
end;

procedure LuaRaiseAndLog(const Error: string); overload;
begin
  RaiseAndLog('CppTranslator: ' + Error + '.');
end;

{ TDFXNamespaceStringList }

function TDFXNamespaceStringList.Add(const S: string): Integer;
var
  sl: TStringList;
  l: string;
begin
  if S.Length = 0 then
    Exit(inherited Add(S));

  sl := TStringList.Create;

  try
    sl.Text := S;

    Result := 0;
    for l in sl do
      Result := inherited Add(StringOfChar(' ', FIndent) + l);
  finally
    sl.Free;
  end;
end;

procedure TDFXNamespaceStringList.BeginNamespace;
var
  n: TDFXLeafNamespace;
  l: TList<TDFXLeafNamespace>;
  i: Integer;
begin
  if not Assigned(FNamespace.Parent) then
    Exit;

  l := TList<TDFXLeafNamespace>.Create;

  try
    n := FNamespace;
    while Assigned(n.Parent) do
    begin
      l.Insert(0, n);
      n := n.Parent;
    end;

    i := FIndent;

    for n in l do
    begin
      FIndent := i + (n.Depth - 1) * GIndentSize;
      Add('namespace ' + n.Name);
      Add('{');
    end;

    Indent;
  finally
    l.Free;
  end;
end;

constructor TDFXNamespaceStringList.Create(const Namespace: TDFXLeafNamespace);
begin
  inherited Create;
  FNamespace := Namespace;
  FIndent := 0;
end;

procedure TDFXNamespaceStringList.EndNamespace;
var
  n: TDFXLeafNamespace;
  i: Integer;
begin
  if not Assigned(FNamespace.Parent) then
    Exit;

  Unindent;
  i := FIndent;

  n := FNamespace;
  while Assigned(n.Parent) do
  begin
    FIndent := i + (n.Depth - 1) * GIndentSize;
    Add('}');
    n := n.Parent;
  end;
end;

procedure TDFXNamespaceStringList.Indent;
begin
  FIndent := FIndent + GIndentSize;
end;

procedure TDFXNamespaceStringList.SaveToFile(const FileName: string);
begin
  ForceDirectories(MakeAbsolutePath(GetCurrentDir, ExtractFilePath(FileName)));
  inherited;
end;

procedure TDFXNamespaceStringList.Unindent;
begin
  FIndent := FIndent - GIndentSize;
end;

{ TDFXCppTranslator }

constructor TDFXCppTranslator.Create;
begin
  if not lua.libLoaded then
    if not lua.LoadLuaLibrary(ExtractFilePath(ParamStr(0)) + LUALIBRARY) then
      LuaRaiseAndLog('Could not load lua library.');

  if GCPPScriptPath.Length > 0 then
  begin
    FLua := TLua.Create;
    L := FLua.LuaInstance;
    lua.luaL_openlibs(L);

    if FLua.DoFile(GCPPScriptPath) <> 0 then
      LuaRaiseAndLog('"' + GCPPScriptPath + '" does not point to a valid C++-translator.');
  end;
end;

destructor TDFXCppTranslator.Destroy;
begin
  FLua.Free;
  inherited;
end;

function TDFXCppTranslator.GetCurrentNamespace(L: TLuaState): Integer;
var
  numArgs: Integer;
begin
  numArgs := lua_gettop(L);
  lua_pop(L, numArgs);

  lua_pushstring(L, PAnsiChar(AnsiString(CurrentNamespace)));
  Result := 1;
end;

function TDFXCppTranslator.IsOpen: Boolean;
begin
  Result := Assigned(L);
end;

function TDFXCppTranslator.LuaQuery(const FuncName: AnsiString;
  Args: array of AnsiString): string;
var
  i: Integer;
begin
  // Push func
  lua_getglobal(L, PAnsiChar(FuncName));

  // Push args
  for i := Low(Args) to High(Args) do
    lua_pushstring(L, PAnsiChar(Args[i]));

  // Call
  if lua_pcall(L, Length(Args), 1, 0) = 0 then
    Result := string(lua_tostring(L, -1))
  else
    LuaRaiseAndLog(string(lua_tostring(L, -1)));
end;

function TDFXCppTranslator.UUnloadShader(const Obj: string): string;
begin
  Result := LuaQuery('unload_shader', [Obj]);
end;

function TDFXCppTranslator.UCompiledShaderFileName(
  const FileName: string; const Profile: AnsiString): string;
begin
  Result := LuaQuery('compiled_shader_filename', [FileName, Profile]);
end;

function TDFXCppTranslator.UConstantBufferClass: string;
begin
  Result := LuaQuery('cbuffer_class', []);
end;

function TDFXCppTranslator.UCppCompiledShaderFileName(const FileName: string;
  const Profile: AnsiString): string;
begin
  Result := LuaQuery('cpp_compiled_shader_filename', [FileName, Profile]);
end;

function TDFXCppTranslator.UForwardDeclareType(const TypeName: string): string;
begin
  Result := LuaQuery('forward_declare_type', [TypeName]);
end;

function TDFXCppTranslator.UIncludeUserTypes: string;
begin
  Result := LuaQuery('include_user_types', []);
end;

function TDFXCppTranslator.ULoadShader(const Obj, FileName,
  Profile: string): string;
begin
  Result := LuaQuery('load_shader', [Obj, FileName, Profile]);
end;

function TDFXCppTranslator.UPrecompiledHeader: string;
begin
  Result := LuaQuery('precompiled_header', []);
end;

function TDFXCppTranslator.UBindConstants(const Shader, Obj: string; Slot: Integer): string;
begin
  Result := LuaQuery('bind_constants', [Shader, Obj, IntToStr(Slot)]);
end;

function TDFXCppTranslator.UBindSampler(const Shader, Obj: string; Slot: Integer): string;
begin
  Result := LuaQuery('bind_sampler', [Shader, Obj, IntToStr(Slot)]);
end;

function TDFXCppTranslator.UBindShader(const Shader, Obj: string): string;
begin
  Result := LuaQuery('bind_shader', [Shader, Obj]);
end;

function TDFXCppTranslator.UBindResource(const Shader, Obj: string; Slot, ResourceIndex: Integer): string;
begin
  Result := LuaQuery('bind_resource', [Shader, Obj, IntToStr(Slot), IntToStr(ResourceIndex)]);
end;

function TDFXCppTranslator.UBindUAV(const Shader, Obj: string; Slot, ResourceIndex, InitialCount: Integer): string;
begin
  Result := LuaQuery('bind_uav', [Shader, Obj, IntToStr(Slot), IntToStr(ResourceIndex), IntToStr(InitialCount)]);
end;

function TDFXCppTranslator.UResourceClass(const HLSLType: string): string;
begin
  Result := LuaQuery('resource_class', [HLSLType]);
end;

function TDFXCppTranslator.UShaderClass: string;
begin
  Result := LuaQuery('shader_class', []);
end;

{ TDFXShaderReflectionData }

constructor TDFXShaderReflectionData.Create;
begin
  inherited;
  SamplerStates := TList<TDFXResourceBindDesc>.Create(TDFXResourceBindDescComparer.Default);
  Textures := TList<TDFXResourceBindDesc>.Create(TDFXResourceBindDescComparer.Default);
  UAVs := TList<TDFXResourceBindDesc>.Create(TDFXResourceBindDescComparer.Default);
  ConstantBuffers := TList<TDFXResourceBindDesc>.Create(TDFXResourceBindDescComparer.Default);
end;

destructor TDFXShaderReflectionData.Destroy;
begin
  Textures.Free;
  SamplerStates.Free;
  ConstantBuffers.Free;
  UAVs.Free;
  inherited;
end;

procedure TDFXShaderReflectionData.Reset;
begin
  Textures.Clear;
  SamplerStates.Clear;
  ConstantBuffers.Clear;
  UAVs.Clear;
  ThreadGroupSize.X := 0;
  ThreadGroupSize.Y := 0;
  ThreadGroupSize.Z := 0;
end;

{ TDFXResourceBindDescComparer }

function TDFXResourceBindDescComparer.Compare(const A,
  B: TDFXResourceBindDesc): Integer;
begin
  if A.BindPoint < B.BindPoint then
    Result := -1
  else if A.BindPoint > B.BindPoint then
    Result := 1
  else
    Result := 0;
end;


end.
