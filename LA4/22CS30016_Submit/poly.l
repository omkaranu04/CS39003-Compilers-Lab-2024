%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
%}

D       [2-9]
space   [ \t]

%%

{space}       { /* ignore */  }
{D}           { yylval.val = strdup(yytext); return D; }
"+"           { yylval.val = strdup(yytext); return PLUS; }
"-"           { yylval.val = strdup(yytext); return MINUS; }
"x"           { yylval.val = strdup(yytext); return VAR; }
"^"           { yylval.val = strdup(yytext); return EXP; }
"0"           { yylval.val = strdup(yytext); return ZERO; }
"1"           { yylval.val = strdup(yytext); return ONE; }
.             { /* ignore */ }

%%