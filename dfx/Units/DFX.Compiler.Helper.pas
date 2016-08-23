unit DFX.Compiler.Helper;

interface

uses
  Classes;

procedure MergeStringLists(Source, Target: TStringList);

implementation

procedure MergeStringLists(Source, Target: TStringList);
var
  s: string;
begin
  for s in Source do
  begin
    if Target.IndexOf(S) <> -1 then
      Continue;

    Target.Add(s);
  end;
end;

end.
