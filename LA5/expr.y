%{

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

struct node
{
    char *name;
    int offset;
    struct node *next;
};
typedef struct node *symboltable;
struct node2
{
    int typeID;
    int typeNUM;
    int typeEXPR;
    char *id;
    int num;
    int expr;
};
typedef struct node2 *type_of_arg;

// symboltable from C
extern symboltable T;

// functions
int add_to_symboltable(char *id);
void handle_set_id_num(char *id, int num);
void handle_set_id_id(char *id1, char *id2);
void handle_set_id_expr(char *id, type_of_arg ex);
type_of_arg handle_num(int num);
type_of_arg handle_id(char *id);
type_of_arg handle_expr(type_of_arg ex);
int return_first_available_reg();
void free_regs(int reg_index);
void handle_when_regs_are_full();
type_of_arg handle_op(int op, type_of_arg arg1, type_of_arg arg2);
void print_accn_to_arg_type(type_of_arg arg);
void handle_expr_stmt(type_of_arg ex);

%}

%union { int val; char* str; struct node2 *arg; }
%token <val> NUM
%token <str> ID
%token SET ADD SUB MUL DIV MOD POW OB CB

%type <val> OP
%type <arg> ARG EXPR

%%

PROGRAM : STMT PROGRAM          
        | STMT                  
        ;

STMT    : SETSTMT               
        | EXPRSTMT              
        ;

SETSTMT : OB SET ID NUM CB      { handle_set_id_num($3, $4);  } 
        | OB SET ID ID CB       { handle_set_id_id($3, $4);   }
        | OB SET ID EXPR CB     { handle_set_id_expr($3, $4);    }
        ;

EXPRSTMT: EXPR                  { handle_expr_stmt($1); }       
        ;

EXPR    : OB OP ARG ARG CB      { $$ = handle_op($2, $3, $4); }
        ;

OP      : ADD           { $$ = ADD; }        
        | SUB           { $$ = SUB; }   
        | MUL           { $$ = MUL; }        
        | DIV           { $$ = DIV; }        
        | MOD           { $$ = MOD; }        
        | POW           { $$ = POW; }        
        ;    

ARG     : NUM           {  $$ = handle_num($1);  }         
        | ID            {  $$ = handle_id($1);   }
        | EXPR          {  $$ = $1; }
        ;

%%

void yyerror(const char *s) 
{
    fprintf(stderr, "Error : %s\n", s);
}