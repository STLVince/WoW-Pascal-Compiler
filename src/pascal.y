%require "3.5"
%language "c++"
%debug
%defines
%define api.namespace {ast}
%define api.value.type variant
%define parse.assert

%code requires {
    #include <iostream>
    #include <memory>
    #include <string>
    #include <stdexcept>
    #include <unordered_map>
    #include "utils/ast.hpp"
}

%code {
    int yylex(ast::parser::semantic_type* yylval);
    std::shared_ptr<ast::Program> astRoot;
}

%token PROGRAM ID SEMI DOT ASSIGN LP RP LB RB COMMA COLON
%token ARRAY _BEGIN SYS_TYPE CONST END FUNCTION PROCEDURE RECORD VAR TYPE
%token DOWNTO DO REPEAT TO THEN WHILE UNTIL FOR IF ELSE CASE OF GOTO
%token EQUAL UNEQUAL GE GT LE LT AND OR NOT PLUS MINUS MUL DIV DIVI MOD
%token INTEGER REAL CHAR STRING SYS_BOOL SYS_CON
%token READ SYS_PROC SYS_FUNCT

%type <std::string> PROGRAM ID SEMI ASSIGN LP RP LB RB DOT COMMA COLON
%type <std::string> ARRAY _BEGIN CONST END FUNCTION PROCEDURE RECORD VAR TYPE
%type <std::string> DOWNTO DO REPEAT TO THEN WHILE UNTIL FOR IF ELSE CASE OF GOTO
%type <std::string> EQUAL UNEQUAL GE GT LE LT AND OR NOT PLUS MINUS MUL DIV DIVI MOD
%type <std::string> CHAR STRING SYS_CON
%type <std::string> READ SYS_PROC SYS_FUNCT
%type <bool> SYS_BOOL
%type <int> INTEGER
%type <double> REAL
%type <ast::TypeName> SYS_TYPE

%type <std::shared_ptr<ast::Expression>>       expression expr term factor
%type <std::shared_ptr<ast::Statement>>        proc_stmt stmt non_label_stmt else_clause for_stmt repeat_stmt while_stmt if_stmt goto_stmt case_stmt
%type <std::shared_ptr<ast::Program>>          program program_head routine_head routine sub_routine
%type <std::shared_ptr<ast::Routine>>          function_decl function_head procedure_head procedure_decl
%type <std::shared_ptr<ast::TypeDecl>>         type_decl array_type_decl record_type_decl simple_type_decl
%type <std::shared_ptr<ast::TypeDef>>          type_definition
%type <std::shared_ptr<ast::ConstType>>        const_value
%type <std::shared_ptr<ast::AssignmentStmt>>   assign_stmt

%type <std::shared_ptr<ast::ConstDeclList>>    const_expr_list const_part
%type <std::shared_ptr<ast::TypeDeclList>>     type_part type_decl_list
%type <std::shared_ptr<ast::VarDeclList>>      parameters para_decl_list para_type_list var_part var_decl_list var_decl
%type <std::shared_ptr<ast::NameList>>         name_list;
%type <std::shared_ptr<ast::RoutineList>>      routine_part
%type <std::shared_ptr<ast::StatementList>>    routine_body compound_stmt stmt_list
%type <std::shared_ptr<ast::ExpressionList>>   expression_list
%type <std::shared_ptr<ast::CaseStmt>>		   case_expr 
%type <std::shared_ptr<ast::CaseList>>		   case_expr_list

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
        $$ = ast::make_node<ast::Program>($2, $3, $4, $5, nullptr);
    };

label_part: {};

const_part: CONST const_expr_list   { $$ = $2; }
    |   { $$ = ast::make_node<ast::ConstDeclList>(); };

const_expr_list:
    const_expr_list ID EQUAL const_value SEMI {
        $$ = $1;
        $$ -> push_back(ast::make_node<ast::ConstDecl>(ast::make_node<ast::Identifier>($2), $4));
    }
    | ID EQUAL const_value SEMI {
        $$ = ast::make_node<ast::ConstDeclList>();
        $$ -> push_back(ast::make_node<ast::ConstDecl>(ast::make_node<ast::Identifier>($1), $3));
    };

const_value:
    INTEGER     { $$ = ast::make_node<ast::IntegerType>($1); }
    | REAL      { $$ = ast::make_node<ast::RealType>($1); }
    | CHAR      { /*$$ = ast::make_node<ast::CharType>($1.c_str());*/ }
    | STRING    { $$ = ast::make_node<ast::StringType>($1); }
    | SYS_CON   { $$ = ast::make_node<ast::IntegerType>(32767); }
    | SYS_BOOL  { $$ = ast::make_node<ast::BooleanType>($1); };

type_part: TYPE type_decl_list  { $$ = $2; }
    |   { $$ = ast::make_node<ast::TypeDeclList>(); };

type_decl_list: type_decl_list type_definition {
        $$ = $1;
        $$ -> push_back($2);
    }
    | type_definition {
        $$ = ast::make_node<ast::TypeDeclList>();
        $$ -> push_back($1);
    };

type_definition: ID EQUAL type_decl SEMI {
        $$ = ast::make_node<ast::TypeDef>(ast::make_node<ast::Identifier>($1), $3);
    };

type_decl: simple_type_decl { $$ = $1; }
    | array_type_decl       { $$ = $1; }
    | record_type_decl      { $$ = $1; };

simple_type_decl:
    SYS_TYPE    { $$ = ast::make_node<ast::TypeDecl>($1); }
    | ID { 
        static std::unordered_map<std::string,ast::TypeName> const table = { {"integer",ast::TypeName::INTEGER}, {"real",ast::TypeName::REAL}, {"string", ast::TypeName::STRING}, {"char", ast::TypeName::STRING}, {"bool", ast::TypeName::BOOLEAN}, {"array", ast::TypeName::ARRAY}, {"record", ast::TypeName::RECORD} };
        auto it = table.find($1);
            if (it != table.end()) {
            $$ = ast::make_node<ast::TypeDecl>(it->second);
        } else {
            $$ = ast::make_node<ast::TypeDecl>(ast::TypeName::UNKNOWN); 
        }
    }
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
        $$ = ast::make_node<ast::NameList>();
        $$ -> push_back($1);
    };

var_part: VAR var_decl_list { $$ = $2; }
    |  	{ $$ = ast::make_node<ast::VarDeclList>(); };

var_decl_list: var_decl_list var_decl {
        $$ = $1;
        $1->insert($1->end(), $2->begin(), $2->end());
    }
    | var_decl {
        $$ = $1;
    };

var_decl: name_list COLON type_decl SEMI {
        $$ = ast::make_node<ast::VarDeclList>(); 
        for(auto name : *($1)) 
            $$->push_back(ast::make_node<ast::VarDecl>(ast::make_node<ast::Identifier>(name), $3));
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
        $$ = ast::make_node<ast::RoutineList>();
    };

function_decl: function_head SEMI sub_routine SEMI {
        $$ = ast::make_node<ast::Routine>($1, $3);
    };

function_head: FUNCTION ID parameters COLON simple_type_decl {
        $$ = ast::make_node<ast::Routine>(ast::make_node<ast::Identifier>($2), $5, $3, ast::RoutineType::FUNCTION);
    };

procedure_decl: procedure_head SEMI sub_routine SEMI {
        $$ = ast::make_node<ast::Routine>($1, $3);
    };

procedure_head: PROCEDURE ID parameters {
        $$ = ast::make_node<ast::Routine>(ast::make_node<ast::Identifier>($2), nullptr, $3,  ast::RoutineType::PROCEDURE);
    };

parameters: LP para_decl_list RP    { $$ = $2; }
    | LP RP { $$ = ast::make_node<ast::VarDeclList>(); };
    |   { $$ = ast::make_node<ast::VarDeclList>(); };

para_decl_list: para_decl_list SEMI para_type_list {
        $$ = $1; 
        $1->insert($1->end(), $3->begin(), $3->end());
    }
    | para_type_list {
        $$ = $1;
    };

para_type_list: VAR name_list COLON simple_type_decl {
        $$ = ast::make_node<ast::VarDeclList>(); 
        for(auto name: *($2)) 
            $$->push_back(ast::make_node<ast::VarDecl>(ast::make_node<ast::Identifier>(name), $4));
    }
    | name_list COLON simple_type_decl {
        $$ = ast::make_node<ast::VarDeclList>(); 
        for(auto name: *($1)) 
            $$->push_back(ast::make_node<ast::VarDecl>(ast::make_node<ast::Identifier>(name), $3));
    };

routine_body: compound_stmt { $$ = $1; };

compound_stmt: _BEGIN stmt_list END { $$ = $2; };

stmt_list: stmt_list  stmt  SEMI {
        $$ = $1;
        $$ -> get_list() -> push_back($2);
    }
    |   { $$ = ast::make_node<ast::StatementList>(); };

stmt: INTEGER COLON non_label_stmt {
        //CodeGenContext::labels.push_back($1);
        $$ = ast::make_node<ast::LabelStmt>($1, $3);
    }
    |   non_label_stmt { $$ = $1; };

non_label_stmt:
    assign_stmt     { $$ = $1; }
    | proc_stmt     { $$ = $1; }
    | compound_stmt { $$ = $1; }
    | if_stmt       { $$ = $1; }
    | repeat_stmt   { $$ = $1; }
    | while_stmt    { $$ = $1; }
    | for_stmt      { $$ = $1; }
    | case_stmt     { $$ = $1; }
    | goto_stmt     { $$ = $1; };

assign_stmt:
    ID ASSIGN expression    { $$ = ast::make_node<ast::AssignmentStmt>(ast::make_node<ast::Identifier>($1), $3); }
    | ID LB expression RB ASSIGN expression {}
    | ID DOT ID ASSIGN expression {};

proc_stmt: ID                           { $$ = ast::make_node<ast::ProcCall>(ast::make_node<ast::Identifier>($1)); }
    | ID LP expression_list RP          { $$ = ast::make_node<ast::ProcCall>(ast::make_node<ast::Identifier>($1), $3); }
    | SYS_PROC                          { $$ = ast::make_node<ast::SysProcCall>(ast::make_node<ast::Identifier>($1)); }
    | SYS_PROC LP expression_list RP    { $$ = ast::make_node<ast::SysProcCall>(ast::make_node<ast::Identifier>($1), $3); }
    | READ LP factor RP {};

if_stmt: IF expression THEN stmt else_clause {
        $$ = ast::make_node<ast::IfStmt>($2, $4, $5);
    };

else_clause: ELSE stmt { $$ = $2; }
    |   { $$ = nullptr; };

repeat_stmt: REPEAT stmt_list UNTIL expression {
        $$ = ast::make_node<ast::RepeatStmt>($4, $2);
    };

while_stmt: WHILE expression DO stmt {
        $$ = ast::make_node<ast::WhileStmt>($2, $4);
    };

for_stmt: FOR ID ASSIGN expression TO expression DO stmt {
        $$ = ast::make_node<ast::ForStmt>(1, ast::make_node<ast::Identifier>($2), $4, $6, $8);
    }
    | FOR ID ASSIGN expression DOWNTO expression DO stmt {
        $$ = ast::make_node<ast::ForStmt>(-1, ast::make_node<ast::Identifier>($2), $4, $6, $8);
    };

case_stmt: CASE expression OF case_expr_list END {
        $$ = ast::make_node<ast::SwitchStmt>($2, $4);
    };

case_expr_list: case_expr_list case_expr {
        $$ -> push_back($2);
    }
    | case_expr {
        $$ = ast::make_node<ast::CaseList>();
        $$ -> push_back($1);
    };

case_expr: const_value COLON stmt SEMI  {
        $$ = ast::make_node<ast::CaseStmt>($1, $3);
    }
    | ID COLON stmt SEMI {
        $$ = ast::make_node<ast::CaseStmt>(ast::make_node<ast::Identifier>($1), $3);
    };

goto_stmt: GOTO INTEGER { $$ = ast::make_node<ast::GotoStmt>($2); };

expression_list: expression_list COMMA expression {
        $$ = $1;
        $$ -> push_back($3);
    }
    | expression{
        $$ = ast::make_node<ast::ExpressionList>();
        $$ -> push_back($1);
    };

expression:
    expression GE expr          { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::GE, $3); }
    | expression GT expr        { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::GT, $3); }
    | expression LE expr        { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::LE, $3); }
    | expression LT expr        { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::LT, $3); }
    | expression EQUAL expr     { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::EQ, $3); }
    | expression UNEQUAL expr   { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::NE, $3); }
    | expr                      { $$ = $1; };

expr:
    expr PLUS term      { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::PLUS, $3); }
    | expr MINUS term   { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::MINUS, $3); }
    | expr OR term      { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::OR, $3); }
    | term              { $$ = $1; };

term:
    term MUL factor     { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::MUL, $3); }
    | term DIV factor   { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::DIV, $3); }
    | term MOD factor   { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::MOD, $3); }
    | term AND factor   { $$ = ast::make_node<ast::BinaryOp>($1, ast::OpType::AND, $3); }
    | factor            { $$ = $1; };

factor:
    ID                                  { $$ = ast::make_node<ast::Identifier>($1); }
    | ID LP expression_list RP          { $$ = ast::make_node<ast::FuncCall>(ast::make_node<ast::Identifier>($1), $3); }
    | SYS_FUNCT                         { $$ = ast::make_node<ast::SysFuncCall>(ast::make_node<ast::Identifier>($1)); }
    | SYS_FUNCT LP expression_list RP   { $$ = ast::make_node<ast::SysFuncCall>(ast::make_node<ast::Identifier>($1), $3); }
    | const_value                       { $$ = $1; }
    | LP expression RP                  { $$ = $2; }
    | NOT factor                        { $$ = ast::make_node<ast::BinaryOp>(ast::make_node<ast::BooleanType>("true"), ast::OpType::XOR, $2); }
    | MINUS factor                      { $$ = ast::make_node<ast::BinaryOp>(ast::make_node<ast::IntegerType>(0), ast::OpType::MINUS, $2); }
    | ID LB expression RB          { //$$ = ast::make_node<ast::ArrayType>(ast::make_node<ast::Identifier>($1), $3); 
    }
    | ID DOT ID                         { //$$ = ast::make_node<ast::RecordType>(ast::make_node<ast::Identifier>($1), ast::make_node<ast::Identifier>($3)); 
    };

%%

extern int line_no;
void ast::parser::error(const std::string& msg)
{
    std::cerr<<"YACC error at line no." << line_no << " : " << msg << std::endl;
}