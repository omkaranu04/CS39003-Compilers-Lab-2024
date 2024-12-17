%{
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
%}


/* LATEX COMMANDS */

LETTER              [A-Za-z]
DIGITS              [0-9]
ACTIVE_CHARACTER    [\~]
COMMAND_1           [\\]{LETTER}+
COMMAND_2           [\\][^{LETTER}] & [\\][^[\~]]


/* LATEX ENVIRONMENTS*/
SPACE               [ \t\n]
SPACES              {SPACE}+
ENV_START           "\\"{SPACE}*"begin"{SPACE}*"\{"{LETTER}+"\}"
ENV_END             "\\"{SPACE}*"end"{SPACE}*"\{"{LETTER}+"\}"


/* DISPLAYED MATH */
DISPLAY_MATH_START  [\$][\$] | [\\]"\["
DISPLAY_MATH_END    [\$][\$] | [\\]"\]"

/* INLINE MATH */
INLINE_MATH_START   [\$]
INLINE_MATH_END     [\$]



/* COMMENT */
COMMENT             ^[\\][\%].*"\n"

/* SKIP */

%%

{SPACES}                { }
{ACTIVE_CHARACTER}      { return (ACTIVE_CHARACTER); }
{COMMAND_1}             { return (COMMAND_1); }
{COMMAND_2}             { return (COMMAND_2); }
{ENV_START}             { return (ENV_START); }     
{ENV_END}               { return (ENV_END); }
{DISPLAY_MATH_START}    { return (DISPLAY_MATH_START); }
{DISPLAY_MATH_END}      { return (DISPLAY_MATH_END); }
{INLINE_MATH_START}     { return (INLINE_MATH_START); }
{INLINE_MATH_END}       { return (INLINE_MATH_END); }
{COMMENT}               { return (COMMENT); } 

%%