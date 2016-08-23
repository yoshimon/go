unit AboutWindow;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls,
  Vcl.Imaging.pngimage;

type
  TAboutForm = class(TForm)
    ButtonBevel: TBevel;
    OkButton: TButton;
    ProjectDirectoryLabel: TLabel;
    Label1: TLabel;
    Label2: TLabel;
    Image1: TImage;
    BitLabel: TLabel;
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  AboutForm: TAboutForm;

implementation

{$R *.dfm}

procedure TAboutForm.FormCreate(Sender: TObject);
begin
  {$IFDEF CPUX86}
  BitLabel.Caption := '(32-bit)';
  {$ENDIF}
end;

end.
