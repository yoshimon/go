unit ThreadWorkWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ComCtrls,
  Vcl.Imaging.GIFImg, Vcl.ExtCtrls, Math, CommCtrl;

type
  TDialogOption = (doCancel, doStatus);
  TDialogOptions = set of TDialogOption;
  TThreadResult = (trSuccess, trFailure, trCanceled);

  TThreadWorkForm = class(TForm)
    HeaderLabel: TLabel;
    ProgressBar: TProgressBar;
    CancelButton: TButton;
    StatusLabel: TLabel;
    Icon: TImage;
    procedure FormCreate(Sender: TObject);
    procedure FormPaint(Sender: TObject);
  private
    { Private declarations }
    FDialogOptions: TDialogOptions;
    FWorkerThread: TThread;
    FOldOnTerminate: procedure(Sender: TObject) of Object;
    FPaintIndex: Integer;
    FThreadResult: TThreadResult;

    procedure SetDialogOptions(NewDialogOptions: TDialogOptions);
    procedure SetWorkerThread(NewWorkerThread: TThread);

    procedure OnTerminateWorkerThread(Sender: TObject);
  public
    { Public declarations }
    property DialogOptions: TDialogOptions read FDialogOptions write SetDialogOptions;
    property WorkerThread: TThread read FWorkerThread write SetWorkerThread;
    property ThreadResult: TThreadResult read FThreadResult write FThreadResult;

    procedure OnCancelButtonClicked(Sender: TObject);
  end;

var
  ThreadWorkForm: TThreadWorkForm;

implementation

{$R *.dfm}

procedure TThreadWorkForm.FormCreate(Sender: TObject);
begin
  (Icon.Picture.Graphic as TGIFImage).Animate := True;
  FPaintIndex := 0;
end;

procedure TThreadWorkForm.SetWorkerThread(NewWorkerThread: TThread);
begin
  FWorkerThread := NewWorkerThread;

  // Replace old terminate callback
  if Assigned(NewWorkerThread) then
  begin
    FOldOnTerminate := NewWorkerThread.OnTerminate;
    NewWorkerThread.OnTerminate := OnTerminateWorkerThread;
  end;
end;

procedure TThreadWorkForm.FormPaint(Sender: TObject);
begin
  FPaintIndex := Min(ControlCount + 1, FPaintIndex + 1);

  if FPaintIndex = ControlCount then
  begin
    if Assigned(WorkerThread) then
      WorkerThread.Start
    else
      OnTerminateWorkerThread(Self);
  end;
end;

procedure TThreadWorkForm.OnCancelButtonClicked(Sender: TObject);
begin
  FWorkerThread.Terminate;
  ModalResult := mrCancel;
end;

procedure TThreadWorkForm.OnTerminateWorkerThread(Sender: TObject);
begin
  StatusLabel.Caption := 'Done.';

  ProgressBar.Max := 1;
  ProgressBar.Position := 1;
  Application.ProcessMessages;

  if Assigned(FOldOnTerminate) then
    FOldOnTerminate(FWorkerThread);

  case ThreadResult of
  trSuccess: ModalResult := mrOk;
  trFailure: ModalResult := mrAbort;
  else ModalResult := mrCancel;
  end;
end;

procedure TThreadWorkForm.SetDialogOptions(NewDialogOptions: TDialogOptions);
var
  h: Integer;
begin
  FDialogOptions := NewDialogOptions;

  h := Height;

  CancelButton.Visible := doCancel in NewDialogOptions;
  CancelButton.OnClick := OnCancelButtonClicked;
  StatusLabel.Visible := doStatus in NewDialogOptions;

  if not CancelButton.Visible then
  begin
    Dec(h, CancelButton.Height + 4);
    BorderIcons := BorderIcons - [biSystemMenu];
  end;

  if not StatusLabel.Visible then
    Dec(h, StatusLabel.Height);

  Height := h;
end;

end.
