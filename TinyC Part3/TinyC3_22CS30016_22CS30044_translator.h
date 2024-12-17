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

// custom linked list for handling all the list type of things
template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &value) : data(value), next(nullptr) {}
    };

    Node *head;
    int size;

public:
    class Iterator
    {
    private:
        Node *current;

    public:
        Iterator(Node *node) : current(node) {}

        Iterator &operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        T &operator*() { return current->data; }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }

        Node *getNode() const { return current; }
        bool operator==(const Iterator &other) const
        {
            return this->current == other.current;
        }
    };

    LinkedList() : head(nullptr), size(0) {}

    LinkedList(const LinkedList &other) : head(nullptr), size(0)
    {
        Node *current = other.head;
        while (current != nullptr)
        {
            push_back(current->data);
            current = current->next;
        }
    }

    LinkedList &operator=(const LinkedList &other)
    {
        if (this == &other)
            return *this;

        clear();
        Node *current = other.head;
        while (current != nullptr)
        {
            push_back(current->data);
            current = current->next;
        }
        return *this;
    }

    ~LinkedList()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }

    void push_back(const T &value)
    {
        Node *newNode = new Node(value);
        if (!head)
        {
            head = newNode;
        }
        else
        {
            Node *current = head;
            while (current->next)
            {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    T &back()
    {
        if (!head)
            throw std::out_of_range("List is empty");
        Node *current = head;
        while (current->next)
        {
            current = current->next;
        }
        return current->data;
    }

    void merge(LinkedList<T> &other)
    {
        if (!head)
        {
            head = other.head;
        }
        else
        {
            Node *current = head;
            while (current->next)
            {
                current = current->next;
            }
            current->next = other.head;
        }
        size += other.size;
        other.head = nullptr;
        other.size = 0;
    }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    bool empty() const { return head == nullptr; }

    void clear()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }
};

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
    LinkedList<symbol> table;
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
    LinkedList<int> nextList;
};

class ExprClass
{
public:
    string exprType;
    symbol *address;
    LinkedList<int> trueList;
    LinkedList<int> falseList;
    LinkedList<int> nextList;
};

/* defining all the necessary functions */
void printout(string opcode, string res, string arg1 = "", string arg2 = "");
void printout(string opcode, string res, int arg1, string arg2 = "");
void printout(string opcode, string res, float arg1, string arg2 = "");

LinkedList<int> makelist(int i);
LinkedList<int> customMerge(LinkedList<int> &p1, LinkedList<int> &p2);
void backpatch(LinkedList<int> p, int i);
bool typecheck(symbolType *t1, symbolType *t2);
symbol *convertType(symbol *StmtClass, string t);

bool typecheck(symbol *&s1, symbol *&s2);
ExprClass *convertIntToBool(ExprClass *ExprClass);
ExprClass *convertBoolToInt(ExprClass *ExprClass);
int sizeOfType(symbolType *t);
string retType(symbolType *t);

#endif