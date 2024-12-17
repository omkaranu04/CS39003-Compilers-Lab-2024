#ifndef CUSTOM_H
#define CUSTOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_NUMBER 1
#define TYPE_VARIABLE 2

struct SymbolNode
{
    char *name;
    int value;
    int is_assigned;
    struct SymbolNode *next;
};
typedef struct SymbolNode *symboltable;

struct ExprNode
{
    int type;
    union
    {
        char *op;
        symboltable variable;
        int number;
    };
    struct ExprNode *left;
    struct ExprNode *right;
};
typedef struct ExprNode *TreeNode;

symboltable add_symbol(symboltable T, char *variable_name);
void set_symbol_value(symboltable *T, char *name, int value);
int get_symbol_value(symboltable *T, char *name);
TreeNode create_leaf_node(symboltable T, int type, char *value);
TreeNode create_op_node(char *operator, TreeNode left, TreeNode right);
int evaluate_expr_tree(TreeNode root);
void free_expr_tree(TreeNode root);
void print_symbol_table(symboltable T);

int yylex(void);
void yyerror(const char *s);

#endif