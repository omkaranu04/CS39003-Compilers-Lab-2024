#ifndef _TRANSLATOR_H
#define _TRANSLATOR_H

#include <bits/stdc++.h>
using namespace std;

/* sizes of datatypes */
#define size_of_void 0
#define size_of_char 1
#define size_of_int 4
#define size_of_float 4
#define size_of_pointer 4

/* defining all the classes */
class symbol;
class symbolType;
class symbolTable;
class quad;
class quadArray;

/* external variables */
extern symbol *currentSymbol;
extern symbolTable *currentSymbolTable;
extern symbolTable *globalSymbolTable;
extern quadArray quadTable;
extern int SymbolTableCount;
extern string currBlock;
extern int yyparse();
extern char *yytext;

/* defining all the classes */
class symbol
{
public:
    string name;
    symbolType *type;
    string initialVal;
    int size;
    int offset;
    symbolTable *nestedTable;

    symbol(string name_, string type_ = "int", symbolType *arrType = NULL, int width = 0);
    symbol *update(symbolType *t);
};

class symbolType
{
public:
    string base;
    int width;
    symbolType *arrType;

    symbolType(string base_, symbolType *arrType_ = NULL, int width_ = 1);
};

class symbolTable
{
public:
    string name;
    int count;
    list<symbol> table;
    symbolTable *parent;

    symbolTable(string name_ = "NULL");

    symbol *lookup(string name);
    static symbol *generateTmp(symbolType *type_, string initValue_ = "");
    void print();
    void update();
};

class quad
{
public:
    string opcode;
    string argument1;
    string argument2;
    string result;

    quad(string res_, string arg1_, string op_ = "", string arg2_ = "");
    quad(string res_, int arg1_, string op_ = "", string arg2_ = "");
    quad(string res_, float arg1_, string op_ = "", string arg2_ = "");

    void print();
};

class quadArray
{
public:
    vector<quad> array;
    void print();
};

class ArrClass
{
public:
    string arrType;
    symbol *address;
    symbol *loc;
    symbolType *type;
};

class StmtClass
{
public:
    list<int> nextList;
};

class ExprClass
{
public:
    string exprType;
    symbol *address;
    list<int> trueList;
    list<int> falseList;
    list<int> nextList;
};

/* defining all the necessary functions */
void printout(string opcode, string res, string arg1 = "", string arg2 = "");
void printout(string opcode, string res, int arg1, string arg2 = "");
void printout(string opcode, string res, float arg1, string arg2 = "");

list<int> makelist(int i);
list<int> merge(list<int> &p1, list<int> &p2);
void backpatch(list<int> p, int i);
bool typecheck(symbolType *t1, symbolType *t2);
symbol *convertType(symbol *StmtClass, string t);

bool typecheck(symbol *&s1, symbol *&s2);
string convertIntTostr(int n);
string convertFloatToStr(float f);
ExprClass *convertIntToBool(ExprClass *ExprClass);
ExprClass *convertBoolToInt(ExprClass *ExprClass);
void switchTable(symbolTable *newTable);
int nxtInstr();
int sizeOfType(symbolType *t);
string retType(symbolType *t);

#endif