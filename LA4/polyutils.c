#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "y.tab.h"

struct node
{
    char *name;
    union
    {
        char sign;
        long long value;
    } inh;
    long long val;
    struct node *children[3];
};
typedef struct node *Node;

// root node taking from the yacc file so extern
extern Node root_node;

// adding various number of nodes
Node addnode0(char *name)
{
    if (name == NULL)
        return NULL;
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->name = strdup(name);
    temp->children[0] = NULL;
    temp->children[1] = NULL;
    temp->children[2] = NULL;
    return temp;
}
Node addnode1(char *name, Node n1)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->name = strdup(name);
    temp->children[0] = n1;
    temp->children[1] = NULL;
    temp->children[2] = NULL;
    return temp;
}
Node addnode2(char *name, Node n1, Node n2)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->name = strdup(name);
    temp->children[0] = n1;
    temp->children[1] = n2;
    temp->children[2] = NULL;
    return temp;
}
Node addnode3(char *name, Node n1, Node n2, Node n3)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->name = strdup(name);
    temp->children[0] = n1;
    temp->children[1] = n2;
    temp->children[2] = n3;
    return temp;
}

// for testing only
void print_tree(Node root_node, long long level)
{
    if (root_node == NULL)
        return;
    for (long long i = 0; i < level; i++)
        printf("  ");
    printf("%s\n", root_node->name);
    for (long long i = 0; i < 3; i++)
    {
        print_tree(root_node->children[i], level + 1);
    }
}

void setatt(Node root)
{
    if (root == NULL)
        return;
    // inh values
    if (strcmp(root->name, "S") == 0)
    {
        if (strcmp(root->children[0]->name, "+") == 0 && strcmp(root->children[1]->name, "P") == 0 && root->children[2] == NULL)
        {
            root->children[1]->inh.sign = '+';
        }
        else if (strcmp(root->children[0]->name, "-") == 0 && strcmp(root->children[1]->name, "P") == 0 && root->children[2] == NULL)
        {
            root->children[1]->inh.sign = '-';
        }
        else
        {
            root->children[1]->inh.sign = '+';
        }
    }
    else if (strcmp(root->name, "P") == 0)
    {
        if (strcmp(root->children[0]->name, "T") == 0 && root->children[1] == NULL && root->children[2] == NULL)
        {
            root->children[0]->inh.sign = root->inh.sign;
        }
        else if (strcmp(root->children[0]->name, "T") == 0 && strcmp(root->children[1]->name, "+") == 0 && strcmp(root->children[2]->name, "P") == 0)
        {
            root->children[0]->inh.sign = root->inh.sign;
            root->children[2]->inh.sign = '+';
        }
        else if (strcmp(root->children[0]->name, "T") == 0 && strcmp(root->children[1]->name, "-") == 0 && strcmp(root->children[2]->name, "P") == 0)
        {
            root->children[0]->inh.sign = root->inh.sign;
            root->children[2]->inh.sign = '-';
        }
    }
    else if (strcmp(root->name, "N") == 0)
    {
        if (root->children[1] != NULL && strcmp(root->children[1]->name, "M") == 0)
        {
            root->children[1]->inh.value = atoll(root->children[0]->name);
        }
    }
    else if (strcmp(root->name, "M") == 0)
    {
        if (root->children[1] != NULL && strcmp(root->children[1]->name, "M") == 0)
        {
            root->children[1]->inh.value = root->inh.value * 10 + atoll(root->children[0]->name);
        }
    }

    // recursion
    for (long long i = 0; i < 3; i++)
    {
        if (root->children[i] != NULL)
        {
            setatt(root->children[i]);
        }
    }

    // val values
    if (strcmp(root->name, "S") == 0)
    {
        if (strcmp(root->children[0]->name, "P") == 0 && root->children[1] == NULL && root->children[2] == NULL)
        {
            root->val = root->children[0]->val;
        }
    }
    else if (strcmp(root->name, "N") == 0)
    {
        if (root->children[1] == NULL && root->children[2] == NULL)
        {
            root->val = atoll(root->children[0]->name);
        }
        else if (root->children[1] != NULL && strcmp(root->children[1]->name, "M") == 0)
        {
            root->val = root->children[1]->val;
        }
    }
    else if (strcmp(root->name, "M") == 0)
    {
        if (root->children[0] != NULL && root->children[1] == NULL && root->children[2] == NULL)
        {
            root->val = root->inh.value * 10 + atoll(root->children[0]->name);
        }
        else if (root->children[1] != NULL && strcmp(root->children[1]->name, "M") == 0 && root->children[2] == NULL)
        {
            root->val = root->children[1]->val;
        }
    }
}
void print_annotated_tree(Node root, long long level)
{
    if (root == NULL)
        return;
    for (long long i = 0; i < level; i++)
        printf("   ");
    printf("==> ");
    if (strcmp(root->name, "S") == 0)
        printf("S []\n");
    else if (strcmp(root->name, "-") == 0)
        printf("- []\n");
    else if (strcmp(root->name, "+") == 0)
        printf("+ []\n");
    else if (strcmp(root->name, "P") == 0)
        printf("P [inh = %c]\n", root->inh.sign);
    else if (strcmp(root->name, "T") == 0)
        printf("T [inh = %c]\n", root->inh.sign);
    else if (strcmp(root->name, "N") == 0)
        printf("N [val = %lld]\n", root->val);
    else if (strcmp(root->name, "M") == 0)
        printf("M [inh = %lld, val = %lld]\n", root->inh.value, root->val);
    else if (strcmp(root->name, "X") == 0)
        printf("X []\n");
    else if (strcmp(root->name, "x") == 0)
        printf("x []\n");
    else if (strcmp(root->name, "^") == 0)
        printf("^ []\n");
    else if (!strcmp(root->name, "0") || !strcmp(root->name, "1") || !strcmp(root->name, "2") || !strcmp(root->name, "3") || !strcmp(root->name, "4") || !strcmp(root->name, "5") || !strcmp(root->name, "6") || !strcmp(root->name, "7") || !strcmp(root->name, "8") || !strcmp(root->name, "9"))
        printf("%lld [val = %lld]\n", atoll(root->name), atoll(root->name));
    for (long long i = 0; i < 3; i++)
    {
        if (root->children[i] != NULL)
        {
            print_annotated_tree(root->children[i], level + 1);
        }
    }
}
long long evalpoly(Node root, long long x)
{
    // return only
    if (root == NULL)
        return 0;
    if (strcmp(root->name, "T") == 0)
    {
        long long coefficient = 1;
        long long exponent = 1;
        // T -> N
        if (strcmp(root->children[0]->name, "N") == 0 && root->children[1] == NULL && root->children[2] == NULL)
        {
            if (root->inh.sign == '-')
                return -root->children[0]->val;
            return root->children[0]->val;
        }
        // T -> 1
        else if (strcmp(root->children[0]->name, "1") == 0 && root->children[1] == NULL && root->children[2] == NULL)
        {
            if (root->inh.sign == '-')
                return -1;
            return 1;
        }
        // T -> NX
        else if (strcmp(root->children[0]->name, "N") == 0 && strcmp(root->children[1]->name, "X") == 0 && root->children[2] == NULL)
        {
            coefficient = root->children[0]->val;
            Node temp = root->children[1];
            if (temp->children[1] != NULL && strcmp(temp->children[1]->name, "^") == 0)
            {
                exponent = temp->children[2]->val;
            }
        }
        // T -> X
        else if (strcmp(root->children[0]->name, "X") == 0)
        {
            Node temp = root->children[0];
            if (temp->children[1] != NULL && strcmp(temp->children[1]->name, "^") == 0)
            {
                exponent = temp->children[2]->val;
            }
        }
        coefficient = (root->inh.sign == '+') ? coefficient : -coefficient;
        long long result = coefficient * (long long)pow(x, exponent);
        return result;
    }
    else if (strcmp(root->name, "P") == 0)
    {
        // P -> T
        if (strcmp(root->children[0]->name, "T") == 0 && root->children[1] == NULL && root->children[2] == NULL)
        {
            return evalpoly(root->children[0], x);
        }
        // P -> P + T  and P -> P - T
        else if (strcmp(root->children[0]->name, "T") == 0 && strcmp(root->children[2]->name, "P") == 0)
        {
            long long left_val = evalpoly(root->children[0], x);
            long long right_val = evalpoly(root->children[2], x);
            return left_val + right_val;
        }
    }
    else if (strcmp(root->name, "S") == 0)
    {
        // S -> P
        if (strcmp(root->children[0]->name, "P") == 0 && root->children[1] == NULL && root->children[2] == NULL)
        {
            return evalpoly(root->children[0], x);
        }
        // S -> + P and S -> - P
        return evalpoly(root->children[1], x);
    }
}

// function for T -> NX (handled specially)
void print_helper(Node root)
{
    if (root == NULL)
        return;
    // incoming root = T
    long long coefficient = root->children[0]->val;
    long long exponent;
    Node temp = root->children[1];
    if (strcmp(temp->children[0]->name, "x") == 0 && temp->children[1] == NULL && temp->children[2] == NULL)
    {
        exponent = 1;
    }
    else if (strcmp(temp->children[0]->name, "x") == 0 && strcmp(temp->children[1]->name, "^") == 0 && strcmp(temp->children[2]->name, "N") == 0)
    {
        coefficient *= temp->children[2]->val;
        exponent = temp->children[2]->val - 1;
    }
    else
        return;

    // print
    if (exponent == 0)
    {
        printf("%lld", coefficient);
    }
    else if (exponent == 1)
    {
        printf("%lldx", coefficient);
    }
    else
    {
        printf("%lldx^%lld", coefficient, exponent);
    }
    return;
}

// main derivative printing function
void printderivative(Node root)
{
    if (root == NULL || (strcmp(root->name, "N") == 0 && strcmp(root->name, "M") == 0))
    {
        return;
    }
    if (strcmp(root->name, "S") == 0)
    {
        if (root->children[0] != NULL && root->children[1] == NULL && root->children[2] == NULL)
        {
            if (strcmp(root->children[0]->name, "P") == 0)
            {
                printderivative(root->children[0]);
            }
        }
        else if (root->children[0] != NULL && root->children[1] != NULL && root->children[2] == NULL)
        {
            if (strcmp(root->children[0]->name, "+") == 0 && strcmp(root->children[1]->name, "P") == 0)
            {
                printderivative(root->children[1]);
            }
            else if (strcmp(root->children[0]->name, "-") == 0 && strcmp(root->children[1]->name, "P") == 0)
            {
                printderivative(root->children[1]);
            }
        }
        else
            return;
    }
    else if (strcmp(root->name, "P") == 0)
    {
        if (root->children[0] != NULL && root->children[1] == NULL && root->children[2] == NULL)
        {
            if (strcmp(root->children[0]->name, "T") == 0)
            {
                printderivative(root->children[0]);
            }
        }
        else if (root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL)
        {
            if (strcmp(root->children[0]->name, "T") == 0 && strcmp(root->children[1]->name, "+") == 0 && strcmp(root->children[2]->name, "P") == 0)
            {
                printderivative(root->children[0]);
                printderivative(root->children[2]);
            }
            else if (strcmp(root->children[0]->name, "T") == 0 && strcmp(root->children[1]->name, "-") == 0 && strcmp(root->children[2]->name, "P") == 0)
            {
                printderivative(root->children[0]);
                printderivative(root->children[2]);
            }
        }
        else
            return;
    }
    else if (strcmp(root->name, "T") == 0)
    {
        if (root->children[0] != NULL && root->children[1] == NULL && root->children[2] == NULL)
        {
            if (strcmp(root->children[0]->name, "X") == 0)
            {
                printf(" %c ", root->inh.sign);
                printderivative(root->children[0]);
            }
        }
        else if (root->children[0] != NULL && root->children[1] != NULL && root->children[2] == NULL)
        {
            if (strcmp(root->children[0]->name, "N") == 0 && strcmp(root->children[1]->name, "X") == 0)
            {
                printf(" %c ", root->inh.sign);
                if (root->children[1]->children[0] != NULL && strcmp(root->children[1]->children[0]->name, "x") == 0 && root->children[1]->children[1] == NULL && root->children[1]->children[2] == NULL)
                {
                    printf("%lld", root->children[0]->val);
                }
                else
                {
                    print_helper(root);
                }
            }
        }
        else
            return;
    }
    else if (strcmp(root->name, "X") == 0)
    {
        if (root->children[0] != NULL && root->children[1] == NULL && root->children[2] == NULL)
        {
            if (strcmp(root->children[0]->name, "x") == 0)
            {
                printf("1");
            }
        }
        else if (root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL)
        {
            if (strcmp(root->children[0]->name, "x") == 0 && strcmp(root->children[1]->name, "^") == 0 && strcmp(root->children[2]->name, "N") == 0)
            {
                if (root->children[2]->val - 1 == 1)
                {
                    printf("%lldx", root->children[2]->val);
                }
                else
                {
                    printf("%lldx^%lld", root->children[2]->val, root->children[2]->val - 1);
                }
            }
        }
        else
            return;
    }
}

int main()
{
    long long result = yyparse();
    if (result == 0)
    {
        setatt(root_node);
        printf("+++ The annotated parse tree is\n");
        print_annotated_tree(root_node, 0);
        printf("\n");
        for (long long i = -5; i <= 5; i++)
        {
            printf("+++ f(%2lld) = %15lld\n", i, evalpoly(root_node, i));
        }
        printf("\n");
        printf("+++ f'(x) =  ");
        printderivative(root_node);
        printf("\n");
    }
    else
    {
        printf("Error in parsing\n");
    }
    return 0;
}
void yyerror(char *err)
{
    fprintf(stderr, "%s\n", err);
}
int yywrap(void)
{
    return 1;
}
// added makefile to accomodate the changes