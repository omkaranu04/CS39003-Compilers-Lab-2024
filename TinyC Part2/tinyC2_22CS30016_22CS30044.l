%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
%}

/* SPACES */
space                       [ \t]
newline                     \n

/* KEYWORDS */
keyword                     "auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"inline"|"int"|"long"|"register"|"restrict"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"|"_Bool"|"_Complex"|"_Imaginary"

/* IDENTIFIERS */
digit                       [0-9]
identifier_nondigit         [_a-zA-Z]
identifier                  {identifier_nondigit}({identifier_nondigit}|{digit})*

/* CONSTANTS */
integer_constant            {digit}+
sign                        [+-]
digit_sequence              {digit}+
exponent_part               (e{sign}?{digit_sequence})|(E{sign}?{digit_sequence})
fractional_constant         ({digit_sequence}?"."{digit_sequence})|({digit_sequence}".")
floating_constant           ({fractional_constant}{exponent_part}?)|({digit_sequence}{exponent_part})
escape_sequence             \\['"?\\abfnrtv]
c_char                      [^'\\\n]|{escape_sequence}
c_char_sequence             {c_char}+
character_constant          \'({c_char_sequence})\'
constant                    {integer_constant}|{floating_constant}|{character_constant}

/* STRING LITERALS */
s_char                      [^"\\\n]|{escape_sequence}
s_char_sequence             {s_char}+
string_literal              \"{s_char_sequence}?\"

/* PUNCTUATORS */
punctuator                  "["|"]"|"("|")"|"{"|"}"|"."|"->"|"++"|"--"|"&"|"*"|"+"|"-"|"~"|"!"|"/"|"%"|"<<"|">>"|"<"|">"|"<="|">="|"=="|"!="|"^"|"|"|"&&"|"||"|"?"|":"|";"|"..."|","|"#"|"="|"*="|"/="|"%="|"+="|"-="|"<<="|">>="|"&="|"^="|"|="

/* COMMENTS */
single_line_comment         \/\/[^\n]*
multi_line_comment          "/*"([^*]|[*][^/])*"*/"
comment                     {single_line_comment}|{multi_line_comment}

%%

{comment}                   { /* ignore */ }
{newline}                   { yylineno++; }

"["                         { return SQUARE_BRAC_OPEN; }
"]"                         { return SQUARE_BRAC_CLOSE; }
"("                         { return ROUND_BRAC_OPEN; }
")"                         { return ROUND_BRAC_CLOSE; }    
"{"                         { return CURLY_BRAC_OPEN; }
"}"                         { return CURLY_BRAC_CLOSE; }
"."                         { return DOT; }  
"->"                        { return ARROW; }
"++"                        { return INCREMENT_OP; }
"--"                        { return DECREMENT_OP; }
"*"                         { return MULTIPLY_OP; }
"+"                         { return ADD_OP; }
"-"                         { return SUBTRACT_OP; }
"&"                         { return BITWISE_AND_OP; }
"~"                         { return BITWISE_NOT_OP; }
"!"                         { return LOGICAL_NOT_OP; }   
"/"                         { return DIVIDE_OP; }
"%"                         { return MODULO_OP; }
"<<"                        { return LEFT_SHIFT_OP; }
">>"                        { return RIGHT_SHIFT_OP; }
"<"                         { return LESS_THAN_OP; } 
">"                         { return GREATER_THAN_OP; }
"<="                        { return LESS_THAN_OR_EQUAL_OP; }
">="                        { return GREATER_THAN_OR_EQUAL_OP; }
"=="                        { return EQUAL_TO_OP; }
"!="                        { return NOT_EQUAL_TO_OP; }
"^"                         { return BITWISE_XOR_OP; }
"|"                         { return BITWISE_OR_OP; }
"&&"                        { return LOGICAL_AND_OP; }
"||"                        { return LOGICAL_OR_OP; }
"?"                         { return QUESTION_MARK; }
":"                         { return COLON; }
";"                         { return SEMICOLON; }
"..."                       { return ELLIPSIS; }
"="                         { return ASSIGN_OP; }
"*="                        { return MULTIPLY_ASSIGN_OP; }
"/="                        { return DIVIDE_ASSIGN_OP; }
"%="                        { return MODULO_ASSIGN_OP; }
"+="                        { return ADD_ASSIGN_OP; }
"-="                        { return SUBTRACT_ASSIGN_OP; }
"<<="                       { return LEFT_SHIFT_ASSIGN_OP; }
">>="                       { return RIGHT_SHIFT_ASSIGN_OP; }
"&="                        { return BITWISE_AND_ASSIGN_OP; }
"^="                        { return BITWISE_XOR_ASSIGN_OP; }
"|="                        { return BITWISE_OR_ASSIGN_OP; }
","                         { return COMMA; }
"#"                         { return HASH; }

"auto"                      { return AUTO; }
"break"                     { return BREAK; }
"case"                      { return CASE; }
"char"                      { return CHAR; }
"const"                     { return CONST; }
"continue"                  { return CONTINUE; }
"default"                   { return DEFAULT; }
"do"                        { return DO; }
"double"                    { return DOUBLE; }
"else"                      { return ELSE; }
"enum"                      { return ENUM; }
"extern"                    { return EXTERN; }
"float"                     { return FLOAT; }
"for"                       { return FOR; }
"goto"                      { return GOTO; }
"if"                        { return IF; }
"inline"                    { return INLINE; }
"int"                       { return INT; }
"long"                      { return LONG; }
"register"                  { return REGISTER; }
"restrict"                  { return RESTRICT; }
"return"                    { return RETURN; }
"short"                     { return SHORT; }
"signed"                    { return SIGNED; }
"sizeof"                    { return SIZEOF; }
"static"                    { return STATIC; }
"struct"                    { return STRUCT; }
"switch"                    { return SWITCH; }
"typedef"                   { return TYPEDEF; }
"union"                     { return UNION; }
"unsigned"                  { return UNSIGNED; }
"void"                      { return VOID; }
"volatile"                  { return VOLATILE; }
"while"                     { return WHILE; }
"_Bool"                     { return BOOL; }
"_Complex"                  { return COMPLEX; }
"_Imaginary"                { return IMAGINARY; }

{identifier}                { yylval.val = strdup(yytext); return IDENTIFIER; }
{integer_constant}          { yylval.val = strdup(yytext); return INTEGER_CONSTANT; }
{floating_constant}         { yylval.val = strdup(yytext); return FLOAT_CONSTANT; }
{character_constant}        { yylval.val = strdup(yytext); return CHAR_CONSTANT; }
{string_literal}            { yylval.val = strdup(yytext); return STRING; }
{space}                     { /* ignore */ }
.                           { /* ignore */ }

%%