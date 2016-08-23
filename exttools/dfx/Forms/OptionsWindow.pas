unit OptionsWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ExtCtrls, Vcl.ComCtrls, Vcl.StdCtrls,
  Vcl.ImgList, Vcl.Samples.Spin, DFX.Project;

type
  TOptionsForm = class(TForm)
    PageView: TListView;
    TopBevel: TBevel;
    BottomBevel: TBevel;
    CancelButton: TButton;
    OKButton: TButton;
    ContentBook: TNotebook;
    PageImages: TImageList;
    ExternalGroupBox: TGroupBox;
    PreBuildLabel: TLabel;
    PreBuildEdit: TButtonedEdit;
    PostBuildLabel: TLabel;
    PostBuildEdit: TButtonedEdit;
    AutoSaveGroupBox: TGroupBox;
    AutoSaveCheckBox: TCheckBox;
    AutoSaveSpinEdit: TSpinEdit;
    Label3: TLabel;
    CppTranslatorGroupBox: TGroupBox;
    LuaScriptLabel: TLabel;
    CppTranslatorEdit: TButtonedEdit;
    procedure PageViewSelectItem(Sender: TObject; Item: TListItem;
      Selected: Boolean);
    procedure HLSLCompilerEditChange(Sender: TObject);
    procedure HLSLCompilerEditRightButtonClick(Sender: TObject);
    procedure AutoSaveCheckBoxClick(Sender: TObject);
    procedure CppTranslatorEditRightButtonClick(Sender: TObject);
  private
    { Private declarations }
    function GetActivePage: Integer;
    procedure SetActivePage(NewValue: Integer);
  public
    { Public declarations }
    property ActivePage: Integer read GetActivePage write SetActivePage;
  end;

var
  OptionsForm: TOptionsForm;

implementation

{$R *.dfm}

uses MainWindow;

{ TOptionsForm }

procedure TOptionsForm.AutoSaveCheckBoxClick(Sender: TObject);
begin
  AutoSaveSpinEdit.Enabled := AutoSaveCheckBox.Checked;
end;

procedure TOptionsForm.CppTranslatorEditRightButtonClick(Sender: TObject);
begin
  if MainForm.ScriptOpenDialog.Execute then
    TButtonedEdit(Sender).Text := MainForm.ScriptOpenDialog.FileName;
end;

function TOptionsForm.GetActivePage: Integer;
begin
  Result := PageView.ItemIndex;
end;

procedure TOptionsForm.HLSLCompilerEditChange(Sender: TObject);
begin
  if not FileExists(ExpandEnvStrings(TButtonedEdit(Sender).Text)) then
    TButtonedEdit(Sender).Font.Color := clRed
  else
    TButtonedEdit(Sender).Font.Color := clBlack;

  LuaScriptLabel.Font.Color := CppTranslatorEdit.Font.Color;
end;

procedure TOptionsForm.HLSLCompilerEditRightButtonClick(Sender: TObject);
begin
  if MainForm.ExeOpenDialog.Execute then
    TButtonedEdit(Sender).Text := MainForm.ExeOpenDialog.FileName;
end;

procedure TOptionsForm.PageViewSelectItem(Sender: TObject; Item: TListItem;
  Selected: Boolean);
begin
  ContentBook.PageIndex := PageView.ItemIndex + 1;
end;

procedure TOptionsForm.SetActivePage(NewValue: Integer);
begin
  PageView.ItemIndex := NewValue;
end;

end.
