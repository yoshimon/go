unit ErrorWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls,
  Vcl.Imaging.pngimage, System.Math, Generics.Collections;

type
  TErrorForm = class(TForm)
    ButtonBevel: TBevel;
    ErrorIcon: TImage;
    MessageLabel: TLabel;
    QuestionIcon: TImage;
    InfoIcon: TImage;
  private
    { Private declarations }
    FDlgType: TMsgDlgType;
    FDlgButtons: TMsgDlgButtons;

    function GetErrorMsg: string;
    procedure SetErrorMsg(const NewErrorMsg: string);
    procedure SetDlgType(NewDlgType: TMsgDlgType);
    procedure SetButtons(NewButtons: TMsgDlgButtons);

    function CreateModalResultButton(LastButton: TButton; const Caption: string;
      ClickResult: TModalResult): TButton;
  public
    { Public declarations }
    property ErrorMsg: string read GetErrorMsg write SetErrorMsg;
    property Buttons: TMsgDlgButtons read FDlgButtons write SetButtons;
    property DlgType: TMsgDlgType read FDlgType write SetDlgType;
  end;

var
  ErrorForm: TErrorForm;

implementation

{$R *.dfm}
{ TErrorForm }

function TErrorForm.CreateModalResultButton(LastButton: TButton;
  const Caption: string; ClickResult: TModalResult): TButton;
begin
  Result := TButton.Create(Self);

  if Assigned(LastButton) then
    Result.Left := LastButton.Left
  else
    Result.Left := ClientWidth - 2;

  Result.Parent := Self;
  Result.Top := ClientHeight - Result.Height - 8;
  Result.Anchors := [akRight, akBottom];
  Result.Left := Result.Left - Result.Width - 6;
  Result.Caption := Caption;
  Result.ModalResult := ClickResult;
  Result.Show;
end;

function TErrorForm.GetErrorMsg: string;
begin
  Result := MessageLabel.Caption;
end;

procedure TErrorForm.SetButtons(NewButtons: TMsgDlgButtons);
var
  b: TButton;
begin
  b := nil;
  
  if mbCancel in NewButtons then
  begin
    b := CreateModalResultButton(b, '&Cancel', mrCancel);
    b.TabOrder := 3;
  end;

  if mbOk in NewButtons then
  begin
    b := CreateModalResultButton(b, '&OK', mrOk);
    b.TabOrder := 2;
  end;

  if mbNo in NewButtons then
  begin
    b := CreateModalResultButton(b, '&No', mrNo);
    b.TabOrder := 1;
  end;

  if mbYes in NewButtons then
  begin
    b.TabOrder := 0;
    CreateModalResultButton(b, '&Yes', mrYes);
  end;
end;

procedure TErrorForm.SetDlgType(NewDlgType: TMsgDlgType);
begin
  case NewDlgType of
    TMsgDlgType.mtWarning:
      begin
        Caption := 'Warning';
        ErrorIcon.Show;
      end;
    TMsgDlgType.mtError:
      begin
        Caption := 'Error';
        ErrorIcon.Show;
      end;
    TMsgDlgType.mtInformation:
      begin
        Caption := 'Information';
        InfoIcon.Show;
      end;
    TMsgDlgType.mtConfirmation:
      begin
        Caption := 'Confirmation';
        QuestionIcon.Show;
      end
  else
    InfoIcon.Show;
  end;

  FDlgType := NewDlgType;
end;

procedure TErrorForm.SetErrorMsg(const NewErrorMsg: string);
begin
  MessageLabel.Caption := NewErrorMsg;
  Application.ProcessMessages;
  Width := Max(Constraints.MinWidth, ErrorIcon.Width + MessageLabel.Width + 48);
  MessageLabel.Top := ErrorIcon.Top + ErrorIcon.Height div 2 - MessageLabel.Height div 2;
  Application.ProcessMessages;
end;

end.
