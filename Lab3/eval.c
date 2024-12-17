#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom.h"
#include "y.tab.h"

symboltable add_symbol(symboltable T, char *variable_name)
{
    if (T == NULL)
    {
        T = (symboltable)malloc(sizeof(struct SymbolNode));
        T->name = strdup(variable_name);
        T->is_assigned = 0;
        T->next = NULL;
        return T;
    }
    symboltable temp = T;
    while (temp->next != NULL)
    {
        if (!strcmp(temp->name, variable_name))
        {
            return T;
        }
        temp = temp->next;
    }
    if (!strcmp(temp->name, variable_name))
    {
        return T;
    }
    symboltable p = (symboltable)malloc(sizeof(struct SymbolNode));
    p->name = strdup(variable_name);
    p->is_assigned = 0;
    p->next = NULL;
    temp->next = p;
    return T;
}

// for debugging purposes only
void print_symbol_table(symboltable T)
{
    symboltable temp = T;
    printf("Printing SymbolTable : \n");
    while (temp)
    {
        printf("%s %d\n", temp->name, temp->value);
        temp = temp->next;
    }
}

void set_symbol_value(symboltable *T, char *name, int value)
{
    symboltable temp = *T;
    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            temp->value = value;
            temp->is_assigned = 1;
            printf("Variable %s is set to %d\n", name, value);
            return;
        }
        temp = temp->next;
    }
    *T = add_symbol(*T, name);
    temp = *T;
    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            temp->value = value;
            temp->is_assigned = 1;
            printf("Variable %s is set to %d\n", name, value);
            return;
        }
        temp = temp->next;
    }
}

int get_symbol_value(symboltable *T, char *name)
{
    symboltable temp = *T;
    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            if (temp->is_assigned)
            {
                return temp->value;
            }
            else
            {
                printf("Error: Variable %s is not initialized\n", name);
                exit(1);
            }
        }
        temp = temp->next;
    }
    printf("Error: Variable %s not found\n", name);
    exit(1);
}

TreeNode create_leaf_node(symboltable T, int type, char *value)
{
    TreeNode node = (TreeNode)malloc(sizeof(struct ExprNode));
    node->left = node->right = NULL;
    node->type = type;

    if (type == TYPE_NUMBER)
    {
        node->number = atoi(value);
    }
    else if (type == TYPE_VARIABLE)
    {
        symboltable temp = T;
        while (temp != NULL)
        {
            if (strcmp(temp->name, value) == 0)
            {
                node->variable = temp;
                return node;
            }
            temp = temp->next;
        }
        printf("Error : Symbol not found in Table\n");
    }
    return node;
}
TreeNode create_op_node(char *operator, TreeNode left, TreeNode right)
{
    TreeNode node = (TreeNode)malloc(sizeof(struct ExprNode));
    node->left = left;
    node->right = right;
    node->type = 0;
    node->op = strdup(operator);
    return node;
}
int evaluate_expr_tree(TreeNode root)
{
    if (root == NULL)
        return 0;
    if (root->type == TYPE_NUMBER)
        return root->number;
    if (root->type == TYPE_VARIABLE)
        return get_symbol_value(&root->variable, root->variable->name);

    int left_val = evaluate_expr_tree(root->left);
    int right_val = evaluate_expr_tree(root->right);

    int result = 0;

    if (strcmp(root->op, "+") == 0)
    {
        return left_val + right_val;
    }
    if (strcmp(root->op, "-") == 0)
    {
        return left_val - right_val;
    }
    if (strcmp(root->op, "*") == 0)
    {
        return left_val * right_val;
    }
    if (strcmp(root->op, "/") == 0)
    {
        if (right_val != 0)
        {
            return left_val / right_val;
        }
        printf("Error: Division by zero\n");
        exit(1);
    }
    if (strcmp(root->op, "%") == 0)
    {
        return left_val % right_val;
    }
    if (strcmp(root->op, "**") == 0)
    {
        int result = 1;
        for (int i = 0; i < right_val; i++)
            result *= left_val;
        return result;
    }

    printf("Error: Unsupported operator %s\n", root->op);
    exit(1);
}
void free_expr_tree(TreeNode root)
{
    if (root == NULL)
        return;
    free_expr_tree(root->left);
    free_expr_tree(root->right);
    if (root->type == 0)
    {
        free(root->op);
    }
    free(root);
}

int main()
{
    // yydebug = 1;
    yyparse();
    return 0;
}