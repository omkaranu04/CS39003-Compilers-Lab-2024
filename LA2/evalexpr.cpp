#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"

using namespace std;

#define ARG 6
#define EXPR 7

struct node1
{
    char *name;
    int value;
    bool val = false;
    struct node1 *next;
};
typedef node1 *symboltable;

symboltable addToSymbolTable(symboltable T, char *variable_name)
{
    if (T == NULL)
    {
        T = (symboltable)malloc(sizeof(struct node1));
        T->name = (char *)malloc((strlen(variable_name) + 1) * sizeof(char));
        strcpy(T->name, variable_name);
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
    symboltable p = (symboltable)malloc(sizeof(struct node1));
    p->name = (char *)malloc((strlen(variable_name) + 1) * sizeof(char));
    strcpy(p->name, variable_name);
    temp->next = p;
    return T;
}

struct node2
{
    int x;
    struct node2 *next;
};
typedef node2 *numbertable;

numbertable addtoNumberTable(numbertable C, int n)
{
    node2 *p;

    p = C;
    while (p)
    {
        if (p->x == n)
        {
            return C;
        }
        p = p->next;
    }
    p = (node2 *)malloc(sizeof(node2));
    p->x = n;
    p->next = C;
    return p;
}

struct node3
{
    struct node3 *left;
    int flag;
    union
    {
        char *op;
        symboltable variable;
        numbertable number;
    };
    struct node3 *right;
    struct node3 *parent;
};
typedef node3 *TreeNode;

void travelParseTree(TreeNode root, int level)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level - 1; i++)
        cout << "\t";
    if (level)
        cout << "--->";

    if (root->flag == 0)
        cout << "OP(" << root->op << ")" << endl;
    if (root->flag == 2)
        cout << "ID(" << root->variable->name << ")" << endl;
    if (root->flag == 1)
        cout << "NUM(" << root->number->x << ")" << endl;
    travelParseTree(root->left, level + 1);
    travelParseTree(root->right, level + 1);
}

int evalParseTree(TreeNode root)
{
    if (root == NULL)
        return 0;
    if (root->flag == 1)
        return root->number->x;
    if (root->flag == 2)
        return root->variable->value;

    int left_val = evalParseTree(root->left);
    int right_val = evalParseTree(root->right);

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
        else
        {
            cout << "Division by zero error!" << endl;
            return 0;
        }
    }

    return 0;
}
void print_stack(stack<int> &st)
{
    stack<int> temp = st;
    while (!temp.empty())
    {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;
}

int main()
{
    int nexttoken;
    symboltable T = NULL;
    numbertable C = NULL;
    stack<int> st;
    st.push(EXPR);
    TreeNode curr = NULL;
    TreeNode root = NULL;

    while (!st.empty() && (nexttoken = yylex()))
    {
        // cout << yytext << endl;
        // print_stack(st);
        if (st.top() == EXPR && strcmp(yytext, "(") == 0)
        {
            st.pop();
            st.push(CB);
            st.push(ARG);
            st.push(ARG);
            st.push(OP);
        }
        else if (st.top() == ARG && strcmp(yytext, "(") == 0)
        {
            st.pop();
            st.push(CB);
            st.push(ARG);
            st.push(ARG);
            st.push(OP);
        }
        else if ((st.top() == ARG) && (nexttoken == 4 || nexttoken == 5))
        {
            if (nexttoken == 4)
            {
                C = addtoNumberTable(C, atoi(yytext));

                numbertable tmp = C;
                while (tmp)
                {
                    if (tmp->x == atoi(yytext))
                        break;
                    tmp = tmp->next;
                }

                TreeNode node = (TreeNode)malloc(sizeof(struct node3));
                node->left = NULL;
                node->right = NULL;
                node->parent = curr;
                node->flag = 1;

                node->number = tmp;

                if (curr->left == NULL)
                    curr->left = node;
                else
                    curr->right = node;
            }
            if (nexttoken == 5)
            {
                T = addToSymbolTable(T, yytext);

                symboltable tmp = T;
                while (tmp)
                {
                    if (strcmp(tmp->name, yytext) == 0)
                        break;
                    tmp = tmp->next;
                }

                TreeNode node = (TreeNode)malloc(sizeof(struct node3));
                node->left = NULL;
                node->right = NULL;
                node->parent = curr;
                node->flag = 2;

                node->variable = tmp;

                if (curr->left == NULL)
                    curr->left = node;
                else
                    curr->right = node;
            }
            st.pop();
        }
        else if (st.top() == OP && nexttoken == 3)
        {
            TreeNode node = (TreeNode)malloc(sizeof(struct node3));
            node->left = NULL;
            node->right = NULL;
            node->parent = curr;
            node->flag = 0;

            node->op = (char *)malloc((strlen(yytext) + 1) * sizeof(char));
            strcpy(node->op, yytext);

            if (curr == NULL)
                root = node;
            else
            {
                if (curr->left == NULL)
                    curr->left = node;
                else
                    curr->right = node;
            }
            curr = node;
            st.pop();
        }
        else if (st.top() == CB && strcmp(yytext, ")") == 0)
        {
            curr = curr->parent;
            st.pop();
        }
        else
        {
            if (nexttoken == 8)
                cout << "*** Error: Invalid operator " << yytext << " found" << endl;
            else if (st.top() == ARG)
                cout << "*** Error: ID/NUM/LP expected in place of " << yytext << endl;
            else if (st.top() == CB)
                cout << "*** Error: Right parenthesis expected in place of " << yytext << endl;
            else if (st.top() == OP)
                cout << "*** Error: Operator expected in place of " << yytext << endl;
            return 0;
        }
    }

    while ((nexttoken = yylex()))
    {
        symboltable temp = T;
        while (temp)
        {
            if (!temp->val)
            {
                if (nexttoken != 4)
                {
                    cout << "Invalid argument passed (Expected a Number)\n";
                    return 0;
                }
                temp->value = stoi(yytext);
                temp->val = true;
                break;
            }
            temp = temp->next;
        }
    }

    if (T != NULL)
    {
        symboltable tmp = T;
        while (tmp)
        {
            if (!tmp->val)
            {
                cout << "Insufficient number of arguments passed" << endl;
                return 0;
            }
            tmp = tmp->next;
        }
    }
    if (root != NULL)
    {
        cout << "Parsing is successful" << endl;
        travelParseTree(root, 0);
        symboltable temp = T;
        if (temp != NULL)
            cout << "Reading variable values from the input" << endl;
        while (temp)
        {
            cout << temp->name << " = " << temp->value << endl;
            temp = temp->next;
        }
        cout << "The expression evaluates to " << evalParseTree(root) << endl;
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }

    return 0;
}

int yywrap(void)
{
    return 1;
}