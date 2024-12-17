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
typedef node *environments;
typedef node *inline_math;
typedef node *display_math;

/* Function to add a new command or environment to the table */
node *addtbl(node *T, char *id)
{
    node *p = T;
    while (p)
    {
        if (!strcmp(p->name, id))
        {
            p->count++;
            return T;
        }
        p = p->next;
    }

    /* Adding new entry */
    p = (node *)malloc(sizeof(node));
    p->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(p->name, id);
    p->count = 1;
    p->next = T;
    return p;
}

/* Function to print the table and free memory */
void print_and_free_table(node *T)
{
    node *temp;
    while (T)
    {
        printf("%s  %d\n", T->name, T->count);
        temp = T;
        T = T->next;
        free(temp->name);
        free(temp);
    }
}

int main(int argc, char const *argv[])
{
    int next_token;
    commands T = NULL;
    environments E = NULL;
    inline_math INLM = NULL;
    display_math DISM = NULL;

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
            E = addtbl(E, yytext);
            break;
        case ENV_END:
            // printf("Environment End\n");
            // T = addtbl(T, yytext);
            E = addtbl(E, yytext);
            break;
        case DISPLAY_MATH_START:
            // printf("Display Math Started\n");
            // T = addtbl(T, yytext);
            DISM = addtbl(DISM, yytext);
            break;
        case DISPLAY_MATH_END:
            // printf("Display Math End\n");
            // T = addtbl(T, yytext);
            DISM = addtbl(DISM, yytext);
            break;
        case INLINE_MATH_START:
            // printf("Inline Math Started\n");
            // T = addtbl(T, yytext);
            INLM = addtbl(INLM, yytext);
            break;
        case INLINE_MATH_END:
            // printf("Inline Math End\n");
            // T = addtbl(T, yytext);
            break;
        case COMMENT:
            // printf("Comment\n");
            INLM = addtbl(INLM, yytext);
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
    print_and_free_table(T);
    printf("\n");

    printf("ENVIRONMENTS\n");
    print_and_free_table(E);
    printf("\n");

    printf("INLINE MATH\n");
    print_and_free_table(INLM);
    printf("\n");

    printf("DISPLAY MATH\n");
    print_and_free_table(DISM);
    printf("\n");

    return 0;
}

int yywrap(void)
{
    return 1;
}
