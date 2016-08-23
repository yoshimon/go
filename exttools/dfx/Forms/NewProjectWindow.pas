unit NewProjectWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls;

type
  TNewProjectForm = class(TForm)
    ButtonBevel: TBevel;
    OkButton: TButton;
    CancelButton: TButton;
    ProjectDirectoryLabel: TLabel;
    ProjectDirectoryEdit: TButtonedEdit;
    OutputDirLabel: TLabel;
    OutputDirEdit: TButtonedEdit;
    procedure ProjectDirectoryEditRightButtonClick(Sender: TObject);
    procedure ProjectDirectoryEditExit(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  NewProjectForm: TNewProjectForm;

implementation

{$R *.dfm}

uses
  MainWindow;

procedure TNewProjectForm.ProjectDirectoryEditExit(Sender: TObject);
begin
  if DirectoryExists(ProjectDirectoryEdit.Text) then
    if OutputDirEdit.Text = '' then
      OutputDirEdit.Text := IncludeTrailingPathDelimiter(ProjectDirectoryEdit.Text) + 'output';
end;

procedure TNewProjectForm.ProjectDirectoryEditRightButtonClick(Sender: TObject);
var
  dir: string;
begin
  if MainForm.PickDirectory(dir) then
  begin
    TButtonedEdit(Sender).Text := dir;
    ProjectDirectoryEdit.OnExit(ProjectDirectoryEdit);
  end;
end;

end.
