unit ProjectOptionsWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ImgList, Vcl.ExtCtrls, Vcl.StdCtrls,
  Vcl.Samples.Spin, Vcl.ComCtrls, D3DCompiler_JSB, Generics.Collections, Math;

type
  TComboBoxEntry = record
    ComboBox: TComboBox;
    ItemIndex: Integer;
  end;

  TProjectOptionsForm = class(TForm)
    TopBevel: TBevel;
    BottomBevel: TBevel;
    PageView: TListView;
    CancelButton: TButton;
    OKButton: TButton;
    ContentBook: TNotebook;
    HLSLGroupBox: TGroupBox;
    PageImages: TImageList;
    PreambleMemo: TMemo;
    GroupBox1: TGroupBox;
    HLSLCompilerLabel: TLabel;
    OutputDirEdit: TButtonedEdit;
    Label1: TLabel;
    ShaderDirEdit: TButtonedEdit;
    GroupBox2: TGroupBox;
    MinimizeShaderCheckBox: TCheckBox;
    HLSLCompilerFlagsGroupBox: TGroupBox;
    CCBDebugInformation: TCheckBox;
    CCBBackwardsCompatibility: TCheckBox;
    CCBStrictness: TCheckBox;
    CCBIEEEStrictness: TCheckBox;
    CCBNoPreshader: TCheckBox;
    CCBPartialPrecision: TCheckBox;
    CCBResourceAlias: TCheckBox;
    CCBSkipValidation: TCheckBox;
    Label2: TLabel;
    CBOptimization: TComboBox;
    Label3: TLabel;
    CBMatrixPackOrder: TComboBox;
    Label4: TLabel;
    CBFlowControl: TComboBox;
    procedure PageViewSelectItem(Sender: TObject; Item: TListItem;
      Selected: Boolean);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
    FHLSLCompilerFlagMap: TDictionary<Cardinal, TCheckBox>;
    FHLSLCompilerFlagMapCombo: TDictionary<Cardinal, TComboBoxEntry>;

    procedure MapHLSLCompilerFlag(Flags: array of Cardinal; Component: TComboBox; SkipZero: Boolean = True); overload;

    function GetHLSLCompilerFlags: Cardinal;
    procedure SetHLSLCompilerFlags(Flags: Cardinal);
  public
    { Public declarations }
    property HLSLCompilerFlags: Cardinal read GetHLSLCompilerFlags write SetHLSLCompilerFlags;
  end;

var
  ProjectOptionsForm: TProjectOptionsForm;

implementation

{$R *.dfm}

procedure TProjectOptionsForm.SetHLSLCompilerFlags(Flags: Cardinal);
var
  pc: TPair<Cardinal, TCheckBox>;
  pcb: TPair<Cardinal, TComboBoxEntry>;
  opt: TComboBox;
begin
  for pc in FHLSLCompilerFlagMap do
    pc.Value.Checked := (Flags and pc.Key) = pc.Key;

  for pcb in FHLSLCompilerFlagMapCombo do
    if (Flags and pcb.Key) = pcb.Key then
      pcb.Value.ComboBox.ItemIndex := Max(pcb.Value.ComboBox.ItemIndex, pcb.Value.ItemIndex);

  // Adjust optimization level manually (weird bit mask)
  opt := CBOptimization;
  if (Flags and D3DCOMPILE_OPTIMIZATION_LEVEL2) = D3DCOMPILE_OPTIMIZATION_LEVEL2 then
    opt.ItemIndex := FHLSLCompilerFlagMapCombo[D3DCOMPILE_OPTIMIZATION_LEVEL2].ItemIndex
  else if (Flags and D3DCOMPILE_OPTIMIZATION_LEVEL3) = D3DCOMPILE_OPTIMIZATION_LEVEL3 then
    opt.ItemIndex := FHLSLCompilerFlagMapCombo[D3DCOMPILE_OPTIMIZATION_LEVEL3].ItemIndex
  else if (Flags and D3DCOMPILE_OPTIMIZATION_LEVEL0) = D3DCOMPILE_OPTIMIZATION_LEVEL0 then
    opt.ItemIndex := FHLSLCompilerFlagMapCombo[D3DCOMPILE_OPTIMIZATION_LEVEL0].ItemIndex
  else if (Flags and D3DCOMPILE_SKIP_OPTIMIZATION) = D3DCOMPILE_SKIP_OPTIMIZATION then
    opt.ItemIndex := FHLSLCompilerFlagMapCombo[D3DCOMPILE_SKIP_OPTIMIZATION].ItemIndex;
end;

procedure TProjectOptionsForm.FormCreate(Sender: TObject);
begin
  PageView.Selected := PageView.Items[0];

  FHLSLCompilerFlagMap := TDictionary<Cardinal, TCheckBox>.Create;
  FHLSLCompilerFlagMapCombo := TDictionary<Cardinal, TComboBoxEntry>.Create;

  FHLSLCompilerFlagMap.Add(D3DCOMPILE_SKIP_VALIDATION, CCBSkipValidation);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_DEBUG, CCBDebugInformation);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY, CCBBackwardsCompatibility);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_ENABLE_STRICTNESS, CCBStrictness);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_IEEE_STRICTNESS, CCBIEEEStrictness);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_NO_PRESHADER, CCBNoPreshader);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_PARTIAL_PRECISION, CCBPartialPrecision);
  FHLSLCompilerFlagMap.Add(D3DCOMPILE_RESOURCES_MAY_ALIAS, CCBResourceAlias);

  MapHLSLCompilerFlag([0, D3DCOMPILE_AVOID_FLOW_CONTROL, D3DCOMPILE_PREFER_FLOW_CONTROL], CBFlowControl);
  MapHLSLCompilerFlag([D3DCOMPILE_SKIP_OPTIMIZATION, D3DCOMPILE_OPTIMIZATION_LEVEL0, D3DCOMPILE_OPTIMIZATION_LEVEL1,
    D3DCOMPILE_OPTIMIZATION_LEVEL2, D3DCOMPILE_OPTIMIZATION_LEVEL3], CBOptimization, False);
  MapHLSLCompilerFlag([0, D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR, D3DCOMPILE_PACK_MATRIX_ROW_MAJOR], CBMatrixPackOrder);
end;

procedure TProjectOptionsForm.MapHLSLCompilerFlag(Flags: array of Cardinal;
  Component: TComboBox; SkipZero: Boolean);
var
  i: Integer;
  r: TComboBoxEntry;
begin
  if Length(Flags) <> Component.Items.Count then
    raise Exception.Create('ComboBox item-count mismatch.');

  r.ComboBox := Component;

  for i := Low(Flags) to High(Flags) do
  begin
    if (SkipZero) and (Flags[i] = 0) then
      Continue;

    r.ItemIndex := i;
    FHLSLCompilerFlagMapCombo.Add(Flags[i], r);
  end;
end;

procedure TProjectOptionsForm.PageViewSelectItem(Sender: TObject;
  Item: TListItem; Selected: Boolean);
begin
  ContentBook.PageIndex := PageView.ItemIndex + 1;
end;

procedure TProjectOptionsForm.FormDestroy(Sender: TObject);
begin
  FHLSLCompilerFlagMap.Free;
  FHLSLCompilerFlagMapCombo.Free;
end;

function TProjectOptionsForm.GetHLSLCompilerFlags: Cardinal;
var
  pc: TPair<Cardinal, TCheckBox>;
  pcb: TPair<Cardinal, TComboBoxEntry>;
begin
  Result := 0;

  for pc in FHLSLCompilerFlagMap do
    if pc.Value.Checked then
      Result := Result or pc.Key;

  for pcb in FHLSLCompilerFlagMapCombo do
    if pcb.Value.ComboBox.ItemIndex = pcb.Value.ItemIndex then
      Result := Result or pcb.Key;
end;

end.
