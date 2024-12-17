#include "TinyC3_22CS30016_22CS30044_translator.h"
#include <bits/stdc++.h>
using namespace std;

symbol *currentSymbol;
symbolTable *currentSymbolTable;
symbolTable *globalSymbolTable;
quadArray quadTable;
int SymbolTableCount;
string currBlock;

string data_type;

symbolType::symbolType(string base_, symbolType *arrType_, int width_) : base(base_), arrType(arrType_), width(width_) {}

symbol::symbol(string name_, string type_, symbolType *arrType, int width) : name(name_), initialVal("-"), offset(0), nestedTable(NULL)
{
    type = new symbolType(type_, arrType, width);
    size = sizeOfType(type);
}

symbol *symbol::update(symbolType *t)
{
    type = t;
    size = sizeOfType(t);
    return this;
}

symbolTable::symbolTable(string name_) : name(name_), count(0), parent(NULL) {}

symbol *symbolTable::lookup(string name)
{
    for (list<symbol>::iterator it = table.begin(); it != table.end(); it++)
    {
        if (it->name == name)
            return &(*it);
    }

    symbol *StmtClass = NULL;
    if (this->parent != NULL)
    {
        StmtClass = this->parent->lookup(name);
    }

    if (currentSymbolTable == this && StmtClass == NULL)
    {
        symbol *sym = new symbol(name);
        table.push_back(*sym);
        return &(table.back());
    }
    else if (StmtClass != NULL)
    {
        return StmtClass;
    }
    return NULL;
}

symbol *symbolTable::generateTmp(symbolType *t, string initialVal)
{
    string name = "t" + convertIntTostr(currentSymbolTable->count++);
    symbol *sym = new symbol(name);
    sym->type = t;
    sym->initialVal = initialVal;
    sym->size = sizeOfType(t);

    currentSymbolTable->table.push_back(*sym);
    return &(currentSymbolTable->table.back());
}

void symbolTable::print()
{
    for (int i = 0; i < 100; i++)
    {
        cout << "-";
    }
    cout << endl;
    cout << "SymbolTable : " << setfill(' ') << left << setw(50) << this->name;
    cout << "ParentTable : " << setfill(' ') << left << setw(50) << ((this->parent == NULL) ? "NULL" : this->parent->name) << endl;
    for (int i = 0; i < 100; i++)
    {
        cout << "-";
    }
    cout << endl;

    cout << setfill(' ') << left << setw(25) << "Name";
    cout << left << setw(25) << "Type";
    cout << left << setw(20) << "initialVal";
    cout << left << setw(15) << "Size";
    cout << left << setw(15) << "Offset";
    cout << left << "NestedTable" << endl;

    for (int i = 0; i < 100; i++)
    {
        cout << '-';
    }
    cout << endl;

    list<symbolTable *> tableList;

    for (list<symbol>::iterator it = this->table.begin(); it != this->table.end(); it++)
    {
        cout << left << setw(25) << it->name;
        cout << left << setw(25) << retType(it->type);
        cout << left << setw(20) << (it->initialVal != "" ? it->initialVal : "-");
        cout << left << setw(15) << it->size;
        cout << left << setw(15) << it->offset;
        cout << left;

        if (it->nestedTable != NULL)
        {
            cout << it->nestedTable->name << endl;
            tableList.push_back(it->nestedTable);
        }
        else
        {
            cout << "NULL" << endl;
        }
    }

    for (int i = 0; i < 120; i++)
    {
        cout << '-';
    }
    cout << endl;

    for (list<symbolTable *>::iterator it = tableList.begin(); it != tableList.end(); it++)
    {
        (*it)->print();
    }
}
void symbolTable::update()
{
    list<symbolTable *> tableList;
    int off_set;

    for (list<symbol>::iterator it = table.begin(); it != table.end(); it++)
    {
        if (it == table.begin())
        {
            it->offset = 0;
            off_set = it->size;
        }
        else
        {
            it->offset = off_set;
            off_set = it->offset + it->size;
        }
        if (it->nestedTable != NULL)
        {
            tableList.push_back(it->nestedTable);
        }

        for (list<symbolTable *>::iterator iter = tableList.begin(); iter != tableList.end(); iter++)
        {
            (*iter)->update();
        }
    }
}

quad::quad(string res, string arg1_, string operation, string arg2_) : result(res), argument1(arg1_), argument2(arg2_), opcode(operation) {}

quad::quad(string res, int arg1_, string operation, string arg2_) : result(res), argument2(arg2_), opcode(operation)
{
    argument1 = convertIntTostr(arg1_);
}
quad::quad(string res, float arg1_, string operation, string arg2_) : result(res), argument2(arg2_), opcode(operation)
{
    argument1 = convertFloatToStr(arg1_);
}

void quad::print()
{
    if (opcode == "=")
        cout << result << " = " << argument1;
    else if (opcode == "*=")
        cout << "*" << result << " = " << argument1;
    else if (opcode == "[]=")
        cout << result << "[" << argument1 << "] = " << argument2;
    else if (opcode == "=[]")
        cout << result << " = " << argument1 << "[" << argument2 << "]";
    else if (opcode == "call")
        cout << result << " = call " << argument1 << ", " << argument2;
    else if (opcode == "label")
        cout << result << ": ";
    else if (opcode == "+" || opcode == "-" || opcode == "*" || opcode == "/" || opcode == "%" || opcode == "^" || opcode == "|" || opcode == "&" || opcode == "<<" || opcode == ">>")
        cout << result << " = " << argument1 << " " << opcode << " " << argument2;
    else if (opcode == "==" || opcode == "!=" || opcode == "<" || opcode == ">" || opcode == "<=" || opcode == ">=")
        cout << "if " << argument1 << " " << opcode << " " << argument2 << " goto " << result;
    else if (opcode == "= &" || opcode == "= *" || opcode == "= -" || opcode == "= ~" || opcode == "= !")
        cout << result << " " << opcode << argument1;
    else if (opcode == "goto" || opcode == "param" || opcode == "return")
        cout << opcode << " " << result;
    else
        cout << "Unknown Operator";
}

void quadArray::print()
{
    for (int i = 0; i < 120; i++)
    {
        cout << "-";
    }
    cout << endl;
    int cnt = 0;
    for (vector<quad>::iterator it = this->array.begin(); it != this->array.end(); it++, cnt++)
    {
        if (it->opcode != "label")
        {
            cout << left << setw(4) << cnt << ":    ";
            it->print();
        }
        else
        {
            cout << endl
                 << left << setw(4) << cnt << ": ";
            it->print();
        }
        cout << endl;
    }
}

void printout(string op, string result, string argument1, string argument2)
{
    quad *q = new quad(result, argument1, op, argument2);
    quadTable.array.push_back(*q);
}
void printout(string op, string result, int argument1, string argument2)
{
    quad *q = new quad(result, argument1, op, argument2);
    quadTable.array.push_back(*q);
}
void printout(string op, string result, float argument1, string argument2)
{
    quad *q = new quad(result, argument1, op, argument2);
    quadTable.array.push_back(*q);
}

list<int> makelist(int i)
{
    list<int> l(1, i);
    return l;
}
list<int> merge(list<int> &ArrClass, list<int> &b)
{
    ArrClass.merge(b);
    return ArrClass;
}

void backpatch(list<int> l, int address)
{
    string str = convertIntTostr(address);
    for (list<int>::iterator it = l.begin(); it != l.end(); it++)
    {
        quadTable.array[*it].result = str;
    }
}

bool typecheck(symbol *&s1, symbol *&s2)
{
    symbolType *t1 = s1->type;
    symbolType *t2 = s2->type;

    if (typecheck(t1, t2))
        return true;
    else if (s1 == convertType(s1, t2->base))
        return true;
    else if (s2 == convertType(s2, t1->base))
        return true;
    else
        return false;
}

bool typecheck(symbolType *t1, symbolType *t2)
{
    if (t1 == NULL && t2 == NULL)
        return true;
    else if (t1 == NULL || t2 == NULL)
        return false;
    else if (t1->base != t2->base)
        return false;

    return typecheck(t1->arrType, t2->arrType);
}

symbol *convertType(symbol *StmtClass, string t)
{
    symbol *temp = symbolTable::generateTmp(new symbolType(t));

    if (StmtClass->type->base == "float")
    {
        if (t == "int")
        {
            printout("=", temp->name, "floatToint(" + StmtClass->name + ")");
            return temp;
        }
        else if (t == "char")
        {
            printout("=", temp->name, "floatTochar(" + StmtClass->name + ")");
            return temp;
        }
        return StmtClass;
    }
    else if (StmtClass->type->base == "int")
    {
        if (t == "float")
        {
            printout("=", temp->name, "intTofloat(" + StmtClass->name + ")");
            return temp;
        }
        else if (t == "char")
        {
            printout("=", temp->name, "intTochar(" + StmtClass->name + ")");
            return temp;
        }
        return StmtClass;
    }
    else if (StmtClass->type->base == "char")
    {
        if (t == "float")
        {
            printout("=", temp->name, "charTofloat(" + StmtClass->name + ")");
            return temp;
        }
        else if (t == "int")
        {
            printout("=", temp->name, "charToint(" + StmtClass->name + ")");
            return temp;
        }
        return StmtClass;
    }

    return StmtClass;
}

string convertIntTostr(int i)
{
    return to_string(i);
}

string convertFloatToStr(float f)
{
    return to_string(f);
}

ExprClass *convertIntToBool(ExprClass *expr)
{
    if (expr->exprType != "BOOL")
    {
        expr->falseList = makelist(nxtInstr());
        printout("==", expr->address->name, "0");
        expr->trueList = makelist(nxtInstr());
        printout("goto", "");
    }
    return expr;
}

ExprClass *convertBoolToInt(ExprClass *expr)
{
    if (expr->exprType == "BOOL")
    {
        expr->address = symbolTable::generateTmp(new symbolType("int"));
        backpatch(expr->trueList, nxtInstr());
        printout("=", expr->address->name, "true");
        printout("goto", convertIntTostr(nxtInstr() + 1));
        backpatch(expr->falseList, nxtInstr());
        printout("=", expr->address->name, "false");
    }
    return expr;
}

void switchTable(symbolTable *newTable)
{
    currentSymbolTable = newTable;
}

int nxtInstr()
{
    return quadTable.array.size();
}

int sizeOfType(symbolType *t)
{
    if (t->base == "void")
        return size_of_void;
    else if (t->base == "char")
        return size_of_char;
    else if (t->base == "int")
        return size_of_int;
    else if (t->base == "ptr")
        return size_of_pointer;
    else if (t->base == "float")
        return size_of_float;
    else if (t->base == "arr")
        return t->width * sizeOfType(t->arrType);
    else if (t->base == "func")
        return 0;
    else
        return -1;
}

string retType(symbolType *t)
{
    if (t == NULL)
        return "NULL";
    else if (t->base == "void" || t->base == "char" || t->base == "int" || t->base == "float" || t->base == "block" || t->base == "func")
        return t->base;
    else if (t->base == "ptr")
        return "ptr(" + retType(t->arrType) + ")";
    else if (t->base == "arr")
        return "arr(" + convertIntTostr(t->width) + ", " + retType(t->arrType) + ")";
    else
        return "Unknown symbolType";
}

int main(int argc, char const *argv[])
{
    SymbolTableCount = 0;
    globalSymbolTable = new symbolTable("Global");
    currentSymbolTable = globalSymbolTable;
    currBlock = "";

    yyparse();

    globalSymbolTable->update();
    quadTable.print();
    cout << "\n";
    globalSymbolTable->print();

    return 0;
}