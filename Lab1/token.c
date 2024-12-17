#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"

typedef struct _node
{
    char *name;
    struct _node *next;
} node;
typedef node *symboltable;

symboltable addtbl(symboltable T, char *id)
{
    node *p;

    p = T;
    while (p)
    {
        if (!strcmp(p->name, id))
        {
            printf("Identifier %s already exists\n", id);
            return T;
        }
        p = p->next;
    }
    printf("Adding new identifier: %s\n", id);
    p = (node *)malloc(sizeof(node));
    p->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(p->name, id);
    p->next = T;
    return p;
}

int main()
{
    int nextok;
    symboltable T = NULL;

    while ((nextok = yylex()))
    {
        printf("lol\n");
        switch (nextok)
        {
        case LP:
            printf("Beginning of subexpression\n");
            break;
        case RP:
            printf("End of subexpression\n");
            break;
        case NUM:
            printf("Number: %s\n", yytext);
            break;
        case OP:
            printf("Operator: %s\n", yytext);
            break;
        case ID:
            T = addtbl(T, yytext);
            break;
        case ASG:
            printf("Assignment\n");
            break;
        default:
            printf("Unknown token\n");
            break;
        }
    }

    exit(0);
}

int yywrap(void)
{
    return 1;
}