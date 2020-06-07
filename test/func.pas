program func;
var
	a:integer;
function max(a:integer):integer;
begin
	max:=a;
end;
function wow:integer;
begin
	wow:=5;
end;
begin
	a:=max(1);
	writeln(a);
	writeln(max(3));
	writeln(wow());
end.