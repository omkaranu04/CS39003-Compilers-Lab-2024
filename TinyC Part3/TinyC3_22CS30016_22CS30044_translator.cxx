#include "TinyC3_22CS30016_22CS30044_translator.h"
#include <bits/stdc++.h>
using namespace std;

// global variables as required are declared here
symbol *currentSymbol;
symbolTable *currentSymbolTable;
symbolTable *globalSymbolTable;
quadArray quadTable;
int SymbolTableCount;
string currBlock;

string data_type;   // last seen data type

// for good printing purposes, left and right padding functions are defined here
string padLeft(int num, int width)
{
    string numStr = to_string(num);
    return string(width - numStr.length(), ' ') + numStr + ": ";
}

string padRight(const string &str, int width)
{
    return str + string(max(0, width - static_cast<int>(str.length())), ' ');
}

// symbolType constructor
symbolType::symbolType(string base_, symbolType *arrType_, int width_) : base(base_), arrType(arrType_), width(width_) {}

// symbol constructor
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

// symbolTable constructor
symbolTable::symbolTable(string name_) : name(name_), count(0), parent(NULL) {}

symbol *symbolTable::lookup(string name)
{
    // start searching from the current symbol table
    for (LinkedList<symbol>::Iterator it = table.begin(); it != table.end(); ++it)
    {
        if ((*it).name == name)
            return &(*it);
    }

    // if not found, move up the hierarchy and search for the symbol
    symbol *StmtClass = NULL;
    if (this->parent != NULL)
    {
        StmtClass = this->parent->lookup(name);
    }
    // if found, return it
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
    // create a new temporary symbol
    string name = "t" + to_string(currentSymbolTable->count++);
    symbol *sym = new symbol(name);
    sym->type = t;
    sym->initialVal = initialVal;
    sym->size = sizeOfType(t);

    // add the symbol to the current symbol table
    currentSymbolTable->table.push_back(*sym);
    return &(currentSymbolTable->table.back());
}

void symbolTable::print()
{
    // printing the symbol table essentials
    cout << string(100, '-') << endl;
    cout << "SymbolTable : " << padRight(this->name, 50);
    cout << "ParentTable : " << padRight((this->parent == nullptr) ? "NULL" : this->parent->name, 50) << endl;
    cout << string(100, '-') << endl;

    cout << padRight("Name", 25);
    cout << padRight("Type", 25);
    cout << padRight("initialVal", 20);
    cout << padRight("Size", 15);
    cout << padRight("Offset", 15);
    cout << "NestedTable" << endl;

    cout << string(100, '-') << endl;

    LinkedList<symbolTable *> tableList;

    for (LinkedList<symbol>::Iterator it = this->table.begin(); it != this->table.end(); ++it)
    {
        cout << padRight((*it).name, 25);
        cout << padRight(retType((*it).type), 25);
        cout << padRight(((*it).initialVal != "" ? (*it).initialVal : "-"), 20);
        cout << padRight(to_string((*it).size), 15);
        cout << padRight(to_string((*it).offset), 15);

        if ((*it).nestedTable != nullptr)
        {
            cout << (*it).nestedTable->name << endl;
            tableList.push_back((*it).nestedTable);
        }
        else
        {
            cout << "NULL" << endl;
        }
    }

    cout << string(120, '-') << endl;

    for (LinkedList<symbolTable *>::Iterator it = tableList.begin(); it != tableList.end(); ++it)
    {
        (*it)->print();
    }
}
void symbolTable::update()
{
    LinkedList<symbolTable *> tableList;
    int curr_offset;

    // update the offsets based on their sizes
    for (LinkedList<symbol>::Iterator it = table.begin(); it != table.end(); ++it)
    {
        if (it == table.begin())
        {
            (*it).offset = 0;
            curr_offset = (*it).size;
        }
        else
        {
            (*it).offset = curr_offset;
            curr_offset = (*it).offset + (*it).size;
        }
        if ((*it).nestedTable != NULL)
        {
            tableList.push_back((*it).nestedTable);
        }

        // recursively update the nested symbol tables
        for (LinkedList<symbolTable *>::Iterator iter = tableList.begin(); iter != tableList.end(); ++iter)
        {
            (*iter)->update();
        }
    }
}

// quad class constructor
quad::quad(string res, string arg1_, string operation, string arg2_) : result(res), argument1(arg1_), argument2(arg2_), opcode(operation) {}

quad::quad(string res, int arg1_, string operation, string arg2_) : result(res), argument2(arg2_), opcode(operation)
{
    argument1 = to_string(arg1_);
}
quad::quad(string res, float arg1_, string operation, string arg2_) : result(res), argument2(arg2_), opcode(operation)
{
    argument1 = to_string(arg1_);
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

// quadArray constructor
void quadArray::print()
{
    cout << string(120, '-') << endl
         << endl;

    int cnt = 0;
    for (auto it = this->array.begin(); it != this->array.end(); ++it, ++cnt)
    {
        cout << padRight(to_string(cnt) + ":", 6);
        if (it->opcode != "label")
        {
            cout << "    ";
        }
        it->print();
        cout << endl;
    }
}

// for printing in the output file
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

// for custom made linked list, merge, and make functions
LinkedList<int> makelist(int i)
{
    LinkedList<int> l;
    l.push_back(i);
    return l;
}
LinkedList<int> customMerge(LinkedList<int> &p1, LinkedList<int> &p2)
{
    LinkedList<int> result = p1;
    result.merge(p2);
    return result;
}

void backpatch(LinkedList<int> l, int address)
{
    string str = to_string(address);
    for (LinkedList<int>::Iterator it = l.begin(); it != l.end(); ++it)
    {
        quadTable.array[*it].result = str;
    }
}

bool typecheck(symbol *&s1, symbol *&s2)
{
    if (s1 == convertType(s1, s2->type->base))
        return true;
    else if (typecheck(s1->type, s2->type))
        return true;
    else if (s2 == convertType(s2, s1->type->base))
        return true;
    else
        return false;
}

bool typecheck(symbolType *t1, symbolType *t2)
{
    if (t1 == NULL && t2 == NULL)
    {
        return true;
    }
    else if (t1 == NULL || t2 == NULL)
        return false;
    else if (t1->base != t2->base)
        return false;

    return typecheck(t1->arrType, t2->arrType);
}

symbol *convertType(symbol *StmtClass, string t)
{
    symbol *temp = symbolTable::generateTmp(new symbolType(t));
    const string &source = StmtClass->type->base;

    if (source == "float")
    {
        if (t == "int")
        {
            quad *q = new quad("=", temp->name, "floatToint(" + StmtClass->name + ")");
            quadTable.array.push_back(*q);
            return temp;
        }
        else if (t == "char")
        {
            quad *q = new quad("=", temp->name, "floatTochar(" + StmtClass->name + ")");
            quadTable.array.push_back(*q);
            return temp;
        }
        else
            return StmtClass;
    }
    else if (source == "int")
    {
        if (t == "float")
        {
            quad *q = new quad("=", temp->name, "intTofloat(" + StmtClass->name + ")");
            quadTable.array.push_back(*q);
            return temp;
        }
        else if (t == "char")
        {
            quad *q = new quad("=", temp->name, "intTochar(" + StmtClass->name + ")");
            quadTable.array.push_back(*q);
            return temp;
        }
        else
            return StmtClass;
    }
    else if (source == "char")
    {
        if (t == "float")
        {
            quad *q = new quad("=", temp->name, "charTofloat(" + StmtClass->name + ")");
            quadTable.array.push_back(*q);
            return temp;
        }
        else if (t == "int")
        {
            quad *q = new quad("=", temp->name, "charToint(" + StmtClass->name + ")");
            quadTable.array.push_back(*q);
            return temp;
        }
        else
            return StmtClass;
    }
    else
        return StmtClass;
}

ExprClass *convertIntToBool(ExprClass *expr)
{
    if (expr->exprType != "BOOL")
    {
        expr->falseList = makelist(quadTable.array.size());
        quad *q = new quad("==", expr->address->name, "0");
        quadTable.array.push_back(*q);
        expr->trueList = makelist(quadTable.array.size());
        quad *qx = new quad("goto", "");
        quadTable.array.push_back(*qx);
    }
    return expr;
}

ExprClass *convertBoolToInt(ExprClass *expr)
{
    if (expr->exprType == "BOOL")
    {
        expr->address = symbolTable::generateTmp(new symbolType("int"));
        backpatch(expr->trueList, quadTable.array.size());
        quad *q = new quad("=", expr->address->name, "true");
        quadTable.array.push_back(*q);
        quad *qx = new quad("goto", to_string(quadTable.array.size() + 1));
        quadTable.array.push_back(*qx);
        backpatch(expr->falseList, quadTable.array.size());
        quad *qy = new quad("=", expr->address->name, "false");
        quadTable.array.push_back(*qy);
    }
    return expr;
}

enum BaseType
{
    VOID,
    CHAR,
    INT,
    PTR,
    FLOAT,
    BLOCK,
    ARR,
    FUNC,
    UNKNOWN
};
BaseType stringToBaseType(const std::string &base)
{
    if (base == "void")
        return VOID;
    if (base == "char")
        return CHAR;
    if (base == "int")
        return INT;
    if (base == "ptr")
        return PTR;
    if (base == "float")
        return FLOAT;
    if (base == "arr")
        return ARR;
    if (base == "func")
        return FUNC;
    if (base == "block")
        return BLOCK;
    return UNKNOWN;
}
int sizeOfType(symbolType *t)
{
    switch (stringToBaseType(t->base))
    {
    case VOID:
        return size_of_void;
    case CHAR:
        return size_of_char;
    case INT:
        return size_of_int;
    case PTR:
        return size_of_pointer;
    case FLOAT:
        return size_of_float;
    case ARR:
        return t->width * sizeOfType(t->arrType);
    case FUNC:
        return 0;
    default:
        return -1;
    }
}

string retType(symbolType *t)
{
    if (t == NULL)
        return "NULL";

    switch (stringToBaseType(t->base))
    {
    case VOID:
    case CHAR:
    case INT:
    case FLOAT:
    case BLOCK:
    case FUNC:
        return t->base;
    case PTR:
        return "ptr(" + retType(t->arrType) + ")";
    case ARR:
        return "arr(" + std::to_string(t->width) + ", " + retType(t->arrType) + ")";
    default:
        return "Unknown symbolType";
    }
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