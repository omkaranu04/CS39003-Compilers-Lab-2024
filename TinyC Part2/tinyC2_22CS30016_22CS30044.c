#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

extern int yylineno;
struct node
{
    char *message;
    struct node *children[5];
};
typedef struct node *Node;
extern Node root;

Node add_node0(char *msg)
{
    if(msg == NULL) return NULL;
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->message = strdup(msg);
    return temp;
}
Node add_node1(char *msg, Node n1)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->message = strdup(msg);
    temp->children[0] = n1;
    temp->children[1] = NULL;
    temp->children[2] = NULL;
    temp->children[3] = NULL;
    temp->children[4] = NULL;
    return temp;
}
Node add_node2(char *msg, Node n1, Node n2)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->message = strdup(msg);
    temp->children[0] = n1;
    temp->children[1] = n2;
    temp->children[2] = NULL;
    temp->children[3] = NULL;
    temp->children[4] = NULL;
    return temp;
}
Node add_node3(char *msg, Node n1, Node n2, Node n3)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->message = strdup(msg);
    temp->children[0] = n1;
    temp->children[1] = n2;
    temp->children[2] = n3;
    temp->children[3] = NULL;
    temp->children[4] = NULL;
    return temp;
}
Node add_node4(char *msg, Node n1, Node n2, Node n3, Node n4)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->message = strdup(msg);
    temp->children[0] = n1;
    temp->children[1] = n2;
    temp->children[2] = n3;
    temp->children[3] = n4;
    temp->children[4] = NULL;
    return temp;
}
Node add_node5(char *msg, Node n1, Node n2, Node n3, Node n4, Node n5)
{
    Node temp = (struct node *)malloc(sizeof(struct node));
    temp->message = strdup(msg);
    temp->children[0] = n1;
    temp->children[1] = n2;
    temp->children[2] = n3;
    temp->children[3] = n4;
    temp->children[4] = n5;
    return temp;
}

void print_tree(Node root, int level)
{
    if (root == NULL)
        return;
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
    printf("%s", root->message);
    for (int i = 0; i < 5; i++)
    {
        print_tree(root->children[i], level + 1);
    }
}
void free_tree(Node root, int level)
{
    if (root == NULL)
        return;
    for (int i = 0; i < 5; i++)
    {
        free_tree(root->children[i], level + 1);
    }
    free(root);
}

int main()
{
    int parse_result = yyparse();
    if (parse_result == 0)
    {
        printf("Parsing Successful!!\n");
        print_tree(root, 0);
        free_tree(root, 0);
        printf("Tree freed successfully!!\n");
    }
    else
    {
        printf("Parsing failed\n");
        printf("Error at line number: %d\n", yylineno);
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