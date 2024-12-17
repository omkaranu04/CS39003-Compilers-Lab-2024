#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"

typedef struct _node
{
    char *name;
    int count;
    struct _node *next;
} node;
typedef node *commands;

commands addtbl(commands T, char *id)
{
    node *p;
    p = T;
    while (p)
    {
        if (!strcmp(p->name, id))
        {
            p->count = p->count + 1;
            return T;
        }
        p = p->next;
    }
    // printf("Adding new command: %s\n", id);
    p = (node *)malloc(sizeof(node));
    p->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(p->name, id);
    p->count = 1;
    p->next = T;
    return p;
}

struct node_
{
    char *name;
    int count;
    struct node_ *next;
};
typedef node_ *environments;

environments addtble(environments T, char *id)
{
    node *p;
    p = T;
    while (p)
    {
        if (!strcmp(p->name, id))
        {
            p->count = p->count + 1;
            return T;
        }
        p = p->next;
    }
    // printf("Adding new command: %s\n", id);
    p = (node_ *)malloc(sizeof(node_));
    p->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(p->name, id);
    p->count = 1;
    p->next = T;
    return p;
}

int main(int argc, char const *argv[])
{
    int next_token;
    commands T = NULL;
    environments E = NULL;

    while ((next_token = yylex()))
    {
        switch (next_token)
        {
        case ACTIVE_CHARACTER:
            // printf("Active Character\n");
            T = addtbl(T, yytext);
            break;
        case COMMAND_1:
            // printf("First Type Command\n");
            T = addtbl(T, yytext);
            break;
        case COMMAND_2:
            // printf("Second Type Command\n");
            T = addtbl(T, yytext);
            break;
        case ENV_START:
            // printf("Environment Start\n");
            // T = addtbl(T, yytext);
            E = addtble(E, yytext);
            break;
        case ENV_END:
            // printf("Environment End\n");
            // T = addtbl(T, yytext);
            E = addtble(E, yytext);
            break;
        case DISPLAY_MATH_START:
            // printf("Display Math Started\n");
            T = addtbl(T, yytext);
            break;
        case DISPLAY_MATH_END:
            // printf("Display Math End\n");
            T = addtbl(T, yytext);
            break;
        case INLINE_MATH_START:
            // printf("Inline Math Started\n");
            T = addtbl(T, yytext);
            break;
        case INLINE_MATH_END:
            // printf("Inline Math End\n");
            T = addtbl(T, yytext);
            break;
        case COMMENT:
            // printf("Comment\n");
            break;
        case SKIP:
            // printf("\n");
            break;
        default:
            // printf("Something Else\n");
            break;
        }
    }

    printf("COMMANDS\n");
    commands temp = T;
    while (temp)
    {
        printf("%s  %d\n", temp->name, temp->count);
        temp = temp->next;
    }

    printf("ENVIRONMENTS\n");
    environments tempi = E;
    while (temp)
    {
        printf("%s  %d\n", temp->name, temp->count);
        temp = temp->next;
    }
    return 0;
}

int yywrap(void)
{
    return 1;
}