%{
#include <stdio.h>

#define ACTIVE_CHARACTER    1
#define COMMAND_1           2
#define COMMAND_2           3
#define ENV_START           4
#define ENV_END             5
#define INLINE_MATH_START   6
#define INLINE_MATH_END     7
#define DISPLAY_MATH_START  8
#define DISPLAY_MATH_END    9
#define COMMENT             10
#define SKIP                11

int in_inline_math = 0;
%}

/* Definitions for LaTeX components */
SPACE               [ \t\n]
SPACES              {SPACE}+
ACTIVE_CHARACTER    [\~]
COMMAND_1           \\[A-Za-z]+
COMMAND_2           \\[^A-Za-z \~]

/* LaTeX environments */
ENV_START           \\begin\s*\{[A-Za-z]+\}
ENV_END             \\end\s*\{[A-Za-z]+\}

/* Displayed math */
DISPLAY_MATH_START  [\$\$] | "\\""\["
DISPLAY_MATH_END    [\$\$] | "\\""\]"

/* Inline math */
INLINE_MATH         [^\\][^\$][\$][^\$]

/* Comments */
COMMENT             %.*\n

/* Skip non-Latex text */
SKIP                [^~\\$\[\]%\n]+

%%

{SPACES}                { /* Ignore spaces */ }
{ACTIVE_CHARACTER}      { return (ACTIVE_CHARACTER); }
{COMMAND_1}             { return (COMMAND_1); }
{COMMAND_2}             { return (COMMAND_2); }
{ENV_START}             { return (ENV_START); }     
{ENV_END}               { return (ENV_END); }
{DISPLAY_MATH_START}    { return (DISPLAY_MATH_START); }
{DISPLAY_MATH_END}      { return (DISPLAY_MATH_END); }
{INLINE_MATH}           { 
                          if (in_inline_math == 0) {
                            in_inline_math = 1;
                            return (INLINE_MATH_START);
                          } else {
                            in_inline_math = 0;
                            return (INLINE_MATH_END);
                          }
                        }
{COMMENT}               { /* Ignore comments */ }
{SKIP}                  { /* Ignore other text */ }

%%