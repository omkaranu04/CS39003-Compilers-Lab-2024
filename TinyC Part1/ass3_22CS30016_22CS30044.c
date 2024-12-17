/*
Name : Omkar Bhandare       Roll No. : 22CS30016
Name : Tanishka Rahate      Roll No. : 22CS30044
Assignment 3 : Lexical Grammar of tinyC
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"

// Linked List for storing all the <tokenname, lexeme> produced by the lexer
struct token_node
{
    int type;
    char *name;
    struct token_node *next;
};
typedef struct token_node *token_table;
token_table add_token(token_table T, int type, char *content)
{
    if (T == NULL)
    {
        T = (token_table)malloc(sizeof(struct token_node));
        T->type = type;
        T->name = (char *)malloc((strlen(content) + 1) * sizeof(char));
        strcpy(T->name, content);
        T->next = NULL;
        return T;
    }
    token_table tmp = T;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    token_table x = (token_table)malloc(sizeof(struct token_node));
    x->type = type;
    x->name = (char *)malloc((strlen(content) + 1) * sizeof(char));
    strcpy(x->name, content);
    x->next = NULL;
    tmp->next = x;
    return T;
}

// Linked List for storing all the identifiers i.e. symbol table
struct iden_node
{
    char *name;
    struct iden_node *next;
};
typedef struct iden_node *symboltable;
symboltable add_iden(symboltable S, char *id)
{
    if (S == NULL)
    {
        S = (symboltable)malloc(sizeof(struct iden_node));
        S->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
        strcpy(S->name, id);
        S->next = NULL;
        return S;
    }
    symboltable tmp = S;
    while (tmp->next != NULL)
    {
        if (strcmp(tmp->name, id) == 0)
            return S;
        tmp = tmp->next;
    }
    if (strcmp(tmp->name, id) == 0)
        return S;
    symboltable x = (symboltable)malloc(sizeof(struct iden_node));
    x->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(x->name, id);
    x->next = NULL;
    tmp->next = x;
    return S;
}

int main(int argc, char const *argv[])
{
    int token;
    token_table T = NULL; // initialising a blank token list
    symboltable S = NULL; // initialising a blank symbol table
    while ((token = yylex()) != 0)
    {
        int l;
        switch (token)
        {
        case KEYWORD:
            T = add_token(T, KEYWORD, yytext);
            break;
        case IDENTIFIER:
            T = add_token(T, IDENTIFIER, yytext);
            S = add_iden(S, yytext);
            break;
        case CONSTANTS:
            T = add_token(T, CONSTANTS, yytext);
            break;
        case STRINGLITERALS:
            T = add_token(T, STRINGLITERALS, yytext);
            break;
        case PUNCTUATORS:
            T = add_token(T, PUNCTUATORS, yytext);
            break;
        case COMMENTS:
            l = strlen(yytext);
            if (l > 0 && yytext[l - 1] == '\n')
            {
                char *com = (char *)malloc(l * sizeof(char));
                strncpy(com, yytext, l - 1);
                com[l - 1] = '\0';
                T = add_token(T, COMMENTS, com);
                free(com);
            }
            else
                T = add_token(T, COMMENTS, yytext);
            break;
        default:
            printf("UNIDENTIFIED TOKEN : %s\n", yytext);
        }
    }
    // printing all the tokens <tokenname, lexeme> produced by the lexer file
    printf("TOKENS : \n");
    token_table temp = T;
    while (temp != NULL)
    {
        printf("<");
        switch (temp->type)
        {
        case KEYWORD:
            printf("KEYWORD");
            break;
        case IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case CONSTANTS:
            printf("CONSTANT");
            break;
        case STRINGLITERALS:
            printf("STRING_LITERAL");
            break;
        case PUNCTUATORS:
            printf("PUNCTUATOR");
            break;
        case COMMENTS:
            printf("COMMENT");
            break;
        default:
            printf("UNKNOWN");
        }
        printf(", '%s'>\n", temp->name);
        temp = temp->next;
    }

    // printing all the identifiers stored in the symbol table
    printf("SYMBOLTABLE : \n");
    symboltable tmp = S;
    while (tmp != NULL)
    {
        printf("%s\n", tmp->name);
        tmp = tmp->next;
    }
    return 0;
}

int yywrap(void)
{
    return 1;
}
