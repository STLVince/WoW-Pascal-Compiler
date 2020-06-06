program variable;
const
    b = 1;
    // c = 2.2;
    teststr = 'aaa';
var
	a,b:integer;
	r:real;
	bool:boolean;
	c:char;
	// s:string;
	x : array [0..2] of integer;
begin
	r:=0.5;
	c:='0';
	// x[2] := 2;
	// a := x[1];
	if b = 1 then
	begin
		a := 6;
	end
	else begin
		a := 9;
	end;
	// writeln(x[2]);
	// writeln(x[0]);
	// s:='aa';
	writeln(teststr);
end.