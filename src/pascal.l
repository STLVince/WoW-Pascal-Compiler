%option noyywrap c++
%{
#include <iostream>
#include "parser.hpp"
#include "utils/ast.hpp"

#define YYLVAL_DEBUG yylval.debug = strdup(yytext); 
extern YYLTYPE yylloc;
int line_no = 1;
%}

ANY .
COMMENT "{"{ANY}*"}" 
DIGIT [0-9]
DIGITS {DIGIT}+
LETTER [_a-zA-Z]
ID {LETTER}({LETTER}|{DIGIT})*
POINT "."{DIGITS}
EXPONENT (e|E)("+"|"-")?{DIGITS}
INTNUM {DIGITS}
REALNUM {DIGITS}({POINT})?({EXPONENT})?

%%
COMMENT {}
":=" { return ASSIGN; }
"=" { return EQUAL; }
"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
"." { return DOT; }
"," { return COMMA; }
":" { return COLON; }
";" { return SEMI; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MUL; }
"/" { return DIVI; }
">=" { return GE; }
">" { return GT; }
"<=" { return LE; }
"<" { return LT; }
"<>" { return UNEQUAL; }

and { return AND; }
abs { YYLVAL_DEBUG; return SYS_FUNCT; }
array { return ARRAY; }
begin { return _BEGIN; }
boolean { YYLVAL_DEBUG; return SYS_TYPE; }
case { return CASE; }
const { return CONST; }
char { YYLVAL_DEBUG; return SYS_TYPE; }
chr { YYLVAL_DEBUG; return SYS_FUNCT; }
downto { return DOWNTO; }
do { return DO; }
div { return DIVI; }
end { return END; }
else { return ELSE; }
false { YYLVAL_DEBUG; return SYS_BOOL; }
for { return FOR; }
function { return FUNCTION; }
goto { return GOTO; }
integer { YYLVAL_DEBUG; return SYS_TYPE; }
if { return IF; }
mod { return MOD; }
maxint { YYLVAL_DEBUG; return SYS_CON; }
not { return NOT; }
odd { YYLVAL_DEBUG; return SYS_FUNCT; }
of { return OF; }
ord { YYLVAL_DEBUG; return SYS_FUNCT; }
or { return OR; }
pred { YYLVAL_DEBUG; return SYS_FUNCT; }
program { return PROGRAM; }
procedure { return PROCEDURE; }
read { return READ; }
real { YYLVAL_DEBUG; return SYS_TYPE; }
record { return RECORD; }
repeat { return REPEAT; }
sqrt { YYLVAL_DEBUG; return SYS_FUNCT; }
sqr { YYLVAL_DEBUG; return SYS_FUNCT; }
string { YYLVAL_DEBUG; return SYS_TYPE; }
succ { YYLVAL_DEBUG; return SYS_FUNCT; }
to { return TO; }
then { return THEN; }
true { YYLVAL_DEBUG; return SYS_BOOL; }
type { return TYPE; }
until { return UNTIL; }
var { return VAR; }
while { return WHILE; }
writeln { YYLVAL_DEBUG; return SYS_PROC; }
write { YYLVAL_DEBUG; return SYS_PROC; }

"'"{ANY}"'" { YYLVAL_DEBUG; return CHAR; }
"'"{ANY}{ANY}+"'" { YYLVAL_DEBUG; return STRING; }
{ID} { YYLVAL_DEBUG; return ID; }
{INTNUM} { YYLVAL_DEBUG; return INTEGER; }
{REALNUM} { YYLVAL_DEBUG; return REAL; }
[ \t]+ {}
[\n] { line_no++; }
. { std::cerr<<"LEX error at line no." << line_no << std::endl; exit(1);}
%%
