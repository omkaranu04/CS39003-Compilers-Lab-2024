#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
int mem_pointer = 0;
int available0 = 1;
int reg_used[12] = {0};
int for_dollar = 1;
struct node
{
    char *name;
    int offset;
    struct node *next;
};
typedef struct node *symboltable;
symboltable T = NULL;
struct node2
{
    int typeID;
    int typeNUM;
    int typeEXPR;
    int id; // here now id will store my offset value or reg value depending on the type
    int num;
    int expr;
};
typedef struct node2 *type_of_arg;

int return_first_available_reg()
{
    for (int i = 2; i < 12; i++)
    {
        if (!reg_used[i])
        {
            reg_used[i] = 1;
            return i;
        }
    }
    return -1;
}
void free_regs(int reg_index)
{
    reg_used[reg_index] = 0;
}
int add_to_symboltable(char *id)
{
    if (T == NULL)
    {
        symboltable temp = (symboltable)malloc(sizeof(struct node));
        temp->name = strdup(id);
        temp->offset = mem_pointer;
        temp->next = NULL;
        T = temp;
        mem_pointer++;
        return temp->offset;
    }
    symboltable temp = T;
    while (temp->next != NULL)
    {
        if (!strcmp(temp->name, id))
        {
            return temp->offset;
        }
        temp = temp->next;
    }
    if (!strcmp(temp->name, id))
    {
        return temp->offset;
    }
    symboltable new = (symboltable)malloc(sizeof(struct node));
    new->name = strdup(id);
    new->offset = mem_pointer;
    new->next = NULL;
    temp->next = new;
    mem_pointer++;
    // printf("\t\t // new offset is %d\n", new->offset);
    return new->offset;
}

void handle_set_id_num(char *id, int num)
{
    int offset = add_to_symboltable(id);
    printf("\tMEM[%d] = %d;\n", offset, num);
    printf("\tmprn(MEM, %d);\n", offset);
}
void handle_set_id_id(char *id1, char *id2)
{
    int offset1 = add_to_symboltable(id1);
    int offset2 = add_to_symboltable(id2);
    printf("\tR[0] = MEM[%d];\n", offset2);
    printf("\tMEM[%d] = R[0];\n", offset1);
    printf("\tmprn(MEM, %d);\n", offset1);
}
void handle_set_id_expr(char *id, type_of_arg ex)
{
    int offset = add_to_symboltable(id);
    printf("\tMEM[%d] = R[%d];\n", offset, ex->expr);
    free_regs(ex->expr);
    printf("\tmprn(MEM, %d);\n", offset);
}
void handle_expr_stmt(type_of_arg ex)
{
    printf("\teprn(R, %d);\n", ex->expr);
}
type_of_arg handle_num(int num)
{
    type_of_arg temp = (type_of_arg)malloc(sizeof(struct node2));
    temp->typeID = 0;
    temp->typeNUM = 1;
    temp->typeEXPR = 0;
    temp->num = num;
    return temp;
}
type_of_arg handle_id(char *id)
{
    type_of_arg temp = (type_of_arg)malloc(sizeof(struct node2));
    temp->typeID = 1;
    temp->typeNUM = 0;
    temp->typeEXPR = 0;
    temp->id = add_to_symboltable(id);
    return temp;
}
type_of_arg handle_expr(type_of_arg ex)
{
    type_of_arg temp = (type_of_arg)malloc(sizeof(struct node2));
    temp->typeID = ex->typeID;
    temp->typeNUM = ex->typeNUM;
    temp->typeEXPR = ex->typeEXPR;
    temp->expr = ex->expr;
    temp->id = ex->id;
    return temp;
}
void print_accn_to_arg_type(type_of_arg arg, int powend)
{
    if (arg->typeNUM)
    {
        printf("%d", arg->num);
        if (powend)
            printf(")");
    }
    else if (arg->typeEXPR)
    {
        printf("R[%d]", arg->expr);
        free_regs(arg->expr);
    }
}
type_of_arg handle_op(int op, type_of_arg arg1, type_of_arg arg2)
{
    int reg_available = return_first_available_reg();
    // printf("// first free register is %d\n", reg_available);
    type_of_arg temp = (type_of_arg)malloc(sizeof(struct node2));
    if (reg_available == -1)
    {
        reg_available = 0;
    }
    // register is available
    int arg1print = 0, arg2print = 0;
    int toprint10 = 0, toprint20 = 0;
    if (arg1->typeID && arg2->typeID)
    {
        // printf("\t\t // arg1 = %d, arg2 = %d\n", arg1->id, arg2->id);
        printf("\tR[0] = MEM[%d];\n", arg1->id);
        printf("\tR[1] = MEM[%d];\n", arg2->id);
        arg1print = 1;
        arg2print = 1;
    }
    else if (arg1->typeID && !arg2->typeID)
    {
        printf("\tR[0] = MEM[%d];\n", arg1->id);
        arg1print = 1;
    }
    else if (!arg1->typeID && arg2->typeID)
    {
        printf("\tR[0] = MEM[%d];\n", arg2->id);
        arg2print = 1;
    }
    printf("\tR[%d] = ", reg_available);
    if (op == POW)
    {
        printf("pwr(");
    }
    if (arg1print)
    {
        printf("R[0]");
        if (op == POW)
        {
            printf(",");
        }
    }
    else
    {
        print_accn_to_arg_type(arg1, 0);
    }
    switch (op)
    {
    case ADD:
        printf(" + ");
        break;
    case SUB:
        printf(" - ");
        break;
    case MUL:
        printf(" * ");
        break;
    case DIV:
        printf(" / ");
        break;
    case MOD:
        printf(" %% ");
        break;
    }
    if (arg2print)
    {
        if (arg1print)
            printf("R[1]");
        else
            printf("R[0]");
        if (op == POW)
        {
            printf(")");
        }
    }
    else
    {
        if (op == POW)
            print_accn_to_arg_type(arg2, 1);
        else
            print_accn_to_arg_type(arg2, 0);
    }
    printf(";\n");
    if (reg_available == 0)
    {
        // printf("\t\t I am here because registers are out\n");
        char buffer[10];
        sprintf(buffer, "$%d", for_dollar);
        int offset = add_to_symboltable(buffer);
        printf("\tMEM[%d] = R[0];\n", offset);
        for_dollar++;
        // printf("fordollar %d\n", for_dollar);
        temp->typeID = 1;
        temp->typeNUM = 0;
        temp->typeEXPR = 0;
        temp->id = offset;
        // printf("\t\t // setted offset temp->id is %d\n", temp->id);
    }
    else
    {
        temp->typeEXPR = 1;
        temp->typeID = 0;
        temp->typeNUM = 0;
        temp->expr = reg_available;
    }
    return temp;
}
int main(int argc, char const *argv[])
{
    printf("#include <stdio.h>\n");
    printf("#include <stdlib.h>\n");
    printf("#include\"auxy.c\"\n\n");
    printf("int main()\n");
    printf("{\n");
    printf("\tint R[12];\n");
    printf("\tint MEM[65536];\n\n");

    yyparse();

    printf("\n\treturn 0;\n");
    printf("}\n");
    return 0;
}