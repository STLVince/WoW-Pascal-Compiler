program funcs;
var
	a:integer;
function f(a:integer):integer;
	function g(b:integer):integer;
	begin
		g:=b * 3;
	end;
begin
	f:=g(a) + 2;
end;
begin
	a:=f(4);
	writeln(a);
end.