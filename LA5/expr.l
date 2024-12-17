%{
    
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

%}

space           [ \n\t]
spaces          {space}+
NUM             [-+]?[0-9]+
letter          [_a-zA-Z]
ID              {letter}[_a-zA-Z0-9]*

%%

{spaces}        {               }
"set"           { return (SET); }  
{ID}            { yylval.str = strdup(yytext); return (ID); }
{NUM}           { yylval.val = atoi(yytext); return (NUM); }
"**"            { return (POW); }
"-"		        { return (SUB); }
"+"		        { return (ADD); }
"*"		        { return (MUL); }
"/"		        { return (DIV); }
"%"		        { return (MOD); }
"("             { return (OB); }
")"             { return (CB); }
.               { /* ignore */ }

%%

int yywrap()
{
    return 1;
}