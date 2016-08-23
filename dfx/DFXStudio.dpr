program DFXStudio;

uses
  Vcl.Forms,
  MainWindow in 'Forms\MainWindow.pas' {MainForm},
  ThreadWorkWindow in 'Forms\ThreadWorkWindow.pas',
  DFX.Project in 'Units\DFX.Project.pas',
  NewProjectWindow in 'Forms\NewProjectWindow.pas' {NewProjectForm},
  AboutWindow in 'Forms\AboutWindow.pas' {AboutForm},
  SHChangeNotify in 'Units\SHChangeNotify.pas',
  ErrorWindow in 'Forms\ErrorWindow.pas' {ErrorForm},
  Vcl.Themes,
  Vcl.Styles,
  OptionsWindow in 'Forms\OptionsWindow.pas' {OptionsForm},
  ProjectOptionsWindow in 'Forms\ProjectOptionsWindow.pas' {ProjectOptionsForm},
  DFX.Compiler in 'Units\DFX.Compiler.pas',
  MenuImageList in 'Units\MenuImageList.pas',
  SynHighlighterHLSL in 'Units\SynHighlighterHLSL.pas',
  VCLStyleSynEdit in 'Units\VCLStyleSynEdit.pas',
  SynEditPopup in 'Units\SynEditPopup.pas',
  DFX.Compiler.Types in 'Units\DFX.Compiler.Types.pas',
  DFX.Compiler.Helper in 'Units\DFX.Compiler.Helper.pas',
  DFX.Compiler2 in 'Units\DFX.Compiler2.pas',
  DFX.Compiler2.Types in 'Units\DFX.Compiler2.Types.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
