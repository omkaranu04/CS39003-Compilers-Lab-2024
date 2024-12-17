%{
    #include<iostream>
    using namespace std;
    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    void yyerror(string StmtClass);
    extern string data_type;
%}

%code requires {
    #include "TinyC3_22CS30016_22CS30044_translator.h"
}

%union {
    int intval;
    char *strval;
    symbol *symboll;
    symbolType *symbollType;
    ExprClass *expr;
    StmtClass *stmt;
    ArrClass *arr;
    int ins_num;
    char unaryOpr;
    int paramCnt;
}


%token SQUARE_BRAC_OPEN
%token SQUARE_BRAC_CLOSE
%token ROUND_BRAC_OPEN
%token ROUND_BRAC_CLOSE
%token CURLY_BRAC_OPEN
%token CURLY_BRAC_CLOSE
%token DOT
%token ARROW
%token INCREMENT_OP
%token DECREMENT_OP
%token MULTIPLY_OP
%token ADD_OP
%token SUBTRACT_OP
%token BITWISE_AND_OP
%token BITWISE_NOT_OP
%token LOGICAL_NOT_OP
%token DIVIDE_OP
%token MODULO_OP
%token LEFT_SHIFT_OP
%token RIGHT_SHIFT_OP
%token LESS_THAN_OP
%token GREATER_THAN_OP
%token LESS_THAN_OR_EQUAL_OP
%token GREATER_THAN_OR_EQUAL_OP
%token EQUAL_TO_OP
%token NOT_EQUAL_TO_OP
%token BITWISE_XOR_OP
%token BITWISE_OR_OP
%token LOGICAL_AND_OP
%token LOGICAL_OR_OP
%token QUESTION_MARK
%token COLON
%token SEMICOLON
%token ELLIPSIS
%token ASSIGN_OP
%token MULTIPLY_ASSIGN_OP
%token DIVIDE_ASSIGN_OP
%token MODULO_ASSIGN_OP
%token ADD_ASSIGN_OP
%token SUBTRACT_ASSIGN_OP
%token LEFT_SHIFT_ASSIGN_OP
%token RIGHT_SHIFT_ASSIGN_OP
%token BITWISE_AND_ASSIGN_OP
%token BITWISE_XOR_ASSIGN_OP
%token BITWISE_OR_ASSIGN_OP
%token COMMA
%token HASH
%token AUTO
%token BREAK
%token CASE
%token CHAR
%token CONST
%token CONTINUE
%token DEFAULT
%token DO
%token DOUBLE
%token ELSE
%token ENUM
%token EXTERN
%token FLOAT
%token FOR
%token GOTO
%token IF
%token INLINE
%token INT
%token LONG
%token REGISTER
%token RESTRICT
%token RETURN
%token SHORT
%token SIGNED
%token SIZEOF
%token STATIC
%token STRUCT
%token SWITCH
%token TYPEDEF
%token UNION
%token UNSIGNED
%token VOID
%token VOLATILE
%token WHILE
%token BOOL
%token COMPLEX
%token IMAGINARY

%token <symboll> IDENTIFIER
%token <intval> INTEGER_CONSTANT
%token <strval> FLOAT_CONSTANT
%token <strval> CHAR_CONSTANT
%token <strval> STRING

%start translation_unit

%right THEN ELSE // for handling the dangling else problem

%type <unaryOpr> unary_operator
%type <paramCnt> argument_expression_list argument_expression_list_opt
%type <expr> expression primary_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression AND_expression exclusive_OR_expression inclusive_OR_expression logical_AND_expression logical_OR_expression conditional_expression assignment_expression expression_statement 
%type <stmt> statement labeled_statement compound_statement selection_statement iteration_statement jump_statement loop_statement block_item block_item_list block_item_list_opt
%type <symbollType> pointer
%type <symboll> constant initializer direct_declarator init_declarator declarator
%type <arr> postfix_expression unary_expression cast_expression
%type <ins_num> M 
%type <stmt> N

%%

primary_expression : IDENTIFIER {
                        $$ = new ExprClass();
                        $$->address = $1;
                        $$->exprType = "NONBOOL";
                    }
                    | constant {
                        $$ = new ExprClass();
                        $$->address = $1;
                    }
                    | STRING {
                        $$ = new ExprClass();
                        $$->address = symbolTable::generateTmp(new symbolType("ptr"), $1);
                        $$->address->type->arrType = new symbolType("char");
                    }
                    | ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE {
                        $$ = $2;
                    }
                    ;

constant :  INTEGER_CONSTANT {
                $$ = symbolTable::generateTmp(new symbolType("int"), convertIntTostr($1)); 
                printout("=", $$->name, $1);
            }
            | FLOAT_CONSTANT {
                $$ = symbolTable::generateTmp(new symbolType("float"), string($1));
                printout("=", $$->name, string($1));
            }
            | CHAR_CONSTANT {
                $$ = symbolTable::generateTmp(new symbolType("char"), string($1));
                printout("=", $$->name, string($1));
            }
            ;

postfix_expression : primary_expression {
                        $$ = new ArrClass();
                        $$->loc = $1->address;
                        $$->type = $1->address->type;
                        $$->address = $$->loc;
                    }
                    | postfix_expression SQUARE_BRAC_OPEN expression SQUARE_BRAC_CLOSE {
                        $$ = new ArrClass();
                        $$->type = $1->type->arrType;
                        $$->loc = $1->loc;
                        $$->address = symbolTable::generateTmp(new symbolType("int"));
                        $$->arrType = "arr";

                        if ($1->arrType == "arr")
                        {
                            symbol *temp = symbolTable::generateTmp(new symbolType("int"));
                            int sz = sizeOfType($$->type);
                            printout("*", temp->name, $3->address->name, convertIntTostr(sz));
                            printout("+", $$->address->name, $1->address->name, temp->name);
                        }
                        else
                        {
                            int sz = sizeOfType($$->type);
                            printout("*", $$->address->name, $3->address->name, convertIntTostr(sz));
                        }
                    }
                    | postfix_expression ROUND_BRAC_OPEN argument_expression_list_opt ROUND_BRAC_CLOSE  {
                        $$ = new ArrClass();
                        $$->loc = symbolTable::generateTmp($1->type);
                        printout("call", $$->loc->name, $1->loc->name, convertIntTostr($3));
                    }
                    | postfix_expression DOT IDENTIFIER {}
                    | postfix_expression ARROW IDENTIFIER {}
                    | postfix_expression INCREMENT_OP {
                        $$ = new ArrClass();
                        $$->loc = symbolTable::generateTmp($1->loc->type);
                        printout("=", $$->loc->name, $1->loc->name);
                        printout("+", $1->loc->name, $1->loc->name, "1");
                    }
                    | postfix_expression DECREMENT_OP {
                        $$ = new ArrClass();
                        $$->loc = symbolTable::generateTmp($1->loc->type);
                        printout("=", $$->loc->name, $1->loc->name);
                        printout("-", $1->loc->name, $1->loc->name, "1");
                    }
                    | ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE CURLY_BRAC_OPEN initializer_list CURLY_BRAC_CLOSE {}
                    | ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE CURLY_BRAC_OPEN initializer_list COMMA CURLY_BRAC_CLOSE {}
                    ;

argument_expression_list_opt : argument_expression_list {
                                $$ = $1;
                            }
                            | %empty {
                                $$ = 0;
                            }
                            ;

argument_expression_list : assignment_expression {
                            $$ = 1;
                            printout("param", $1->address->name);
                        }
                        | argument_expression_list COMMA assignment_expression {
                            $$ = $1 + 1;
                            printout("param", $3->address->name);
                        }
                        ;

unary_expression : postfix_expression {
                    $$ = $1;
                }
                | INCREMENT_OP unary_expression {
                    printout("+", $2->loc->name, $2->loc->name, "1");
                    $$ = $2;
                }
                | DECREMENT_OP unary_expression {
                    printout("-", $2->loc->name, $2->loc->name, "1");
                    $$ = $2;
                }
                | unary_operator cast_expression {
                    $$ = new ArrClass();
                    if($1 == '&')
                    {
                        $$->loc = symbolTable::generateTmp(new symbolType("ptr"));
                        $$->loc->type->arrType = $2->loc->type;
                        printout("= &", $$->loc->name, $2->loc->name);
                    }
                    else if($1 == '*')
                    {
                        $$->arrType = "ptr";
                        $$->address = symbolTable::generateTmp($2->loc->type->arrType);
                        $$->loc = $2->loc;
                        printout("= *", $$->address->name, $2->loc->name);
                    }
                    else if($1 == '+')
                    {
                        $$ = $2;
                    }
                    else if($1 == '-')
                    {
                        $$->loc = symbolTable::generateTmp(new symbolType($2->loc->type->base));
                        printout("= -", $$->loc->name, $2->loc->name);
                    }
                    else if($1 == '~')
                    {
                        $$->loc = symbolTable::generateTmp(new symbolType($2->loc->type->base));
                        printout("= ~", $$->loc->name, $2->loc->name);
                    }
                    else if($1 == '!')
                    {
                        $$->loc = symbolTable::generateTmp(new symbolType($2->loc->type->base));
                        printout("= !", $$->loc->name, $2->loc->name);
                    }
                }
                | SIZEOF unary_expression {}
                | SIZEOF ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE {}
                ;

unary_operator : BITWISE_AND_OP     { $$ = '&'; }
                | MULTIPLY_OP       { $$ = '*'; }
                | ADD_OP            { $$ = '+'; }
                | SUBTRACT_OP       { $$ = '-'; }
                | BITWISE_NOT_OP    { $$ = '~'; }
                | LOGICAL_NOT_OP    { $$ = '!'; }
                ;

cast_expression : unary_expression {
                    $$ = $1;
                }
                | ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE cast_expression {
                    $$ = new ArrClass();
                    $$->loc = convertType($4->loc, data_type);
                }
                ;

multiplicative_expression : cast_expression {
                            $$ = new ExprClass();
                            if($1->arrType == "arr")
                            {
                                $$->address = symbolTable::generateTmp($1->address->type);
                                printout("=[]", $$->address->name, $1->loc->name, $1->address->name);
                            }
                            else if($1->arrType == "ptr")
                            {
                                $$->address = $1->address;
                            }
                            else
                            {
                                $$->address = $1->loc;
                            }
                        }
                        | multiplicative_expression MULTIPLY_OP cast_expression {
                            if(typecheck($1->address, $3->loc))
                            {
                                $$ = new ExprClass();
                                $$->address = symbolTable::generateTmp(new symbolType($1->address->type->base));
                                printout("*", $$->address->name, $1->address->name, $3->loc->name);
                            }
                            else
                            {
                                yyerror("Type Mismatch");
                            }
                        }
                        | multiplicative_expression DIVIDE_OP cast_expression {
                            if(typecheck($1->address, $3->loc))
                            {
                                $$ = new ExprClass();
                                $$->address = symbolTable::generateTmp(new symbolType($1->address->type->base));
                                printout("/", $$->address->name, $1->address->name, $3->loc->name);
                            }
                            else
                            {
                                yyerror("Type Mismatch");
                            }
                        }
                        | multiplicative_expression MODULO_OP cast_expression {
                            if(typecheck($1->address, $3->loc))
                            {
                                $$ = new ExprClass();
                                $$->address = symbolTable::generateTmp(new symbolType($1->address->type->base));
                                printout("%", $$->address->name, $1->address->name, $3->loc->name);
                            }
                            else
                            {
                                yyerror("Type Mismatch");
                            }
                        }
                        ;

additive_expression : multiplicative_expression {
                        $$ = $1;
                    }
                    | additive_expression ADD_OP multiplicative_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            $$ = new ExprClass();
                            $$->address = symbolTable::generateTmp(new symbolType($1->address->type->base));
                            printout("+", $$->address->name, $1->address->name, $3->address->name);
                        }
                        else
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    | additive_expression SUBTRACT_OP multiplicative_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            $$ = new ExprClass();
                            $$->address = symbolTable::generateTmp(new symbolType($1->address->type->base));
                            printout("-", $$->address->name, $1->address->name, $3->address->name);
                        }
                        else
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    ;

shift_expression : additive_expression {
                    $$ = $1;
                }
                | shift_expression LEFT_SHIFT_OP additive_expression {
                    if($3->address->type->base == "int")
                    {
                        $$ = new ExprClass();
                        $$->address = symbolTable::generateTmp(new symbolType("int"));
                        printout("<<", $$->address->name, $1->address->name, $3->address->name);
                    }
                    else
                    {
                        yyerror("Type Mismatch");
                    }
                }
                | shift_expression RIGHT_SHIFT_OP additive_expression {
                    if($3->address->type->base == "int")
                    {
                        $$ = new ExprClass();
                        $$->address = symbolTable::generateTmp(new symbolType("int"));
                        printout(">>", $$->address->name, $1->address->name, $3->address->name);
                    }
                    else
                    {
                        yyerror("Type Mismatch");
                    }
                }
                ;

relational_expression : shift_expression {
                        $$ = $1;
                    }
                    | relational_expression LESS_THAN_OP shift_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            $$->trueList = makelist(nxtInstr());
                            $$->falseList = makelist(nxtInstr() + 1);
                            printout("<", "", $1->address->name, $3->address->name);
                            printout("goto", "");
                        }
                        else 
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    | relational_expression GREATER_THAN_OP shift_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            $$->trueList = makelist(nxtInstr());
                            $$->falseList = makelist(nxtInstr() + 1);
                            printout(">", "", $1->address->name, $3->address->name);
                            printout("goto", "");
                        }
                        else 
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    | relational_expression LESS_THAN_OR_EQUAL_OP shift_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            $$->trueList = makelist(nxtInstr());
                            $$->falseList = makelist(nxtInstr() + 1);
                            printout("<=", "", $1->address->name, $3->address->name);
                            printout("goto", "");
                        }
                        else 
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    | relational_expression GREATER_THAN_OR_EQUAL_OP shift_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            $$->trueList = makelist(nxtInstr());
                            $$->falseList = makelist(nxtInstr() + 1);
                            printout(">=", "", $1->address->name, $3->address->name);
                            printout("goto", "");
                        }
                        else 
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    ;

equality_expression : relational_expression {
                        $$ = $1;
                    }
                    | equality_expression EQUAL_TO_OP relational_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            convertBoolToInt($1);
                            convertBoolToInt($3);
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            $$->trueList = makelist(nxtInstr());
                            $$->falseList = makelist(nxtInstr() + 1);
                            printout("==", "", $1->address->name, $3->address->name);
                            printout("goto", "");
                        }
                        else
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    | equality_expression NOT_EQUAL_TO_OP relational_expression {
                        if(typecheck($1->address, $3->address))
                        {
                            convertBoolToInt($1);
                            convertBoolToInt($3);
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            $$->trueList = makelist(nxtInstr());
                            $$->falseList = makelist(nxtInstr() + 1);
                            printout("!=", "", $1->address->name, $3->address->name);
                            printout("goto", "");
                        }
                        else
                        {
                            yyerror("Type Mismatch");
                        }
                    }
                    ;

AND_expression : equality_expression {
                    $$ = $1;
                }
                | AND_expression BITWISE_AND_OP equality_expression {
                    if(typecheck($1->address, $3->address))
                    {
                        convertBoolToInt($1);
                        convertBoolToInt($3);
                        $$ = new ExprClass();
                        $$->exprType = "NONBOOL";
                        $$->address = symbolTable::generateTmp(new symbolType("int"));
                        printout("&", $$->address->name, $1->address->name, $3->address->name);
                    }
                    else
                    {
                        yyerror("Type Mismatch");
                    }
                }
                ;

exclusive_OR_expression : AND_expression {
                            $$ = $1;
                        }
                        | exclusive_OR_expression BITWISE_XOR_OP AND_expression {
                            if(typecheck($1->address, $3->address))
                            {
                                convertBoolToInt($1);
                                convertBoolToInt($3);
                                $$ = new ExprClass();
                                $$->exprType = "NONBOOL";
                                $$->address = symbolTable::generateTmp(new symbolType("int"));
                                printout("^", $$->address->name, $1->address->name, $3->address->name);
                            }
                            else
                            {
                                yyerror("Type Mismatch");
                            }
                        }
                        ;

inclusive_OR_expression : exclusive_OR_expression {
                            $$ = $1;
                        }
                        | inclusive_OR_expression BITWISE_OR_OP exclusive_OR_expression {
                            if(typecheck($1->address, $3->address))
                            {
                                convertBoolToInt($1);
                                convertBoolToInt($3);
                                $$ = new ExprClass();
                                $$->exprType = "NONBOOL";
                                $$->address = symbolTable::generateTmp(new symbolType("int"));
                                printout("|", $$->address->name, $1->address->name, $3->address->name);
                            }
                            else
                            {
                                yyerror("Type Mismatch");
                            }
                        }
                        ;

logical_AND_expression : inclusive_OR_expression {
                            $$ = $1;
                        }
                        | logical_AND_expression LOGICAL_AND_OP M inclusive_OR_expression {
                            convertIntToBool($1);
                            convertIntToBool($4);
                            $$ = new ExprClass();
                            $$->exprType = "BOOL";
                            backpatch($1->trueList, $3);
                            $$->trueList = $4->trueList;
                            $$->falseList = merge($1->falseList, $4->falseList);
                        }
                        ;

logical_OR_expression : logical_AND_expression {
                        $$ = $1;
                    }
                    | logical_OR_expression LOGICAL_OR_OP M logical_AND_expression {
                        convertIntToBool($1);
                        convertIntToBool($4);
                        $$ = new ExprClass();
                        $$->exprType = "BOOL";
                        backpatch($1->falseList, $3);
                        $$->falseList = $4->falseList;
                        $$->trueList = merge($1->trueList, $4->trueList);
                    }
                    ;

conditional_expression : logical_OR_expression {
                            $$ = $1;
                        }
                        | logical_OR_expression N QUESTION_MARK M expression N COLON M conditional_expression {
                            $$->address = symbolTable::generateTmp($5->address->type);
                            $$->address->update($5->address->type);
                            printout("=", $$->address->name, $9->address->name);
                            list <int> templist1 = makelist(nxtInstr());
                            printout("goto", "");
                            backpatch($6->nextList, nxtInstr());
                            printout("=", $$->address->name, $5->address->name);
                            list <int> templist2 = makelist(nxtInstr());
                            templist1 = merge(templist1, templist2);
                            printout("goto", "");
                            backpatch($2->nextList, nxtInstr());
                            convertIntToBool($1);
                            backpatch($1->trueList, $4);
                            backpatch($1->falseList, $8);
                            backpatch(templist1, nxtInstr());
                        }
                        ;

M : %empty {
        $$ = nxtInstr();
    }

N : %empty {
        $$ = new StmtClass();
        $$->nextList = makelist(nxtInstr());
        printout("goto", "");
    }

assignment_expression : conditional_expression {
                        $$ = $1;
                    }
                    | unary_expression assignment_operator assignment_expression {
                        if($1->arrType == "arr")
                        {
                            $3->address = convertType($3->address, $1->type->base);
                            printout("[]=", $1->loc->name, $1->address->name, $3->address->name);
                        }
                        else if($1->arrType == "ptr")
                        {
                            printout("*=", $1->loc->name, $3->address->name);
                        }
                        else
                        {
                            $3->address = convertType($3->address, $1->loc->type->base);
                            printout("=", $1->loc->name, $3->address->name);
                        }
                        $$ = $3;
                    }
                    ;

assignment_operator : ASSIGN_OP {}
                    | MULTIPLY_ASSIGN_OP {}
                    | DIVIDE_ASSIGN_OP {}
                    | MODULO_ASSIGN_OP {}
                    | ADD_ASSIGN_OP {}
                    | SUBTRACT_ASSIGN_OP {}
                    | LEFT_SHIFT_ASSIGN_OP {}
                    | RIGHT_SHIFT_ASSIGN_OP {}
                    | BITWISE_AND_ASSIGN_OP {}
                    | BITWISE_XOR_ASSIGN_OP {}
                    | BITWISE_OR_ASSIGN_OP {}
                    ;

expression : assignment_expression {
                $$ = $1;
            }
            | expression COMMA assignment_expression {}
            ;

constant_expression :  conditional_expression {}
                    ;

declaration : declaration_specifiers init_declarator_list SEMICOLON {}
            | declaration_specifiers SEMICOLON {}
            ;

declaration_specifiers  : storage_class_specifier declaration_specifiers {}
                        | storage_class_specifier {}
                        | type_specifier declaration_specifiers {}
                        | type_specifier {}
                        | type_qualifier declaration_specifiers {}
                        | type_qualifier {}
                        | function_specifier declaration_specifiers {}
                        | function_specifier {}
                        ;

init_declarator_list: init_declarator_list COMMA init_declarator {}
                    | init_declarator {}
                    ;

init_declarator : declarator {
                    $$ = $1;
                }
                | declarator ASSIGN_OP initializer {
                    if($3->initialVal != "")
                    {
                        $1->initialVal = $3->initialVal;
                    }
                    printout("=", $1->name, $3->name);
                }
                ;

storage_class_specifier : EXTERN {}
                        | STATIC {}
                        | AUTO {}
                        | REGISTER {}
                        ;

type_specifier  : VOID { data_type = "void"; }
                | CHAR { data_type = "char"; }
                | SHORT {}
                | INT { data_type = "int"; }
                | LONG {}
                | FLOAT { data_type = "float"; }
                | DOUBLE {}
                | SIGNED {}
                | UNSIGNED {}
                | BOOL {}
                | COMPLEX {}
                | IMAGINARY {}
                | enum_specifier {}
                ;

specifier_qualifier_list    : type_specifier specifier_qualifier_list_opt {}
                            | type_qualifier specifier_qualifier_list_opt {}
                            ;

specifier_qualifier_list_opt    : specifier_qualifier_list {}
                                | %empty {}
                                ;

enum_specifier  : ENUM identifier_opt CURLY_BRAC_OPEN enumerator_list CURLY_BRAC_CLOSE {}
                | ENUM identifier_opt CURLY_BRAC_OPEN enumerator_list COMMA CURLY_BRAC_CLOSE {}
                | ENUM IDENTIFIER {}
                ;

identifier_opt  : IDENTIFIER {}
                | %empty {}
                ;

enumerator_list : enumerator {}
                | enumerator_list COMMA enumerator {}
                ;
enumerator  : IDENTIFIER {}
            | IDENTIFIER ASSIGN_OP constant_expression {}
            ;

type_qualifier  : CONST {}
                | RESTRICT {}
                | VOLATILE {}
                ;

function_specifier  : INLINE {}
                    ;

declarator  : pointer direct_declarator {
                symbolType* t = $1;
                while(t->arrType != NULL) t = t->arrType;
                t->arrType = $2->type;
                $$ = $2->update($1);
            }
            | direct_declarator {}
            ;

direct_declarator : IDENTIFIER {
                    $$ = $1->update(new symbolType(data_type));
                    currentSymbol = $1;
                }
                | ROUND_BRAC_OPEN declarator ROUND_BRAC_CLOSE {
                    $$ = $2;
                }
                | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list assignment_expression SQUARE_BRAC_CLOSE {}
                | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list SQUARE_BRAC_CLOSE {}
                | direct_declarator SQUARE_BRAC_OPEN assignment_expression SQUARE_BRAC_CLOSE {
                    symbolType* t = $1->type;
                    symbolType* prev = NULL;
                    while(t->base == "arr")
                    {
                        prev = t;
                        t = t->arrType;
                    }
                    if(prev == NULL)
                    {
                        int temp = atoi($3->address->initialVal.c_str());
                        symbolType* tp = new symbolType("arr", $1->type, temp);
                        $$ = $1->update(tp);
                    }
                    else
                    {
                        int temp = atoi($3->address->initialVal.c_str());
                        prev->arrType = new symbolType("arr", t, temp);
                        $$ = $1->update($1->type);
                    }
                }
                | direct_declarator SQUARE_BRAC_OPEN SQUARE_BRAC_CLOSE {
                    symbolType* t = $1->type;
                    symbolType* prev = NULL;
                    while(t->base == "arr")
                    {
                        prev = t;
                        t = t->arrType;
                    }
                    if(prev == NULL)
                    {
                        symbolType* tp = new symbolType("arr", $1->type, 0);
                        $$ = $1->update(tp);
                    }
                    else
                    {
                        prev->arrType = new symbolType("arr", t, 0);
                        $$ = $1->update($1->type);
                    }
                }
                | direct_declarator SQUARE_BRAC_OPEN STATIC type_qualifier_list assignment_expression SQUARE_BRAC_CLOSE {}
                | direct_declarator SQUARE_BRAC_OPEN STATIC assignment_expression SQUARE_BRAC_CLOSE {}
                | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list STATIC assignment_expression SQUARE_BRAC_CLOSE {}
                | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list MULTIPLY_OP SQUARE_BRAC_CLOSE {}
                | direct_declarator SQUARE_BRAC_OPEN MULTIPLY_OP SQUARE_BRAC_CLOSE {}
                | direct_declarator ROUND_BRAC_OPEN change_table parameter_type_list ROUND_BRAC_CLOSE {
                    currentSymbolTable->name = $1->name;
                    if($1->type->base != "void")
                    {
                        symbol* StmtClass = currentSymbolTable->lookup("return");
                        StmtClass->update($1->type);
                    }
                    $1->nestedTable = currentSymbolTable;
                    currentSymbolTable->parent = globalSymbolTable;
                    switchTable(globalSymbolTable);
                    currentSymbol = $$;
                }
                | direct_declarator ROUND_BRAC_OPEN identifier_list ROUND_BRAC_CLOSE {}
                | direct_declarator ROUND_BRAC_OPEN change_table ROUND_BRAC_CLOSE {
                    currentSymbolTable->name = $1->name;
                    if($1->type->base != "void")
                    {
                        symbol* StmtClass = currentSymbolTable->lookup("return");
                        StmtClass->update($1->type);
                    }
                    $1->nestedTable = currentSymbolTable;
                    currentSymbolTable->parent = globalSymbolTable;
                    switchTable(globalSymbolTable);
                    currentSymbol= $$;
                }
                ;

type_qualifier_list_opt : type_qualifier_list {}
                        | %empty {}
                        ;

pointer : MULTIPLY_OP type_qualifier_list_opt {
            $$ = new symbolType("ptr");
        }
        | MULTIPLY_OP type_qualifier_list_opt pointer {
            $$ = new symbolType("ptr", $3);
        }

type_qualifier_list : type_qualifier {}
                    | type_qualifier_list type_qualifier {}
                    ;

parameter_type_list : parameter_list {}
                    | parameter_list COMMA ELLIPSIS {}
                    ;

parameter_list  : parameter_declaration {}
                | parameter_list COMMA parameter_declaration {}
                ;

parameter_declaration   : declaration_specifiers declarator {}
                        | declaration_specifiers {}
                        ;

identifier_list : IDENTIFIER {}
                | identifier_list COMMA IDENTIFIER {}
                ;

type_name   : specifier_qualifier_list {}
            ;

initializer : assignment_expression { $$ = $1->address; }
            | CURLY_BRAC_OPEN initializer_list CURLY_BRAC_CLOSE {}
            | CURLY_BRAC_OPEN initializer_list COMMA CURLY_BRAC_CLOSE {}
            ;

initializer_list    : designation_opt initializer {}
                    | initializer_list COMMA designation_opt initializer {}
                    ;

designation_opt : designation {}
                | %empty {}
                ;

designation : designator_list ASSIGN_OP {}
            ;

designator_list : designator {}
                | designator_list designator {}
                ;

designator  : SQUARE_BRAC_OPEN constant_expression SQUARE_BRAC_CLOSE {}
            | DOT IDENTIFIER {}
            ;


statement   : labeled_statement {}
            | compound_statement { $$ = $1; }
            | expression_statement {
                $$ = new StmtClass();
                $$->nextList = $1->nextList;
            }
            | selection_statement { $$ = $1; }
            | iteration_statement { $$ = $1; }
            | jump_statement { $$ = $1; }
            ;

loop_statement : labeled_statement {}
            | expression_statement {
                $$ = new StmtClass();
                $$->nextList = $1->nextList;
            }
            | selection_statement { $$ = $1; }
            | iteration_statement { $$ = $1; }
            | jump_statement { $$ = $1; }
            ;

labeled_statement : IDENTIFIER COLON statement {}
                | CASE constant_expression COLON statement {}
                | DEFAULT COLON statement {}
                ;

compound_statement : CURLY_BRAC_OPEN X change_table block_item_list_opt CURLY_BRAC_CLOSE {
                        $$ = $4;
                        switchTable(currentSymbolTable->parent);
                    }
                    ;

block_item_list_opt : block_item_list { $$ = $1; }
                    | %empty { $$ = new StmtClass(); }
                    ;

block_item_list : block_item { $$ = $1; }
                | block_item_list M block_item {
                    $$ = $3;
                    backpatch($1->nextList, $2);
                }
                ;

block_item : declaration { $$ = new StmtClass(); }
            | statement { $$ = $1; }
            ;

expression_statement : expression SEMICOLON { $$ = $1; }
                    | SEMICOLON { $$ = new ExprClass(); }
                    ;

selection_statement : IF ROUND_BRAC_OPEN expression N ROUND_BRAC_CLOSE M statement N %prec THEN {
                        backpatch($4->nextList, nxtInstr());
                        convertIntToBool($3);
                        $$ = new StmtClass();
                        backpatch($3->trueList, $6);
                        list<int> temp = merge($3->falseList, $7->nextList);
                        $$->nextList = merge($8->nextList, temp);
                    }
                    | IF ROUND_BRAC_OPEN expression N ROUND_BRAC_CLOSE M statement N ELSE M statement {
                        backpatch($4->nextList, nxtInstr());
                        convertIntToBool($3);
                        $$ = new StmtClass();
                        backpatch($3->trueList, $6);
                        backpatch($3->falseList, $10);
                        list<int> temp = merge($7->nextList, $8->nextList);
                        $$->nextList = merge($11->nextList, temp);
                    }
                    | SWITCH ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE statement {}
                    ;

iteration_statement : WHILE W ROUND_BRAC_OPEN X change_table M expression ROUND_BRAC_CLOSE M loop_statement {
                        $$ = new StmtClass();
                        convertIntToBool($7);
                        backpatch($10->nextList, $6);
                        backpatch($7->trueList, $9);
                        $$->nextList = $7->falseList;
                        printout("goto", convertIntTostr($6));
                        currBlock = "";
                        switchTable(currentSymbolTable->parent);
                    }
                    | WHILE W ROUND_BRAC_OPEN X change_table M expression ROUND_BRAC_CLOSE CURLY_BRAC_OPEN M block_item_list_opt CURLY_BRAC_CLOSE {
                        $$ = new StmtClass();
                        convertIntToBool($7);
                        backpatch($11->nextList, $6);
                        backpatch($7->trueList, $10);
                        $$->nextList = $7->falseList;
                        printout("goto", convertIntTostr($6));
                        currBlock = "";
                        switchTable(currentSymbolTable->parent);
                    }
                    | DO D M loop_statement M WHILE ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE SEMICOLON {
                        $$ = new StmtClass();
                        convertIntToBool($8);
                        backpatch($8->trueList, $3);    
                        backpatch($4->nextList, $5);  
                        $$->nextList = $8->falseList;  
                        currBlock = "";
                    }
                    | DO D CURLY_BRAC_OPEN M block_item_list_opt CURLY_BRAC_CLOSE M WHILE ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE SEMICOLON {
                        $$ = new StmtClass();
                        convertIntToBool($10);
                        backpatch($10->trueList, $4);
                        backpatch($5->nextList, $7);
                        $$->nextList = $10->falseList;
                        currBlock = "";
                    }
                    | FOR F ROUND_BRAC_OPEN X change_table declaration M expression_statement M expression N ROUND_BRAC_CLOSE M loop_statement {
                        $$ = new StmtClass();
                        convertIntToBool($8);
                        backpatch($8->trueList, $13);
                        backpatch($11->nextList, $7);
                        backpatch($14->nextList, $9); 
                        printout("goto", convertIntTostr($9)); 
                        $$->nextList = $8->falseList;
                        currBlock = "";
                        switchTable(currentSymbolTable->parent);
                    }
                    | FOR F ROUND_BRAC_OPEN X change_table expression_statement M expression_statement M expression N ROUND_BRAC_CLOSE M loop_statement {
                        $$ = new StmtClass();
                        convertIntToBool($8);
                        backpatch($8->trueList, $13); 
                        backpatch($11->nextList, $7);
                        backpatch($14->nextList, $9); 
                        printout("goto", convertIntTostr($9)); 
                        $$->nextList = $8->falseList;  
                        currBlock = "";
                        switchTable(currentSymbolTable->parent);
                    }
                    | FOR F ROUND_BRAC_OPEN X change_table declaration M expression_statement M expression N ROUND_BRAC_CLOSE M CURLY_BRAC_OPEN block_item_list_opt CURLY_BRAC_CLOSE {
                        $$ = new StmtClass();
                        convertIntToBool($8);
                        backpatch($8->trueList, $13);
                        backpatch($11->nextList, $7);
                        backpatch($15->nextList, $9);
                        printout("goto", convertIntTostr($9));
                        $$->nextList = $8->falseList;
                        currBlock = "";
                        switchTable(currentSymbolTable->parent);
                    }
                    | FOR F ROUND_BRAC_OPEN X change_table expression_statement M expression_statement M expression N ROUND_BRAC_CLOSE M CURLY_BRAC_OPEN block_item_list_opt CURLY_BRAC_CLOSE {
                         $$ = new StmtClass();
                        convertIntToBool($8);
                        backpatch($8->trueList, $13);
                        backpatch($11->nextList, $7);
                        backpatch($15->nextList, $9);
                        printout("goto", convertIntTostr($9));
                        $$->nextList = $8->falseList;
                        currBlock = "";
                        switchTable(currentSymbolTable->parent);
                    }
                    ;

F : %empty { currBlock = "FOR"; }
  ;

W : %empty { currBlock = "WHILE"; }
  ;

D : %empty { currBlock = "DO"; }
  ;

X : %empty {
    string newSymbolTableName = currentSymbolTable->name + "." + currBlock + "$" + to_string(SymbolTableCount++);
    symbol* symbolFound = currentSymbolTable->lookup(newSymbolTableName);
    symbolFound->nestedTable = new symbolTable(newSymbolTableName);
    symbolFound->name = newSymbolTableName;
    symbolFound->nestedTable->parent = currentSymbolTable;
    symbolFound->type = new symbolType("block");
    currentSymbol = symbolFound;
  }

change_table : %empty {
                if(currentSymbol->nestedTable != NULL)
                {
                    switchTable(currentSymbol->nestedTable);
                    printout("label", currentSymbolTable->name);
                }
                else
                {
                    switchTable(new symbolTable(""));
                }
            }

jump_statement : GOTO IDENTIFIER SEMICOLON {}
            | CONTINUE SEMICOLON { $$ = new StmtClass(); }
            | BREAK SEMICOLON { $$ = new StmtClass(); }
            | RETURN expression SEMICOLON {
                $$ = new StmtClass();
                printout("return", $2->address->name);
            }
            | RETURN SEMICOLON {
                $$ = new StmtClass();
                printout("return", "");
            }
            ;

translation_unit : external_declaration {}
                | translation_unit external_declaration {}
                ;

external_declaration : function_definition {}
                    | declaration {}
                    ;

function_definition : declaration_specifiers declarator declaration_list_opt change_table CURLY_BRAC_OPEN block_item_list_opt CURLY_BRAC_CLOSE {
                        currentSymbolTable->parent = globalSymbolTable;
                        SymbolTableCount = 0;
                        switchTable(globalSymbolTable);
                    }
                    ;

declaration_list_opt : declaration_list {}
                    | %empty {}
                    ;

declaration_list : declaration {}
                | declaration_list declaration {}
                ;

%%

void yyerror(string StmtClass)
{
    cout << "ERROR : " << StmtClass << endl;
    cout << "AT LINE : " << yylineno << endl;
    cout << "NEAR : " << yytext << endl;
}