%{

#include"custom.h"
symboltable symbolTable = NULL;
TreeNode myTree = NULL;

%}

%debug

%union {char* str; TreeNode node; int num; }

%type <node> PROGRAM STMT SETSTMT EXPRSTMT EXPR ARG
%type <str> OP

%token <str> ID NUM
%token SET POW ADD SUB MUL DIV MOD
%token OB CB INVALID


%%

PROGRAM : STMT PROGRAM          
        | STMT                  
        ;

STMT    : SETSTMT               
        | EXPRSTMT              
        ;

SETSTMT : OB SET ID NUM CB    { set_symbol_value(&symbolTable, $3, atoi($4)); }
        | OB SET ID ID CB     { set_symbol_value(&symbolTable, $3, get_symbol_value(&symbolTable, $4)); }
        | OB SET ID EXPR CB   { set_symbol_value(&symbolTable, $3, evaluate_expr_tree($4)); free_expr_tree($4); }
        ;

EXPRSTMT: EXPR                { printf("Standalone expression evaluates to %d\n", evaluate_expr_tree($1)); free_expr_tree($1); }
        ;

EXPR    : OB OP ARG ARG CB    { $$ = create_op_node($2, $3, $4); }
        ;

OP      : ADD                   { $$ = "+"; }
        | SUB                   { $$ = "-"; }
        | MUL                   { $$ = "*"; }
        | DIV                   { $$ = "/"; }
        | MOD                   { $$ = "%"; }
        | POW                   { $$ = "**"; }
        ;    

ARG     : NUM                   { $$ = create_leaf_node(symbolTable, TYPE_NUMBER, $1); }
        | ID                    { $$ = create_leaf_node(symbolTable, TYPE_VARIABLE, $1); }
        | EXPR                  { $$ = $1; }
        ;

%%

void yyerror(const char *s) 
{
    fprintf(stderr, "Error : %s\n", s);
}