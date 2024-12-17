%{
#define     KEYWORD         1   
#define     IDENTIFIER      2
#define     CONSTANTS       3
#define     STRINGLITERALS  4
#define     PUNCTUATORS     5
#define     COMMENTS        6
%}
/* SPACES */
space                       [ \t\n]

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
single_line_comment         \/\/[^\n]*\n
multi_line_comment          "/*"([^*]|[*][^/])*"*/"
comment                     {single_line_comment}|{multi_line_comment}

%%
{space}                     {                           }
{keyword}                   { return KEYWORD;           }
{identifier}                { return IDENTIFIER;        }
{constant}                  { return CONSTANTS;         }
{string_literal}            { return STRINGLITERALS;    }
{punctuator}                { return PUNCTUATORS;       }
{comment}                   { return COMMENTS;          }
%%