%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
void yyerror(char *err);
struct node
{
    char *name;
    union 
    {
        char sign;
        int value;
    } inh;
    int val;
    struct node *children[3];
};
typedef struct node* Node;
Node addnode0(char *name);
Node addnode1(char *name, Node n1);
Node addnode2(char *name, Node n1, Node n2);
Node addnode3(char *name, Node n1, Node n2, Node n3);

Node root_node;

%}

%union { char *val; struct node* tree_node; }
%token <val> D PLUS MINUS VAR EXP ZERO ONE
%type  <tree_node> S P T X N M 
%start S

%%

S 
    : P 
    { $$ = addnode2("S", addnode0("+"), $1); root_node = $$; }
    | PLUS P 
    { $$ = addnode2("S", addnode0("+"), $2); root_node = $$; }
    | MINUS P
    { $$ = addnode2("S", addnode0("-"), $2); root_node = $$; }
    ;

P 
    : T 
    { $$ = addnode1("P", $1); }
    | T PLUS P
    { $$ = addnode3("P", $1, addnode0("+"), $3); }
    | T MINUS P
    { $$ = addnode3("P", $1, addnode0("-"), $3); }
    ;

T 
    : ONE
    { $$ = addnode1("T", addnode0("1")); }
    | N 
    { $$ = addnode1("T", $1); }
    | X
    { $$ = addnode1("T", $1); }
    | N X
    { $$ = addnode2("T", $1, $2); }
    ;

X
    : VAR
    { $$ = addnode1("X", addnode0("x")); }
    | VAR EXP N
    { $$ = addnode3("X", addnode0("x"), addnode0("^"), $3); }
    ;

N 
    : D
    { $$ = addnode1("N", addnode0($1)); }
    | ONE M
    { $$ = addnode2("N", addnode0("1"), $2); }
    | D M
    { $$ = addnode2("N", addnode0($1), $2); }
    ;

M 
    : ZERO
    { $$ = addnode1("M", addnode0("0")); }
    | ONE
    { $$ = addnode1("M", addnode0("1")); }
    | D
    { $$ = addnode1("M", addnode0($1)); }
    | ZERO M 
    { $$ = addnode2("M", addnode0("0"), $2); }
    | ONE M
    { $$ = addnode2("M", addnode0("1"), $2); }
    | D M
    { $$ = addnode2("M", addnode0($1), $2); }
    ;

%%