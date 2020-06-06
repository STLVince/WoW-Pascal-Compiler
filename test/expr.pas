program exprcheck;
const
    y = 33;
    a = 1;
    b = 2.2;
    c = 'string';
    d = false;

var
    e : integer;
    f : real;
    g : integer;
    h : string;
    i : boolean;
    j : array [0..2] of integer;
    k : integer;

function test : integer;
begin
    test := 3;
end;

begin
    e := a + y;
    g := a - y;
    f := a * b;
    f := a / b;
    i := a = b;
    i := a <> b;
    i := a > b;
    i := a < b;
    // h := 'dydxh';
    f := 2.2;
    g := 3;
    i := false;
    e := a and y;
    e := a or y;
    // e := a xor y;
    e := a / g;
    e := a mod g;
    // e := not g;
    j[0] := 0;
    j[1] := 1;
    j[2] := 2;

    writeln(j[0] + a - g);
    // writeln(test());
    // k := (j[0] + a - g) * 4 / 3 * 3 + test();

end.