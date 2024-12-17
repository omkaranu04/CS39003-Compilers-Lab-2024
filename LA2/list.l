%{
#define OB          1
#define CB          2
#define OP          3
#define NUM         4
#define ID          5
#define INVALID     8
%}

space           [ \n\t]
spaces          {space}+
NUM             [-]?[0-9]+
letter          [_a-zA-Z]
ID              {letter}[_a-zA-Z0-9]*

%%

{spaces}        { }
{ID}            { return (ID); }
{NUM}           { return (NUM); }
"-"		        { return(OP); }
"+"		        { return(OP); }
"*"		        { return(OP); }
"/"		        { return(OP); }
"%"		        { return(OP); }
"("             { return (OB); }
")"             { return (CB); }
.               { return (INVALID); }

%%