%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern int yylineno;
void yyerror(char *err);
void print_statement(char *);
struct node
{
    char *message;
    struct node *children[5];
};
typedef struct node *Node;
Node add_node0(char *msg);
Node add_node1(char *msg, Node n1);
Node add_node2(char *msg, Node n1, Node n2);
Node add_node3(char *msg, Node n1, Node n2, Node n3);
Node add_node4(char *msg, Node n1, Node n2, Node n3, Node n4);
Node add_node5(char *msg, Node n1, Node n2, Node n3, Node n4, Node n5);
void print_tree(Node root);
Node root;
%}

%union {char *val; struct node* tree_node; }
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
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
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
%token <val> IDENTIFIER
%token <val> INTEGER_CONSTANT
%token <val> FLOAT_CONSTANT
%token <val> CHAR_CONSTANT
%token <val> STRING
%type <tree_node> primary_expression postfix_expression argument_expression_list_opt argument_expression_list 
%type <tree_node> unary_expression unary_operator cast_expression multiplicative_expression additive_expression shift_expression 
%type <tree_node> relational_expression equality_expression AND_expression exclusive_OR_expression inclusive_OR_expression
%type <tree_node> logical_AND_expression logical_OR_expression conditional_expression assignment_expression assignment_operator
%type <tree_node> expression constant_expression declaration init_declarator_list_opt declaration_specifiers declaration_specifiers_opt
%type <tree_node> init_declarator_list init_declarator storage_class_specifier type_specifier specifier_qualifier_list
%type <tree_node> specifier_qualifier_list_opt type_qualifier function_specifier declarator pointer_opt direct_declarator pointer
%type <tree_node> type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list_opt identifier_list
%type <tree_node> type_name initializer initializer_list designation_opt designation designator_list designator statement
%type <tree_node> labeled_statement compound_statement block_item_list_opt block_item_list block_item expression_statement expression_opt
%type <tree_node> selection_statement jump_statement type_qualifier_list_opt iteration_statement
%type <tree_node> declaration_list_opt function_definition external_declaration translation_unit assignment_expression_opt declaration_list
%start translation_unit

%%

primary_expression
    : IDENTIFIER
    { char *content = (char *)malloc(24 + strlen($1)); sprintf(content, "primary_expression -> %s\n", $1); $$ = add_node0(content); }
    | INTEGER_CONSTANT
    { char *content = (char *)malloc(24 + strlen($1)); sprintf(content, "primary_expression -> %s\n", $1); $$ = add_node0(content); }                              
    | FLOAT_CONSTANT
    { char *content = (char *)malloc(24 + strlen($1)); sprintf(content, "primary_expression -> %s\n", $1); $$ = add_node0(content); }
    | CHAR_CONSTANT
    { char *content = (char *)malloc(24 + strlen($1)); sprintf(content, "primary_expression -> %s\n", $1); $$ = add_node0(content); }
    | STRING
    { char *content = (char *)malloc(24 + strlen($1)); sprintf(content, "primary_expression -> %s\n", $1); $$ = add_node0(content); }
    | ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE
    { $$ = add_node1("primary_expression -> (expression)\n", $2); }
    ;

postfix_expression
    : primary_expression
    { $$ = add_node1("postfix_expression -> primary_expression\n", $1); }
    | postfix_expression SQUARE_BRAC_OPEN expression SQUARE_BRAC_CLOSE
    { $$ = add_node2("postfix_expression -> postfix_expression (expression)", $1, $3); }
    | postfix_expression ROUND_BRAC_OPEN argument_expression_list_opt ROUND_BRAC_CLOSE
    { $$ = add_node2("postfix_expression -> postfix_expression (argument_expression_list_opt)\n", $1, $3); }
    | postfix_expression DOT IDENTIFIER
    { $$ = add_node1("postfix_expression -> postfix_expression . identifier\n", $1); }                                                                     
    | postfix_expression ARROW IDENTIFIER
    { $$ = add_node1("postfix_expression -> postfix_expression -> identifier\n", $1); }                           
    | postfix_expression INCREMENT_OP
    { $$ = add_node1("postfix_expression -> postfix_expression ++\n", $1); }                                                                      
    | postfix_expression DECREMENT_OP    
    { $$ = add_node1("postfix_expression -> postfix_expression --\n", $1); }                                                                   
    | ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE CURLY_BRAC_OPEN initializer_list CURLY_BRAC_CLOSE
    { $$ = add_node2("postfix_expression -> (type_name) {initializer_list}\n", $2, $5); }       
    | ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE CURLY_BRAC_OPEN initializer_list COMMA CURLY_BRAC_CLOSE 
    { $$ = add_node2("postfix_expression -> (type_name) {initializer_list,}\n", $2, $5); }
    ;   

argument_expression_list_opt
    : argument_expression_list
    { $$ = add_node1("argument_expression_list_opt -> argument_expression_list\n", $1); }                                                                        
    |
    { $$ = add_node0("argument_expression_list_opt -> epsilon\n"); }                                                                                                       
    ;

argument_expression_list
    : assignment_expression
    { $$ = add_node1("argument_expression_list -> assignment_expression\n", $1); }                                                                                
    | argument_expression_list COMMA assignment_expression
    { $$ = add_node2("argument_expression_list -> argument_expression_list , assignment_expression\n", $1, $3); }                                                  
    ;

unary_expression
    : postfix_expression
    { $$ = add_node1("unary_expression -> postfix_expression\n", $1); }                                                                                   
    | INCREMENT_OP unary_expression
    { $$ = add_node1("unary_expression -> ++ unary_expression\n", $2); }                                                                        
    | DECREMENT_OP unary_expression
    { $$ = add_node1("unary_expression -> -- unary_expression\n", $2); }                                                                         
    | unary_operator cast_expression
    { $$ = add_node2("unary_expression -> unary_operator cast_expression\n", $1, $2); }                                                                        
    | SIZEOF unary_expression
    { $$ = add_node1("unary_expression -> sizeof unary_expression\n", $2); }                                                                              
    | SIZEOF ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE
    { $$ = add_node1("unary_expression -> sizeof (type_name)\n", $3); }                                                    
    ;

unary_operator
    : BITWISE_AND_OP
    { $$ = add_node0("unary_operator -> &\n"); }                                                                                        
    | MULTIPLY_OP
    { $$ = add_node0("unary_operator -> *\n"); }                                                                                          
    | ADD_OP
    { $$ = add_node0("unary_operator -> +\n"); }                                                                                                
    | SUBTRACT_OP
    { $$ = add_node0("unary_operator -> -\n"); }                                                                                           
    | BITWISE_NOT_OP
    { $$ = add_node0("unary_operator -> ~\n"); }                                                                                        
    | LOGICAL_NOT_OP
    { $$ = add_node0("unary_operator -> !\n"); }                                                                                        
    ;

cast_expression
    : unary_expression
    { $$ = add_node1("cast_expression -> unary_expression\n", $1); }                                                                                     
    | ROUND_BRAC_OPEN type_name ROUND_BRAC_CLOSE cast_expression
    { $$ = add_node2("cast_expression -> (type_name) cast_expression\n", $2, $4); }                                            
    ;

multiplicative_expression
    : cast_expression
    { $$ = add_node1("multiplicative_expression -> cast_expression\n", $1); }                                                                                       
    | multiplicative_expression MULTIPLY_OP cast_expression
    { $$ = add_node2("multiplicative_expression -> multiplicative_expression * cast_expression\n", $1, $3); }                                                 
    | multiplicative_expression DIVIDE_OP cast_expression
    { $$ = add_node2("multiplicative_expression -> multiplicative_expression / cast_expression\n", $1, $3); }                                                   
    | multiplicative_expression MODULO_OP cast_expression
    { $$ = add_node2("multiplicative_expression -> multiplicative_expression % cast_expression\n", $1, $3); }                                                   
    ;

additive_expression
    : multiplicative_expression
    { $$ = add_node1("additive_expression -> multiplicative_expression\n", $1); }                                                                            
    | additive_expression ADD_OP multiplicative_expression
    { $$ = add_node2("additive_expression -> additive_expression + multiplicative_expression\n", $1, $3); }                                                  
    | additive_expression SUBTRACT_OP multiplicative_expression
    { $$ = add_node2("additive_expression -> additive_expression - multiplicative_expression\n", $1, $3); }                                             
    ;

shift_expression
    : additive_expression
    { $$ = add_node1("shift_expression -> additive_expression\n", $1); }                                                                                   
    | shift_expression LEFT_SHIFT_OP additive_expression
    { $$ = add_node2("shift_expression -> shift_expression << additive_expression\n", $1, $3); }                                                    
    | shift_expression RIGHT_SHIFT_OP additive_expression
    { $$ = add_node2("shift_expression -> shift_expression >> additive_expression\n", $1, $3); }                                                   
    ;

relational_expression
    : shift_expression
    { $$ = add_node1("relational_expression -> shift_expression\n", $1); }                                                                                      
    | relational_expression LESS_THAN_OP shift_expression
    { $$ = add_node2("relational_expression -> relational_expression < shift_expression\n", $1, $3); }                                                   
    | relational_expression GREATER_THAN_OP shift_expression
    { $$ = add_node2("relational_expression -> relational_expression > shift_expression\n", $1, $3); }                                                
    | relational_expression LESS_THAN_OR_EQUAL_OP shift_expression
    { $$ = add_node2("relational_expression -> relational_expression <= shift_expression\n", $1, $3); }                                          
    | relational_expression GREATER_THAN_OR_EQUAL_OP shift_expression
    { $$ = add_node2("relational_expression -> relational_expression >= shift_expression\n", $1, $3); }                                       
    ;

equality_expression
    : relational_expression
    { $$ = add_node1("equality_expression -> relational_expression\n", $1); }                                                                                 
    | equality_expression EQUAL_TO_OP relational_expression
    { $$ = add_node2("equality_expression -> equality_expression == relational_expression\n", $1, $3); }                                                 
    | equality_expression NOT_EQUAL_TO_OP relational_expression
    { $$ = add_node2("equality_expression -> equality_expression != relational_expression\n", $1, $3); }                                             
    ;

AND_expression
    : equality_expression
    { $$ = add_node1("AND_expression -> equality_expression\n", $1); }                                                                                   
    | AND_expression BITWISE_AND_OP equality_expression
    { $$ = add_node2("AND_expression -> AND_expression & equality_expression\n", $1, $3); }
    ;                                                     

exclusive_OR_expression
    : AND_expression
    { $$ = add_node1("exclusive_OR_expression -> AND_expression\n", $1); }                                                                                        
    | exclusive_OR_expression BITWISE_XOR_OP AND_expression
    { $$ = add_node2("exclusive_OR_expression -> exclusive_OR_expression ^ AND_expression\n", $1, $3); }                                                 
    ;

inclusive_OR_expression
    : exclusive_OR_expression
    { $$ = add_node1("inclusive_OR_expression -> exclusive_OR_expression\n", $1); }                                                                               
    | inclusive_OR_expression BITWISE_OR_OP exclusive_OR_expression
    { $$ = add_node2("inclusive_OR_expression -> inclusive_OR_expression | exclusive_OR_expression\n", $1, $3); }                                         
    ;

logical_AND_expression
    : inclusive_OR_expression
    { $$ = add_node1("logical_AND_expression -> inclusive_OR_expression\n", $1); }                                                                               
    | logical_AND_expression LOGICAL_AND_OP inclusive_OR_expression
    { $$ = add_node2("logical_AND_expression -> logical_AND_expression && inclusive_OR_expression\n", $1, $3); }                                         
    ;

logical_OR_expression
    : logical_AND_expression
    { $$ = add_node1("logical_OR_expression -> logical_AND_expression\n", $1); }                                                                                
    | logical_OR_expression LOGICAL_OR_OP logical_AND_expression
    { $$ = add_node2("logical_OR_expression -> logical_OR_expression || logical_AND_expression\n", $1, $3); }                                            
    ;

conditional_expression
    : logical_OR_expression
    { $$ = add_node1("conditional_expression -> logical_OR_expression\n", $1); }                                                                                                           
    | logical_OR_expression QUESTION_MARK expression COLON conditional_expression
    { $$ = add_node3("conditional_expression -> logical_OR_expression ? expression : conditional_expression\n", $1, $3, $5); }                           
    ;

assignment_expression
    : conditional_expression
    { $$ = add_node1("assignment_expression -> conditional_expression\n", $1); }                                                                                
    | unary_expression assignment_operator assignment_expression
    { $$ = add_node3("assignment_expression -> unary_expression assignment_operator assignment_expression\n", $1, $2, $3); }                                            
    ;

assignment_operator
    : ASSIGN_OP
    { $$ = add_node0("assignment_operator -> =\n"); }                                                                                            
    | MULTIPLY_ASSIGN_OP
    { $$ = add_node0("assignment_operator -> *=\n"); }                                                                                   
    | DIVIDE_ASSIGN_OP                                
    { $$ = add_node0("assignment_operator -> /=\n"); }                                                      
    | MODULO_ASSIGN_OP                                
    { $$ = add_node0("assignment_operator -> %=\n"); }                                                      
    | ADD_ASSIGN_OP                                   
    { $$ = add_node0("assignment_operator -> +=\n"); }                                                      
    | SUBTRACT_ASSIGN_OP                              
    { $$ = add_node0("assignment_operator -> -=\n"); }                                                      
    | LEFT_SHIFT_ASSIGN_OP                              
    { $$ = add_node0("assignment_operator -> <<= \n"); }                                                      
    | RIGHT_SHIFT_ASSIGN_OP                             
    { $$ = add_node0("assignment_operator -> >>= \n"); }                                                    
    | BITWISE_AND_ASSIGN_OP                             
    { $$ = add_node0("assignment_operator -> &= \n"); }                                                    
    | BITWISE_XOR_ASSIGN_OP                            
    { $$ = add_node0("assignment_operator -> ^= \n"); }                                                     
    | BITWISE_OR_ASSIGN_OP                             
    { $$ = add_node0("assignment_operator -> |= \n"); }                                                     
    ;

expression
    : assignment_expression 
    { $$ = add_node1("expression -> assignment_expression\n", $1); }                                                                                
    | expression COMMA assignment_expression                        
    { $$ = add_node2("expression -> expression , assignment_expression\n", $1, $3); }                                        
    ;

constant_expression
    : conditional_expression 
    { $$ = add_node1("constant_expression -> conditional_expression\n", $1); }                                                                               
    ;

declaration
    : declaration_specifiers init_declarator_list_opt SEMICOLON 
    { $$ = add_node2("declaration -> declaration_specifiers init_declarator_list_opt ;\n", $1, $2); }                                            
    ;

init_declarator_list_opt
    : init_declarator_list 
    { $$ = add_node1("init_declarator_list_opt -> init_declarator_list\n", $1); }                                                                                 
    |                                                                            
    { $$ = add_node0("init_declarator_list_opt -> epsilon\n"); }                           
    ;

declaration_specifiers
    : storage_class_specifier declaration_specifiers_opt
    { $$ = add_node2("declaration_specifiers -> storage_class_specifier declaration_specifiers_opt\n", $1, $2); }                                                    
    | type_specifier declaration_specifiers_opt                                                             
    { $$ = add_node2("declaration_specifiers -> type_specifier declaration_specifiers_opt\n", $1, $2); }
    | type_qualifier declaration_specifiers_opt                                                         
    { $$ = add_node2("declaration_specifiers -> type_qualifier declaration_specifiers_opt\n", $1, $2); }    
    | function_specifier declaration_specifiers_opt                                                         
    { $$ = add_node2("declaration_specifiers -> function_specifier declaration_specifiers_opt\n", $1, $2); }    
    ;

declaration_specifiers_opt
    : declaration_specifiers
    { $$ = add_node1("declaration_specifiers_opt -> declaration_specifiers\n", $1); }                                                                                
    |
    { $$ = add_node0("declaration_specifiers_opt -> epsilon\n"); }                                                                                                       
    ;

init_declarator_list
    : init_declarator 
    { $$ = add_node1("init_declarator_list -> init_declarator\n", $1); }                                                                                      
    | init_declarator_list COMMA init_declarator 
    { $$ = add_node2("init_declarator_list -> init_declarator_list , init_declarator\n", $1, $3); }                                                           
    ;

init_declarator
    : declarator 
    { $$ = add_node1("init_declarator -> declarator\n", $1); }                                                                                           
    | declarator ASSIGN_OP initializer
    { $$ = add_node2("init_declarator -> declarator = initializer\n", $1, $3); }                                                                      
    ;

storage_class_specifier
    : EXTERN 
    { $$ = add_node0("storage_class_specifier -> extern\n"); }               
    | STATIC                
    { $$ = add_node0("storage_class_specifier -> static\n"); }
    | AUTO                  
    { $$ = add_node0("storage_class_specifier -> auto\n"); }
    | REGISTER              
    { $$ = add_node0("storage_class_specifier -> register\n"); }
    ;

type_specifier
    : VOID
    { $$ = add_node0("type_specifier -> void\n"); }              
    | CHAR                  
    { $$ = add_node0("type_specifier -> char\n"); }
    | SHORT              
    { $$ = add_node0("type_specifier -> short\n"); }
    | INT               
    { $$ = add_node0("type_specifier -> int\n"); }
    | LONG              
    { $$ = add_node0("type_specifier -> long\n"); }
    | FLOAT              
    { $$ = add_node0("type_specifier -> float\n"); }
    | DOUBLE            
    { $$ = add_node0("type_specifier -> double\n"); }
    | SIGNED            
    { $$ = add_node0("type_specifier -> signed\n"); }
    | UNSIGNED          
    { $$ = add_node0("type_specifier -> unsigned\n"); }
    | BOOL              
    { $$ = add_node0("type_specifier -> bool\n"); }
    | COMPLEX   
    { $$ = add_node0("type_specifier -> complex\n"); }        
    | IMAGINARY         
    { $$ = add_node0("type_specifier -> imaginary\n"); }
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list_opt 
    { $$ = add_node2("specifier_qualifier_list -> type_specifier specifier_qualifier_list_opt\n", $1, $2); }  
    | type_qualifier specifier_qualifier_list_opt   
    { $$ = add_node2("specifier_qualifier_list -> type_qualifier specifier_qualifier_list_opt\n", $1, $2); }
    ;

specifier_qualifier_list_opt
    : specifier_qualifier_list
    { $$ = add_node1("specifier_qualifier_list_opt -> specifier_qualifier_list\n", $1); }  
    |                           
    { $$ = add_node0("specifier_qualifier_list_opt -> epsilon\n"); }
    ;

type_qualifier
    : CONST 
    { $$ = add_node0("type_qualifier -> const\n"); }        
    | RESTRICT      
    { $$ = add_node0("type_qualifier -> restrict\n"); }
    | VOLATILE      
    { $$ = add_node0("type_qualifier -> volatile\n"); }
    ;

function_specifier
    : INLINE
    { $$ = add_node0("function_specifier -> inline\n"); }        
    ;

declarator
    : pointer_opt direct_declarator 
    { $$ = add_node2("declarator -> pointer_opt direct_declarator\n", $1, $2); }    
    ;
    
pointer_opt
    : pointer
    { $$ = add_node1("pointer_opt -> pointer\n", $1); }       
    |               
    { $$ = add_node0("pointer_opt -> epsilon\n"); }
    ;

direct_declarator
    : IDENTIFIER
    { char *content = (char *)malloc(23 + strlen($1)); sprintf(content, "direct_declarator -> %s\n", $1); $$ = add_node0(content); }                                                                                                    
    | ROUND_BRAC_OPEN declarator ROUND_BRAC_CLOSE                                                                    
    { $$ = add_node1("direct_declarator -> (declarator)\n", $2); }
    | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list_opt assignment_expression_opt SQUARE_BRAC_CLOSE   
    { $$ = add_node3("direct_declarator -> direct_declarator [type_qualifier_list_opt assignment_expression_opt]\n", $1, $3, $4); }      
    | direct_declarator SQUARE_BRAC_OPEN STATIC type_qualifier_list_opt assignment_expression SQUARE_BRAC_CLOSE 
    { $$ = add_node3("direct_declarator -> direct_declarator [static type_qualifier_list_opt assignment_expression]\n", $1, $4, $5); }     
    | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list STATIC assignment_expression SQUARE_BRAC_CLOSE
    { $$ = add_node3("direct_declarator -> direct_declarator [type_qualifier_list static assignment_expression]\n", $1, $3, $5); }          
    | direct_declarator SQUARE_BRAC_OPEN type_qualifier_list_opt MULTIPLY_OP SQUARE_BRAC_CLOSE                       
    { $$ = add_node2("direct_declarator -> direct_declarator [type_qualifier_list_opt *]\n", $1, $3); }
    | direct_declarator ROUND_BRAC_OPEN parameter_type_list ROUND_BRAC_CLOSE                           
    { $$ = add_node2("direct_declarator -> direct_declarator (parameter_type_list)\n", $1, $3); }  
    | direct_declarator ROUND_BRAC_OPEN identifier_list_opt ROUND_BRAC_CLOSE
    { $$ = add_node2("direct_declarator -> direct_declarator (identifier_list_opt)\n", $1, $3); }            
    ;

type_qualifier_list_opt
    : type_qualifier_list
    { $$ = add_node1("type_qualifier_list_opt -> type_qualifier_list\n", $1); }       
    |                           
    { $$ = add_node0("type_qualifier_list_opt -> epsilon\n"); }
    ;

assignment_expression_opt       
    : assignment_expression 
    { $$ = add_node1("assignment_expression_opt -> assignment_expression\n", $1); }    
    |                           
    { $$ = add_node0("assignment_expression_opt -> epsilon\n"); }
    ;

identifier_list_opt 
    : identifier_list
    { $$ = add_node1("identifier_list_opt -> identifier_list\n", $1); }           
    | 
    { $$ = add_node0("identifier_list_opt -> epsilon\n"); }                          
    ;

pointer
    : MULTIPLY_OP type_qualifier_list_opt 
    { $$ = add_node1("pointer -> * type_qualifier_list_opt\n", $2); }              
    | MULTIPLY_OP type_qualifier_list_opt pointer 
    { $$ = add_node2("pointer -> * type_qualifier_list_opt pointer\n", $2, $3); }      
    ;

type_qualifier_list
    : type_qualifier
    { $$ = add_node1("type_qualifier_list -> type_qualifier\n", $1); }                        
    | type_qualifier_list type_qualifier    
    { $$ = add_node2("type_qualifier_list -> type_qualifier_list type_qualifier\n", $1, $2); }
    ;

parameter_type_list
    : parameter_list
    { $$ = add_node1("parameter_type_list -> parameter_list\n", $1); }                        
    | parameter_list COMMA ELLIPSIS         
    { $$ = add_node1("parameter_type_list -> parameter_list , ...\n", $1); }
    ;

parameter_list
    : parameter_declaration 
    { $$ = add_node1("parameter_list -> parameter_declaration\n", $1); }                            
    | parameter_list COMMA parameter_declaration 
    { $$ = add_node2("parameter_list -> parameter_list , parameter_declaration\n", $1, $3); }       
    ;

parameter_declaration
    : declaration_specifiers declarator
    { $$ = add_node2("parameter_declaration -> declaration_specifiers declarator\n", $1, $2); }         
    | declaration_specifiers                    
    { $$ = add_node1("parameter_declaration -> declaration_specifiers\n", $1); }
    ;

identifier_list
    : IDENTIFIER
    { char *content = (char *)malloc(21 + strlen($1)); sprintf(content, "identifier_list -> %s\n", $1); $$ = add_node0(content); }                               
    | identifier_list COMMA IDENTIFIER   
    { char *content = (char *)malloc(39 + strlen($3)); sprintf(content, "identifier_list -> identifier_list , %s\n", $3); $$ = add_node1(content, $1); }      
    ;

type_name
    : specifier_qualifier_list
    { $$ = add_node1("type_name -> specifier_qualifier_list\n", $1); }          
    ;

initializer
    : assignment_expression
    { $$ = add_node1("initializer -> assignment_expression\n", $1); }                                       
    | CURLY_BRAC_OPEN initializer_list CURLY_BRAC_CLOSE            
    { $$ = add_node1("initializer -> {initializer_list}\n", $2); }
    | CURLY_BRAC_OPEN initializer_list COMMA CURLY_BRAC_CLOSE     
    { $$ = add_node1("initializer -> {initializer_list,}\n", $2); } 
    ;

initializer_list
    : designation_opt initializer
    { $$ = add_node2("initializer_list -> designation_opt initializer\n", $1, $2); }                              
    | initializer_list COMMA designation_opt initializer      
    { $$ = add_node3("initializer_list -> initializer_list , designation_opt initializer\n", $1, $3, $4); }
    ;

designation_opt
    : designation
    { $$ = add_node1("designation_opt -> designation\n", $1); }      
    |                 
    { $$ = add_node0("designation_opt -> epsilon\n"); }
    ;

designation
    : designator_list ASSIGN_OP
    { $$ = add_node1("designation -> designator_list =\n", $1); }    
    ;

designator_list
    : designator
    { $$ = add_node1("designator_list -> designator\n", $1); }                   
    | designator_list designator
    { $$ = add_node2("designator_list -> designator_list designator\n", $1, $2); }    
    ;

designator
    : SQUARE_BRAC_OPEN constant_expression SQUARE_BRAC_CLOSE
    { $$ = add_node1("designator -> [constant_expression]\n", $2); }  
    | DOT IDENTIFIER 
    { char *content = (char *)malloc(18 + strlen($2)); sprintf(content, "designator -> . %s\n", $2); $$ = add_node0(content); }                                           
    ;

statement
    : labeled_statement
    { $$ = add_node1("statement -> labeled_statement\n", $1); }       
    | compound_statement     
    { $$ = add_node1("statement -> compound_statement\n", $1); }
    | expression_statement      
    { $$ = add_node1("statement -> expression_statement\n", $1); }
    | selection_statement       
    { $$ = add_node1("statement -> selection_statement\n", $1); }
    | iteration_statement       
    { $$ = add_node1("statement -> iteration_statement\n", $1); }
    | jump_statement           
    { $$ = add_node1("statement -> jump_statement\n", $1); }
    ;

labeled_statement
    : IDENTIFIER COLON statement
    { char *content = (char *)malloc(35 + strlen($1)); sprintf(content, "labeled_statement -> %s : statement\n", $1); $$ = add_node1(content, $3); }                   
    | CASE constant_expression COLON statement
    { $$ = add_node2("labeled_statement -> case constant_expression : statement\n", $2, $4); }      
    | DEFAULT COLON statement                      
    { $$ = add_node1("labeled_statement -> default : statement\n", $3); }
    ;

compound_statement
    : CURLY_BRAC_OPEN block_item_list_opt CURLY_BRAC_CLOSE 
    { $$ = add_node1("compound_statement -> {block_item_list_opt}\n", $2); }  
    ;

block_item_list_opt
    : block_item_list 
    { $$ = add_node1("block_item_list_opt -> block_item_list\n", $1); }     
    |
    { $$ = add_node0("block_item_list_opt -> epsilon\n"); }                      
    ;

block_item_list
    : block_item 
    { $$ = add_node1("block_item_list -> block_item\n", $1); }                      
    | block_item_list block_item       
    { $$ = add_node2("block_item_list -> block_item_list block_item\n", $1, $2); }
    ;

block_item
    : declaration
    { $$ = add_node1("block_item -> declaration\n", $1); }            
    | statement                
    { $$ = add_node1("block_item -> statement\n", $1); }
    ;

expression_statement
    : expression_opt SEMICOLON
    { $$ = add_node1("expression_statement -> expression_opt ;\n", $1); } 
    ;

expression_opt
    : expression
    { $$ = add_node1("expression_opt -> expression\n", $1); }              
    |                         
    { $$ = add_node0("expression_opt -> epsilon\n"); }
    ;

selection_statement
    : IF ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE statement %prec LOWER_THAN_ELSE 
    { $$ = add_node2("selection_statement -> if (expression) statement\n", $3, $5); }          
    | IF ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE statement ELSE statement        
    { $$ = add_node3("selection_statement -> if (expression) statement else statement\n", $3, $5, $7); }          
    | SWITCH ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE statement                             
    { $$ = add_node2("selection_statement -> switch (expression) statement\n", $3, $5); }
    ;

iteration_statement
    : WHILE ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE statement
    { $$ = add_node2("iteration_statement -> while (expression) statement\n", $3, $5); }                                                          
    | DO statement WHILE ROUND_BRAC_OPEN expression ROUND_BRAC_CLOSE SEMICOLON
    { $$ = add_node2("iteration_statement -> do statement while (expression) ;\n", $2, $5); }                                            
    | FOR ROUND_BRAC_OPEN expression_opt SEMICOLON expression_opt SEMICOLON expression_opt ROUND_BRAC_CLOSE statement
    { $$ = add_node4("iteration_statement -> for (expression_opt; expression_opt; expression_opt) statement\n", $3, $5, $7, $9); }      
    | FOR ROUND_BRAC_OPEN declaration expression_opt SEMICOLON expression_opt ROUND_BRAC_CLOSE statement                    
    { $$ = add_node4("iteration_statement -> for (declaration expression_opt; expression_opt) statement\n", $3, $4, $6, $8); }
    ;

jump_statement
    : GOTO IDENTIFIER SEMICOLON 
    { char *content = (char *)malloc(27 + strlen($2)); sprintf(content, "jump_statement -> goto %s ;\n", $2); $$ = add_node0(content); }           
    | CONTINUE SEMICOLON 
    { $$ = add_node0("jump_statement -> continue ;\n"); }                 
    | BREAK SEMICOLON                      
    { $$ = add_node0("jump_statement -> break ;\n"); }
    | RETURN expression_opt SEMICOLON      
    { $$ = add_node1("jump_statement -> return expression_opt ;\n", $2); }
    ;

translation_unit
    : external_declaration
    { $$ = add_node1("translation_unit -> external_declaration\n", $1); root = $$; }                  
    | translation_unit external_declaration    
    { $$ = add_node2("translation_unit -> translation_unit external_declaration\n", $1, $2); root = $$; }
    ;

external_declaration
    : function_definition 
    { $$ = add_node1("external_declaration -> function_definition\n", $1); }                     
    | declaration  
    { $$ = add_node1("external_declaration -> declaration\n", $1); }                            
    ;   

function_definition
    : declaration_specifiers declarator declaration_list_opt compound_statement
    { $$ = add_node4("function_definition -> declaration_specifiers declarator declaration_list_opt compound_statement\n", $1, $2, $3, $4); }    

declaration_list_opt
    : declaration_list
    { $$ = add_node1("declaration_list_opt -> declaration_list\n", $1); }      
    |
    { $$ = add_node0("declaration_list_opt -> epsilon\n"); }
    ;

declaration_list
    : declaration
    { $$ = add_node1("declaration_list -> declaration\n", $1); }                       
    | declaration_list declaration 
    { $$ = add_node2("declaration_list -> declaration_list declaration\n", $1, $2); }    
    ;

%%