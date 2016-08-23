unit DFX.Project;

interface

uses
  Windows, System.SysUtils, SHChangeNotify, Vcl.Forms, Generics.Collections,
  IOUtils, ShellApi, Vcl.Dialogs, System.UITypes, Xml.XMLDoc, Xml.XMLIntf,
  Classes, Math, Rtti, System.TypInfo;

type
  TDFXProject = class;

  TDFXNamespace = class;
  TDFXEffect = class;
  TDFXFunction = class;
  TDFXLeafNamespace = class;
  TDFXConstantBuffer = class;
  TDFXConstant = class;
  TDFXResource = class;
  TDFXStructure = class;
  TDFXTextItem = class;
  TDFXEffectPass = class;

  TDFXHLSLRegister = -1..128;

  IDFXProjectListener = interface
    procedure OnInitializeProject(Project: TDFXProject);
    procedure OnDestroyProject;
    procedure OnProjectModified(Modified: Boolean);

    procedure OnAddNamespace(Parent, NewNamespace: TDFXNamespace);
    procedure OnAddEffect(Effect: TDFXEffect);
    procedure OnAddFunction(Func: TDFXFunction);
    procedure OnAddConstantBuffer(CB: TDFXConstantBuffer);
    procedure OnAddConstant(C: TDFXConstant);
    procedure OnAddResource(Resource: TDFXResource);
    procedure OnAddStructure(S: TDFXStructure);
    procedure OnAddEffectPass(P: TDFXEffectPass);

    procedure OnDeleteNamespace(Namespace: TDFXNamespace);
    procedure OnDeleteEffect(Effect: TDFXEffect);
    procedure OnDeleteFunction(Func: TDFXFunction);
    procedure OnDeleteConstantBuffer(CB: TDFXConstantBuffer);
    procedure OnDeleteResource(Resource: TDFXResource);
    procedure OnDeleteConstant(C: TDFXConstant);
    procedure OnDeleteStructure(S: TDFXStructure);
    procedure OnDeleteEffectPass(P: TDFXEffectPass);

    function OnCanDeleteNamespace(N: TDFXNamespace): Boolean;
    function OnCanDeleteEffect(Effect: TDFXEffect): Boolean;
    function OnCanDeleteFunction(Func: TDFXFunction): Boolean;
    function OnCanDeleteConstantBuffer(CB: TDFXConstantBuffer): Boolean;
    function OnCanDeleteConstant(C: TDFXConstant): Boolean;
    function OnCanDeleteResource(Resource: TDFXResource): Boolean;
    function OnCanDeleteStructure(S: TDFXStructure): Boolean;
    function OnCanDeleteEffectPass(P: TDFXEffectPass): Boolean;

    procedure OnRenameNamespace(Namespace: TDFXNamespace);
    procedure OnRenameEffect(Effect: TDFXEffect);
    procedure OnRenameFunction(Func: TDFXFunction);
    procedure OnRenameConstantBuffer(CB: TDFXConstantBuffer);
    procedure OnRenameResource(Resource: TDFXResource);
    procedure OnRenameConstant(C: TDFXConstant);
    procedure OnRenameStructure(S: TDFXStructure);
    procedure OnRenameEffectPass(P: TDFXEffectPass);
  end;

  TDFXShaderFunction = record
    Reference: string;
    Attributes: string;
  end;

  PDFXShaderFunction = ^TDFXShaderFunction;

  TDFXTextItem = class
  private
    procedure SetName(const Value: string); virtual; abstract;
  protected
    FName: string;
    FText: string;
  public
    property Text: string read FText write FText;
  published
    property Name: string read FName write SetName;
  end;

  TDFXNamespacedItem = class
  private
    procedure SetName(const Value: string); virtual; abstract;
  protected
    FName: string;
    FNamespace: TDFXLeafNamespace;
  public
    property Namespace: TDFXLeafNamespace read FNamespace;

    constructor Create(Namespace: TDFXLeafNamespace);

    function FilePath(const Delimiter: string = PathDelim): string;
    function DiskPath(Delim: Char = PathDelim): string; virtual; abstract;
    function RelativeDiskPath: string;
  published
    property Name: string read FName write SetName;
  end;

  TDFXNamespacedTextItem = class(TDFXTextItem)
  protected
    FNamespace: TDFXLeafNamespace;
  public
    property Namespace: TDFXLeafNamespace read FNamespace;

    constructor Create(Namespace: TDFXLeafNamespace);

    function FilePath(const Delimiter: string = PathDelim): string;
    function DiskPath(Delim: Char = PathDelim): string; virtual; abstract;
    function RelativeDiskPath: string;
  end;

  TDFXEffectPassType = (ptCompute, ptGeometry);

  TDFXEffectPass = class
  private
    FTitle: string;
    FVertexShader,
    FPixelShader,
    FComputeShader,
    FGeometryShader: TDFXShaderFunction;
    FPassType: TDFXEffectPassType;
  public
    property Title: string read FTitle write FTitle;
    property VertexShader: TDFXShaderFunction read FVertexShader write FVertexShader;
    property PixelShader: TDFXShaderFunction read FPixelShader write FPixelShader;
    property ComputeShader: TDFXShaderFunction read FComputeShader write FComputeShader;
    property GeometryShader: TDFXShaderFunction read FGeometryShader write FGeometryShader;
    property PassType: TDFXEffectPassType read FPassType write FPassType;

    constructor Create(const Title: string);
  end;

  TDFXFunction = class(TDFXNamespacedTextItem)
  private
    FShaderProfile: AnsiString;

    procedure SetName(const Value: string); override;
    procedure SetShaderProfile(const Value: AnsiString);
  public
    function DiskPath(Delim: Char = PathDelim): string;  override;
  published
    property ShaderProfile: AnsiString read FShaderProfile write SetShaderProfile;
  end;

  TDFXConstantBufferType = (cbtConstant, cbtTexture);

  TDFXConstantBuffer = class(TDFXNamespacedTextItem)
  private
    FBufferType: TDFXConstantBufferType;
    FRegister: TDFXHLSLRegister;

    procedure SetName(const Value: string); override;
  public
    constructor Create(Namespace: TDFXLeafNamespace);

    function DiskPath(Delim: Char = PathDelim): string; override;
  published
    property BufferType: TDFXConstantBufferType read FBufferType write FBufferType;
    property ForceRegister: TDFXHLSLRegister read FRegister write FRegister;
  end;

  TDFXLeafNamespace = class abstract
  protected
    FFunctions: TList<TDFXFunction>;
    FConstantBuffers: TList<TDFXConstantBuffer>;
    FResources: TList<TDFXResource>;
    FStructures: TList<TDFXStructure>;
    FName: string;
    FParent: TDFXNamespace;
    FProject: TDFXProject;
  public
    property Functions: TList<TDFXFunction> read FFunctions;
    property ConstantBuffers: TList<TDFXConstantBuffer> read FConstantBuffers;
    property Resources: TList<TDFXResource> read FResources;
    property Structures: TList<TDFXStructure> read FStructures;
    property Name: string read FName;
    property Parent: TDFXNamespace read FParent;
    property Project: TDFXProject read FProject;

    constructor Create(Project: TDFXProject; Parent: TDFXNamespace);
    destructor Destroy; override;

    function FilePath(const Delimiter: string = PathDelim): string;
    function DiskPath(Delim: Char = PathDelim): string; virtual; abstract;
    function RelativeDiskPath: string;
    function Depth: Integer;

    function FindGlobalFunctionReference(const Name: string): TDFXFunction;
    function FindGlobalResourceReference(const Name: string): TDFXResource;
    function FindGlobalConstantBufferReference(const Name: string): TDFXConstantBuffer;

    function FindResourceReference(const Name: string): TDFXResource;
    function FindConstantBufferReference(const Name: string): TDFXConstantBuffer;

    function FindFunction(const Name: string): TDFXFunction;
    function FindConstantBuffer(const Name: string): TDFXConstantBuffer;
    function FindResource(const Name: string): TDFXResource;
    function FindStructure(const Name: string): TDFXStructure;

    function CreateFunction(const Name: string): TDFXFunction;
    procedure RenameFunction(Func: TDFXFunction; const Name: string);
    procedure DeleteFunction(Func: TDFXFunction);
    function MoveFunction(Func: TDFXFunction): Boolean;

    function CreateConstantBuffer(const Name: string): TDFXConstantBuffer;
    procedure RenameConstantBuffer(CB: TDFXConstantBuffer; const Name: string);
    procedure DeleteConstantBuffer(CB: TDFXConstantBuffer);
    function MoveConstantBuffer(CB: TDFXConstantBuffer): Boolean;

    function CreateResource(const Name: string): TDFXResource;
    procedure RenameResource(Resource: TDFXResource; const Name: string);
    procedure DeleteResource(Resource: TDFXResource);
    function MoveResource(Resource: TDFXResource): Boolean;

    function CreateStructure(const Name: string): TDFXStructure;
    procedure RenameStructure(Structure: TDFXStructure; const Name: string);
    procedure DeleteStructure(Structure: TDFXStructure);
    function MoveStructure(Structure: TDFXStructure): Boolean;
  end;

  TDFXNamespace = class(TDFXLeafNamespace)
  private
    FSubnamespaces: TList<TDFXNamespace>;
    FEffects: TList<TDFXEffect>;

    function FindGlobalNamespaceReference(const Name: string; Namespace: TDFXNamespace; I: Integer = 1): TDFXLeafNamespace;
  public
    property Effects: TList<TDFXEffect> read FEffects;
    property Subnamespaces: TList<TDFXNamespace> read FSubnamespaces;

    constructor Create(Project: TDFXProject; Parent: TDFXNamespace);
    destructor Destroy; override;

    function DiskPath(Delim: Char = PathDelim): string; override;

    function FindGlobalFunctionReference(const Name: string): TDFXFunction;
    function FindGlobalResourceReference(const Name: string): TDFXResource;
    function FindGlobalConstantBufferReference(const Name: string): TDFXConstantBuffer;

    function FindNamespace(const Name: string): TDFXNamespace;
    function FindEffect(const Name: string): TDFXEffect;

    function MoveNamespace(Namespace: TDFXNamespace): Boolean;

    function CreateEffect(const Name: string): TDFXEffect;
    procedure RenameEffect(Effect: TDFXEffect; const Name: string);
    procedure DeleteEffect(Effect: TDFXEffect);
    function MoveEffect(Effect: TDFXEffect): Boolean;
  end;

  TDFXEffect = class(TDFXLeafNamespace)
  private
    FPasses: TList<TDFXEffectPass>;
    FParticipateInBuild: Boolean;
    procedure SetParticipateInBuild(const Value: Boolean);
  public
    property Passes: TList<TDFXEffectPass> read FPasses;

    constructor Create(Namespace: TDFXNamespace);
    destructor Destroy; override;

    function DiskPath(Delim: Char = PathDelim): string; override;

    function CreatePass(const Title: string): TDFXEffectPass;
    procedure RenameEffectPass(P: TDFXEffectPass; const Title: string);
    procedure DeleteEffectPass(P: TDFXEffectPass);
    procedure MovePass(OldIndex, NewIndex: Integer);
  published
    property ParticipateInBuild: Boolean read FParticipateInBuild write SetParticipateInBuild;
  end;

  TDFXCreateMode = (cmOpen, cmNew);

  TDFXConstant = class(TDFXTextItem)
  private
    procedure SetName(const Value: string); override;
  end;

  TDFXResourceType = (
    rtAppendStructuredBuffer,
    rtBuffer,
    rtByteAddressBuffer,
    rtConsumeStructuredBuffer,
    rtStructuredBuffer,
    rtTexture1D,
    rtTexture1DArray,
    rtTexture2D,
    rtTexture2DArray,
    rtTexture3D,
    rtTextureCube,
    rtTextureCubeArray,
    rtSamplerState,
    rtSamplerComparisonState
  );

  TDFXSampleCount = 1..127;

  TDFXGPUAccessMode = (amRead, amWrite, amReadWrite);

  TDFXResource = class(TDFXNamespacedItem)
  private
    FComponentType: string;
    FHLSLType: TDFXResourceType;
    FGPUAccessMode: TDFXGPUAccessMode;
    FSampleCount: TDFXSampleCount;
    FSemantic: string;
    FUAVCount: Integer;
    FRegister: TDFXHLSLRegister;
    FSubresourceIndex: Integer;

    function SupportsUAVCounter: Boolean;

    procedure SetName(const Value: string); override;
    procedure SetSampleCount(const Value: TDFXSampleCount);
    procedure SetHLSLType(const Value: TDFXResourceType);
    procedure SetComponentType(const Value: string);
    procedure SetGPUAccessMode(const Value: TDFXGPUAccessMode);
    procedure SetSemantic(const Value: string);
    function GetUAVInitialCount: string;
    procedure SetUAVInitialCount(const Value: string);
  public
    function DiskPath(Delim: Char = PathDelim): string; override;

    constructor Create(Namespace: TDFXLeafNamespace);

    function GetHLSLTypeString: string;
    function IsTexture: Boolean;
    function MustInitialize: Boolean;
    function IsSamplerStateType: Boolean;
  published
    property ComponentType: string read FComponentType write SetComponentType;
    property HLSLType: TDFXResourceType read FHLSLType write SetHLSLType;
    property GPUAccessMode: TDFXGPUAccessMode read FGPUAccessMode write SetGPUAccessMode;
    property SampleCount: TDFXSampleCount read FSampleCount write SetSampleCount;
    property Semantic: string read FSemantic write SetSemantic;
    property UAVInitialCount: string read GetUAVInitialCount write SetUAVInitialCount;
    property ForceRegister: TDFXHLSLRegister read FRegister write FRegister;
    property SubresourceIndex: Integer read FSubresourceIndex write FSubresourceIndex;
  end;

  TDFXStructure = class(TDFXNamespacedTextItem)
  private
    procedure SetName(const Value: string); override;
  public
    function DiskPath(Delim: Char = PathDelim): string; override;
  end;

  TDFXProject = class
  private
    FName: string;
    FRootDir: string;
    FOutputDir: string;
    FShaderDir: string;
    FPreamble: string;
    FListener: IDFXProjectListener;
    FRootNamespace: TDFXNamespace;
    FConstants: TList<TDFXConstant>;
    FModified: Boolean;
    FMinimizeShaders: Boolean;
    FHLSLCompilerFlags: Cardinal;

    // Only allowed in Create
    procedure LoadFromRootDirectory;

    procedure SetRootDir(const NewValue: string);
    procedure SetPreamble(const Value: string);
    procedure SetModified(const Value: Boolean);
    procedure SetOutputDir(const Value: string);
    procedure SetShaderDir(const Value: string);
    procedure SetMinimizeShaders(const Value: Boolean);
    procedure SetHLSLCompilerFlags(const Value: Cardinal);
  public
    property Name: string read FName;
    property RootDir: string read FRootDir write SetRootDir;
    property RootNamespace: TDFXNamespace read FRootNamespace;
    property Listener: IDFXProjectListener read FListener;
    property Constants: TList<TDFXConstant> read FConstants;
    property Preamble: string read FPreamble write SetPreamble;
    property Modified: Boolean read FModified write SetModified;

    constructor Create(Listener: IDFXProjectListener; Directory: string; Mode: TDFXCreateMode);
    destructor Destroy; override;

    procedure Save(Directory: string = '');
    function HasValidRootDirectory: Boolean;

    function ExpandedRootDir: string;
    function ExpandedOutputDir: string;

    function AddNamespace(Parent: TDFXNamespace; const Name: string): TDFXNamespace;
    procedure RenameNamespace(Namespace: TDFXNamespace; const Name: string);
    procedure DeleteNamespace(Namespace: TDFXNamespace);

    function AddConstant(const Description: string): TDFXConstant;
    procedure DeleteConstant(Constant: TDFXConstant);
    procedure RenameConstant(Constant: TDFXConstant; const Description: string);
  published
    property ShaderDir: string read FShaderDir write SetShaderDir;
    property MinimizeShaders: Boolean read FMinimizeShaders write SetMinimizeShaders;
    property OutputDir: string read FOutputDir write SetOutputDir;
    property HLSLCompilerFlags: Cardinal read FHLSLCompilerFlags write SetHLSLCompilerFlags;
  end;

function ExtractLastDirectory(Directory: string): string;
function ExecuteProgram(const FileName, Parameters: string): DWORD;
function MakeRelativePath(const BasePath, Path: string): string;
function MakeAbsolutePath(const BasePath, Path: string): string;
function IsIdentifier(const S: string): Boolean;
function ExpandEnvStrings(const AString: String): String;
function IsAlphaNum(C: Char): Boolean;
procedure SaveText(const FileName, S: string; const Preamble: string = '');
procedure LoadText(const FileName: string; var S: string);
procedure DeleteDirectory(const DirName: string);
function LastPos(const SubStr: String; const S: String): Integer;


function PathRelativePathTo(pszPath: PChar; pszFrom: PChar; dwAttrFrom: DWORD;
  pszTo: PChar; dwAtrTo: DWORD): LongBool; stdcall; external 'shlwapi.dll' name {$IFDEF UNICODE}'PathRelativePathToW'{$ELSE}'PathRelativePathToA'{$ENDIF};
function PathCanonicalize(lpszDst: PChar; lpszSrc: PChar): LongBool; stdcall;
  external 'shlwapi.dll' name {$IFDEF UNICODE}'PathCanonicalizeW'{$ELSE}'PathCanonicalizeW'{$ENDIF};
function PathCombine(lpszDest: PChar; const lpszDir, lpszFile: PChar):
  PChar; stdcall; external 'shlwapi.dll' name {$IFDEF UNICODE}'PathCombineW'{$ELSE}'PathCombineA'{$ENDIF};

implementation

uses
  MainWindow;

procedure DeserializeProperty(I: Pointer; P: TRttiProperty; s: string);
var
  v: TValue;
  pt: PTypeInfo;
  ev: Integer;
begin
  pt := p.GetValue(I).TypeInfo;
  case p.PropertyType.TypeKind of
    tkInteger: v := StrToInt(s);
    tkFloat: v := StrToFloat(s);
    tkChar,
    tkLString,
    tkWChar,
    tkUString,
    tkString: v := s;
    tkSet:
      begin
        try
          ev := StringToSet(pt, s);
          TValue.Make(@ev, pt, v);
        except
          MainForm.Log(Format('[WARNING] Invalid set value "%s" used in property "%s".', [s, P.Name]));
          Exit;
        end;
      end;
    tkEnumeration:
      begin
        ev := GetEnumValue(pt, s);
        if ev = -1 then
        begin
          MainForm.Log(Format('[WARNING] Unknown property value "%s" used in property "%s".', [s, P.Name]));
          Exit;
        end;

        v := TValue.FromOrdinal(pt, ev);
      end;
  end;
  p.SetValue(I, v);
end;

procedure DeleteDirectory(const DirName: string);
var
  fileOp: TSHFileOpStruct;
begin
  FillChar(FileOp, SizeOf(fileOp), 0);
  fileOp.wFunc := FO_DELETE;
  fileOp.pFrom := PChar(DirName + #0);
  fileOp.fFlags := FOF_SILENT or FOF_NOERRORUI or FOF_NOCONFIRMATION;
  SHFileOperation(fileOp);
end;

procedure SaveText(const FileName, S: string; const Preamble: string);
var
  sl: TStringList;
  fn: string;
begin
  fn := MakeAbsolutePath(GetCurrentDir, ExtractFilePath(FileName));
  ForceDirectories(fn);
  fn := fn + ExtractFileName(FileName);

  sl := TStringList.Create;

  try
    if Preamble.Length > 0 then
      sl.Text := Preamble;

    sl.Text := sl.Text + S;
    sl.SaveToFile(fn);
  finally
    sl.Free;
  end;
end;

procedure LoadText(const FileName: string; var S: string);
var
  sl: TStringList;
begin
  sl := TStringList.Create;

  try
    sl.LoadFromFile(FileName);
    S := sl.Text;
  finally
    sl.Free;
  end;
end;

function LastPos(const SubStr: String; const S: String): Integer;
var
  Found, Len, Pos: integer;
begin
  Pos := Length(S);
  Len := Length(SubStr);
  Found := 0;
  while (Pos > 0) and (Found = 0) do
  begin
    if Copy(S, Pos, Len) = SubStr then
      Found := Pos;
    Dec(Pos);
  end;
  LastPos := Found;
end;

function ExpandEnvStrings(const AString: String): String;
var
  s: Integer;
begin
  s := ExpandEnvironmentStrings(PChar(AString), nil, 0);
  SetLength(result, s);
  ExpandEnvironmentStrings(PChar(AString), PChar(result), s);
  Result := TrimRight(Result);
end;

function IsAlphaNum(C: Char): Boolean;
begin
  Result := CharInSet(C, ['a'..'z', 'A'..'Z', '0'..'9']);
end;

function IsIdentifier(const S: string): Boolean;
var
  c: Char;
begin
  if S.Length = 0 then
    Exit(False);

  if CharInSet(S[1], ['0'..'9']) then
    Exit(False);

  for c in S do
  begin
    if (not IsAlphaNum(c)) and (c <> '_') then
      Exit(False);
  end;

  Result := True;
end;

function MakeRelativePath(const BasePath, Path: string): string;
var
  p: array[0..MAX_PATH-1] of Char;
begin
  if PathRelativePathTo(@p[0], PChar(BasePath), FILE_ATTRIBUTE_DIRECTORY, PChar(Path), 0) then
    Result := p
  else
    Result := Path;
end;

function MakeAbsolutePath(const BasePath, Path: string): string;
var
  p: array[0..MAX_PATH-1] of Char;
begin
  if Pos(BasePath, Path) = 1 then
    Exit(Path);

  PathCanonicalize(@p[0], PChar(IncludeTrailingPathDelimiter(BasePath) + Path));
  result := p;
end;

function ExecuteProgram(const FileName, Parameters: string): DWORD;
 var
  sei: TShellExecuteInfo;
begin
  FillChar(sei, SizeOf(sei), 0);
  sei.cbSize := SizeOf(sei);
  with sei do
  begin
    fMask := SEE_MASK_NOCLOSEPROCESS;
    Wnd := Application.Handle;
    lpFile := PChar(FileName);
    lpParameters := PChar(Parameters);
    nShow := SW_SHOWNORMAL;
  end;

  if ShellExecuteEx(@sei) then
  begin
    repeat
      Sleep(1);
      Application.ProcessMessages;
      GetExitCodeProcess(sei.hProcess, Result);
    until (ExitCode <> STILL_ACTIVE) or Application.Terminated;
  end
  else
    Result := DWORD(-1);

  if Result = 0 then
    MainForm.ShowError(Format(ExtractFileName(FileName) + ' terminated with errorcode %d.', [Result]));
end;

function AddUniqueString(Container: TStringList; Element: string): Boolean;
var
  s: string;
begin
  for s in Container do
    if s = Element then
      Exit(False);

  Container.Add(Element);
  Result := True;
end;

function ExtractLastDirectory(Directory: string): string;
var
  i: Integer;
begin
  Directory := ExcludeTrailingPathDelimiter(Directory);
  i := LastDelimiter(PathDelim, Directory);
  if i > 0 then
    Result := Copy(Directory, i + 1, Directory.Length)
  else
    Result := Directory;
end;

function TDFXProject.AddConstant(const Description: string): TDFXConstant;
begin
  Result := TDFXConstant.Create;
  Result.FName := Description;
  FConstants.Add(Result);

  Modified := True;
  Listener.OnAddConstant(Result);
end;

function TDFXProject.AddNamespace(Parent: TDFXNamespace; const Name: string): TDFXNamespace;
begin
  if not Assigned(Parent) then
    Parent := RootNamespace;

  Result := TDFXNamespace.Create(Self, Parent);
  try
    RenameNamespace(Result, Name);
  except
    on e: Exception do
    begin
      Result.Free;
      raise;
    end;
  end;
  Parent.Subnamespaces.Add(Result);

  Modified := True;
  FListener.OnAddNamespace(Parent, Result);
end;

constructor TDFXProject.Create(Listener: IDFXProjectListener;
  Directory: string; Mode: TDFXCreateMode);
begin
  Assert(Assigned(Listener));

  Directory := Trim(ExpandEnvStrings(Directory));

  // Initialize directories
  RootDir := Directory;
  OutputDir := RootDir + 'output' + PathDelim;

  FConstants := TList<TDFXConstant>.Create;

  FHLSLCompilerFlags := 0;

  // Register listener
  FListener := Listener;

  // Initialize root namespace
  FRootNamespace := TDFXNamespace.Create(Self, nil);
  FRootNamespace.FName := 'Global';

  // Create project directories
  if Mode = cmNew then
  begin
    if not ForceDirectories(ExpandedRootDir) then
      raise Exception.Create('Could not create project directory.');
  end;

  // Verify project directories
  if not DirectoryExists(ExpandedRootDir) then
    raise Exception.Create('Invalid project directory.');

  // Notify listener
  FListener.OnInitializeProject(Self);
  FListener.OnAddNamespace(nil, FRootNamespace);

  if Mode = cmOpen then
    LoadFromRootDirectory
  else
    Save;

  Modified := False;
end;

procedure TDFXProject.DeleteNamespace(Namespace: TDFXNamespace);
begin
  if not Assigned(Namespace) or not Assigned(Namespace.Parent) then
    Exit;

  if Listener.OnCanDeleteNamespace(Namespace) then
    if Namespace.Parent.Subnamespaces.Remove(Namespace) <> -1 then
    begin
      Listener.OnDeleteNamespace(Namespace);
      Namespace.Free;
    end;
end;

destructor TDFXProject.Destroy;
begin
  FListener.OnDestroyProject;
  FRootNamespace.Free;
  FConstants.Free;
end;

function TDFXProject.ExpandedOutputDir: string;
begin
  Result := IncludeTrailingPathDelimiter(ExpandEnvStrings(OutputDir));
end;

function TDFXProject.ExpandedRootDir: string;
begin
  Result := IncludeTrailingPathDelimiter(ExpandEnvStrings(RootDir));
end;

function TDFXProject.HasValidRootDirectory: Boolean;
begin
  Result := DirectoryExists(ExpandedRootDir);
end;

function MatchChildNode(N: IXMLNode; I: Integer; const NodeName: string): IXMLNode;
begin
  Result := N.ChildNodes[i];
  if Result.NodeName <> NodeName then
    raise Exception.Create('Invalid project file.');
end;

function TryFindChild(N: IXMLNode; const NodeName: string; var OutNode: IXMLNode): Boolean;
var
  c: IXMLNode;
begin
  c := N.ChildNodes.First;
  while Assigned(c) do
  begin
    if c.NodeName = NodeName then
    begin
      OutNode := c;
      Exit(True);
    end;

    c := c.NextSibling;
  end;

  Result := False;
end;

procedure LoadProperties(N: IXMLNode; C: TClass; I: Pointer);
var
  rContext: TRttiContext;
  rType: TRttiType;
  rProp: TRttiProperty;
  cn: IXMLNode;
begin
  rType := rContext.GetType(C);
  for rProp in rType.GetProperties do
    if (rProp.IsWritable) and (rProp.Visibility = mvPublished) then
    begin
      cn := N.AttributeNodes.FindNode(LowerCase(rProp.Name));
      if Assigned(cn) then
        DeserializeProperty(I, rProp, cn.Text);
    end;
end;

procedure TDFXProject.LoadFromRootDirectory;
var
  doc: IXMLDocument;
  fileDir: TStringList;

  procedure LoadLeafNamespace(N: IXMLNode; NS: TDFXLeafNamespace);
  var
    c, c0: IXMLNode;
    i, folderIdx: Integer;
    ti: TDFXTextItem;
    r: TDFXResource;
  begin
    // <cbuffers>
    if TryFindChild(N, 'cbuffers', c) then
    begin
      folderIdx := fileDir.Add('cbuffers');
      for i := 0 to c.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(c, i, 'cbuffer');
        ti := NS.CreateConstantBuffer(c0.Attributes['name']);
        LoadProperties(c0, TDFXConstantBuffer, ti);
        LoadText(fileDir.DelimitedText + PathDelim + c0.Attributes['name'], ti.FText);
      end;
      fileDir.Delete(folderIdx);
    end;

    // <function>
    if TryFindChild(N, 'functions', c) then
    begin
      folderIdx := fileDir.Add('functions');
      for i := 0 to c.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(c, i, 'function');
        ti := NS.CreateFunction(c0.Attributes['name']);
        LoadProperties(c0, TDFXFunction, ti);
        LoadText(fileDir.DelimitedText + PathDelim + c0.Attributes['name'], ti.FText);
      end;
      fileDir.Delete(folderIdx);
    end;

    // <resource>
    if TryFindChild(N, 'resources', c) then
    begin
      folderIdx := fileDir.Add('resources');
      for i := 0 to c.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(c, i, 'resource');
        r := NS.CreateResource(c0.Attributes['name']);
        LoadProperties(c0, TDFXResource, r);
      end;
      fileDir.Delete(folderIdx);
    end;

    // Structures
    if TryFindChild(N, 'structures', c) then
    begin
      folderIdx := fileDir.Add('structures');
      for i := 0 to c.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(c, i, 'structure');
        ti := NS.CreateStructure(c0.Attributes['name']);
        LoadProperties(c0, TDFXStructure, ti);
        LoadText(fileDir.DelimitedText + PathDelim + c0.Attributes['name'], ti.FText);
      end;
      fileDir.Delete(folderIdx);
    end;
  end;

  procedure LoadEffect(N: IXMLNode; E: TDFXEffect);
  var
    i: Integer;
    c0, c1: IXMLNode;
    p: TDFXEffectPass;
    sf: PDFXShaderFunction;
  begin
    LoadProperties(N, TDFXEffect, E);

    if TryFindChild(N, 'passes', N) then
    begin
      for i := 0 to N.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(N, i, 'pass');
        p := E.CreatePass(c0.Attributes['title']);
        p.PassType := c0.Attributes['type'];

        c0 := c0.ChildNodes.FindNode('shaders');

        c1 := c0.ChildNodes.FindNode('vertex');
        if Assigned(c1) then
        begin
          sf := @p.VertexShader;
          sf.Reference := c1.Attributes['reference'];
          sf.Attributes := c1.Attributes['attributes'];
        end;

        c1 := c0.ChildNodes.FindNode('pixel');

        if Assigned(c1) then
        begin
          sf := @p.PixelShader;
          sf.Reference := c1.Attributes['reference'];
          sf.Attributes := c1.Attributes['attributes'];
        end;

        c1 := c0.ChildNodes.FindNode('compute');
        if Assigned(c1) then
        begin
          sf := @p.ComputeShader;
          sf.Reference := c1.Attributes['reference'];
          sf.Attributes := c1.Attributes['attributes'];
        end;

        c1 := c0.ChildNodes.FindNode('geometry');
        if Assigned(c1) then
        begin
          sf := @p.GeometryShader;
          sf.Reference := c1.Attributes['reference'];
          sf.Attributes := c1.Attributes['attributes'];
        end;
      end;
    end;
  end;

  procedure LoadNamespace(N: IXMLNode; NS: TDFXNamespace); overload;
  var
    c, c0: IXMLNode;
    i, nsIdx, folderIdx: Integer;
    e: TDFXEffect;
  begin
    if not Assigned(NS.Parent) then
      nsIdx := fileDir.Add('sources')
    else
      nsIdx := fileDir.Add(NS.Name);

    LoadLeafNamespace(N, TDFXLeafNamespace(NS));

    // <effects>
    if TryFindChild(N, 'effects', c) then
    begin
      folderIdx := fileDir.Add('effects');
      for i := 0 to c.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(c, i, 'effect');
        e := NS.CreateEffect(c0.Attributes['name']);
        LoadEffect(c0, e);
        fileDir.Add(e.Name);
        LoadLeafNamespace(c0, e);
        fileDir.Delete(fileDir.Count - 1);
      end;
      fileDir.Delete(folderIdx);
    end;

    if TryFindChild(N, 'namespaces', c) then
    begin
      folderIdx := fileDir.Add('namespaces');
      for i := 0 to c.ChildNodes.Count - 1 do
      begin
        c0 := MatchChildNode(c, i, 'namespace');
        LoadNamespace(c0, AddNamespace(NS, c0.Attributes['name']));
      end;
      fileDir.Delete(folderIdx);
    end;

    if nsIdx <> -1 then
      fileDir.Delete(nsIdx);
  end;
var
  n, c0, c1: IXMLNode;
  i: Integer;
begin
  fileDir := TStringList.Create;

  try
    fileDir.Delimiter := PathDelim;
    fileDir.DelimitedText := ExcludeTrailingPathDelimiter(ExpandedRootDir);

    doc := LoadXMLDocument(ExpandedRootDir + 'project.dfx');
    doc.Active := True;
    n := doc.DocumentElement;
    if n.NodeName <> 'project' then
      raise Exception.Create('Invalid project file.');

    // OutputDir
    LoadProperties(n, TDFXProject, Self);
    //OutputDir := MakeAbsolutePath(RootDir, n.Attributes['output']);
    //ShaderDir := MakeAbsolutePath(RootDir, n.Attributes['shaders']);
    //MinimizeShaders := StrToBool(n.Attributes['minimize_shaders']);

    // Preamble
    if TryFindChild(n, 'preamble', c0) then
      Preamble := c0.Text;

    // Constants
    if TryFindChild(n, 'constants', c0) then
      for i := 0 to c0.ChildNodes.Count - 1 do
      begin
        c1 := MatchChildNode(c0, i, 'constant');
        AddConstant(c1.Attributes['name']).Text := c1.Text;
      end;

    LoadNamespace(n.ChildNodes.FindNode('sources'), RootNamespace);
  finally
    fileDir.Free;
  end;
end;

procedure TDFXProject.DeleteConstant(Constant: TDFXConstant);
begin
  if Listener.OnCanDeleteConstant(Constant) then
  begin
    FConstants.Remove(Constant);
    Modified := True;
    Listener.OnDeleteConstant(Constant);
    Constant.Free;
  end;
end;

procedure TDFXProject.RenameConstant(Constant: TDFXConstant; const Description: string);
begin
  Constant.FName := Description;
  Modified := True;
  Listener.OnRenameConstant(Constant);
end;

procedure TDFXProject.RenameNamespace(Namespace: TDFXNamespace;
  const Name: string);
begin
  if not Assigned(Namespace.Parent) then
    Exit;

  if not IsIdentifier(Name) then
    raise Exception.Create('Namespace names have to be valid HLSL identifiers.');

  if Assigned(Namespace.Parent.FindNamespace(Name)) then
    raise Exception.Create('A namespace with that name already exists.');

  Namespace.FName := Name;
  Modified := True;
  Listener.OnRenameNamespace(Namespace);
end;

procedure WriteProperties(N: IXMLNode; C: TClass; I: Pointer);
var
  rContext: TRttiContext;
  rType: TRttiType;
  rProp: TRttiProperty;
begin
  rType := rContext.GetType(C);
  for rProp in rType.GetProperties do
    if rProp.Visibility = mvPublished then
      N.Attributes[LowerCase(rProp.Name)] := rProp.GetValue(I).ToString;
end;

procedure TDFXProject.Save(Directory: string);
var
  doc: IXMLDocument;

  procedure SaveNamespace(N: IXMLNode; NS: TDFXLeafNamespace); overload;
  var
    c0, c1: IXMLNode;
    i: Integer;
  begin
    // <constantbuffers>
    if NS.ConstantBuffers.Count > 0 then
    begin
      c0 := N.AddChild('cbuffers');
      for i := 0 to NS.ConstantBuffers.Count - 1 do
      begin
        c1 := c0.AddChild('cbuffer');
        WriteProperties(c1, TDFXConstantBuffer, NS.ConstantBuffers[i]);
        SaveText(NS.ConstantBuffers[i].DiskPath, NS.ConstantBuffers[i].Text);
      end;
    end;

    // <function>
    if NS.Functions.Count > 0 then
    begin
      c0 := N.AddChild('functions');
      for i := 0 to NS.Functions.Count - 1 do
      begin
        c1 := c0.AddChild('function');
        WriteProperties(c1, TDFXFunction, NS.Functions[i]);
        SaveText(NS.Functions[i].DiskPath, NS.Functions[i].Text);
      end;
    end;

    // <resource>
    if NS.Resources.Count > 0 then
    begin
      c0 := N.AddChild('resources');
      for i := 0 to NS.Resources.Count - 1 do
      begin
        c1 := c0.AddChild('resource');
        WriteProperties(c1, TDFXResource, NS.Resources[i]);
      end;
    end;

    // <structures>
    if NS.Structures.Count > 0 then
    begin
      c0 := N.AddChild('structures');
      for i := 0 to NS.Structures.Count - 1 do
      begin
        c1 := c0.AddChild('structure');
        WriteProperties(c1, TDFXStructure, NS.Structures[i]);
        SaveText(NS.Structures[i].DiskPath, NS.Structures[i].Text);
      end;
    end;
  end;

  procedure SaveEffect(N: IXMLNode; E: TDFXEffect);
  var
    c0, c1, c2: IXMLNode;
    i: Integer;
  begin
    // Passes
    WriteProperties(N, TDFXEffect, E);

    c0 := N.AddChild('passes');
    for i := 0 to E.Passes.Count - 1 do
    begin
      c1 := c0.AddChild('pass');
      c1.Attributes['title'] := E.Passes[i].Title;
      c1.Attributes['type'] := Integer(E.Passes[i].PassType);

      c1 := c1.AddChild('shaders');
      c2 := c1.AddChild('vertex');
      c2.Attributes['reference'] := E.Passes[i].VertexShader.Reference;
      c2.Attributes['attributes'] := E.Passes[i].VertexShader.Attributes;

      c2 := c1.AddChild('pixel');
      c2.Attributes['reference'] := E.Passes[i].PixelShader.Reference;
      c2.Attributes['attributes'] := E.Passes[i].PixelShader.Attributes;

      c2 := c1.AddChild('compute');
      c2.Attributes['reference'] := E.Passes[i].ComputeShader.Reference;
      c2.Attributes['attributes'] := E.Passes[i].ComputeShader.Attributes;

      c2 := c1.AddChild('geometry');
      c2.Attributes['reference'] := E.Passes[i].GeometryShader.Reference;
      c2.Attributes['attributes'] := E.Passes[i].GeometryShader.Attributes;
    end;
  end;

  procedure SaveNamespace(N: IXMLNode; NS: TDFXNamespace); overload;
  var
    c0, c1: IXMLNode;
    i: Integer;
  begin
    SaveNamespace(N, TDFXLeafNamespace(NS));

    // <effects>
    if NS.Effects.Count > 0 then
    begin
      c0 := N.AddChild('effects');
      for i := 0 to NS.Effects.Count - 1 do
      begin
        c1 := c0.AddChild('effect');
        c1.Attributes['name'] := NS.Effects[i].Name;

        SaveEffect(c1, NS.Effects[i]);

        SaveNamespace(c1, NS.Effects[i]);
      end;
    end;

    if NS.Subnamespaces.Count > 0 then
    begin
      c0 := N.AddChild('namespaces');
      for i := 0 to NS.Subnamespaces.Count - 1 do
      begin
        c1 := c0.AddChild('namespace');
        c1.Attributes['name'] := NS.Subnamespaces[i].Name;

        SaveNamespace(c1, NS.Subnamespaces[i]);
      end;
    end;
  end;
var
  n, c0, c1: IXMLNode;
  i: Integer;
begin
  if Directory <> '' then
    RootDir := Directory;

  ForceDirectories(ExpandedRootDir + 'sources');
  SetCurrentDir(ExpandedRootDir + 'sources');

  doc := TXMLDocument.Create(nil);
  doc.Active := True;
  doc.Options := [doNodeAutoCreate, doNodeAutoIndent, doAttrNull];

  // <project>
  n := doc.CreateNode('project');
  doc.DocumentElement := n;
  WriteProperties(n, TDFXProject, Self);
  // n.Attributes['shaders'] := MakeRelativePath(RootDir, ShaderDir);
  // n.Attributes['output'] := MakeRelativePath(RootDir, OutputDir);
  // n.Attributes['minimize_shaders'] := BoolToStr(MinimizeShaders);

  if Preamble.Length > 0 then
    n.AddChild('preamble').Text := Preamble;

  // <constants>
  if Constants.Count > 0 then
  begin
    c0 := n.AddChild('constants');
    for i := 0 to Constants.Count - 1 do
    begin
      c1 := c0.AddChild('constant');
      c1.Attributes['name'] := Constants[i].Name;
      c1.Text := Constants[i].Text;
    end;
  end;

  // <namespaces>
  c0 := n.AddChild('sources');
  SaveNamespace(c0, RootNamespace);

  doc.SaveToFile(ExpandedRootDir + 'project.dfx');

  Modified := False;
end;

procedure TDFXProject.SetHLSLCompilerFlags(const Value: Cardinal);
begin
  if Value <> FHLSLCompilerFlags then
  begin
    FHLSLCompilerFlags := Value;
    Modified := True;
  end;
end;

procedure TDFXProject.SetMinimizeShaders(const Value: Boolean);
begin
  if Value <> FMinimizeShaders then
  begin
    FMinimizeShaders := Value;
    Modified := True;
  end;
end;

procedure TDFXProject.SetModified(const Value: Boolean);
begin
  if Value <> FModified then
  begin
    FModified := Value;

    if Assigned(Listener) then
      Listener.OnProjectModified(Value);
  end;
end;

procedure TDFXProject.SetOutputDir(const Value: string);
begin
  if Value <> FOutputDir then
  begin
    FOutputDir := IncludeTrailingPathDelimiter(Value);
    Modified := True;
  end;
end;

procedure TDFXProject.SetPreamble(const Value: string);
begin
  if Value <> FPreamble then
  begin
    FPreamble := Value;
    Modified := True;
  end;
end;

procedure TDFXProject.SetRootDir(const NewValue: string);
var
  rootDirNoDelim: string;
begin
  if NewValue <> FRootDir then
    Modified := True;

  if NewValue.Length > 0 then
    FRootDir := IncludeTrailingPathDelimiter(NewValue)
  else
    FRootDir := '';

  // Derive project name
  rootDirNoDelim := ExcludeTrailingPathDelimiter(ExpandedRootDir);
  FName := Copy(rootDirNoDelim, LastDelimiter(PathDelim, ExcludeTrailingPathDelimiter(rootDirNoDelim)) + 1, rootDirNoDelim.Length);
end;

procedure TDFXProject.SetShaderDir(const Value: string);
begin
  if Value <> FShaderDir then
  begin
    FShaderDir := IncludeTrailingPathDelimiter(Value);
    Modified := True;
  end;
end;

{ TDFXNamespace }

function TDFXNamespace.FindGlobalConstantBufferReference(
  const Name: string): TDFXConstantBuffer;
var
  i: Integer;
  ns: TDFXLeafNamespace;
begin
  ns := FindGlobalNamespaceReference(Name, Project.RootNamespace);
  if not Assigned(ns) then
    Exit(nil);

  i := LastPos('::', Name);
  if i > 0 then
    Result := ns.FindConstantBuffer(Copy(Name, i + 2, Name.Length))
  else
    Result := ns.FindConstantBuffer(Name);
end;

function TDFXNamespace.FindGlobalFunctionReference(const Name: string): TDFXFunction;
var
  i: Integer;
  ns: TDFXLeafNamespace;
begin
  ns := FindGlobalNamespaceReference(Name, Project.RootNamespace);
  if not Assigned(ns) then
    Exit(nil);

  i := LastPos('::', Name);
  if i > 0 then
    Result := ns.FindFunction(Copy(Name, i + 2, Name.Length))
  else
    Result := ns.FindFunction(Name);
end;

function TDFXNamespace.FindGlobalNamespaceReference(const Name: string;
  Namespace: TDFXNamespace; I: Integer): TDFXLeafNamespace;
var
  p: Integer;
  s: string;
  ns: TDFXNamespace;
begin
  p := Pos('::', Name, I);
  // ::Root
  if p = I then
    Result := FindGlobalNamespaceReference(Name, Project.RootNamespace, I + 2)
  else if p > 0 then
  begin
    // NewNamespace::
    s := Copy(Name, I, p - I);
    ns := Namespace.FindNamespace(s);
    if Assigned(ns) then
      Result := FindGlobalNamespaceReference(Name, ns, p + 2)
    else
      Exit(Namespace.FindEffect(s));
  end
  else
    // RemainingName
    Result := Namespace
end;

function TDFXNamespace.FindGlobalResourceReference(
  const Name: string): TDFXResource;
var
  i: Integer;
  ns: TDFXLeafNamespace;
begin
  ns := FindGlobalNamespaceReference(Name, Project.RootNamespace);
  if not Assigned(ns) then
    Exit(nil);

  i := LastPos('::', Name);
  if i > 0 then
    Result := ns.FindResource(Copy(Name, i + 2, Name.Length))
  else
    Result := ns.FindResource(Name);
end;

function TDFXNamespace.FindNamespace(const Name: string): TDFXNamespace;
var
  ns: TDFXNamespace;
begin
  for ns in Subnamespaces do
    if ns.Name = Name then
      Exit(ns);

  Result := nil;
end;

constructor TDFXNamespace.Create(Project: TDFXProject; Parent: TDFXNamespace);
begin
  inherited Create(Project, Parent);
  FSubnamespaces := TList<TDFXNamespace>.Create;
  FEffects := TList<TDFXEffect>.Create;
end;

destructor TDFXNamespace.Destroy;
var
  ns: TDFXNamespace;
  e: TDFXEffect;
begin
  for e in Effects do
    e.Free;
  FEffects.Free;

  for ns in Subnamespaces do
    ns.Free;
  FSubnamespaces.Free;

  inherited;
end;

function TDFXNamespace.DiskPath(Delim: Char = PathDelim): string;
begin
  if Assigned(Parent) then
    Result := Parent.DiskPath(Delim) + 'namespaces' + Delim + Name + Delim
  else
    Result := '';
end;

procedure TDFXNamespace.RenameEffect(Effect: TDFXEffect;
  const Name: string);
begin
  if not Assigned(Effect) then
    Exit;

  if not IsIdentifier(Name) then
    raise Exception.Create('Effect names have to be valid HLSL identifiers.');

  if Assigned(FindEffect(Name)) then
    raise Exception.Create('An effect with that name already exists.');

  Effect.FName := Name;
  Project.Modified := True;
  Project.Listener.OnRenameEffect(Effect);
end;

function TDFXNamespace.MoveEffect(Effect: TDFXEffect): Boolean;
begin
  Result := not Assigned(FindEffect(Effect.Name));
  if not Result then
    Exit;

  Effect.Parent.FEffects.Remove(Effect);
  Effects.Add(Effect);
  Effect.FParent := Self;
  Project.Modified := True;
end;

function TDFXNamespace.MoveNamespace(Namespace: TDFXNamespace): Boolean;
begin
  if not Assigned(Namespace.Parent) then
    Exit(False);

  Result := not Assigned(FindNamespace(Namespace.Name));
  if not Result then
    Exit;

  Namespace.Parent.FSubnamespaces.Remove(Namespace);
  Subnamespaces.Add(Namespace);
  Namespace.FParent := Self;
  Project.Modified := True;
end;

procedure TDFXNamespace.DeleteEffect(Effect: TDFXEffect);
var
  p: Pointer;
begin
  if Project.Listener.OnCanDeleteEffect(Effect) then
    if Effects.Remove(Effect) <> -1 then
    begin
      // Delete entire namespace
      for p in Effect.Functions do
        Project.Listener.OnDeleteFunction(p);

      for p in Effect.Resources do
        Project.Listener.OnDeleteResource(p);

      for p in Effect.ConstantBuffers do
        Project.Listener.OnDeleteConstantBuffer(p);

      Project.Modified := True;
      Project.Listener.OnDeleteEffect(Effect);
      Effect.Free;
    end;
end;

function TDFXNamespace.FindEffect(const Name: string): TDFXEffect;
var
  e: TDFXEffect;
begin
  for e in Effects do
    if e.Name = Name then
      Exit(e);

  Result := nil;
end;

{ TDFXLeafNamespace }

constructor TDFXLeafNamespace.Create(Project: TDFXProject; Parent: TDFXNamespace);
begin
  FProject := Project;
  FFunctions := TList<TDFXFunction>.Create;
  FConstantBuffers := TList<TDFXConstantBuffer>.Create;
  FResources := TList<TDFXResource>.Create;
  FStructures := TList<TDFXStructure>.Create;
  FParent := Parent;
end;

function TDFXNamespace.CreateEffect(const Name: string): TDFXEffect;
begin
  Result := TDFXEffect.Create(Self);
  try
    RenameEffect(Result, Name);
  except
    on e: Exception do
    begin
      Result.Free;
      raise;
    end;
  end;
  FEffects.Add(Result);

  Project.Modified := True;
  Project.Listener.OnAddEffect(Result);
end;

function TDFXLeafNamespace.CreateConstantBuffer(
  const Name: string): TDFXConstantBuffer;
begin
  Result := TDFXConstantBuffer.Create(Self);
  try
    RenameConstantBuffer(Result, Name);
  except
    on e: Exception do
    begin
      Result.Free;
      raise;
    end;
  end;
  FConstantBuffers.Add(Result);

  Project.Modified := True;
  Project.Listener.OnAddConstantBuffer(Result);
end;

function TDFXLeafNamespace.CreateFunction(const Name: string): TDFXFunction;
begin
  Result := TDFXFunction.Create(Self);
  try
    RenameFunction(Result, Name);
  except
    on e: Exception do
    begin
      Result.Free;
      raise;
    end;
  end;
  FFunctions.Add(Result);

  Project.Modified := True;
  Project.Listener.OnAddFunction(Result);
end;

function TDFXLeafNamespace.CreateResource(const Name: string): TDFXResource;
begin
  Result := TDFXResource.Create(Self);
  try
    RenameResource(Result, Name);
  except
    on e: Exception do
    begin
      Result.Free;
      raise;
    end;
  end;
  FResources.Add(Result);

  Project.Modified := True;
  Project.Listener.OnAddResource(Result);
end;

function TDFXLeafNamespace.CreateStructure(const Name: string): TDFXStructure;
begin
  Result := TDFXStructure.Create(Self);
  try
    RenameStructure(Result, Name);
  except
    on e: Exception do
    begin
      Result.Free;
      raise;
    end;
  end;
  FStructures.Add(Result);

  Project.Modified := True;
  Project.Listener.OnAddStructure(Result);
end;

procedure TDFXLeafNamespace.DeleteConstantBuffer(CB: TDFXConstantBuffer);
begin
  if Project.Listener.OnCanDeleteConstantBuffer(CB) then
    if ConstantBuffers.Remove(CB) <> -1 then
    begin
      Project.Modified := True;
      Project.Listener.OnDeleteConstantBuffer(CB);
      CB.Free;
    end;
end;

procedure TDFXLeafNamespace.DeleteFunction(Func: TDFXFunction);
begin
  if Project.Listener.OnCanDeleteFunction(Func) then
    if Functions.Remove(Func) <> -1 then
    begin
      Project.Modified := True;
      Project.Listener.OnDeleteFunction(Func);
      Func.Free;
    end;
end;

procedure TDFXLeafNamespace.DeleteResource(Resource: TDFXResource);
begin
  if Project.Listener.OnCanDeleteResource(Resource) then
    if Resources.Remove(Resource) <> -1 then
    begin
      Project.Modified := True;
      Project.Listener.OnDeleteResource(Resource);
      Resource.Free;
    end;
end;

procedure TDFXLeafNamespace.DeleteStructure(Structure: TDFXStructure);
begin
  if Project.Listener.OnCanDeleteStructure(Structure) then
    if Structures.Remove(Structure) <> -1 then
    begin
      Project.Modified := True;
      Project.Listener.OnDeleteStructure(Structure);
      Structure.Free;
    end;
end;

function TDFXLeafNamespace.Depth: Integer;
var
  n: TDFXLeafNamespace;
begin
  Result := 0;
  n := Self;
  while Assigned(n.Parent) do
  begin
    Inc(Result);
    n := n.Parent;
  end;
end;

destructor TDFXLeafNamespace.Destroy;
var
  f: TDFXFunction;
  c: TDFXConstantBuffer;
  r: TDFXResource;
  s: TDFXStructure;
begin
  for f in Functions do
    f.Free;
  FFunctions.Free;

  for c in ConstantBuffers do
    c.Free;
  FConstantBuffers.Free;

  for r in Resources do
    r.Free;
  FResources.Free;

  for s in Structures do
    s.Free;
  FStructures.Free;

  inherited;
end;

function TDFXLeafNamespace.FilePath(const Delimiter: string): string;
begin
  if Assigned(Parent) then
    Result := Parent.FilePath(Delimiter) + Name + Delimiter
  else
    Result := '';
end;

function TDFXLeafNamespace.FindConstantBuffer(
  const Name: string): TDFXConstantBuffer;
var
  f: TDFXConstantBuffer;
begin
  for f in ConstantBuffers do
    if f.Name = Name then
      Exit(f);

  Result := nil;
end;

function TDFXLeafNamespace.FindConstantBufferReference(
  const Name: string): TDFXConstantBuffer;
var
  ns: TDFXLeafNamespace;
begin
  ns := Self;
  while Assigned(ns) do
  begin
    Result := ns.FindConstantBuffer(Name);
    if Assigned(Result) then
      Exit;
    ns := ns.Parent;
  end;
  Result := nil;
end;

function TDFXLeafNamespace.FindFunction(const Name: string): TDFXFunction;
var
  f: TDFXFunction;
begin
  for f in Functions do
    if f.Name = Name then
      Exit(f);

  Result := nil;
end;

function TDFXLeafNamespace.FindGlobalConstantBufferReference(
  const Name: string): TDFXConstantBuffer;
begin
  Result := FindConstantBuffer(Name);
  if not Assigned(Result) then
    Result := Parent.FindGlobalConstantBufferReference(Name);
end;

function TDFXLeafNamespace.FindGlobalFunctionReference(
  const Name: string): TDFXFunction;
begin
  Result := FindFunction(Name);
  if not Assigned(Result) then
    Result := Parent.FindGlobalFunctionReference(Name);
end;

function TDFXLeafNamespace.FindGlobalResourceReference(
  const Name: string): TDFXResource;
begin
  Result := FindResource(Name);
  if not Assigned(Result) then
    Result := Parent.FindGlobalResourceReference(Name);
end;

function TDFXLeafNamespace.FindResource(const Name: string): TDFXResource;
var
  r: TDFXResource;
begin
  for r in Resources do
    if r.Name = Name then
      Exit(r);

  Result := nil;
end;

function TDFXLeafNamespace.FindResourceReference(
  const Name: string): TDFXResource;
var
  ns: TDFXLeafNamespace;
begin
  ns := Self;
  while Assigned(ns) do
  begin
    Result := ns.FindResource(Name);
    if Assigned(Result) then
      Exit;
    ns := ns.Parent;
  end;
  Result := nil;
end;

function TDFXLeafNamespace.FindStructure(const Name: string): TDFXStructure;
var
  s: TDFXStructure;
begin
  for s in Structures do
    if s.Name = Name then
      Exit(s);

  Result := nil;
end;

function TDFXLeafNamespace.MoveConstantBuffer(CB: TDFXConstantBuffer): Boolean;
begin
  Result := not Assigned(FindConstantBuffer(CB.Name));
  if not Result then
    Exit;

  Project.Modified := True;

  CB.Namespace.FConstantBuffers.Remove(CB);
  ConstantBuffers.Add(CB);
  CB.FNamespace := Self;
end;

type
  TDFXEffectPassIteratorCallback = function(var P: TDFXEffectPass): Boolean;

function TDFXLeafNamespace.MoveFunction(Func: TDFXFunction): Boolean;
begin
  Result := not Assigned(FindFunction(Func.Name));
  if not Result then
    Exit;

  Project.Modified := True;

  Func.Namespace.FFunctions.Remove(Func);
  Functions.Add(Func);
  Func.FNamespace := Self;
end;

function TDFXLeafNamespace.MoveResource(Resource: TDFXResource): Boolean;
begin
  Result := not Assigned(FindResource(Resource.Name));
  if not Result then
    Exit;

  Project.Modified := True;
  Resource.Namespace.FResources.Remove(Resource);
  Resources.Add(Resource);
  Resource.FNamespace := Self;
end;

function TDFXLeafNamespace.MoveStructure(Structure: TDFXStructure): Boolean;
begin
  Result := not Assigned(FindResource(Structure.Name));
  if not Result then
    Exit;

  Project.Modified := True;
  Structure.Namespace.FStructures.Remove(Structure);
  Structures.Add(Structure);
  Structure.FNamespace := Self;
end;

function TDFXLeafNamespace.RelativeDiskPath: string;
begin
  Result := MakeRelativePath(GetCurrentDir, DiskPath);
end;

procedure TDFXLeafNamespace.RenameConstantBuffer(CB: TDFXConstantBuffer;
  const Name: string);
begin
  if not IsIdentifier(Name) then
    raise Exception.Create('Constant buffer names have to be valid HLSL identifiers.');

  if (Assigned(FindConstantBuffer(Name))) or (Assigned(FindResource(Name))) or (Assigned(FindStructure(Name))) then
    raise Exception.Create('An entity with that name already exists.');

  CB.FName := Name;
  Project.Modified := True;
  Project.Listener.OnRenameConstantBuffer(CB);
end;

procedure TDFXLeafNamespace.RenameFunction(Func: TDFXFunction; const Name: string);
begin
  if not IsIdentifier(Name) then
    raise Exception.Create('Function names have to be valid HLSL identifiers.');

  if Assigned(FindFunction(Name)) then
    raise Exception.Create('A function with that name already exists.');

  Func.FName := Name;
  Project.Modified := True;
  Project.Listener.OnRenameFunction(Func);
end;

procedure TDFXLeafNamespace.RenameResource(Resource: TDFXResource;
  const Name: string);
begin
  if not IsIdentifier(Name) then
    raise Exception.Create('Resource names have to be valid HLSL identifiers.');

  if (Assigned(FindConstantBuffer(Name))) or (Assigned(FindResource(Name))) or (Assigned(FindStructure(Name))) then
    raise Exception.Create('An entity with that name already exists.');

  Resource.FName := Name;
  Project.Modified := True;
  Project.Listener.OnRenameResource(Resource);
end;

procedure TDFXLeafNamespace.RenameStructure(Structure: TDFXStructure;
  const Name: string);
begin
  if not IsIdentifier(Name) then
    raise Exception.Create('Structure names have to be valid HLSL identifiers.');

  if (Assigned(FindConstantBuffer(Name))) or (Assigned(FindResource(Name))) or (Assigned(FindStructure(Name))) then
    raise Exception.Create('An entity with that name already exists.');

  Structure.FName := Name;
  Project.Modified := True;
  Project.Listener.OnRenameStructure(Structure);
end;

{ TDFXEffect }

constructor TDFXEffect.Create(Namespace: TDFXNamespace);
begin
  inherited Create(Namespace.Project, Namespace);
  FPasses := TList<TDFXEffectPass>.Create;
  FParticipateInBuild := True;
end;

function TDFXEffect.CreatePass(const Title: string): TDFXEffectPass;
begin
  Result := TDFXEffectPass.Create(Title);
  FPasses.Add(Result);
  Project.Modified := True;
  Project.Listener.OnAddEffectPass(Result);
end;

procedure TDFXEffect.DeleteEffectPass(P: TDFXEffectPass);
begin
  if Project.Listener.OnCanDeleteEffectPass(P) then
    if Passes.Remove(P) <> -1 then
    begin
      Project.Modified := True;
      Project.Listener.OnDeleteEffectPass(P);
      P.Free;
    end;
end;

destructor TDFXEffect.Destroy;
var
  p: TDFXEffectPass;
begin
  for p in Passes do
    p.Free;
  FPasses.Free;
  inherited;
end;

function TDFXEffect.DiskPath(Delim: Char = PathDelim): string;
begin
  Result := Parent.DiskPath(Delim) + 'effects' + Delim + Name + Delim;
end;

procedure TDFXEffect.RenameEffectPass(P: TDFXEffectPass; const Title: string);
begin
  P.Title := Title;
  Project.Modified := True;
  Project.Listener.OnRenameEffectPass(P);
end;

procedure TDFXEffect.SetParticipateInBuild(const Value: Boolean);
begin
  if FParticipateInBuild <> Value then
  begin
    Project.Modified := True;
    FParticipateInBuild := Value;
  end;
end;

procedure TDFXEffect.MovePass(OldIndex, NewIndex: Integer);
begin
  FPasses.Move(OldIndex, NewIndex);
end;

{ TDFXEffectPass }

constructor TDFXEffectPass.Create(const Title: string);
begin
  FTitle := Title;
  FPassType := ptGeometry;
end;

{ TDFXNamespacedTextItem }

constructor TDFXNamespacedTextItem.Create(Namespace: TDFXLeafNamespace);
begin
  inherited Create;
  FNamespace := Namespace;
end;

function TDFXNamespacedTextItem.FilePath(const Delimiter: string): string;
begin
  Result := Namespace.FilePath(Delimiter) + Name;
end;

function TDFXNamespacedTextItem.RelativeDiskPath: string;
begin
  Result := MakeRelativePath(GetCurrentDir, DiskPath);
end;

{ TDFXConstantBuffer }

constructor TDFXConstantBuffer.Create(Namespace: TDFXLeafNamespace);
begin
  inherited Create(Namespace);
  FRegister := -1;
end;

function TDFXConstantBuffer.DiskPath(Delim: Char = PathDelim): string;
begin
  Result := Namespace.DiskPath(Delim) + 'cbuffers' + Delim + Name;
end;

procedure TDFXConstantBuffer.SetName(const Value: string);
begin
  if Value <> Name then
    Namespace.RenameConstantBuffer(Self, Value);
end;

{ TDFXFunction }

function TDFXFunction.DiskPath(Delim: Char = PathDelim): string;
begin
  Result := Namespace.DiskPath(Delim) + 'functions' + Delim + Name;
end;

procedure TDFXFunction.SetName(const Value: string);
begin
  if Value <> Name then
    Namespace.RenameFunction(Self, Value);
end;

procedure TDFXFunction.SetShaderProfile(const Value: AnsiString);
begin
  FShaderProfile := Trim(LowerCase(Value));
end;

{ TDFXResource }

constructor TDFXResource.Create(Namespace: TDFXLeafNamespace);
begin
  inherited Create(Namespace);

  HLSLType := rtTexture2D;
  ComponentType := 'float4';
  SampleCount := 1;
  ForceRegister := -1;
  SubresourceIndex := 0;
end;

function TDFXResource.DiskPath(Delim: Char = PathDelim): string;
begin
  Result := Namespace.DiskPath(Delim) + 'resources' + Delim + Name;
end;

function TDFXResource.GetHLSLTypeString: string;
var
  obj: string;
begin
  // Object name
  obj := GetEnumName(TypeInfo(TDFXResourceType), Integer(FHLSLType));
  Delete(obj, 1, 2);

  // RW prefix
  if not (FHLSLType in [rtAppendStructuredBuffer, rtConsumeStructuredBuffer]) then
    case FGPUAccessMode of
      amWrite,
      amReadWrite: obj := 'RW' + obj;
    end;

  if SampleCount > 1 then
    Result := Format('%s<%s, %d>', [obj, ComponentType, SampleCount])
  else if not IsSamplerStateType then
    Result := Format('%s<%s>', [obj, ComponentType])
  else
    Result := obj;
end;

function TDFXResource.GetUAVInitialCount: string;
begin
  Result := Format('0x%x', [FUAVCount]);
end;

function TDFXResource.IsSamplerStateType: Boolean;
begin
  Result := HLSLType in [rtSamplerState, rtSamplerComparisonState];
end;

function TDFXResource.IsTexture: Boolean;
begin
  case HLSLType of
    rtTexture1D,
    rtTexture1DArray,
    rtTexture2D,
    rtTexture2DArray,
    rtTexture3D,
    rtTextureCube,
    rtTextureCubeArray: Exit(True);
  end;
  Result := False;
end;

function TDFXResource.MustInitialize: Boolean;
begin
  if IsTexture then
    Exit(False)
  else
    case HLSLType of
      rtAppendStructuredBuffer,
      rtBuffer,
      rtByteAddressBuffer,
      rtConsumeStructuredBuffer,
      rtStructuredBuffer: Exit(False);
    end;
end;

procedure TDFXResource.SetComponentType(const Value: string);
begin
  if not IsSamplerStateType then
    FComponentType := Value;
end;

procedure TDFXResource.SetGPUAccessMode(const Value: TDFXGPUAccessMode);
begin
  if not IsSamplerStateType then
    FGPUAccessMode := Value;
end;

procedure TDFXResource.SetHLSLType(const Value: TDFXResourceType);
begin
  FHLSLType := Value;

  if IsSamplerStateType then
  begin
    FSampleCount := 1;
    FComponentType := '';
    FGPUAccessMode := amRead;
    if not SupportsUAVCounter then
      FUAVCount := 0;
  end;
end;

procedure TDFXResource.SetName(const Value: string);
begin
  if Value <> Name then
    Namespace.RenameResource(Self, Value);
end;

procedure TDFXResource.SetSampleCount(const Value: TDFXSampleCount);
begin
  if not IsSamplerStateType then
    FSampleCount := Value;
end;

procedure TDFXResource.SetSemantic(const Value: string);
begin
  if (Value.Length > 0) and (not IsIdentifier(Value)) then
    Exit;

  FSemantic := Value;
end;

procedure TDFXResource.SetUAVInitialCount(const Value: string);
begin
  if SupportsUAVCounter then
    TryStrToInt(Value, FUAVCount);
end;

function TDFXResource.SupportsUAVCounter: Boolean;
begin
  Result := (rtAppendStructuredBuffer = HLSLType) or
            (rtConsumeStructuredBuffer = HLSLType) or
            ((rtStructuredBuffer = HLSLType) and ((GPUAccessMode = amWrite) or (GPUAccessMode = amReadWrite)));
end;

{ TDFXStructure }

function TDFXStructure.DiskPath(Delim: Char = PathDelim): string;
begin
  Result := Namespace.DiskPath(Delim) + 'structures' + Delim + Name;
end;

procedure TDFXStructure.SetName(const Value: string);
begin
  if Value <> Name then
    Namespace.RenameStructure(Self, Value);
end;

{ TDFXNamespacedItem }

constructor TDFXNamespacedItem.Create(Namespace: TDFXLeafNamespace);
begin
  inherited Create;
  FNamespace := Namespace;
end;

function TDFXNamespacedItem.FilePath(const Delimiter: string): string;
begin
  Result := Namespace.FilePath(Delimiter) + Name;
end;

function TDFXNamespacedItem.RelativeDiskPath: string;
begin
  Result := MakeRelativePath(GetCurrentDir, DiskPath);
end;

{ TDFXConstant }

procedure TDFXConstant.SetName(const Value: string);
begin
  FName := Value;
end;

end.
