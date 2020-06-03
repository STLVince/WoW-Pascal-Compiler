%{
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include "parser.hpp"
#include "utils/ast.hpp"

#undef YY_DECL
#define YY_DECL int yylex(ast::parser::semantic_type* yylval)
using token = ast::parser::token::yytokentype;
int line_no = 1;
%}

%option noyywrap 
%option caseless

NQUOTE [^']
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
":=" { return token::ASSIGN; }
"=" { return token::EQUAL; }
"(" { return token::LP; }
")" { return token::RP; }
"[" { return token::LB; }
"]" { return token::RB; }
"." { return token::DOT; }
"," { return token::COMMA; }
":" { return token::COLON; }
";" { return token::SEMI; }
"+" { return token::PLUS; }
"-" { return token::MINUS; }
"*" { return token::MUL; }
"/" { return token::DIV; }
">=" { return token::GE; }
">" { return token::GT; }
"<=" { return token::LE; }
"<" { return token::LT; }
"<>" { return token::UNEQUAL; }

and { return token::AND; }
abs { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
array { return token::ARRAY; }
begin { return token::_BEGIN; }
boolean { yylval->build<ast::TypeName>(ast::TypeName::BOOLEAN); return token::SYS_TYPE; }
case { return token::CASE; }
const { return token::CONST; }
char { yylval->build<ast::TypeName>(ast::TypeName::CHARACTER); return token::SYS_TYPE; }
chr { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
downto { return token::DOWNTO; }
do { return token::DO; }
div { return token::DIV; }
end { return token::END; }
else { return token::ELSE; }
false { yylval->build<bool>(false); return token::SYS_BOOL; }
for { return token::FOR; }
function { return token::FUNCTION; }
goto { return token::GOTO; }
integer { yylval->build<ast::TypeName>(ast::TypeName::INTEGER); return token::SYS_TYPE; }
if { return token::IF; }
mod { return token::MOD; }
maxint { yylval->build<std::string>(yytext); return token::SYS_CON; }
not { return token::NOT; }
odd { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
of { return token::OF; }
ord { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
or { return token::OR; }
pred { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
program { return token::PROGRAM; }
procedure { return token::PROCEDURE; }
read { return token::READ; }
real { yylval->build<ast::TypeName>(ast::TypeName::REAL); return token::SYS_TYPE; }
record { return token::RECORD; }
repeat { return token::REPEAT; }
sqrt { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
sqr { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
string { yylval->build<ast::TypeName>(ast::TypeName::STRING); return token::SYS_TYPE; }
succ { yylval->build<std::string>(yytext); return token::SYS_FUNCT; }
to { return token::TO; }
then { return token::THEN; }
true { yylval->build<bool>(true); return token::SYS_BOOL; }
type { return token::TYPE; }
until { return token::UNTIL; }
var { return token::VAR; }
while { return token::WHILE; }
writeln { yylval->build<std::string>(yytext); return token::SYS_PROC; }
write { yylval->build<std::string>(yytext); return token::SYS_PROC; }

"'"{ANY}"'" { yylval->build<char>(yytext[0]); return token::CHAR; }
"'"{ANY}{ANY}+"'" { yylval->build<std::string>(yytext); return token::STRING; }
{ID} { yylval->build<std::string>(yytext); return token::ID; }
{INTNUM} { yylval->build<int>(atoi(yytext)); return token::INTEGER; }
{REALNUM} { yylval->build<double>(atof(yytext)); return token::REAL; }
[ \t]+ {}
\n { line_no++; }
. { std::cerr<<"LEX error at line no." << line_no << std::endl; exit(1);}
%%
