%{
#include <iostream>
#include "parser.hpp"
#include "utils/ast.hpp"

ast::Program* astRoot;
%}

%union{
    char*                   debug;
    ast::Node*              astNode;
    ast::Expression*        astExpression;
    ast::Statement*         astStatement;
    ast::Program*           astProgram;
    ast::Routine*           astRoutine;
    ast::TypeDecl*          astTypeDecl;
    ast::TypeDef*           astTypeDef;
    ast::ConstDecl*         astConstDecl;
    ast::VarDecl*           astVarDecl;
    ast::Identifier*        astIdentifier;
    ast::AssignmentStmt*    astAssignmentStmt;

    ast::ConstDeclList*     astConstDeclList;
    ast::TypeDeclList*      astTypeDeclList;
    ast::VarDeclList*       astVarDeclList;
    ast::RoutineList*       astRoutineList;
    ast::StatementList*     astStatementList;
    ast::ExpressionList*    astExpressionList;
}

%token PROGRAM ID SEMI DOT ASSIGN LP RP LB RB DOT COMMA COLON SEMI
%token ARRAY _BEGIN SYS_TYPE CONST END FUNCTION PROCEDURE RECORD VAR TYPE
%token DOWNTO DO REPEAT TO THEN WHILE UNTIL FOR IF ELSE CASE OF GOTO
%token EQUAL UNEQUAL GE GT LE LT AND OR NOT PLUS MINUS MUL DIV DIVI MOD
%token INTEGER REAL CHAR STRING SYS_BOOL SYS_CON
%token READ SYS_PROC SYS_FUNCT

%type <debug> PROGRAM ID SEMI DOT ASSIGN LP RP LB RB DOT COMMA COLON SEMI
%type <debug> ARRAY _BEGIN SYS_TYPE CONST END FUNCTION PROCEDURE RECORD VAR TYPE
%type <debug> DOWNTO DO REPEAT TO THEN WHILE UNTIL FOR IF ELSE CASE OF GOTO
%type <debug> EQUAL UNEQUAL GE GT LE LT AND OR NOT PLUS MINUS MUL DIV DIVI MOD
%type <debug> INTEGER REAL CHAR STRING SYS_BOOL SYS_CON
%type <debug> READ SYS_PROC SYS_FUNCT

%type <astExpression>       expression expr term factor
%type <astStatement>        proc_stmt stmt non_label_stmt else_clause for_stmt repeat_stmt while_stmt if_stmt goto_stmt case_stmt
%type <astProgram>          program program_head routine_head routine sub_routine
%type <astRoutine>          function_decl function_head procedure_head procedure_decl
%type <astTypeDecl>         type_decl array_type_decl record_type_decl simple_type_decl
%type <astTypeDef>          type_definition
%type <astConstDecl>        const_value
%type <astAssignmentStmt>   assign_stmt

%type <astConstDeclList>    const_expr_list const_part
%type <astTypeDeclList>     type_part type_decl_list
%type <astVarDeclList>      parameters para_decl_list para_type_list var_part var_decl_list var_decl
%type <astRoutineList>      routine_part
%type <astStatementList>    routine_body compound_stmt stmt_list
%type <astExpressionList>   expression_list

%start program
%%

program: program_head routine DOT   { astRoot = $2; };

program_head: PROGRAM ID SEMI {} | {};

routine: routine_head routine_body {
        $$ = $1;
        $$ -> routine_body = $2;
    };

sub_routine: routine_head routine_body {
        $$ = $1;
        $$ -> routine_body = $2;
    };

routine_head:
    label_part const_part type_part var_part routine_part {
        $$ = new ast::Program($2, $3, $4, $5, nullptr);
    };

label_part: {};

const_part: CONST const_expr_list   { $$ = $2; }
    |   { $$ = new ast::ConstDeclList(); };

const_expr_list:
    const_expr_list ID EQUAL const_value SEMI {
        $$ = $1;
        $$ -> push_back(new ast::ConstDecl(new ast::Identifier($2), $4);)
    }
    | ID EQUAL const_value SEMI {
        $$ = new ast::ConstDeclList();
        $$ -> push_back(new ast::ConstDecl(new ast::Identifier($1), $3);)
    };

const_value:
    INTEGER     { $$ = new ast::IntegerType(atoi($1)); }
    | REAL      { $$ = new ast::RealType(atof($1)); }
    | CHAR      { $$ = new ast::CharType($1); }
    | STRING    { $$ = new ast::StringType($1); }
    | SYS_CON   { $$ = new ast::IntegerType(32767); }
    | SYS_BOOL  { $$ = new ast::BooleanType($1); };

type_part: TYPE type_decl_list  { $$ = $2; }
    |   { $$ = new ast::TypeDeclList(); };

type_decl_list: type_decl_list type_definition {
        $$ = $1;
        $$ -> push_back($2);
    }
    | type_definition {
        $$ = new ast::TypeDeclList();
        $$ -> push_back($1);
    };

type_definition: ID EQUAL type_decl SEMI {
        $$ = new ast::TypeDef(new ast::Identifier($1), $3);
    };

type_decl: simple_type_decl { $$ = $1; }
    | array_type_decl       { $$ = $1; }
    | record_type_decl      { $$ = $1; };

simple_type_decl:
    SYS_TYPE    { $$ = new ast::TypeDecl($1); }
    | ID        { $$ = new ast::TypeDecl(std::string($1)); }
    | LP name_list RP {}
    | INTEGER DOT DOT INTEGER 	{}
    | CHAR DOT DOT CHAR 	{}
    | SYS_BOOL DOT DOT SYS_BOOL {} 
    | MINUS INTEGER DOT DOT INTEGER	{}
    | MINUS INTEGER DOT DOT MINUS INTEGER {}
    | STRING DOT DOT STRING {}
    | ID DOT DOT ID {};

array_type_decl:
	ARRAY LB simple_type_decl RB OF type_decl {};

record_type_decl:
	RECORD field_decl_list END 	{};

field_decl_list:
	field_decl_list field_decl {}
    | field_decl {};

field_decl:
	name_list COLON type_decl SEMI {};

name_list: name_list COMMA ID {
        $$ = $1;
        $$ -> push_back($3);
    }
    | ID {
        $$ = new ast::Identifier($1);
        // $$ -> push_back($1);
    };

var_part: VAR var_decl_list { $$ = $2; }
    |  	{ $$ = new ast::VarDeclList(); };

var_decl_list: var_decl_list var_decl {
        $$ = $1;
        $$ -> push_back($2);
    }
    | var_decl {
        $$ = new ast::VarDeclList();
        $$ -> push_back($1);
    };

var_decl: name_list COLON type_decl SEMI {
        $$ = new ast::VarDecl($1, $3);
    };

routine_part: routine_part function_decl {
        $$ = $1;
        $$ -> push_back($2);
    }
    | routine_part procedure_decl {
        $$ = $1;
        $$ -> push_back($2);
    }
    | {
        $$ = new ast::RoutineList();
    };

function_decl: function_head SEMI sub_routine SEMI {
        $$ = new ast::Routine($1, $3);
    };

function_head: FUNCTION parameters COLON simple_type_decl {
        $$ = new ast::Routine(new ast::Identifier($2), ast::RoutineType::FUNCTION, $3, $5);
    };

procedure_decl: procedure_head SEMI sub_routine SEMI {
        $$ = new ast::Routine($1, $3);
    };

procedure_head: PROCEDURE ID parameters {
        $$ = new ast::Routine(new ast::Identifier($2), ast::RoutineType::PROCEDURE, $3, nullptr);
    };

parameters: LP para_decl_list RP    { $$ = $2; }
    |   { $$ = new ast::VarDeclList(); };

para_decl_list: para_decl_list SEMI para_type_list {
        $$ = $1;
        $$ -> push_back($3);
    }
    | para_type_list {
        $$ = new ast::VarDeclList();
        $$ -> push_back($1);
    };

para_type_list: VAR name_list COLON simple_type_decl {
        $$ = new ast::VarDecl($2,$4);
    }
    | name_list COLON simple_type_decl {
        $$ = new ast::VarDecl($1,$3);
    };

routine_body: compound_stmt { $$ = $1; };

compound_stmt: _BEGIN stmt_list END { $$ = $2; };

stmt_list: stmt_list  stmt  SEMI {
        $$ = $1;
        $$ -> push_back($2);
    }
    |   { $$ = new ast::StatementList(); };

stmt: INTEGER COLON non_label_stmt {
        $$ = new ast::LabelStmt(atoi($1), $3);
    }
    |   { $$ = $1; };

non_label_stmt:
    assign_stmt     { $$ = (ast::Statement*) $1; }
    | proc_stmt     { $$ = (ast::Statement*) $1; }
    | compound_stmt { $$ = (ast::Statement*) $1; }
    | if_stmt       { $$ = (ast::Statement*) $1; }
    | repeat_stmt   { $$ = (ast::Statement*) $1; }
    | while_stmt    { $$ = (ast::Statement*) $1; }
    | for_stmt      { $$ = (ast::Statement*) $1; }
    | case_stmt     { $$ = (ast::Statement*) $1; }
    | goto_stmt     { $$ = (ast::Statement*) $1; };

assign_stmt:
    ID ASSIGN expression    { $$ = new ast::AssignmentStmt(new ast::Identifier($1), $3); }
    | ID LB expression RB ASSIGN expression {}
    | ID DOT ID ASSIGN expression {};

proc_stmt: ID                           { $$ = new ast::ProcCall(new ast::Identifier($1)); }
    | ID LP expression_list RP          { $$ = new ast::ProcCall(new ast::Identifier($1), $3); }
    | SYS_PROC                          { $$ = new ast::SysProcCall(new ast::Identifier($1)); }
    | SYS_PROC LP expression_list RP    { $$ = new ast::SysProcCall(new ast::Identifier($1), $3); }
    | READ LP factor RP {};

if_stmt: IF expression THEN stmt else_clause {
        $$ = new ast::IfStmt($2, $4, $5);
    };

else_clause: ELSE stmt { $$ = $2; }
    |   { $$ = nullptr; };

repeat_stmt: REPEAT stmt_list UNTIL expression {
        $$ = new ast::RepeatStmt($4, $2);
    };

while_stmt: WHILE expression DO stmt {
        $$ = new ast::WhileStmt($2, $4);
    };

for_stmt: FOR ID ASSIGN expression TO expression DO stmt {
        $$ = new ast::ForStmt(1, new ast::Identifier($2), $4, $6, $8);
    }
    | FOR ID ASSIGN expression DOWNTO expression DO stmt {
        $$ = new ast::ForStmt(-1, new ast::Identifier($2), $4, $6, $8);
    };

case_stmt: CASE expression OF case_expr_list END {
        $$ = new ast::SwitchStmt($2, $4);
    };

case_expr_list: case_expr_list case_expr {
        $$ = $1;
        $$ -> push_back($2);
    }
    | case_expr {
        $$ = new ast::CaseList();
        $$ -> push_back($1);
    };

case_expr: const_value COLON stmt SEMI  {
        $$ = new ast::CaseStmt($1, $3);
    }
    | ID COLON stmt SEMI {
        $$ = new ast::CaseStmt(new ast::Identifier($1), $3);
    };

goto_stmt: GOTO INTEGER { $$ = new ast::GotoStmt(atoi($2)); };

expression_list: expression_list COMMA expression {
        $$ = $1;
        $$ -> push_back($3);
    }
    | expression{
        $$ = new ast::ExpressionList();
        $$ -> push_back($1);
    };

expression:
    expression GE expr          { $$ = new ast::BinaryOp($1, ast::OpType::GE, $3); }
    | expression GT expr        { $$ = new ast::BinaryOp($1, ast::OpType::GT, $3); }
    | expression LE expr        { $$ = new ast::BinaryOp($1, ast::OpType::LE, $3); }
    | expression LT expr        { $$ = new ast::BinaryOp($1, ast::OpType::LT, $3); }
    | expression EQUAL expr     { $$ = new ast::BinaryOp($1, ast::OpType::EQ, $3); }
    | expression UNEQUAL expr   { $$ = new ast::BinaryOp($1, ast::OpType::NE, $3); }
    | expr                      { $$ = $1; };

expr:
    expr PLUS term      { $$ = new ast::BinaryOp($1, ast::OpType::PLUS, $3); }
    | expr MINUS term   { $$ = new ast::BinaryOp($1, ast::OpType::MINUS, $3); }
    | expr OR term      { $$ = new ast::BinaryOp($1, ast::OpType::OR, $3); }
    | term              { $$ = $1; };

term:
    term MUL factor     { $$ = new ast::BinaryOp($1, ast::OpType::MUL, $3); }
    | term DIV factor   { $$ = new ast::BinaryOp($1, ast::OpType::DIV, $3); }
    | term MOD factor   { $$ = new ast::BinaryOp($1, ast::OpType::MOD, $3); }
    | term AND factor   { $$ = new ast::BinaryOp($1, ast::OpType::AND, $3); }
    | factor            { $$ = $1; };

factor:
    ID                                  { $$ = new ast::Identifier($1); }
    | ID LP expression_list RP          { $$ = new ast::FuncCall(new ast::Identifier($1), $3); }
    | SYS_FUNCT                         { $$ = new ast::SysFuncCall(new ast::Identifier($1)); }
    | SYS_FUNCT LP expression_list RP   { $$ = new ast::SysFuncCall(new ast::Identifier($1), $3); }
    | const_value                       { $$ = $1; }
    | LP expression RP                  { $$ = $2; }
    | NOT factor                        { $$ = new ast::BinaryOp(new ast::BooleanType("true"), ast::OpType::XOR, $2); }
    | MINUS factor                      { $$ = new ast::BinaryOp(new ast::IntegerType(0), ast::OpType::MINUS, $2); }
    | ID LB expression_list RB          { $$ = new ast::ArrayType(new ast::Identifier($1), $3); }
    | ID DOT ID                         { $$ = new ast::RecordType(new ast::Identifier($1), new ast::Identifier($3)); };

%%