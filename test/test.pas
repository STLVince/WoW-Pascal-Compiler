program test;
var
	i,j:integer;
	n,v:integer;
	c,w,f:array[1..100] of integer;
begin
	n:=3;
	v:=10;
	c[1]:=1; w[1]:=2;
	c[2]:=4; w[2]:=6;
	c[3]:=6; w[3]:=8;
	for i:=1 to n do
	begin
		writeln(i);
		j:=v;
		repeat
			if f[j-c[i]]+w[i]>f[j] then
			begin
				f[j]:=f[j-c[i]]+w[i];
			end
			else
				writeln(f[j]);
			j := j - 1;
		until j = c[i] - 1;
	end;
end.