unit MainWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ComCtrls,
  Vcl.PlatformDefaultStyleActnCtrls, System.Actions, Vcl.ActnList, Vcl.ActnMan,
  Vcl.ToolWin, Vcl.ActnCtrls, Vcl.ActnMenus, Vcl.StdCtrls, Vcl.ExtCtrls,
  Vcl.Imaging.pngimage, Vcl.Tabs, Vcl.ImgList, Vcl.AppEvnts, DFX.Project, ShlObj,
  Generics.Collections, CommCtrl, IniFiles, Clipbrd, RegularExpressions,
  ShellApi,
  Vcl.Menus, Vcl.ActnPopup, ThreadWorkWindow, SynEdit, SynEditHighlighter,
  SynHighlighterCpp, IceTabSet, SynCompletionProposal, Math, System.UITypes,
  Vcl.ButtonGroup, DFX.Compiler2, DFX.Compiler2.Types, System.Types, SynEditMiscClasses, SynEditSearch,
  SynEditTypes, SynHighlighterHLSL, SynEditPopup, JvExControls, JvInspector,
  JvComponentBase, SynEditKeyCmds;

type
  TComboBoxEx = class(Vcl.ComCtrls.TComboBoxEx)
  protected
    procedure SetItemIndex(const Value: Integer); override;
  published
    property ItemIndex: Integer read GetItemIndex write SetItemIndex;
  end;

  TButtonClickCallback = procedure(Sender: TObject) of object;

  TMainForm = class(TForm, IDFXProjectListener, IDFXCompilerHandler)
    StatusBar: TStatusBar;
    MainMenuBar: TActionMainMenuBar;
    ActionManager: TActionManager;
    MainToolBar: TActionToolBar;
    LeftBGPanel: TPanel;
    ProjectOverviewPanel: TPanel;
    WindowHierarchyHeader: TPanel;
    ProjectOverviewImage: TImage;
    ProjectOverviewLabel: TLabel;
    ProjectOverviewIcon: TImage;
    ProjectOverviewSearchEdit: TButtonedEdit;
    NamespaceView: TTreeView;
    ActionImages: TImageList;
    ScreenImages: TImageList;
    LeftSplitter: TSplitter;
    acExit: TAction;
    acNewProject: TAction;
    ApplicationEvents: TApplicationEvents;
    SearchImages: TImageList;
    EditImages: TImageList;
    acOpenProject: TAction;
    acStatusBar: TAction;
    acAboutGoProjectManager: TAction;
    NamespaceViewImages: TImageList;
    acCloseAll: TAction;
    MiddlePanel: TPanel;
    TextEditorPanel: TPanel;
    RightSplitter: TSplitter;
    RightBGPanel: TPanel;
    RightNotebook: TNotebook;
    EffectsBGPanel: TPanel;
    EffectsPanel: TPanel;
    Panel2: TPanel;
    EffectsHeaderPanel: TPanel;
    EffectsHeaderImage: TImage;
    EffectsLabel: TLabel;
    EffectsIcon: TImage;
    acCut: TAction;
    acCopy: TAction;
    acPaste: TAction;
    acSelectAll: TAction;
    acDelete: TAction;
    BGImageList: TImageList;
    NamespacePopup: TPopupActionBar;
    acRename: TAction;
    Rename1: TMenuItem;
    acCreateNamespace: TAction;
    AddNamespace1: TMenuItem;
    acCreateEffect: TAction;
    CreateEffect1: TMenuItem;
    N1: TMenuItem;
    PassPanel: TPanel;
    PassDetailPanel: TPanel;
    PassHeaderPanel: TPanel;
    PassHeaderImage: TImage;
    PassLabel: TLabel;
    PassIcon: TImage;
    EffectSplitter: TSplitter;
    TextItemPopup: TPopupActionBar;
    MenuItem4: TMenuItem;
    Delete1: TMenuItem;
    Delete2: TMenuItem;
    EffectsViewImages: TImageList;
    OutputPanel: TPanel;
    Panel4: TPanel;
    OutputHeaderPanel: TPanel;
    OutputHeaderImage: TImage;
    OutputHeaderLabel: TLabel;
    OutputHeaderIcon: TImage;
    TabSet: TIceTabSet;
    Bevel1: TBevel;
    OutputSplitter: TSplitter;
    TextEditorHeaderPanel: TPanel;
    TextEditorHeaderImage: TImage;
    Label2: TLabel;
    Image4: TImage;
    BGPaintBox: TPaintBox;
    VignetteImage: TImage;
    Logo: TImage;
    acClose: TAction;
    acCreateFunction: TAction;
    CreateFunction1: TMenuItem;
    acOpen: TAction;
    Edit1: TMenuItem;
    acOptions: TAction;
    ExeOpenDialog: TOpenDialog;
    acGeneralOptions: TAction;
    acToolsOptions: TAction;
    acSave: TAction;
    TxtSaveDialog: TSaveDialog;
    acCreateConstantBuffer: TAction;
    CreateConstantBuffer1: TMenuItem;
    AutoSaveTimer: TTimer;
    acBuild: TAction;
    acCreateConstants: TAction;
    acCreateResource: TAction;
    CreateResource1: TMenuItem;
    acProjectOptions: TAction;
    acCreateStructure: TAction;
    CreateStructure1: TMenuItem;
    GeometryShaderImages: TImageList;
    EffectImages: TImageList;
    EffectListBox: TListBox;
    acCreateEffectPass: TAction;
    Constants1: TMenuItem;
    PassNameEdit: TEdit;
    PassTypeComboBox: TComboBoxEx;
    PassTypeImages: TImageList;
    EffectPassPopup: TPopupActionBar;
    MenuItem3: TMenuItem;
    EffectPass1: TMenuItem;
    GeometryScrollBox: TScrollBox;
    ActiveShaderReferenceLabel: TLabel;
    Label4: TLabel;
    AttributesMemo: TMemo;
    ComputeShaderButtonGroup: TButtonGroup;
    GeometryShaderButtonGroup: TButtonGroup;
    ComputeShaderImages: TImageList;
    acSaveAs: TAction;
    ShaderEdit: TButtonedEdit;
    FunctionPickImageList: TImageList;
    OutputMemo: TRichEdit;
    ScriptOpenDialog: TOpenDialog;
    FindDialog: TFindDialog;
    SynEditSearch: TSynEditSearch;
    acFind: TAction;
    PropertyEditorPanel: TPanel;
    PropertyEditorHeaderPanel: TPanel;
    PropertyEditorHeaderImage: TImage;
    Label1: TLabel;
    PropertyEditorIcon: TImage;
    PropertyEditor: TJvInspector;
    PropertyEditorSplitter: TSplitter;
    acUndo: TAction;
    acRedo: TAction;
    ItemPopup: TPopupActionBar;
    MenuItem2: TMenuItem;
    MenuItem5: TMenuItem;
    TabSetPopup: TPopupMenu;
    Close1: TMenuItem;
    Create1: TMenuItem;
    N2: TMenuItem;
    ActiveEffectPopup: TPopupActionBar;
    MenuItem1: TMenuItem;
    MenuItem6: TMenuItem;
    MenuItem7: TMenuItem;
    Create2: TMenuItem;
    N3: TMenuItem;
    ConstantBuffer1: TMenuItem;
    Constants2: TMenuItem;
    N4: TMenuItem;
    Function1: TMenuItem;
    Resource1: TMenuItem;
    Structure1: TMenuItem;
    EffectNameLabel: TLabel;
    CloseAll1: TMenuItem;
    DarkPainter: TJvInspectorDotNETPainter;
    N5: TMenuItem;
    acIncludeInBuild: TAction;
    acExcludeFromBuild: TAction;
    IncludeinBuild1: TMenuItem;
    InactiveEffectPopup: TPopupActionBar;
    MenuItem8: TMenuItem;
    MenuItem9: TMenuItem;
    MenuItem10: TMenuItem;
    MenuItem11: TMenuItem;
    MenuItem12: TMenuItem;
    MenuItem13: TMenuItem;
    MenuItem14: TMenuItem;
    MenuItem15: TMenuItem;
    MenuItem16: TMenuItem;
    MenuItem17: TMenuItem;
    MenuItem18: TMenuItem;
    MenuItem19: TMenuItem;
    MenuItem20: TMenuItem;
    acProjectOptionsGeneral: TAction;
    acProjectOptionsHLSL: TAction;
    acProjectOptionsPaths: TAction;
    acProjectOptionsPreProcessing: TAction;
    ShaderPopup: TPopupActionBar;
    MenuItem21: TMenuItem;
    MenuItem22: TMenuItem;
    MenuItem23: TMenuItem;
    ShowDisassembly1: TMenuItem;
    acShowDisassembly: TAction;
    procedure acExitExecute(Sender: TObject);
    procedure StatusBarResize(Sender: TObject);
    procedure ApplicationEventsHint(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure ApplicationEventsShortCut(var Msg: TWMKey; var Handled: Boolean);
    procedure acNewProjectExecute(Sender: TObject);
    procedure acOpenProjectExecute(Sender: TObject);
    procedure acStatusBarExecute(Sender: TObject);
    procedure acAboutGoProjectManagerExecute(Sender: TObject);
    procedure acCloseAllExecute(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure FormShow(Sender: TObject);
    procedure ProjectOverviewSearchEditKeyPress(Sender: TObject; var Key: Char);
    procedure ProjectOverviewSearchEditRightButtonClick(Sender: TObject);
    procedure NamespaceViewEditing(Sender: TObject; Node: TTreeNode;
      var AllowEdit: Boolean);
    procedure acRenameExecute(Sender: TObject);
    procedure NamespaceViewEdited(Sender: TObject; Node: TTreeNode;
      var S: string);
    procedure acCreateNamespaceExecute(Sender: TObject);
    procedure NamespaceViewCancelEdit(Sender: TObject; Node: TTreeNode);
    procedure acCreateEffectUpdate(Sender: TObject);
    procedure acCreateEffectExecute(Sender: TObject);
    procedure NamespaceViewChange(Sender: TObject; Node: TTreeNode);
    procedure acRenameUpdate(Sender: TObject);
    procedure acDeleteUpdate(Sender: TObject);
    procedure acDeleteExecute(Sender: TObject);
    procedure NamespaceViewKeyPress(Sender: TObject; var Key: Char);
    procedure TabSetTabClose(Sender: TObject; ATab: TIceTab);
    procedure BGPaintBoxPaint(Sender: TObject);
    procedure acCloseAllUpdate(Sender: TObject);
    procedure TabSetTabSelected(Sender: TObject; ATab: TIceTab;
      ASelected: Boolean);
    procedure acCloseExecute(Sender: TObject);
    procedure acCloseUpdate(Sender: TObject);
    procedure acCreateFunctionExecute(Sender: TObject);
    procedure NamespaceViewDblClick(Sender: TObject);
    procedure acCopyExecute(Sender: TObject);
    procedure acCopyUpdate(Sender: TObject);
    procedure acOpenExecute(Sender: TObject);
    procedure NamespaceViewMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure acOpenUpdate(Sender: TObject);
    procedure acOptionsExecute(Sender: TObject);
    procedure FormShortCut(var Msg: TWMKey; var Handled: Boolean);
    procedure acGeneralOptionsExecute(Sender: TObject);
    procedure acToolsOptionsExecute(Sender: TObject);
    procedure acSaveUpdate(Sender: TObject);
    procedure NamespaceViewDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure NamespaceViewDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure acSaveExecute(Sender: TObject);
    procedure VignetteImageDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure VignetteImageDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure acCreateConstantBufferExecute(Sender: TObject);
    procedure AutoSaveTimerTimer(Sender: TObject);
    procedure NamespaceViewKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure acBuildUpdate(Sender: TObject);
    procedure acCreateConstantsExecute(Sender: TObject);
    procedure acCreateResourceExecute(Sender: TObject);
    procedure acProjectOptionsExecute(Sender: TObject);
    procedure acCreateStructureExecute(Sender: TObject);
    procedure ApplicationEventsException(Sender: TObject; E: Exception);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure EffectListBoxDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure acCreateEffectPassUpdate(Sender: TObject);
    procedure acCreateEffectPassExecute(Sender: TObject);
    procedure EffectListBoxClick(Sender: TObject);
    procedure PassNameEditChange(Sender: TObject);
    procedure EffectListBoxDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure EffectListBoxDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure PassTypeComboBoxChange(Sender: TObject);
    procedure ComputeShaderButtonGroupButtonClicked(Sender: TObject; Index: Integer);
    procedure ShaderEditChange(Sender: TObject);
    procedure AttributesMemoChange(Sender: TObject);
    procedure EffectListBoxMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure acBuildExecute(Sender: TObject);
    procedure ShaderEditDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure ShaderEditDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure acRunExecute(Sender: TObject);
    procedure acSaveAsExecute(Sender: TObject);
    procedure NamespaceViewGetImageIndex(Sender: TObject; Node: TTreeNode);
    procedure ShaderEditLeftButtonClick(Sender: TObject);
    procedure ShaderEditRightButtonClick(Sender: TObject);
    procedure VignetteImageClick(Sender: TObject);
    procedure acSelectAllExecute(Sender: TObject);
    procedure acSelectAllUpdate(Sender: TObject);
    procedure acPasteUpdate(Sender: TObject);
    procedure acPasteExecute(Sender: TObject);
    procedure acCutUpdate(Sender: TObject);
    procedure acCutExecute(Sender: TObject);
    procedure acFindExecute(Sender: TObject);
    procedure FindDialogFind(Sender: TObject);
    procedure acFindUpdate(Sender: TObject);
    procedure acUndoExecute(Sender: TObject);
    procedure acUndoUpdate(Sender: TObject);
    procedure acRedoUpdate(Sender: TObject);
    procedure acRedoExecute(Sender: TObject);
    procedure RightSplitterMoved(Sender: TObject);
    procedure acSaveAsUpdate(Sender: TObject);
    procedure CloseAll1Click(Sender: TObject);
    procedure PropertyEditorItemValueChanged(Sender: TObject;
      Item: TJvCustomInspectorItem);
    procedure acIncludeInBuildExecute(Sender: TObject);
    procedure acExcludeFromBuildExecute(Sender: TObject);
    procedure acProjectOptionsGeneralExecute(Sender: TObject);
    procedure PropertyEditorSplitterMoved(Sender: TObject);
    procedure DarkPainterSetItemColors(Item: TJvCustomInspectorItem;
      Canvas: TCanvas);
    procedure FormMouseWheelDown(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure FormMouseWheelUp(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure acShowDisassemblyExecute(Sender: TObject);
  type
    TApplicationSettings = record
      InitialProjectRootDir: string;
      PreBuild: string;
      PostBuild: string;
      DoAutoSave: Boolean;
      AutoSaveInterval: Integer;
    end;
  type
    TBuildThreadMode = (btmBuildProject, btmQueryDASM);

    TBuildThread = class(TThread)
    protected
      FMsg: string;
      FMode: TBuildThreadMode;

      procedure Execute; override;

      procedure LogMsg;
      procedure LogError;
    public
      constructor Create(Mode: TBuildThreadMode = btmBuildProject);

      procedure InitializeStatus;
      procedure UpdateStatus;
      procedure IncreaseStep;
    end;
  private
    { Private declarations }
    FProject: TDFXProject;

    FSynHLSLSyn: TSynHLSLSyn;

    // Startup settings
    FFirstShow: Boolean;
    FAppSettings: TApplicationSettings;

    // AppCache
    FLastOptionsPage,
    FLastProjectOptionsPage: Integer;

    // Threads
    FThreadWorkForm: TThreadWorkForm;

    // Cache
    FActiveNamespace: TDFXLeafNamespace;
    FActiveNamespaceNode: TTreeNode;
    FActiveEffect: TDFXEffect;
    FActiveEffectPass: TDFXEffectPass;
    FActiveShaderFunction: PDFXShaderFunction;

    // Dirty
    FIsAutoSaving: Boolean;
    FNodeConstants: TTreeNode;

    // GUI
    procedure ActiveControlChanged(Sender: TObject);
    procedure RefreshShaderReferences;
    procedure SaveProjectUI;
    procedure LoadProjectUI;

    // Project
    procedure CreateNewProject(const Directory: string);

    // Build
    procedure InternalBuildProject(Mode: TBuildThreadMode);

    // Forms
    procedure ShowAboutDialog;
    procedure ShowNewProjectDialog;
    procedure ShowOpenProjectDialog;
    procedure ShowToolsDialog(PageIndex: Integer = -1);
    procedure ShowProjectOptionsDialog;

    // NamespaceView
    function FindNode(Node: TTreeNode; Data: Pointer): TTreeNode; overload;
    function FindNode(View: TTreeView; Data: Pointer): TTreeNode; overload;
    function AddFolder(Node: TTreeNode; const Name: string; Namespace: TDFXLeafNamespace): TTreeNode;
    procedure AddDummyNode(const Name: string; ImageIndex: Integer; const ChildName: string = ''; Node: TTreeNode = nil);
    function ReplaceDummyNode(Node: TTreeNode; Text: string): Boolean;
    procedure UpdateNodeText(P: Pointer; const Text: string);

    // Effect view
    function ActiveShaderButtonGroup: TButtonGroup;

    // Threading
    function CreateBlockingThreadWork(WorkerThread: TThread;
      DialogOptions: TDialogOptions): TModalResult;
    // App Settings
    procedure LoadSettings;
    procedure SaveSettings;
    // Drag & Drop
    procedure OnDropFiles(var Msg: TMessage); message WM_DROPFILES;
    procedure OnDropFile(const FileName: string; Size: Integer);

    // Text Editor
    procedure NewTextTab(Data: Pointer; const Name: string; ImageIndex: Integer); overload;
    procedure NewTextTab(Data: Pointer; const Name, Text: string; ImageIndex: Integer); overload;
    procedure SynEditStatusChange(Sender: TObject; Changes: TSynStatusChanges);
    procedure SynEditKeyPress(Sender: TObject; var Key: Char);
    procedure SaveTab(Tab: TIceTab);
    procedure ForceCloseTab(T: TIceTab);
    procedure OnSynEditKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure OnProcessSynEditCommand(Sender: TObject; var Command: TSynEditorCommand;
      var AChar: Char; Data: Pointer);


    // Callbacks
    procedure RefreshNamespaceStatus;
    procedure RefreshEffectLabel;

    // Properties
    procedure SetActiveNamespace(NewValue: TDFXLeafNamespace);
    procedure SetActiveEffect(NewValue: TDFXEffect);
    procedure SetActiveEffectPass(NewValue: TDFXEffectPass);

    function TabToEdit(T: TIceTab): TSynEdit;
    function TabData(T: TIceTab): TObject;
    function EditToTab(E: TSynEdit): TIceTab;

    procedure UpdateAutoTimers;
    procedure UpdateCaption(const Suffix: string = '');

    procedure SetProject(NewValue: TDFXProject);

    function IsTextAreaActive: Boolean;
    function IsEditableTextAreaActive: Boolean;
  public
    { Public declarations }
    property Project: TDFXProject read FProject write SetProject;
    property ThreadWorkForm: TThreadWorkForm read FThreadWorkForm;
    property ActiveNamespace: TDFXLeafNamespace read FActiveNamespace write SetActiveNamespace;
    property ActiveNamespaceNode: TTreeNode read FActiveNamespaceNode;
    property ActiveEffect: TDFXEffect read FActiveEffect write SetActiveEffect;
    property ActiveEffectPass: TDFXEffectPass read FActiveEffectPass write SetActiveEffectPass;

    // IDFXCompilerErrorLogger
    procedure OnError(const S: string);
    procedure OnLog(const S: string);
    function OnProgress: Boolean;
    procedure OnBeginBuildEffects;
    procedure OnBeginCompileShaders;
    procedure OnStatus(const Msg: string);

    procedure OnDoBeginBuildEffects;
    procedure OnDoBeginCompileShaders;

    // IDFXProjectListener
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

    // New
    function FindTab(Data: Pointer): TIceTab;
    function AddNamespace(Node: TTreeNode; const Namespace: TDFXNamespace): TTreeNode;
    function AddNode(Node: TTreeNode; const Text: string; P: Pointer; ImageIndex: Integer): TTreeNode;
    function CloseProject: Boolean;
    procedure UpdateStatus(const Text: string);
    procedure OpenProject(const ProjectDir: string);
    procedure ShowError(const Msg: string);
    function PickDirectory(var Dir: string): Boolean;
    function Ask(const Msg: string; DlgType: TMsgDlgType;
      Buttons: TMsgDlgButtons): TModalResult;
    procedure Log(const Text: string);
    function EffectCount(Namespace: TDFXNamespace): Integer; overload;
    function EffectCount: Integer; overload;
    function ShaderCount(Namespace: TDFXNamespace): Integer; overload;
    function ShaderCount: Integer; overload;
  end;

var
  MainForm: TMainForm;

implementation

{$R *.dfm}

uses
  NewProjectWindow, AboutWindow, ErrorWindow, OptionsWindow, ProjectOptionsWindow;

const
  kIniFileSettings = 'settings.ini';

  kImageNamespace = 0;
  kImageFolder = 1;
  kImageEffect = 2;
  kImageFunction = 3;
  kImageConstantBuffer = 4;
  kImageConstants = 5;
  kImageConstant = 6;
  kImageResource = 7;
  kImageStructure = 8;
  kImageHotFolder = 9;
  kImageShader = 10;
  kImageSampler = 11;
  kImageBuffer = 12;
  kImageInactiveEffect = 13;
  kImageDASM = 14;

  kToolsPageGeneral = 0;
  kToolsPageTools = 1;

  kProjectOptionsPageGeneral = 0;
  kProjectOptionsPageHLSL = 1;
  kProjectOptionsPagePaths = 2;
  kProjectOptionsPagePreProcessing = 3;

  kFolderEffects = 'Effects';
  kFolderFunctions = 'Functions';
  kFolderConstantBuffers = 'Constant Buffers';
  kFolderResources = 'Resources';
  kFolderStructures = 'Structures';

  kSectionConstants = 'Constants';

procedure TMainForm.InternalBuildProject(Mode: TBuildThreadMode);
var
  wt: TBuildThread;
begin
  acSave.Execute;
  OutputMemo.Clear;

  wt := TBuildThread.Create(Mode);
  CreateBlockingThreadWork(wt, [doStatus]);
end;

function TMainForm.IsEditableTextAreaActive: Boolean;
begin
  Result := Assigned(ActiveControl) and (
            (ActiveControl is TCustomEdit and not TCustomEdit(ActiveControl).ReadOnly) or
            (ActiveControl is TMemo and not TMemo(ActiveControl).ReadOnly) or
            (ActiveControl is TRichEdit and not TRichEdit(ActiveControl).ReadOnly) or
            (ActiveControl is TSynEdit and not TSynEdit(ActiveControl).ReadOnly)
            );
end;

function TMainForm.IsTextAreaActive: Boolean;
begin
  Result := Assigned(ActiveControl) and (
            (ActiveControl is TCustomEdit) or
            (ActiveControl is TMemo) or
            (ActiveControl is TRichEdit) or
            (ActiveControl is TSynEdit)
            );
end;

function CountIndentSpaces(const S: string): Integer;
var
  i: Integer;
begin
  Result := 0;
  for i := 1 to S.Length do
    if S[i] = ' ' then
      Inc(Result)
    else
      Break;
end;

function IsTextItem(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (TObject(Node.Data) is TDFXTextItem);
end;

function IsFolder(Node: TTreeNode): Boolean;
begin
  Result := Assigned(Node) and (Node.ImageIndex in [kImageFolder, kImageHotFolder]);
end;

function IsNamespace(Node: TTreeNode): Boolean;
begin
  Result := Assigned(Node) and (Node.ImageIndex = kImageNamespace);
end;

function IsEffect(Node: TTreeNode): Boolean;
begin
  Result := Assigned(Node) and (Node.ImageIndex in [kImageEffect, kImageInactiveEffect]);
end;

function IsFunction(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (Node.ImageIndex in [kImageFunction, kImageShader]);
end;

function IsShader(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (Node.ImageIndex = kImageShader);
end;

function IsConstant(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (Node.ImageIndex = kImageConstant);
end;

function IsResource(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (Node.ImageIndex in [kImageResource, kImageSampler, kImageBuffer]);
end;

function IsConstantBuffer(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (Node.ImageIndex = kImageConstantBuffer);
end;

function IsStructure(Node: TTreeNode): Boolean; overload;
begin
  Result := Assigned(Node) and (Node.ImageIndex = kImageStructure);
end;

function FindNearestNamespace(Node: TTreeNode): TTreeNode;
begin
  if IsNamespace(Node) or IsEffect(Node) then
    Exit(Node);

  if not Assigned(Node.Parent) then
    Exit(nil);

  Result := FindNearestNamespace(Node.Parent);
end;

function FindFilter(Node: TTreeNode; const S: string): TTreeNode;
var
  c: TTreeNode;
begin
  c := Node.getFirstChild;
  while Assigned(c) do
  begin
    if c.Text = S then
      Exit(c);

    c := c.getNextSibling;
  end;

  Result := MainForm.AddFolder(Node, S, Node.Data);
end;

function NamespaceSort(lParam1, lParam2, lParamSort: LPARAM): Integer stdcall;
var
  n1, n2: TTreeNode;
begin
  n1 := TTreeNode(lParam1);
  n2 := TTreeNode(lParam2);

  if not Assigned(n1) or not Assigned(n2) then
    Exit(0);

  if IsFolder(n1) and not IsFolder(n2) then
    Exit(0);

  if IsFolder(n2) and not IsFolder(n1) then
    Exit(1);

  Result := StrIComp(PChar(n1.Text), PChar(n2.Text));
end;

procedure BoldTreeNode(TreeNode: TTreeNode; Value: Boolean);
var
  ti: TTVItem;
begin
  if not Assigned(TreeNode) then
    Exit;

  with ti do
  begin
    hItem := TreeNode.ItemId;
    stateMask := TVIS_BOLD;
    mask := TVIF_HANDLE or TVIF_STATE;
    if Value then
      state := TVIS_BOLD
    else
      state := 0;
  end;

  TreeView_SetItem(TreeNode.Handle, ti);
end;

function FindNodeByPath(Node: TTreeNode; PathFolders: TStringList): TTreeNode; overload;
begin
  if (not Assigned(Node)) or (Node.Level >= PathFolders.Count) then
    Exit(nil);

  while Assigned(Node) do
  begin
    if CompareText(Node.Text, PathFolders[Node.Level]) = 0 then
    begin
      if Node.Level = PathFolders.Count - 1 then
        Exit(Node)
      else
        Exit(FindNodeByPath(Node.getFirstChild, PathFolders));
    end;

    Node := Node.getNextSibling;
  end;

  Result := nil;
end;

function FindNodeByPath(View: TTreeView; PathFolders: TStringList): TTreeNode; overload;
var
  n: TTreeNode;
begin
  Result := nil;
  n := View.Items.GetFirstNode;
  while Assigned(n) do
  begin
    Result := FindNodeByPath(n, PathFolders);
    if Assigned(Result) then
      Exit;
    n := n.getNextSibling;
  end;
end;

procedure ExpandNode(View: TTreeView; Path: TStringList);
var
  n: TTreeNode;
begin
  n := FindNodeByPath(View, Path);
  if Assigned(n) then
    n.Expanded := True;
end;

function FindNextNode(Node: TTreeNode; const TextRegEx: TRegEx): TTreeNode;
var
  c: TTreeNode;
begin
  if not Assigned(Node) then
    Exit(nil);

  c := Node.getFirstChild;
  while Assigned(c) do
  begin
    if TextRegEx.IsMatch(c.Text) then
      Exit(c);

    Result := FindNextNode(c, TextRegEx);
    if Assigned(Result) then
      Exit;

    c := c.getNextSibling;
  end;

  Node := Node.getNextSibling;

  if Assigned(Node) then
    if TextRegEx.IsMatch(Node.Text) then
      Exit(Node);

  Result := FindNextNode(Node, TextRegEx);
end;

function TMainForm.CreateBlockingThreadWork(WorkerThread: TThread;
  DialogOptions: TDialogOptions): TModalResult;
begin
  while Assigned(FThreadWorkForm) do
  begin
    Sleep(1);
    Application.ProcessMessages;
  end;

  FThreadWorkForm := TThreadWorkForm.Create(Application);

  try
    FThreadWorkForm.DialogOptions := DialogOptions;
    FThreadWorkForm.WorkerThread := WorkerThread;
    Result := FThreadWorkForm.ShowModal;
  finally
    FreeAndNil(FThreadWorkForm);
  end;
end;

function IsChildWindowFocused(Root: TWinControl): Boolean;
var
  i: Integer;
begin
  if Root.Focused then
    Exit(True);

  for i := 0 to Root.ControlCount - 1 do
    if Root.Controls[i] is TWinControl then
      if IsChildWindowFocused(Root.Controls[i] as TWinControl) then
        Exit(True);

  Result := False;
end;

procedure TMainForm.PassNameEditChange(Sender: TObject);
begin
  if Assigned(ActiveEffectPass) then
    ActiveEffect.RenameEffectPass(ActiveEffectPass, TEdit(Sender).Text);
end;

procedure TMainForm.PassTypeComboBoxChange(Sender: TObject);
begin
  case PassTypeComboBox.ItemIndex of
  0:
    begin
      ActiveEffectPass.PassType := ptCompute;
      ComputeShaderButtonGroup.Show;
      GeometryShaderButtonGroup.Hide;
    end;
  1:
    begin
      ActiveEffectPass.PassType := ptGeometry;
      GeometryShaderButtonGroup.Show;
      ComputeShaderButtonGroup.Hide;
    end;
  end;

  if TComboBoxEx(Sender).ItemIndex <> -1 then
  begin
    ActiveShaderButtonGroup.Top := ActiveShaderReferenceLabel.Top;
    ActiveShaderButtonGroup.OnButtonClicked(ActiveShaderButtonGroup, 0);
  end;

  EffectListBox.Repaint;
end;

function TMainForm.PickDirectory(var Dir: string): Boolean;
var
  bi: TBrowseInfo;
  tmp: array [0 .. MAX_PATH - 1] of Char;
begin
  FillChar(bi, SizeOf(BrowseInfo), 0);
  bi.hwndOwner := Handle;
  bi.lpszTitle := 'Select an existing directory';
  bi.ulFlags := BIF_NEWDIALOGSTYLE;

  Result := SHGetPathFromIDList(SHBrowseForFolder(bi), tmp);

  if Result then
    Dir := tmp;
end;

procedure TMainForm.ProjectOverviewSearchEditKeyPress(Sender: TObject;
  var Key: Char);
var
  TextRegEx: TRegEx;
begin
  if Key = #13 then
  begin
    Key := #0;

    if Length(TEdit(Sender).Text) > 0 then
    begin
      TextRegEx := TRegEx.Create(TEdit(Sender).Text, [roIgnoreCase]);

      if Assigned(NamespaceView.Selected) then
        NamespaceView.Select(FindNextNode(NamespaceView.Selected, TextRegEx))
      else
        NamespaceView.Select(FindNextNode(FindNode(NamespaceView, Project.RootNamespace), TextRegEx));
    end;
  end;
end;

procedure TMainForm.ProjectOverviewSearchEditRightButtonClick(Sender: TObject);
begin
  TEdit(Sender).Clear;
end;

procedure TMainForm.PropertyEditorItemValueChanged(Sender: TObject;
  Item: TJvCustomInspectorItem);
begin
  NamespaceView.Refresh;
end;

procedure TMainForm.PropertyEditorSplitterMoved(Sender: TObject);
begin
  PropertyEditorSplitter.Top := PropertyEditorPanel.Top;
end;

procedure TMainForm.RefreshEffectLabel;
begin
  if Assigned(ActiveEffect) then
  begin
    if ActiveEffect.Passes.Count <> 1 then
      EffectNameLabel.Caption := Format('%s (%d passes)', [ActiveEffect.Name, ActiveEffect.Passes.Count])
    else
      EffectNameLabel.Caption := Format('%s (1 pass)', [ActiveEffect.Name]);
  end
  else
    EffectNameLabel.Caption := 'No effect selected.';
end;

function TMainForm.ReplaceDummyNode(Node: TTreeNode; Text: string): Boolean;
begin
  Result := Node.Data = nil;

  if Result then
  begin
    try
      if IsNamespace(Node) then Project.AddNamespace(TDFXNamespace(ActiveNamespace), Text)
      else if IsEffect(Node) then ActiveEffect := TDFXNamespace(ActiveNamespace).CreateEffect(Text)
      else if IsFunction(Node) then ActiveNamespace.CreateFunction(Text).Text := 'void $()' + #13#10 + '{' + #13#10 + '}'
      else if IsConstantBuffer(Node) then ActiveNamespace.CreateConstantBuffer(Text)
      else if IsConstant(Node) then Project.AddConstant(Text)
      else if IsResource(Node) then ActiveNamespace.CreateResource(Text)
      else if IsStructure(Node) then ActiveNamespace.CreateStructure(Text);
    finally
      Node.Delete;
    end;
  end;
end;

procedure TMainForm.RightSplitterMoved(Sender: TObject);
begin
  RightSplitter.Left := RightBGPanel.Left - 8;
end;

procedure TMainForm.NamespaceViewCancelEdit(Sender: TObject; Node: TTreeNode);
begin
  ReplaceDummyNode(Node, Node.Text);
  NamespaceView.CustomSort(NamespaceSort, 0);
end;

procedure TMainForm.NamespaceViewChange(Sender: TObject; Node: TTreeNode);
var
  s: TTreeNode;
begin
  PropertyEditor.Clear;

  // Delete empty folder
  if IsFolder(Node) then
    if Node.getFirstChild = nil then
    begin
      Node.Delete;
      Exit;
    end;

  s := NamespaceView.Selected;
  // Property editor update
  if IsResource(s) or IsTextItem(s) then
    PropertyEditor.AddComponent(NamespaceView.Selected.Data);

  // Ignore dummy node
  if not Assigned(Node) or not Assigned(Node.Data) then
    Exit;

  // Switch namespace
  if Node.Selected then
  begin
    if IsConstant(Node) then
      Exit;

    Node := FindNearestNamespace(Node);
    if Assigned(Node) then
      ActiveNamespace := Node.Data
    else
      ActiveNamespace := nil;
  end;

  RefreshShaderReferences;
end;

procedure TMainForm.NamespaceViewDblClick(Sender: TObject);
var
  s: TTreeNode;
begin
  s := NamespaceView.Selected;

  if not Assigned(s) then
    Exit;

  if IsTextItem(s) then
    NewTextTab(s.Data, s.Text, TDFXTextItem(s.Data).Text, s.ImageIndex);
end;

procedure TMainForm.NamespaceViewDragDrop(Sender, Source: TObject; X,
  Y: Integer);
var
  src, dst, dstNSNode: TTreeNode;
  dstNS: TDFXNamespace;
  doMove: Boolean;
begin
  src := NamespaceView.Selected;
  dst := FindNearestNamespace(NamespaceView.GetNodeAt(X, Y));

  if not Assigned(dst) then
    Exit;

  if IsNamespace(src) and IsEffect(dst) then
    Exit;

  dstNSNode := dst;
  dstNS := dst.Data;

  if IsEffect(src) then
  begin
    dst := FindFilter(dst, kFolderEffects);
    doMove := dstNS.MoveEffect(src.Data);
  end
  else if IsFunction(src) then
  begin
    dst := FindFilter(dst, kFolderFunctions);
    doMove := dstNS.MoveFunction(src.Data);
  end
  else if IsConstantBuffer(src) then
  begin
    dst := FindFilter(dst, kFolderConstantBuffers);
    doMove := dstNS.MoveConstantBuffer(src.Data);
  end
  else if IsNamespace(src) then
    doMove := dstNS.MoveNamespace(src.Data)
  else if IsResource(src) then
  begin
    dst := FindFilter(dst, kFolderResources);
    doMove := dstNS.MoveResource(src.Data)
  end
  else if IsStructure(src) then
  begin
    dst := FindFilter(dst, kFolderStructures);
    doMove := dstNS.MoveStructure(src.Data);
  end
  else
    doMove := False;

  if doMove then
  begin
    src.MoveTo(dst, naAddChild);
    ActiveNamespace := dstNSNode.Data;
    NamespaceView.CustomSort(NamespaceSort, 0);
    RefreshShaderReferences;
  end;
end;

procedure TMainForm.NamespaceViewDragOver(Sender, Source: TObject; X,
  Y: Integer; State: TDragState; var Accept: Boolean);
var
  src, dst: TTreeNode;
begin
  src := NamespaceView.Selected;
  dst := NamespaceView.GetNodeAt(X, Y);
  Accept := (Source = NamespaceView) and
            (Assigned(dst)) and
            (Assigned(FindNearestNamespace(dst))) and
            (FindNearestNamespace(dst) <> FindNearestNamespace(src)) and
            (IsNamespace(src) or
            IsEffect(src) or
            IsFunction(src) or
            IsConstantBuffer(src) or
            IsResource(src) or
            IsStructure(src)
            );
end;

procedure TMainForm.NamespaceViewEdited(Sender: TObject; Node: TTreeNode;
  var S: string);
var
  d, pd: Pointer;
  r: Boolean;
begin
  r := not ReplaceDummyNode(Node, S); // Try replacing dummy node

  if r then
  begin
    d := Node.Data;
    pd := Node.Parent.Data;

    if IsNamespace(Node) then
      Project.RenameNamespace(d, S)
    else if IsEffect(Node) then
      TDFXNamespace(pd).RenameEffect(d, S)
    else if IsFunction(Node) then
      TDFXLeafNamespace(pd).RenameFunction(d, S)
    else if IsConstantBuffer(Node) then
      TDFXLeafNamespace(pd).RenameConstantBuffer(d, S)
    else if IsConstant(Node) then
      Project.RenameConstant(d, S)
    else if IsResource(Node) then
      TDFXLeafNamespace(pd).RenameResource(d, S)
    else if IsStructure(Node) then
      TDFXLeafNamespace(pd).RenameStructure(d, S)
    else
      raise Exception.Create('Developer Error! Namespaceview item should not be editable!');
  end;

  // Update property editor
  PropertyEditor.SetFocus;
  NamespaceView.SetFocus;
end;

procedure TMainForm.NamespaceViewEditing(Sender: TObject; Node: TTreeNode;
  var AllowEdit: Boolean);
begin
  AllowEdit := Assigned(Node.Parent) and not IsFolder(Node);
end;

procedure TMainForm.NamespaceViewGetImageIndex(Sender: TObject;
  Node: TTreeNode);
begin
  if IsFolder(Node) then
  begin
    if Node.Count > 0 then
      Node.ImageIndex := kImageHotFolder
    else
      Node.ImageIndex := kImageFolder;
  end
  else if Assigned(Node.Data) then
  begin
    if IsFunction(Node) then
    begin
      if TDFXFunction(Node.Data).ShaderProfile <> '' then
        Node.ImageIndex := kImageShader
      else
        Node.ImageIndex := kImageFunction;
    end
    else if IsResource(Node) then
    begin
      if TDFXResource(Node.Data).IsSamplerStateType then
        Node.ImageIndex := kImageSampler
      else if TDFXResource(Node.Data).IsTexture then
        Node.ImageIndex := kImageResource
      else
        Node.ImageIndex := kImageBuffer;
    end
    else if IsEffect(Node) then
    begin
      if TDFXEffect(Node.Data).ParticipateInBuild then
        Node.ImageIndex := kImageEffect
      else
        Node.ImageIndex := kImageInactiveEffect;
    end;
  end;

  Node.SelectedIndex := Node.ImageIndex;
end;

procedure TMainForm.NamespaceViewKeyPress(Sender: TObject; var Key: Char);
begin
  if Key = #13 then
  begin
    Key := #0;

    NamespaceView.OnDblClick(Sender);
  end;
end;

procedure TMainForm.NamespaceViewKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if not NamespaceView.IsEditing then
    NamespaceView.CustomSort(NamespaceSort, 0);
end;

procedure TMainForm.NamespaceViewMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
  s: TTreeNode;
begin
  NamespaceView.Selected := NamespaceView.GetNodeAt(X, Y);

  NamespaceView.PopupMenu := nil;
  s := NamespaceView.Selected;

  if IsNamespace(s) or IsFolder(s) then
    NamespaceView.PopupMenu := NamespacePopup
  else if IsEffect(s) then
  begin
    if TDFXEffect(s.Data).ParticipateInBuild then
      NamespaceView.PopupMenu := InactiveEffectPopup
    else
      NamespaceView.PopupMenu := ActiveEffectPopup;
  end
  else if IsShader(s) then
    NamespaceView.PopupMenu := ShaderPopup
  else if IsTextItem(s) then
    NamespaceView.PopupMenu := TextItemPopup
  else if IsResource(s) then
    NamespaceView.PopupMenu := ItemPopup;
end;

procedure TMainForm.NewTextTab(Data: Pointer; const Name, Text: string;
  ImageIndex: Integer);
var
  c: TCollectionItem;
  t: TIceTab;
  e: TSynEdit;
  o: set of TSynEditorOption;
begin
  for c in TabSet.Tabs do
    if TIceTab(c).Tag = NativeUint(Data) then
    begin
      TIceTab(c).Selected := True;
      Exit;
    end;

  t := TabSet.AddTab(Name, ImageIndex);
  t.Tag := NativeInt(Data);
  t.Selected := True;

  e := TSynEdit.Create(TextEditorPanel);
  e.AddKey(ecDeleteWord, VK_DELETE, [ssCtrl]);
  e.AddKey(ecBlockIndent, VK_TAB, []);
  e.AddKey(ecBlockUnindent, VK_TAB, [ssShift]);
  e.OnKeyDown := OnSynEditKeyDown;
  e.Color := clWebWhiteSmoke;
  e.SearchEngine := SynEditSearch;
  e.Font.Name := 'Consolas';
  e.Tag := NativeInt(t);
  e.OnStatusChange := SynEditStatusChange;
  e.OnKeyPress := SynEditKeyPress;
  e.Parent := TextEditorPanel;
  e.WantTabs := True;
  e.TabWidth := 4;
  e.Highlighter := FSynHLSLSyn;
  e.Gutter.ShowLineNumbers := True;
  e.Gutter.BorderColor := clSilver;
  e.ActiveLineColor := $EAEAEA; //$F5F5F5;
  e.BorderStyle := bsNone;
  e.Align := alClient;
  e.AlignWithMargins := True;
  e.Margins.Top := 1;
  e.Margins.Bottom := 0;
  e.Margins.Left := 0;
  e.Margins.Right := 0;
  e.OnMouseWheelUp := OnMouseWheelUp;
  e.OnMouseWheelDown := OnMouseWheelDown;
  e.RightEdge := 120;

  o := e.Options;
  Exclude(o, eoSmartTabs);
  Include(o, eoTabIndent);
  Include(o, eoTrimTrailingSpaces);
  e.Options := o;

  t.Data := e;
  e.SetFocus;
  e.Text := Text;

  e.OnProcessCommand := OnProcessSynEditCommand;
end;

function GetNodePath(N: TTreeNode; Delim: Char): string;
begin
  if not Assigned(N) then
    Exit('');

  Result := N.Text;

  while Assigned(N.Parent) do
  begin
    Result := N.Parent.Text + PathDelim + Result;
    N := N.Parent;
  end;
end;

procedure FillExpandedNodes(Node: TTreeNode; Output: TStringList); overload;
var
  c: TTreeNode;
begin
  if Node.Expanded then
    Output.Add(GetNodePath(Node, PathDelim));

  c := Node.getFirstChild;
  while Assigned(c) do
  begin
    FillExpandedNodes(c, Output);
    c := c.getNextSibling;
  end;
end;

procedure FillExpandedNodes(View: TTreeView; Output: TStringList); overload;
var
  n: TTreeNode;
begin
  n := View.Items.GetFirstNode;
  while Assigned(n) do
  begin
    FillExpandedNodes(n, Output);
    n := n.getNextSibling;
  end;
end;

procedure TMainForm.NewTextTab(Data: Pointer; const Name: string; ImageIndex: Integer);
begin
  NewTextTab(Data, Name, '', ImageIndex);
end;

procedure TMainForm.SaveProjectUI;
var
  ini: TIniFile;
  s: string;
  sl: TStringList;
  item: TDFXTextItem;
  nsItem: TDFXNamespacedTextItem;
  i: Integer;
  d: TObject;
begin
  s := Project.ExpandedRootDir + 'project.ui';
  DeleteFile(s);
  ini := TIniFile.Create(s);
  sl := TStringList.Create;

  try
    // NamespaceView
    FillExpandedNodes(NamespaceView, sl);
    for i := 0 to sl.Count - 1 do
      ini.WriteString('Nodes', sl[i], 'true');

    // Selected
    ini.WriteString('Selected', 'Node', GetNodePath(NamespaceView.Selected, PathDelim));

    // Open tabs
    for i := 0 to TabSet.Tabs.Count - 1 do
    begin
      d := TabData(TabSet.Tabs[i]);
      if Assigned(d) then
      begin
        item := TDFXTextItem(d);
        if item is TDFXNamespacedTextItem then
        begin
          nsItem := TDFXNamespacedTextItem(item);
          ini.WriteString('Tabs', 'Global' + PathDelim + nsItem.DiskPath, 'true');
        end
        else
          ini.WriteString('Tabs', 'Constants' + PathDelim + item.Name, 'true');
      end;
    end;

    // Active Tab
    ini.WriteInteger('ActiveTab', 'Index', TabSet.TabIndex);
  finally
    sl.Free;
    ini.Free;
  end;
end;

procedure TMainForm.LoadProjectUI;
var
  ini: TIniFile;
  sl, del: TStringList;
  n: TTreeNode;
  i: Integer;
begin
  ini := TIniFile.Create(Project.ExpandedRootDir + 'project.ui');
  sl := TStringList.Create;
  del := TStringList.Create;

  try
    del.Delimiter := PathDelim;
    sl.Delimiter := PathDelim;
    sl.StrictDelimiter := True;
    del.StrictDelimiter := True;

    // NamespaceView
    ini.ReadSectionValues('Nodes', sl);
    for i := 0 to sl.Count - 1 do
    begin
      del.DelimitedText := sl.Names[i];
      ExpandNode(NamespaceView, del);
    end;

    // Selected
    del.Clear;
    del.DelimitedText := ini.ReadString('Selected', 'Node', '');
    n := FindNodeByPath(NamespaceView, del);
    if Assigned(n) then
      n.Selected := True;

    // Opened
    sl.Clear;
    ini.ReadSectionValues('Tabs', sl);
    for i := 0 to sl.Count - 1 do
    begin
      del.DelimitedText := sl.Names[i];
      n := FindNodeByPath(NamespaceView, del);
      if Assigned(n) then
      begin
        n.Selected := True;
        acOpen.Execute;
      end;
    end;

    // Active Tab
    i := ini.ReadInteger('ActiveTab', 'Index', -1);
    if (i >= 0) and (i < TabSet.Tabs.Count) then
      TabSet.TabIndex := i;
  finally
    sl.Free;
    del.Free;
    ini.Free;
  end;
end;

procedure TMainForm.SaveSettings;
var
  ini: TIniFile;
begin
  ini := TIniFile.Create(ExtractFilePath(Application.ExeName) +
    kIniFileSettings);

  try
    ini.WriteString('General', 'LastProject', FAppSettings.InitialProjectRootDir);
    ini.WriteBool('General', 'AutoSave', FAppSettings.DoAutoSave);
    ini.WriteInteger('General', 'AutoSaveInterval', FAppSettings.AutoSaveInterval);
    ini.WriteString('Tools', 'PreBuild', FAppSettings.PreBuild);
    ini.WriteString('Tools', 'PostBuild', FAppSettings.PostBuild);
    ini.WriteString('Tools', 'CppTranslator', GCPPScriptPath);

    ini.WriteInteger('General', 'WindowState', Integer(WindowState));
    ini.WriteInteger('General', 'X', Left);
    ini.WriteInteger('General', 'Y', Top);
    ini.WriteInteger('General', 'Width', Width);
    ini.WriteInteger('General', 'Height', Height);

    ini.WriteInteger('UI', 'LeftPane', LeftBGPanel.Width);
    ini.WriteInteger('UI', 'ProjectPane', PropertyEditorPanel.Height);
    ini.WriteInteger('UI', 'RightPane', RightBGPanel.Width);
    ini.WriteInteger('UI', 'EffectPane', PassPanel.Height);
    ini.WriteInteger('UI', 'OutputPane', OutputPanel.Height);
  finally
    ini.Free;
  end;
end;

procedure TMainForm.SaveTab(Tab: TIceTab);
var
  e: TSynEdit;
  d: TObject;
begin
  if Assigned(TabSet.Selected) then
  begin
    e := TabToEdit(Tab);

    d := TabData(Tab);
    if Assigned(d) then
      TDFXTextItem(d).Text := e.Text;

    e.Modified := False;
    Tab.Modified := False;
  end;
end;

procedure TMainForm.SetActiveEffect(NewValue: TDFXEffect);
var
  p: TDFXEffectPass;
begin
  if not Assigned(NewValue) then
  begin
    EffectListBox.Clear;
    ActiveEffectPass := nil;
  end;

  if NewValue <> FActiveEffect then
  begin
    FActiveEffect := NewValue;
    EffectListBox.Clear;

    if Assigned(ActiveEffect) then
      for p in ActiveEffect.Passes do
        OnAddEffectPass(p);
  end;

  RefreshEffectLabel;
end;

procedure TMainForm.SetActiveEffectPass(NewValue: TDFXEffectPass);
begin
  if not Assigned(NewValue) then
    PassTypeComboBox.ItemIndex := -1;

  PassTypeComboBox.Enabled := Assigned(NewValue);
  PassNameEdit.Enabled := Assigned(NewValue);
  ShaderEdit.Enabled := Assigned(NewValue);
  AttributesMemo.Enabled := Assigned(NewValue);

  if ActiveShaderButtonGroup <> nil then
    ActiveShaderButtonGroup.Visible := Assigned(NewValue);

  if FActiveEffectPass <> NewValue then
  begin
    FActiveEffectPass := NewValue;

    if Assigned(NewValue) then
    begin
      PassNameEdit.Text := FActiveEffectPass.Title;

      if FActiveEffectPass.PassType = ptCompute then
        PassTypeComboBox.ItemIndex := 0
      else
        PassTypeComboBox.ItemIndex := 1;

      ActiveShaderButtonGroup.OnButtonClicked(ActiveShaderButtonGroup, 0);
    end
    else
      PassNameEdit.Clear;
  end;
end;

procedure TMainForm.SetActiveNamespace(NewValue: TDFXLeafNamespace);
begin
  if (NewValue <> FActiveNamespace) or (not Assigned(FActiveNamespaceNode)) then
  begin
    BoldTreeNode(ActiveNamespaceNode, False);
    BoldTreeNode(FindNode(NamespaceView, NewValue), True);

    FActiveNamespace := NewValue;
    FActiveNamespaceNode := FindNode(NamespaceView, FActiveNamespace);
  end;

  RefreshNamespaceStatus;
end;

procedure TMainForm.SetProject(NewValue: TDFXProject);
var
  i: Integer;
begin
  if FProject = NewValue then
    Exit;

  PropertyEditor.Clear;
  ActiveNamespace := nil;
  ActiveEffect := nil;
  OutputMemo.Clear;

  FreeAndNil(FProject);

  for i := TabSet.Tabs.Count - 1 downto 0 do
    TabSet.OnTabClose(TabSet, TIceTab(TabSet.Tabs[i]));

  FProject := NewValue;

  // Reset timers
  UpdateAutoTimers;

  UpdateCaption;
end;

procedure TMainForm.ComputeShaderButtonGroupButtonClicked(Sender: TObject;
  Index: Integer);
  function ShaderIndexToPtr(Sender: TObject; I: Integer): PDFXShaderFunction;
  begin
    if Sender = ComputeShaderButtonGroup then
      Inc(I, 3); // SEE BELOW

    case I of
    0: Exit(@ActiveEffectPass.VertexShader);
    1: Exit(@ActiveEffectPass.PixelShader);
    2: Exit(@ActiveEffectPass.GeometryShader);
    // SEE ABOVE
    3: Exit(@ActiveEffectPass.ComputeShader);
    else Result := nil;
    end;
  end;
  function GetRelativeImageIndex(Current, ImageBase, Offset: Integer): Integer;
  begin
    Result := Offset + Current - Current mod ImageBase;
  end;
var
  b: TGrpButtonItem;
  c: TCollectionItem;
  ib: Integer;
begin
  ib := TButtonGroup(Sender).Images.Count div TButtonGroup(Sender).Items.Count;

  // Deselect old button
  for c in TButtonGroup(Sender).Items do
  begin
    TGrpButtonItem(c).ImageIndex := GetRelativeImageIndex(TGrpButtonItem(c).ImageIndex, ib, 0);

    if ShaderIndexToPtr(Sender, c.Index).Reference.Length > 0 then
      TGrpButtonItem(c).ImageIndex := GetRelativeImageIndex(TGrpButtonItem(c).ImageIndex, ib, 2);
  end;

  // Highlight new button
  b := TButtonGroup(Sender).Items[Index];
  b.ImageIndex := GetRelativeImageIndex(TGrpButtonItem(b).ImageIndex, ib, 1);

  FActiveShaderFunction := ShaderIndexToPtr(Sender, Index);

  ShaderEdit.Text := Trim(FActiveShaderFunction.Reference);
  AttributesMemo.Text := Trim(FActiveShaderFunction.Attributes);
end;

function TMainForm.ShaderCount(Namespace: TDFXNamespace): Integer;
var
  i, j: Integer;
begin
  Result := 0;

  for i := 0 to Namespace.Functions.Count - 1 do
    if Namespace.Functions[i].ShaderProfile <> '' then
      Inc(Result);

  for i := 0 to Namespace.Effects.Count - 1 do
    for j := 0 to Namespace.Effects[i].Functions.Count - 1 do
      if Namespace.Effects[i].Functions[j].ShaderProfile <> '' then
        Inc(Result);

  for i := 0 to Namespace.Subnamespaces.Count - 1 do
    Inc(Result, ShaderCount(Namespace.Subnamespaces[i]));
end;

function TMainForm.ShaderCount: Integer;
begin
  if not Assigned(Project) then
    Exit(0);

  Result := ShaderCount(Project.RootNamespace);
end;

procedure TMainForm.ShaderEditChange(Sender: TObject);
var
  newRef: string;
begin
  newRef := Trim(TCustomEdit(Sender).Text);
  if newRef <> FActiveShaderFunction.Reference then
  begin
    FActiveShaderFunction.Reference := newRef;
    Project.Modified := True;
  end;

  if Project.RootNamespace.FindGlobalFunctionReference(FActiveShaderFunction.Reference) = nil then
    TButtonedEdit(Sender).Font.Color := clRed
  else
    TButtonedEdit(Sender).Font.Color := clGreen;
end;

procedure TMainForm.ShaderEditDragDrop(Sender, Source: TObject; X, Y: Integer);
begin
  TEdit(Sender).Text := TDFXNamespacedTextItem(NamespaceView.Selected.Data).FilePath('::');
end;

procedure TMainForm.ShaderEditDragOver(Sender, Source: TObject; X, Y: Integer;
  State: TDragState; var Accept: Boolean);
begin
  Accept := (Source = NamespaceView) and (IsFunction(NamespaceView.Selected));
end;

procedure TMainForm.ShaderEditLeftButtonClick(Sender: TObject);
var
  accept: Boolean;
begin
  TButtonedEdit(Sender).OnDragOver(Sender, NamespaceView, 0, 0, TDragState.dsDragEnter, accept);
  if accept then
    TButtonedEdit(Sender).OnDragDrop(Sender, NamespaceView, 0, 0);
end;

procedure TMainForm.ShaderEditRightButtonClick(Sender: TObject);
var
  f: TDFXFunction;
begin
  f := Project.RootNamespace.FindGlobalFunctionReference(TCustomEdit(Sender).Text);
  if Assigned(f) then
  begin
    FindNode(NamespaceView, f).Selected := True;
    acOpen.Execute;
  end;
end;

procedure TMainForm.ShowAboutDialog;
var
  f: TAboutForm;
begin
  f := TAboutForm.Create(Application);

  try
    f.ShowModal;
  finally
    f.Free;
  end;
end;

procedure TMainForm.ShowError(const Msg: string);
begin
  Ask(Msg, mtError, [mbOk]);
end;

procedure TMainForm.ShowNewProjectDialog;
var
  f: TNewProjectForm;
begin
  f := TNewProjectForm.Create(Application);

  try
    if f.ShowModal = mrOk then
      CreateNewProject(f.ProjectDirectoryEdit.Text);
  finally
    f.Free;
  end;
end;

procedure TMainForm.ShowOpenProjectDialog;
var
  Dir: string;
begin
  if PickDirectory(Dir) then
    OpenProject(Dir);
end;

procedure TMainForm.ShowProjectOptionsDialog;
var
  f: TProjectOptionsForm;
begin
  f := TProjectOptionsForm.Create(Application);

  try
    f.OutputDirEdit.Text := Project.OutputDir;
    f.PreambleMemo.Text := Project.Preamble;
    f.ShaderDirEdit.Text := Project.ShaderDir;
    f.MinimizeShaderCheckBox.Checked := Project.MinimizeShaders;
    f.HLSLCompilerFlags := Project.HLSLCompilerFlags;
    f.PageView.ItemIndex := FLastProjectOptionsPage;

    if f.ShowModal = mrOk then
    begin
      Project.OutputDir := f.OutputDirEdit.Text;
      Project.Preamble := f.PreambleMemo.Text;
      Project.ShaderDir := f.ShaderDirEdit.Text;
      Project.MinimizeShaders := f.MinimizeShaderCheckBox.Checked;
      Project.HLSLCompilerFlags := f.HLSLCompilerFlags;
    end;

    FLastProjectOptionsPage := f.PageView.ItemIndex;
  finally
    f.Free;
  end;
end;

procedure TMainForm.ShowToolsDialog(PageIndex: Integer);
var
  f: TOptionsForm;
begin
  if PageIndex = -1 then
    PageIndex := FLastOptionsPage;

  f := TOptionsForm.Create(Application);

  try
    f.AutoSaveCheckBox.Checked := FAppSettings.DoAutoSave;
    f.AutoSaveSpinEdit.Value := FAppSettings.AutoSaveInterval;
    f.PreBuildEdit.Text := FAppSettings.PreBuild;
    f.PostBuildEdit.Text := FAppSettings.PostBuild;
    f.CppTranslatorEdit.Text := GCPPScriptPath;

    f.ActivePage := PageIndex;
    if f.ShowModal = mrOk then
    begin
      FAppSettings.DoAutoSave := f.AutoSaveCheckBox.Checked;
      FAppSettings.AutoSaveInterval := f.AutoSaveSpinEdit.Value;
      FAppSettings.PreBuild := f.PreBuildEdit.Text;
      FAppSettings.PostBuild := f.PostBuildEdit.Text;
      GCPPScriptPath := f.CppTranslatorEdit.Text;

      UpdateAutoTimers;
    end;

    FLastOptionsPage := f.ActivePage;
  finally
    f.Free;
  end;
end;

procedure TMainForm.UpdateAutoTimers;
begin
  AutoSaveTimer.Enabled := FAppSettings.DoAutoSave;
  AutoSaveTimer.Interval := FAppSettings.AutoSaveInterval * 1000 * 60;
end;

procedure TMainForm.UpdateCaption(const Suffix: string);
var
  prefix: string;
begin
  {$IFDEF DEBUG}
  prefix := ' (Debug Build) ';
  {$ELSE}
  prefix := '';
  {$ENDIF}

  if Assigned(Project) then
    Caption := 'DirectX FX Studio' + prefix + ' - [' + ExcludeTrailingPathDelimiter(Project.RootDir) + ']' + Suffix
  else
    Caption := 'DirectX FX Studio' + prefix + Suffix;
end;

procedure TMainForm.UpdateStatus(const Text: string);
begin
  StatusBar.Panels[0].Text := Text;
end;

procedure TMainForm.VignetteImageClick(Sender: TObject);
begin
  TabSet.SetFocus;
end;

procedure TMainForm.VignetteImageDragDrop(Sender, Source: TObject; X,
  Y: Integer);
begin
  NamespaceView.OnDblClick(NamespaceView);
end;

procedure TMainForm.VignetteImageDragOver(Sender, Source: TObject; X,
  Y: Integer; State: TDragState; var Accept: Boolean);
var
  s: TTreeNode;
begin
  s := NamespaceView.Selected;
  Accept := (Source = NamespaceView) and (IsTextItem(s));
end;

procedure TMainForm.OnDropFile(const FileName: string; Size: Integer);
begin
  if CloseProject then
  begin
    if DirectoryExists(FileName) then
      OpenProject(FileName)
    else if ExtractFileExt(FileName) = '.dfx' then
      OpenProject(ExtractFilePath(FileName));
  end;
end;

procedure TMainForm.OnDropFiles(var Msg: TMessage);
var
  Size: Integer;
  FileName: PChar;
begin
  inherited;
  if DragQueryFile(Msg.WParam, $FFFFFFFF, nil, 0) > 0 then
  begin
    Size := DragQueryFile(Msg.WParam, 0, nil, 0) + 1;
    FileName := StrAlloc(Size);
    try
      DragQueryFile(Msg.WParam, 0, FileName, Size);
      OnDropFile(StrPas(FileName), Size);
    finally
      StrDispose(FileName);
    end;
  end;
  DragFinish(Msg.WParam);
end;

procedure TMainForm.OnError(const S: string);
var
  t: TBuildThread;
begin
  t := TBuildThread(MainForm.ThreadWorkForm.WorkerThread);
  t.FMsg := S;
  t.Synchronize(t.LogError);
end;

procedure TMainForm.acNewProjectExecute(Sender: TObject);
begin
  ShowNewProjectDialog;
end;

procedure TMainForm.acOpenProjectExecute(Sender: TObject);
begin
  ShowOpenProjectDialog;
end;

procedure TMainForm.acRedoExecute(Sender: TObject);
begin
  TSynEdit(ActiveControl).Redo;
end;

procedure TMainForm.acRedoUpdate(Sender: TObject);
var
  b: Boolean;
begin
  b := False;

  if ActiveControl is TSynEdit then
    b := TSynEdit(ActiveControl).CanRedo;

  TAction(Sender).Enabled := b;
end;

procedure TMainForm.acRenameExecute(Sender: TObject);
begin
  NamespaceView.Selected.EditText;
end;

procedure TMainForm.acRenameUpdate(Sender: TObject);
var
  s: TTreeNode;
begin
  s := NamespaceView.Selected;
  TAction(Sender).Enabled := not IsFolder(s) and Assigned(s.Parent);
end;

procedure TMainForm.acRunExecute(Sender: TObject);
begin
  acBuild.Execute;
end;

procedure TMainForm.acSaveAsExecute(Sender: TObject);
var
  s: string;
begin
  if not PickDirectory(s) then
    Exit;

  Project.Save(s);
  SaveProjectUI;
  UpdateStatus('Saved.');
end;

procedure TMainForm.acSaveAsUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := Assigned(Project);
end;

procedure TMainForm.acSaveExecute(Sender: TObject);
var
  t: TCollectionItem;
begin
  // Save all tabs
  for t in TabSet.Tabs do
    SaveTab(TIceTab(t));

  if not Project.HasValidRootDirectory then
    acSaveAs.Execute
  else
  begin
    Project.Save;
    SaveProjectUI;
    UpdateStatus('Saved.');
  end;
end;

procedure TMainForm.acSaveUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := Assigned(Project) or (ActiveControl = OutputMemo);
end;

procedure TMainForm.acSelectAllExecute(Sender: TObject);
var
  wc: TWinControl;
begin
  wc := FindControl(GetFocus);
  if wc is TSynEdit then
    TSynEdit(wc).SelectAll
  else
    SendMessage(wc.Handle, EM_SETSEL, 0, -1);
end;

procedure TMainForm.acSelectAllUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := IsTextAreaActive;
end;

procedure TMainForm.acShowDisassemblyExecute(Sender: TObject);
begin
  InternalBuildProject(btmQueryDASM);
end;

procedure TMainForm.acStatusBarExecute(Sender: TObject);
begin
  StatusBar.Visible := acStatusBar.Checked;
end;

procedure TMainForm.ActiveControlChanged(Sender: TObject);
begin
  ProjectOverviewImage.Visible := IsChildWindowFocused(ProjectOverviewPanel);
  EffectsHeaderImage.Visible := IsChildWindowFocused(EffectsPanel);
  PassHeaderImage.Visible := IsChildWindowFocused(PassPanel);
  OutputHeaderImage.Visible := IsChildWindowFocused(OutputPanel);
  TextEditorHeaderImage.Visible := IsChildWindowFocused(TextEditorPanel);
  PropertyEditorHeaderImage.Visible := IsChildWIndowFocused(PropertyEditorPanel);
end;

function TMainForm.ActiveShaderButtonGroup: TButtonGroup;
begin
  if GeometryShaderButtonGroup.Visible then
    Result := GeometryShaderButtonGroup
  else if ComputeShaderButtonGroup.Visible then
    Result := ComputeShaderButtonGroup
  else
    Result := nil;
end;

procedure TMainForm.acToolsOptionsExecute(Sender: TObject);
begin
  ShowToolsDialog(kToolsPageTools);
end;

procedure TMainForm.acUndoExecute(Sender: TObject);
begin
  if ActiveControl is TSynEdit then
    TSynEdit(ActiveControl).Undo
  else if ActiveControl is TCustomEdit then
    TCustomEdit(ActiveControl).Undo
  else if ActiveControl is TRichEdit then
    TRichEdit(ActiveControl).Undo
  else if ActiveControl is TMemo then
    TMemo(ActiveControl).Undo;
end;

procedure TMainForm.acUndoUpdate(Sender: TObject);
var
  b: Boolean;
begin
  b := False;

  if ActiveControl is TSynEdit then
    b := TSynEdit(ActiveControl).CanUndo
  else if ActiveControl is TCustomEdit then
    b := TCustomEdit(ActiveControl).CanUndo
  else if ActiveControl is TRichEdit then
    b := TRichEdit(ActiveControl).CanUndo
  else if ActiveControl is TMemo then
    b := TMemo(ActiveControl).CanUndo;

  TAction(Sender).Enabled := b;
end;

procedure TMainForm.AddDummyNode(const Name: string; ImageIndex: Integer;
  const ChildName: string; Node: TTreeNode);
var
  p, n: TTreeNode;
begin
  if not Assigned(Node) then
    p := ActiveNamespaceNode
  else
    p := Node;

  if ChildName <> '' then
    p := FindFilter(p, ChildName);

  n := NamespaceView.Items.AddChild(p, Name);
  n.ImageIndex := ImageIndex;
  n.SelectedIndex := ImageIndex;
  NamespaceView.Select(n);
  n.EditText;
end;

function TMainForm.AddFolder(Node: TTreeNode; const Name: string;
  Namespace: TDFXLeafNamespace): TTreeNode;
begin
  Result := NamespaceView.Items.AddChildObject(Node, Name, Namespace);
  Result.ImageIndex := kImageFolder;
  Result.SelectedIndex := Node.ImageIndex;
end;

function TMainForm.AddNamespace(Node: TTreeNode; const Namespace: TDFXNamespace): TTreeNode;
var
  p: TTreeNode;
  ns: TDFXNamespace;
begin
  p := NamespaceView.Items.AddChildObject(Node, Namespace.Name, Namespace);
  p.ImageIndex := kImageNamespace;
  p.SelectedIndex := p.ImageIndex;

  for ns in Namespace.Subnamespaces do
    AddNamespace(p, ns);

  NamespaceView.Select(p);

  NamespaceView.CustomSort(NamespaceSort, 0);
  Result := p;
end;

function TMainForm.AddNode(Node: TTreeNode; const Text: string; P: Pointer;
  ImageIndex: Integer): TTreeNode;
var
  n: TTreeNode;
begin
  n := NamespaceView.Items.AddChildObject(Node, Text, P);

  n.ImageIndex := ImageIndex;
  n.SelectedIndex := n.ImageIndex;

  NamespaceView.Select(n);

  NamespaceView.CustomSort(NamespaceSort, 0);
  Result := n;
end;

procedure TMainForm.acAboutGoProjectManagerExecute(Sender: TObject);
begin
  ShowAboutDialog;
end;

procedure TMainForm.acCreateConstantBufferExecute(Sender: TObject);
begin
  AddDummyNode('NewConstantBuffer', kImageConstantBuffer, kFolderConstantBuffers);
end;

procedure TMainForm.acCreateConstantsExecute(Sender: TObject);
begin
  AddDummyNode('NewConstants', kImageConstant, '', FNodeConstants);
end;

procedure TMainForm.acCreateEffectExecute(Sender: TObject);
begin
  AddDummyNode('NewEffect', kImageEffect, kFolderEffects);
end;

procedure TMainForm.acCreateEffectUpdate(Sender: TObject);
var
  e: Boolean;
begin
  e := Assigned(Project) and Assigned(ActiveNamespace) and (not NamespaceView.IsEditing);

  // LeafNamespace
  if (Sender = acCreateEffect) or (Sender = acCreateNamespace) then
    e := e and IsNamespace(ActiveNamespaceNode);

  TAction(Sender).Enabled := e;
end;

procedure TMainForm.acCreateFunctionExecute(Sender: TObject);
begin
  AddDummyNode('NewFunction', kImageFunction, kFolderFunctions);
end;

procedure TMainForm.acCreateNamespaceExecute(Sender: TObject);
begin
  AddDummyNode('NewNamespace', kImageNamespace);
end;

procedure TMainForm.acCreateEffectPassExecute(Sender: TObject);
begin
  ActiveEffect.CreatePass('NewPass');
end;

procedure TMainForm.acCreateEffectPassUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := Assigned(ActiveEffect);
end;

procedure TMainForm.acCreateResourceExecute(Sender: TObject);
begin
  AddDummyNode('NewResource', kImageResource, kFolderResources);
end;

procedure TMainForm.acCreateStructureExecute(Sender: TObject);
begin
  AddDummyNode('NewStructure', kImageStructure, kFolderStructures);
end;

procedure TMainForm.acCutExecute(Sender: TObject);
begin
  if ActiveControl is TSynEdit then
    TSynEdit(ActiveControl).CutToClipboard
  else if ActiveControl is TCustomEdit then
    TCustomEdit(ActiveControl).CutToClipboard
  else if ActiveControl is TRichEdit then
    TRichEdit(ActiveControl).CutToClipboard
  else if ActiveControl is TMemo then
    TMemo(ActiveControl).CutToClipboard
end;

procedure TMainForm.acCutUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := IsEditableTextAreaActive;
end;

procedure TMainForm.acDeleteExecute(Sender: TObject);
var
  s: TTreeNode;
begin
  if NamespaceView.Focused then
  begin
    s := NamespaceView.Selected;

    if IsNamespace(s) then
      Project.DeleteNamespace(s.Data)
    else if IsEffect(s) then
      ActiveNamespace.Parent.DeleteEffect(s.Data)
    else if IsFunction(s) then
      ActiveNamespace.DeleteFunction(s.Data)
    else if IsConstantBuffer(s) then
      ActiveNamespace.DeleteConstantBuffer(s.Data)
    else if IsConstant(s) then
      Project.DeleteConstant(s.Data)
    else if IsResource(s) then
     ActiveNamespace.DeleteResource(s.Data)
    else if IsStructure(s) then
      ActiveNamespace.DeleteStructure(s.Data);
  end
  else if EffectListBox.Focused then
  begin
    if Assigned(ActiveEffectPass) then
      ActiveEffect.DeleteEffectPass(ActiveEffectPass);
  end;
end;

procedure TMainForm.acDeleteUpdate(Sender: TObject);
var
  s: TTreeNode;
begin
  s := NamespaceView.Selected;
  TAction(Sender).Enabled := (NamespaceView.Focused and (
                             (IsNamespace(s) and Assigned(s.Parent)) or
                             IsEffect(s) or
                             IsConstantBuffer(s) or
                             IsConstant(s) or
                             IsResource(s) or
                             IsFunction(s) or
                             IsStructure(s)
                             ))
                             or
                             (EffectListBox.Focused and (
                             Assigned(ActiveEffectPass)
                             ));
end;

procedure TMainForm.acBuildExecute(Sender: TObject);
begin
  if EffectCount(Project.RootNamespace) = 0 then
  begin
    Log('No effects to compile.');
    Exit;
  end;

  InternalBuildProject(btmBuildProject);
end;

procedure TMainForm.acBuildUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := Assigned(Project);
end;

procedure TMainForm.acCloseExecute(Sender: TObject);
begin
  TabSet.OnTabClose(TabSet, TabSet.Selected);
end;

procedure TMainForm.acCloseAllExecute(Sender: TObject);
begin
  CloseProject;
end;

procedure TMainForm.acCloseAllUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := Assigned(Project);
end;

procedure TMainForm.acCloseUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := Assigned(TabSet.Selected);
end;

procedure TMainForm.acCopyExecute(Sender: TObject);
begin
  if ActiveControl is TTreeView then
  begin
    if Assigned(TTreeView(ActiveControl).Selected) then
      Clipboard.SetTextBuf(PChar(TTreeView(ActiveControl).Selected.Text));
  end
  else
    PostMessage(GetFocus, WM_COPY, 0, 0);
end;

procedure TMainForm.acCopyUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := IsTextAreaActive or (ActiveControl is TTreeView);
end;

procedure TMainForm.acOpenExecute(Sender: TObject);
var
  s: TTreeNode;
begin
  s := NamespaceView.Selected;
  if IsEffect(s) then
    EffectListBox.OnDragDrop(Sender, NamespaceView, 0, 0)
  else
    NamespaceView.OnDblClick(Sender);
end;

procedure TMainForm.acOpenUpdate(Sender: TObject);
var
  s: TTreeNode;
begin
  s := NamespaceView.Selected;
  TAction(Sender).Enabled := IsFunction(s) or
                             IsEffect(s) or
                             IsConstantBuffer(s) or
                             IsConstant(s) or
                             IsStructure(s) or
                             IsResource(s);
end;

procedure TMainForm.acOptionsExecute(Sender: TObject);
begin
  ShowToolsDialog;
end;

procedure TMainForm.acPasteExecute(Sender: TObject);
begin
  SendMessage(GetFocus, WM_PASTE, 0, 0);
end;

procedure TMainForm.acPasteUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := IsEditableTextAreaActive and Clipboard.HasFormat(CF_TEXT);
end;

procedure TMainForm.acProjectOptionsExecute(Sender: TObject);
begin
  ShowProjectOptionsDialog;
end;

procedure TMainForm.acProjectOptionsGeneralExecute(Sender: TObject);
begin
  FLastProjectOptionsPage := TAction(Sender).Tag;
  ShowProjectOptionsDialog;
end;

procedure TMainForm.acExcludeFromBuildExecute(Sender: TObject);
begin
  TDFXEffect(NamespaceView.Selected.Data).ParticipateInBuild := False;
  NamespaceView.Refresh;
end;

procedure TMainForm.acExitExecute(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.acFindExecute(Sender: TObject);
begin
  FindDialog.Execute;
end;

procedure TMainForm.acFindUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := ActiveControl is TSynEdit;
end;

procedure TMainForm.acGeneralOptionsExecute(Sender: TObject);
begin
  ShowToolsDialog(kToolsPageGeneral);
end;

procedure TMainForm.acIncludeInBuildExecute(Sender: TObject);
begin
  TDFXEffect(NamespaceView.Selected.Data).ParticipateInBuild := True;
  NamespaceView.Refresh;
end;

procedure TMainForm.ApplicationEventsException(Sender: TObject; E: Exception);
var
  s: string;
begin
  s := E.Message;
  if (E.Message.Length > 0) and (E.Message[E.Message.Length] <> '.') then
    s := s + '.';
  ShowError(s);
  Log('[ERROR] ' + s + #13#10);
end;

procedure TMainForm.ApplicationEventsHint(Sender: TObject);
begin
  UpdateStatus(Application.Hint);
end;

procedure TMainForm.ApplicationEventsShortCut(var Msg: TWMKey;
  var Handled: Boolean);
begin
  if Msg.CharCode = VK_ESCAPE then
  begin
    if not Assigned(ThreadWorkForm) then
    begin
      Screen.ActiveForm.ModalResult := mrCancel;
      Handled := True;
    end;
  end
end;

function TMainForm.Ask(const Msg: string; DlgType: TMsgDlgType;
  Buttons: TMsgDlgButtons): TModalResult;
var
  f: TErrorForm;
begin
  f := TErrorForm.Create(Application);

  try
    f.ErrorMsg := Msg;
    f.DlgType := DlgType;
    f.Buttons := Buttons;
    Result := f.ShowModal;
  finally
    f.Free;
  end;
end;

procedure TMainForm.AttributesMemoChange(Sender: TObject);
var
  newAttr: string;
begin
  newAttr := Trim(TMemo(Sender).Text);
  if newAttr <> FActiveShaderFunction.Attributes then
  begin
    FActiveShaderFunction.Attributes := newAttr;
    Project.Modified := True;
  end;
end;

procedure TMainForm.AutoSaveTimerTimer(Sender: TObject);
begin
  if Assigned(Project) then
  begin
    FIsAutoSaving := True;
    acSave.Execute;
    FIsAutoSaving := False;
    UpdateStatus('Autosave.');
  end;
end;

procedure TMainForm.BGPaintBoxPaint(Sender: TObject);
var
  X, Y: Integer;
begin
  X := 0;
  while X <= BGPaintBox.Width do
  begin
    Y := 0;
    while Y <= BGPaintBox.Height do
    begin
      BGImageList.Draw(BGPaintBox.Canvas, X, Y, 0);
      Inc(Y, BGImageList.Height);
    end;
    Inc(X, BGImageList.Width);
  end;
end;

procedure TMainForm.CloseAll1Click(Sender: TObject);
var
  i: Integer;
begin
  for i := TabSet.Tabs.Count - 1 downto 0 do
    TabSet.OnTabClose(Sender, TabSet.Tabs[i]);
end;

function TMainForm.CloseProject: Boolean;
begin
  if Assigned(Project) then
    SaveProjectUI;

  // TODO: query if close OK
  Project := nil;

  Application.ProcessMessages;

  Result := True;
end;

procedure TMainForm.CreateNewProject(const Directory: string);
begin
  if CloseProject then
  begin
    try
      Project := TDFXProject.Create(Self, Directory, cmNew);
    except
      CloseProject;
    end;
  end;
end;

procedure TMainForm.DarkPainterSetItemColors(Item: TJvCustomInspectorItem;
  Canvas: TCanvas);
begin
  Canvas.Pen.Color := clBlack;
end;

function TMainForm.EditToTab(E: TSynEdit): TIceTab;
begin
  Result := TIceTab(E.Tag);
end;

function TMainForm.FindNode(View: TTreeView; Data: Pointer): TTreeNode;
var
  n: TTreeNode;
begin
  Result := nil;

  if not Assigned(Data) then
    Exit;

  n := View.Items.GetFirstNode;
  while Assigned(n) do
  begin
    Result := FindNode(n, Data);

    if Assigned(Result) then
      Exit;

    n := n.getNextSibling;
  end;
end;

procedure TMainForm.FindDialogFind(Sender: TObject);
var
  e: TSynEdit;
  i: Integer;
  o: TSynSearchOptions;
  up: Boolean;
begin
  if ActiveControl is TSynEdit then
  begin
    e := TSynEdit(ActiveControl);

    if frMatchCase in FindDialog.Options then
      o := o + [ssoMatchCase]
    else
      o := o - [ssoMatchCase];

    if frWholeWord in FindDialog.Options then
      o := o + [ssoWholeWord]
    else
      o := o - [ssoWholeWord];

    up := not (frDown in FindDialog.Options);

    e.SearchEngine.Options := o;
    e.SearchEngine.Pattern := FindDialog.FindText;
    e.SearchEngine.FindAll(e.Text);

    if e.SearchEngine.ResultCount > 0 then
    begin
      // Skip to next
      if up then
      begin
        for i := e.SearchEngine.ResultCount - 1 downto 0 do
        if (e.SearchEngine.Results[i] - 1) < e.SelStart then
        begin
          e.SelStart := e.SearchEngine.Results[i] - 1;
          e.SelLength := FindDialog.FindText.Length;
          Break;
        end;
      end
      else
      begin
        for i := 0 to e.SearchEngine.ResultCount - 1 do
        if (e.SearchEngine.Results[i] - 1) > e.SelStart then
        begin
          e.SelStart := e.SearchEngine.Results[i] - 1;
          e.SelLength := FindDialog.FindText.Length;
          Break;
        end;
      end;
    end;
  end;
end;

function TMainForm.FindNode(Node: TTreeNode; Data: Pointer): TTreeNode;
begin
  if not Assigned(Node) then
    Exit(nil);

  if Node.Data = Data then
    Exit(Node);

  Node := Node.getFirstChild;
  while Assigned(Node) do
  begin
    Result := FindNode(Node, Data);

    if Assigned(Result) then
      Exit;

    Node := Node.getNextSibling;
  end;

  Result := nil;
end;

function TMainForm.FindTab(Data: Pointer): TIceTab;
var
  t: TCollectionItem;
begin
  for t in TabSet.Tabs do
    if Pointer(TIceTab(t).Tag) = Data then
      Exit(TIceTab(t));

  Result := nil;
end;

procedure TMainForm.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
var
  i: Integer;
begin
  if Assigned(Project) then
  begin
    // Try closing all tabs
    for i := TabSet.Tabs.Count - 1 downto 0 do
      if TabSet.Tabs[i].Modified then
      begin
        Project.Modified := True;
        Break;
      end;

    if Project.Modified then
      case Ask('Would you like to save your changes to the project?', mtConfirmation, [mbYes, mbNo, mbCancel]) of
        mrYes: acSave.Execute;
        mrNo: ;
        else
          begin
            CanClose := False;
            Exit;
          end;
      end;

    FAppSettings.InitialProjectRootDir := Project.RootDir;
    CanClose := CloseProject;
  end
  else
  begin
    FAppSettings.InitialProjectRootDir := '';
    CanClose := True;
  end;
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  FSynHLSLSyn := TSynHLSLSyn.Create(Self);

  // UI-stuff
  FFirstShow := True;
  Screen.OnActiveControlChange := ActiveControlChanged;

  // Application settings
  LoadSettings;

  // Enable Drop-Files
  DragAcceptFiles(Handle, True);

  // Focus NamespaceView
  ActiveControl := NamespaceView;

  // AppCache
  FLastOptionsPage := kToolsPageGeneral;
  FLastProjectOptionsPage := kProjectOptionsPageGeneral;

  // Hide TabSet
  TabSet.Height := 0;

  // Reset pass UI
  ActiveEffectPass := nil;
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
  Screen.OnActiveControlChange := nil;
  SaveSettings;
end;

procedure TMainForm.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if not (ActiveControl is TSynEdit) then
    Exit;

  if Key <> VK_TAB then
    Exit;

  if ssCtrl in Shift then
  begin
    if ssShift in Shift then
    begin
      if TabSet.TabIndex > 0 then
        TabSet.TabIndex := (TabSet.TabIndex - 1)
      else
        TabSet.TabIndex := TabSet.Tabs.Count - 1;
    end
    else
      TabSet.TabIndex := (TabSet.TabIndex + 1) mod (TabSet.Tabs.Count);
  end;
end;

procedure TMainForm.FormMouseWheelDown(Sender: TObject; Shift: TShiftState;
  MousePos: TPoint; var Handled: Boolean);
var
  c: TWinControl;
begin
  c := FindVCLWindow(Mouse.CursorPos);

  Handled := False;

  if (c is TTreeView) or (c is TListBox) or (c is TMemo) or (c is TRichEdit) then
  begin
    SendMessage(c.Handle, WM_VSCROLL, SB_LINEDOWN, 0);

    if (c is TTreeView) then
    begin
      SendMessage(c.Handle, WM_VSCROLL, SB_LINEDOWN, 0);
      SendMessage(c.Handle, WM_VSCROLL, SB_LINEDOWN, 0);
      SendMessage(c.Handle, WM_VSCROLL, SB_LINEDOWN, 0);
    end;

    Handled := True;
  end
  else if (c is TSynEdit)  then
  begin
    SendMessage(c.Handle, WM_VSCROLL, SB_LINEDOWN, 0);

    if (ssShift in Shift) and (ssCtrl in Shift) then
      TSynEdit(c).Font.Size := Max(8, TSynEdit(c).Font.Size - 2);

    Handled := True;
  end;
end;

procedure TMainForm.FormMouseWheelUp(Sender: TObject; Shift: TShiftState;
  MousePos: TPoint; var Handled: Boolean);
var
  c: TWinControl;
begin
  c := FindVCLWindow(Mouse.CursorPos);

  Handled := False;

  if (c is TTreeView) or (c is TListBox) or (c is TMemo) or (c is TRichEdit) then
  begin
    SendMessage(c.Handle, WM_VSCROLL, SB_LINEUP, 0);

    if (c is TTreeView) then
    begin
      SendMessage(c.Handle, WM_VSCROLL, SB_LINEUP, 0);
      SendMessage(c.Handle, WM_VSCROLL, SB_LINEUP, 0);
      SendMessage(c.Handle, WM_VSCROLL, SB_LINEUP, 0);
    end;

    Handled := True;
  end
  else if (c is TSynEdit) then
  begin
    SendMessage(c.Handle, WM_VSCROLL, SB_LINEUP, 0);

    if (ssShift in Shift) and (ssCtrl in Shift) then
      TSynEdit(c).Font.Size := Min(64, TSynEdit(c).Font.Size + 2);

    Handled := True;
  end;
end;

procedure TMainForm.FormShortCut(var Msg: TWMKey; var Handled: Boolean);
var
  ctrl: TWinControl;
  comp: TComponent;
  i: Integer;
Begin
  Handled := False;
  ctrl := ActiveControl;
  if ctrl <> nil then
  begin
    repeat
      ctrl := ctrl.Parent
    until (ctrl = nil) or (ctrl is TControl);

    if Assigned(ctrl) then
    begin
      for i := 0 to ctrl.ComponentCount-1 do
      begin
        comp:= ctrl.Components[i];
        if comp is TCustomActionList then
        begin
          Handled := TCustomActionList(comp).IsShortcut(Msg);
          if Handled Then
            Exit;
        end;
      end;
    end;
  end;
end;

procedure TMainForm.FormShow(Sender: TObject);
begin
  if FFirstShow then
  begin
    // Try loading the project
    try
      OpenProject(FAppSettings.InitialProjectRootDir);
    except
      // Ignore errors - just start the application
      FAppSettings.InitialProjectRootDir := '';
    end;

    FFirstShow := False;
  end;
end;

function TMainForm.TabData(T: TIceTab): TObject;
begin
  Result := TObject(T.Tag);
end;

procedure TMainForm.TabSetTabClose(Sender: TObject; ATab: TIceTab);
begin
  if Assigned(Project) then
    if ATab.Modified then
    begin
      case Ask('Would you like to save the changes to the document before closing?',
               mtConfirmation, [mbYes, mbNo, mbCancel]) of
      mrYes: acSave.Execute;
      mrCancel: Exit;
      end;
    end;

  TabToEdit(ATab).Free;
  TabSet.RemoveTab(ATab);

  if Assigned(TabSet.Selected) then
    TabToEdit(TabSet.Selected).OnStatusChange(TabSet.Selected.Data, []);

  if TabSet.Tabs.Count = 0 then
  begin
    TabSet.Height := 0;
    StatusBar.Panels[1].Text := '';
  end;
end;

procedure TMainForm.TabSetTabSelected(Sender: TObject; ATab: TIceTab;
  ASelected: Boolean);
begin
  if Assigned(ATab.Data) then
  begin
    TabToEdit(ATab).Visible := ASelected;

    if ASelected then
    begin
      TabToEdit(ATab).SetFocus;
      TabToEdit(ATab).OnStatusChange(Sender, []);
    end;
  end
  else
    StatusBar.Panels[1].Text := '1: 1';

  TabSet.Height := 30;
end;

function TMainForm.TabToEdit(T: TIceTab): TSynEdit;
begin
  Result := TSynEdit(T.Data);
end;

function TMainForm.EffectCount(Namespace: TDFXNamespace): Integer;
var
  i: Integer;
begin
  Result := Namespace.Effects.Count;

  for i := 0 to Namespace.Subnamespaces.Count - 1 do
    Inc(Result, EffectCount(Namespace.Subnamespaces[i]));
end;

function TMainForm.EffectCount: Integer;
begin
  Result := 0;

  if not Assigned(Project) then
    Exit;

  Result := EffectCount(Project.RootNamespace);
end;

procedure TMainForm.EffectListBoxClick(Sender: TObject);
begin
  if EffectListBox.ItemIndex <> -1 then
    ActiveEffectPass := TDFXEffectPass(EffectListBox.Items.Objects[EffectListBox.ItemIndex])
  else
    ActiveEffectPass := nil;
end;

procedure TMainForm.EffectListBoxDragDrop(Sender, Source: TObject; X,
  Y: Integer);
var
  dp: Integer;
  dpt: TPoint;
begin
  if Source = NamespaceView then
    ActiveEffect := NamespaceView.Selected.Data
  else
  begin
    dpt := Point(X, Y);

    with Source as TListBox do
    begin
      dp := ItemAtPos(dpt, True);

      if dp = -1 then
        dp := TListBox(Source).Count - 1;

      ActiveEffect.MovePass(TListBox(Source).ItemIndex, dp);
      Items.Move(TListBox(Source).ItemIndex, dp);
    end;
  end;
end;

procedure TMainForm.EffectListBoxDragOver(Sender, Source: TObject; X,
  Y: Integer; State: TDragState; var Accept: Boolean);
begin
  Accept := ((Source = NamespaceView) and (IsEffect(NamespaceView.Selected))) or
            ((Source = Sender) and (EffectListBox.ItemIndex <> -1));
end;

procedure TMainForm.EffectListBoxDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
var
  c: Integer;
  p: TDFXEffectPass;
begin
  with EffectListBox do
  begin
    Canvas.FillRect (rect);

    p := TDFXEffectPass(Items.Objects[Index]);

    if p.PassType = ptCompute then
      EffectImages.Draw(Canvas, Rect.Left + 4, Rect.Top + 4, 1)
    else
      EffectImages.Draw(Canvas, Rect.Left + 4, Rect.Top + 4, 0);

    c := (Rect.Bottom - Rect.Top - Canvas.TextHeight(Text)) div 2;

    Canvas.TextOut(Rect.Left + EffectImages.Width + 8, Rect.Top + c, p.Title);
  end;
end;

procedure TMainForm.EffectListBoxMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  TListBox(Sender).SetFocus;
end;

procedure TMainForm.LoadSettings;
var
  ini: TIniFile;
begin
  ini := TIniFile.Create(ExtractFilePath(Application.ExeName) +
    kIniFileSettings);

  try
    FAppSettings.InitialProjectRootDir := ini.ReadString('General', 'LastProject', '');
    FAppSettings.DoAutoSave := ini.ReadBool('General', 'AutoSave', False);
    FAppSettings.AutoSaveInterval := Max(1, ini.ReadInteger('General', 'AutoSaveInterval', 1));
    FAppSettings.PreBuild := ini.ReadString('Tools', 'PreBuild', '');
    FAppSettings.PostBuild := ini.ReadString('Tools', 'PostBuild', '');

    LeftBGPanel.Width := ini.ReadInteger('UI', 'LeftPane', Width div 3);
    PropertyEditorPanel.Height := ini.ReadInteger('UI', 'ProjectPane', Height div 2);
    RightBGPanel.Width := ini.ReadInteger('UI', 'RightPane', Width div 3);
    PassPanel.Height := ini.ReadInteger('UI', 'EffectPane', Height div 2);
    OutputPanel.Height := ini.ReadInteger('UI', 'OutputPane', Height div 3);

    Left := Max(Screen.WorkAreaLeft, ini.ReadInteger('General', 'X', Screen.WorkAreaLeft));
    Top := Max(Screen.WorkAreaTop, ini.ReadInteger('General', 'Y', Screen.WorkAreaTop));
    Width := ini.ReadInteger('General', 'Width', Screen.WorkAreaWidth);
    Height := ini.ReadInteger('General', 'Height', Screen.WorkAreaHeight);

    WindowState := TWindowState(ini.ReadInteger('General', 'WindowState', 0));

    if not (WindowState in [wsNormal, wsMaximized]) then
      WindowState := wsNormal;

    GCPPScriptPath := ini.ReadString('Tools', 'CppTranslator', '');
  finally
    ini.Free;
  end;
end;

procedure TMainForm.Log(const Text: string);
var
  fmt: string;
begin
  fmt := Format('[%s] %s', [TimeToStr(Now), Text]);
  if Pos('ERROR', Text) > 0 then
  begin
    OutputMemo.SelAttributes.Color := clRed;
    OutputMemo.SelText := fmt;
  end
  else
  begin
    OutputMemo.SelAttributes.Color := clBlack;
    OutputMemo.Lines.Add(fmt);
  end;

  OutputMemo.Perform(EM_LineScroll, 0, 1);
end;

procedure TMainForm.RefreshNamespaceStatus;
begin
  if Assigned(ActiveNamespace) then
    StatusBar.Panels[2].Text := ActiveNamespace.Name
  else
    StatusBar.Panels[2].Text := '';
end;

procedure TMainForm.RefreshShaderReferences;
begin
  if ShaderEdit.Enabled then
    ShaderEdit.OnChange(ShaderEdit);
end;

procedure TMainForm.OnAddConstant(C: TDFXConstant);
var
  n: TTreeNode;
begin
  n := NamespaceView.Items.AddChild(FNodeConstants, C.Name);
  n.Data := c;
  n.ImageIndex := kImageConstant;
  n.SelectedIndex := n.ImageIndex;
  n.Selected := True;
end;

procedure TMainForm.OnAddConstantBuffer(CB: TDFXConstantBuffer);
begin
  AddNode(FindFilter(FindNode(NamespaceView, CB.Namespace), kFolderConstantBuffers), CB.Name, CB, kImageConstantBuffer);
end;

procedure TMainForm.OnAddEffect(Effect: TDFXEffect);
begin
  AddNode(FindFilter(FindNode(NamespaceView, Effect.Parent), kFolderEffects), Effect.Name, Effect, kImageEffect);
  NamespaceView.CustomSort(NamespaceSort, 0);
end;

procedure TMainForm.OnAddEffectPass(P: TDFXEffectPass);
begin
  EffectListBox.Items.AddObject(P.Title, P);
  RefreshEffectLabel;
end;

procedure TMainForm.OnAddFunction(Func: TDFXFunction);
begin
  AddNode(FindFilter(FindNode(NamespaceView, Func.Namespace), kFolderFunctions), Func.Name, Func, kImageFunction);
end;

procedure TMainForm.OnAddNamespace(Parent, NewNamespace: TDFXNamespace);
begin
  AddNamespace(FindNode(NamespaceView, Parent), NewNamespace);
  NamespaceView.CustomSort(NamespaceSort, 0);
end;

procedure TMainForm.OnAddResource(Resource: TDFXResource);
begin
  AddNode(FindFilter(FindNode(NamespaceView, Resource.Namespace), kFolderResources), Resource.Name, Resource, kImageResource);
end;

procedure TMainForm.OnAddStructure(S: TDFXStructure);
begin
  AddNode(FindFilter(FindNode(NamespaceView, S.Namespace), kFolderStructures), S.Name, S, kImageStructure);
end;

procedure TMainForm.OnBeginBuildEffects;
var
  t: TBuildThread;
begin
  t := TBuildThread(ThreadWorkForm.WorkerThread);
  t.Synchronize(OnDoBeginBuildEffects);
end;

procedure TMainForm.OnBeginCompileShaders;
var
  t: TBuildThread;
begin
  t := TBuildThread(ThreadWorkForm.WorkerThread);
  t.Synchronize(OnDoBeginCompileShaders);
end;

function TMainForm.OnCanDeleteConstantBuffer(CB: TDFXConstantBuffer): Boolean;
begin
  Result := Ask('Do you really want to delete the constant buffer ' + CB.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

function TMainForm.OnCanDeleteEffect(Effect: TDFXEffect): Boolean;
begin
  Result := Ask('Do you really want to delete the effect ' + Effect.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

function TMainForm.OnCanDeleteEffectPass(P: TDFXEffectPass): Boolean;
begin
  Result := Ask('Do you really want to delete the effect pass ' + P.Title + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

function TMainForm.OnCanDeleteFunction(Func: TDFXFunction): Boolean;
begin
  Result := Ask('Do you really want to delete the function ' + Func.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

function TMainForm.OnCanDeleteNamespace(N: TDFXNamespace): Boolean;
begin
  Result := Ask('Do you really want to delete the namespace ' + N.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

function TMainForm.OnCanDeleteResource(Resource: TDFXResource): Boolean;
begin
  Result := Ask('Do you really want to delete the resource ' + Resource.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

function TMainForm.OnCanDeleteStructure(S: TDFXStructure): Boolean;
begin
  Result := Ask('Do you really want to delete the structure ' + S.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

procedure TMainForm.OnProcessSynEditCommand(Sender: TObject;
  var Command: TSynEditorCommand; var AChar: Char; Data: Pointer);
begin
  // FIXME: attempt at fixing false shortcut behaviour but it's not working
  if ActiveControl <> Sender then
    Command := ecNone;
end;

function TMainForm.OnProgress: Boolean;
var
  t: TBuildThread;
begin
  t := TBuildThread(ThreadWorkForm.WorkerThread);
  t.Synchronize(t.IncreaseStep);
  t.Synchronize(t.UpdateStatus);
  Result := True;
end;

function TMainForm.OnCanDeleteConstant(C: TDFXConstant): Boolean;
begin
  Result := Ask('Do you really want to remove the constant group ' + C.Name + '?', mtConfirmation, [mbYes, mbNo]) = mrYes;
end;

procedure TMainForm.ForceCloseTab(T: TIceTab);
begin
  if Assigned(T) then
  begin
    T.Modified := False;
    TabSet.OnTabClose(TabSet, T);
  end;
end;

procedure TMainForm.OnDeleteConstantBuffer(CB: TDFXConstantBuffer);
begin
  FindNode(NamespaceView, CB).Delete;
  ForceCloseTab(FindTab(CB));
end;

procedure TMainForm.OnDeleteEffect(Effect: TDFXEffect);
begin
  FindNode(NamespaceView, Effect).Delete;
  ActiveEffect := nil;
end;

procedure TMainForm.OnDeleteEffectPass(P: TDFXEffectPass);
begin
  EffectListBox.Items.Delete(EffectListBox.Items.IndexOfObject(P));

  if ActiveEffectPass = p then
    ActiveEffectPass := nil;

  RefreshEffectLabel;
end;

procedure TMainForm.OnDeleteFunction(Func: TDFXFunction);
begin
  FindNode(NamespaceView, Func).Delete;
  ForceCloseTab(FindTab(Func));
end;

procedure TMainForm.OnDeleteNamespace(Namespace: TDFXNamespace);
begin
  FindNode(NamespaceView, Namespace).Delete;
end;

procedure TMainForm.OnDeleteResource(Resource: TDFXResource);
begin
  FindNode(NamespaceView, Resource).Delete;
  ForceCloseTab(FindTab(Resource));
end;

procedure TMainForm.OnDeleteStructure(S: TDFXStructure);
begin
  FindNode(NamespaceView, S).Delete;
  ForceCloseTab(FindTab(S));
end;

procedure TMainForm.OnDestroyProject;
begin
  NamespaceView.Items.Clear;
  ActiveNamespace := nil;
  ActiveEffect := nil;
  FNodeConstants := nil;
end;

procedure TMainForm.OnDoBeginBuildEffects;
begin
  MainForm.ThreadWorkForm.ProgressBar.Max := MainForm.EffectCount;
  MainForm.ThreadWorkForm.ProgressBar.Step := 1;
end;

procedure TMainForm.OnDoBeginCompileShaders;
begin
  MainForm.ThreadWorkForm.ProgressBar.Max := MainForm.ShaderCount;
  MainForm.ThreadWorkForm.ProgressBar.Step := 1;
end;

procedure TMainForm.OnInitializeProject;
begin
  FNodeConstants := NamespaceView.Items.Add(nil, kSectionConstants);
  FNodeConstants.ImageIndex := kImageConstants;
  FNodeConstants.SelectedIndex := FNodeConstants.ImageIndex;
end;

procedure TMainForm.OnLog(const S: string);
var
  t: TBuildThread;
begin
  t := TBuildThread(MainForm.ThreadWorkForm.WorkerThread);
  t.FMsg := S;
  t.Synchronize(t.LogMsg);
end;

procedure TMainForm.OnProjectModified(Modified: Boolean);
begin
  if Modified then
    UpdateCaption('*')
  else
    UpdateCaption;
end;

procedure TMainForm.OnDeleteConstant(C: TDFXConstant);
begin
  FindNode(FNodeConstants, C).Delete;
  ForceCloseTab(FindTab(C));
end;

procedure TMainForm.OnRenameConstant(C: TDFXConstant);
var
  t: TIceTab;
begin
  t := FindTab(C);

  if Assigned(t) then
    t.Caption := C.Name;

  UpdateNodeText(C, C.Name);
end;

procedure TMainForm.OnRenameConstantBuffer(CB: TDFXConstantBuffer);
var
  t: TIceTab;
begin
  t := FindTab(CB);

  if Assigned(t) then
    t.Caption := CB.Name;

  UpdateNodeText(CB, CB.Name);
end;

procedure TMainForm.OnRenameEffect(Effect: TDFXEffect);
begin
  RefreshEffectLabel;
  RefreshNamespaceStatus;
  UpdateNodeText(Effect, Effect.Name);
end;

procedure TMainForm.OnRenameEffectPass(P: TDFXEffectPass);
begin
  EffectListBox.Items.Strings[EffectListBox.Items.IndexOfObject(P)] := P.Title;
end;

procedure TMainForm.OnRenameFunction(Func: TDFXFunction);
var
  t: TIceTab;
begin
  t := FindTab(Func);

  if Assigned(t) then
    t.Caption := Func.Name;

  UpdateNodeText(Func, Func.Name);
end;

procedure TMainForm.OnRenameNamespace(Namespace: TDFXNamespace);
begin
  RefreshNamespaceStatus;
  UpdateNodeText(Namespace, Namespace.Name);
end;

procedure TMainForm.UpdateNodeText(P: Pointer; const Text: string);
var
  n: TTreeNode;
begin
  n := FindNode(NamespaceView, P);
  if Assigned(n) then
    n.Text := Text;
end;

procedure TMainForm.OnRenameResource(Resource: TDFXResource);
var
  t: TIceTab;
begin
  t := FindTab(Resource);

  if Assigned(t) then
    t.Caption := Resource.Name;

  UpdateNodeText(Resource, Resource.Name);
end;

procedure TMainForm.OnRenameStructure(S: TDFXStructure);
var
  t: TIceTab;
begin
  t := FindTab(S);

  if Assigned(t) then
    t.Caption := S.Name;

  UpdateNodeText(S, S.Name);
end;

procedure TMainForm.OnStatus(const Msg: string);
var
  t: TBuildThread;
begin
  t := TBuildThread(FThreadWorkForm.WorkerThread);
  t.FMsg := Msg;
  t.Synchronize(t.UpdateStatus);
end;

procedure TMainForm.OnSynEditKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
var
  e: TSynEdit;
begin
  e := TSynEdit(Sender);

  // Editor enhancements
  if ssCtrl in Shift then
  begin
    if Key = Ord('D') then
    begin
      e.Lines.Insert(e.CaretY, e.Lines[e.CaretY - 1]);
      e.Modified := True;
    end
    else if Key = Ord('L') then
      e.CaretX := TrimRight(e.Lines[e.CaretY - 1]).Length + 1
    else if Key = Ord('K') then
      e.CaretXY := e.NextWordPos
    else if Key = Ord('J') then
      e.CaretXY := e.PrevWordPos
    else if Key = Ord('U') then
      e.CaretY := Max(0, e.CaretY - 1)
    else if Key = Ord('I') then
      e.CaretY := Min(e.Lines.Count, e.CaretY + 1);
  end
  else if Key = VK_F3 then
    // Find next
    FindDialog.OnFind(FindDialog);
end;

procedure TMainForm.OpenProject(const ProjectDir: string);
var
  i: Integer;
begin
  if CloseProject then
  begin
    NamespaceView.Items.BeginUpdate;
    try
      try
        Project := TDFXProject.Create(Self, ProjectDir, cmOpen);

        for i := 0 to NamespaceView.Items.Count - 1 do
          NamespaceView.Items[i].Collapse(True);

        LoadProjectUI;
      except
        CloseProject;
      end;
    finally
      NamespaceView.Items.EndUpdate;
    end;
  end;
end;

procedure TMainForm.StatusBarResize(Sender: TObject);
begin
  StatusBar.Panels[0].Width := StatusBar.ClientWidth - 256;
  StatusBar.Panels[1].Width := 64;
  StatusBar.Panels[2].Width := StatusBar.ClientWidth;
end;

procedure TMainForm.SynEditKeyPress(Sender: TObject; var Key: Char);
var
  e: TSynEdit;
  l, t: string;
  cnt: Integer;
begin
  // Indent-extension for {} scoped blocks
  e := TSynEdit(Sender);

  if Key = #13 then
  begin
    l := e.Lines[e.CaretY-2];
    t := Trim(l);
    if t.Length > 0 then
    begin
      if t[t.Length] = '{' then
      begin
        if Trim(e.Lines[e.CaretY-1]).Length = 0 then
        begin
          cnt := CountIndentSpaces(l) + 1 + e.TabWidth;
          l := StringOfChar(' ', cnt);
          e.Lines[e.CaretY-1] := l;
          e.CaretX := cnt;
        end;
      end;
    end
  end
  else if Key = '}' then
  begin
    l := e.Lines[e.CaretY-1];
    t := Trim(l);
    if t.Length = 0 then
    begin
      Delete(l, 1, CountIndentSpaces(e.Lines[e.CaretY-2]));
      e.Lines[e.CaretY-1] := l;
      e.CaretX := e.CaretX - e.TabWidth;
    end;
  end;
end;

procedure TMainForm.SynEditStatusChange(Sender: TObject;
  Changes: TSynStatusChanges);
var
  e: TSynEdit;
  t: TIceTab;
begin
  if scModified in Changes then
  begin
    e := TSynEdit(Sender);
    t := EditToTab(e);

    if (e.Modified) and (not t.Modified) then
      t.Caption := t.Caption + '*'
    else
      t.Caption := Copy(t.Caption, 1, Length(t.Caption) - 1);

    t.Modified := e.Modified;
  end;

  StatusBar.Panels[1].Text := IntToStr(TSynEdit(Sender).CaretY) + ': ' + IntToStr(TSynEdit(Sender).CaretX);
end;

{ TComboBoxEx }

procedure TComboBoxEx.SetItemIndex(const Value: Integer);
begin
  inherited;
  if Assigned(OnChange) then
    OnChange(Self);
end;

{ TMainForm.TBuildThread }

constructor TMainForm.TBuildThread.Create(Mode: TBuildThreadMode);
begin
  FreeOnTerminate := True;
  FMode := Mode;
  inherited Create(True);
end;

procedure TMainForm.TBuildThread.Execute;
var
  f: TDFXFunction;
  s: string;
begin
  inherited;

  Synchronize(InitializeStatus);

  try
    if FMode = btmBuildProject then
      CompileDFXProject(MainForm.Project, MainForm)
    else
    begin
      f := MainForm.NamespaceView.Selected.Data;
      if GetShaderDASM(MainForm.Project, MainForm, f, s) then
        MainForm.NewTextTab(nil, f.Name, s, kImageDASM);
    end;
  except
  end;
end;

procedure TMainForm.TBuildThread.IncreaseStep;
begin
  MainForm.ThreadWorkForm.ProgressBar.StepIt;
end;

procedure TMainForm.TBuildThread.InitializeStatus;
begin
  MainForm.ThreadWorkForm.ProgressBar.Max := 1;
  MainForm.ThreadWorkForm.ProgressBar.Step := 0;
end;

procedure TMainForm.TBuildThread.LogError;
begin
  MainForm.Log('[ERROR] ' + FMsg);
end;

procedure TMainForm.TBuildThread.LogMsg;
begin
  MainForm.Log(FMsg);
end;

procedure TMainForm.TBuildThread.UpdateStatus;
var
  f: TThreadWorkForm;
  w: Integer;
begin
  f := MainForm.ThreadWorkForm;
  f.StatusLabel.Caption := Format('%s (%d / %d)', [FMsg, f.ProgressBar.Position, f.ProgressBar.Max]);
  w := f.StatusLabel.Canvas.TextWidth(f.StatusLabel.Caption) + f.StatusLabel.Left + 16;
  f.ClientWidth := Max(w, 373);
  f.Left := MainForm.Left + MainForm.Width div 2 - f.Width div 2;
  f.Top := MainForm.Top + MainForm.Height div 2 - f.Height div 2;
end;

end.
