#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;
#define MAXXX 5000

int yylex();
void yyerror(string s);
extern int yyparse();

// defining all the required structs and variables
struct quad
{
    string op;
    string arg1;
    string arg2;
    string result;
    int alt_ins;
};

struct symbol
{
    string name;
    int regno;
    int is_temp;
    int is_const;
    int is_mem;
    struct symbol *next;
};

struct symtab
{
    symbol *head;
    struct symtab *next;
};

struct RegisterBank
{
    symtab *symbols;
};

// vector to define the quads for the intermediate code
vector<quad> quads(MAXXX);
// vector to define the quads for the assembly code
vector<quad> quads2(MAXXX);
// leaders of the blocks
vector<int> leaders(MAXXX);
// register bank
vector<RegisterBank> reg_bank(MAXXX);
// global symbol table
symbol *SymbolTable;
// additonal required variables
int temp_count, next_instr, ass_instr, quad_cnt, reg_nos;

// function to add new symbol, and declare its type, if is a constant or temporary
symbol *new_Symbol(const string &name, int is_temp, int is_const)
{
    symbol *tmp = new symbol;
    tmp->name = name;
    tmp->regno = -1;
    tmp->is_temp = is_temp;
    tmp->is_const = is_const;
    tmp->is_mem = 1;
    tmp->next = nullptr;
    return tmp;
}

// function to generate the temporary variables
symbol *gen_temp()
{
    string temp = "$" + to_string(temp_count++);
    symbol *t = new_Symbol(temp, 1, 0);
    if (SymbolTable == NULL)
    {
        SymbolTable = t;
        return t;
    }
    symbol *prev = SymbolTable;
    while (prev->next != NULL)
    {
        prev = prev->next;
    }
    prev->next = t;
    return t;
}
// searching for the new symbol, if not available adding, if not initialised, then initialising the table
symbol *symbol_search(string name, int is_temp, int is_const)
{
    if (SymbolTable == NULL)
    {
        SymbolTable = new_Symbol(name, is_temp, is_const);
        return SymbolTable;
    }
    symbol *temp = SymbolTable;
    symbol *prev = NULL;

    while (temp != NULL)
    {
        if (temp->name == name)
            return temp;
        prev = temp;
        temp = temp->next;
    }
    symbol *t = new_Symbol(name, is_temp, is_const);
    prev->next = t;
    return t;
}

// to fill in the quads table, both for intermediate and assembly code, signaled by the assembly flag
void emit(const string &op, const string &arg1, const string &arg2, const string &result, int assembly)
{
    if (assembly == 0)
    {
        quads[next_instr].op = op;
        quads[next_instr].arg1 = arg1;
        quads[next_instr].arg2 = arg2;
        quads[next_instr].result = result;
        next_instr++;
    }
    else
    {
        quads2[ass_instr].op = op;
        quads2[ass_instr].arg1 = arg1;
        quads2[ass_instr].arg2 = arg2;
        quads2[ass_instr].result = result;
        quads2[ass_instr].alt_ins = quad_cnt;
        ass_instr++;
    }
}

// print the quads in the intermediate code
void print_quads()
{
    int block = 1;
    for (int i = 1; i < next_instr; i++)
    {
        if (leaders[i] != 0)
        {
            if (i != 1)
            {
                cout << "\n";
            }
            cout << "Block " << block++ << "\n";
        }
        cout << "  ";

        map<string, int> types{
            {"=", 0},
            {"gt", 1},
            {"if", 2}};

        switch (types[quads[i].op])
        {
        case 0:
            if (quads[i].op == "=")
            {
                cout << i << " :\t " << quads[i].result << " = " << quads[i].arg1 << "\n";
            }
            else
            {
                cout << i << " :\t " << quads[i].result << " = " << quads[i].arg1 << " " << quads[i].op << " " << quads[i].arg2 << "\n";
            }
            continue;
            break;
        case 1:
            cout << i << " :\t goto " << quads[i].result << "\n";
            continue;
            break;
        case 2:
            cout << i << " :\t iffalse " << "(" << quads[i].arg1 << ")" << " goto " << quads[i].result << "\n";
            continue;
            break;
        }
        cout << i << " :\t " << quads[i].result << " = " << quads[i].arg1 << " " << quads[i].op << " " << quads[i].arg2 << "\n";
    }
    cout << "\n"
         << "  " << next_instr << " :";
}

// spill function will be used to store the values of the registers in the memory, covered in the cases where registers are full
void spill(int reg, int forced)
{
    symtab *tmp = reg_bank[reg].symbols;
    symtab *prev = NULL;
    while (tmp != NULL)
    {
        symbol *sym = tmp->head;
        if (sym->is_const == 0 && sym->is_mem == 0 && (forced == 1 || sym->is_temp == 0))
        {
            string s = "R" + to_string(reg);
            emit("ST", s, "", sym->name, 1);
        }
        sym->regno = -1;
        sym->is_mem = 1;
        prev = tmp;
        tmp = tmp->next;
        delete prev;
    }
    reg_bank[reg].symbols = NULL;
}

// remove the symbol from register descriptor
void remove_symbol(symbol *sym)
{
    symtab *temp = reg_bank[sym->regno].symbols;
    if (temp->head == sym)
    {
        reg_bank[sym->regno].symbols = temp->next;
        delete temp;
        return;
    }
    symtab *prev = NULL;
    while (temp != NULL)
    {
        if (temp->head == sym)
        {
            prev->next = temp->next;
            delete temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

// add symbol to the register descriptor
void add_symbol(int reg, symbol *sym)
{
    symtab *temp = reg_bank[reg].symbols;
    if (sym->regno != -1)
    {
        remove_symbol(sym);
    }

    if (temp == NULL)
    {
        symtab *tempo = new symtab;
        tempo->head = sym;
        tempo->next = NULL;
        reg_bank[reg].symbols = tempo;
        sym->regno = reg;
        return;
    }

    if (temp->head == sym)
    {
        return;
    }

    while (temp->next != NULL)
    {
        temp = temp->next;
        if (temp->head == sym)
        {
            return;
        }
    }

    symtab *tempo = new symtab;
    tempo->head = sym;
    tempo->next = NULL;
    temp->next = tempo;
    sym->regno = reg;
}

// deciding for which register to used based on the "score" of the register, the one with min. score will be used
int min_score()
{
    int min_score = INT16_MAX;
    int ret_val = -1;
    for (int i = 1; i <= reg_nos; i++)
    {
        int in_mem = 0, in_temp = 0;
        symtab *temp = reg_bank[i].symbols;
        while (temp != NULL)
        {
            if (!temp->head->is_mem)
                in_mem++;
            if (temp->head->is_temp)
                in_temp++;
            temp = temp->next;
        }
        if (in_mem + 5 * in_temp < min_score)
        {
            min_score = in_mem + 5 * in_temp;
            ret_val = i;
        }
    }
    return ret_val;
}

// allocate the register to the symbol, if the symbol is not in the register and register is free
int allocate_free_register(symbol *sym, int reg_index, int is_res)
{
    if (is_res == 1)
    {
        sym->is_mem = 0;
    }
    else
    {
        string tmp = "R" + to_string(reg_index);
        emit("LD", sym->name, "", tmp, 1);
    }
    add_symbol(reg_index, sym);
    return reg_index;
}

// handle the existing register, if the symbol is already in the register
int handle_existing_register(symbol *sym, int is_res)
{
    if (reg_bank[sym->regno].symbols->next == NULL || is_res == 0)
    {
        if (is_res == 1)
        {
            sym->is_mem = 0;
        }
        return sym->regno;
    }
    return -1;
}

// returing the register to be used for the symbol, handling all the cases within here with the help of other 2 functions
int get_reg(symbol *sym, int is_res)
{
    if (sym->regno != -1)
    {
        int reg = handle_existing_register(sym, is_res);
        if (reg != -1)
            return reg;
    }

    for (int i = 1; i <= reg_nos; i++)
    {
        if (reg_bank[i].symbols == NULL)
        {
            return allocate_free_register(sym, i, is_res);
        }
    }

    int min_reg = min_score();
    spill(min_reg, 1);
    return allocate_free_register(sym, min_reg, is_res);
}

// get the name of the symbol, if it is a constant, return the name, else return the register name
string get_name(const string &name)
{
    symbol *sym = symbol_search(name, 0, 0);
    if (sym->is_const)
    {
        return sym->name;
    }
    int regallot = get_reg(sym, 0);
    return "R" + to_string(regallot);
}

// function to generate the target code, and handle the cases of the intermediate code
void generateTargetCode()
{
    int block = 1;
    for (int i = 1; i < next_instr; i++)
    {
        quads[i].alt_ins = ass_instr;

        if (quads[i].op == "=")
        {
            symbol *arg1 = symbol_search(quads[i].arg1, 0, 0);
            symbol *result = symbol_search(quads[i].result, 0, 0);
            if (arg1->is_const)
            {
                int result_reg = get_reg(result, 1);
                string tmp = "R" + to_string(result_reg);
                emit("LDI", quads[i].arg1, "", tmp, 1);
            }
            else
            {
                int arg_reg = get_reg(arg1, 0);
                add_symbol(arg_reg, result);
                result->is_mem = 0;
            }

            if (leaders[i + 1] != 0)
            {
                for (int i = 1; i < reg_nos + 1; i++)
                {
                    spill(i, 0);
                }
            }
        }
        else if (quads[i].op == "gt")
        {
            for (int i = 1; i < reg_nos + 1; i++)
            {
                spill(i, 0);
            }
            emit("JMP", "", "", quads[i].result, 1);
        }
        else if (quads[i].op == "if")
        {
            string arg1, operation, arg2;
            istringstream iss(quads[i].arg1);
            iss >> arg1 >> operation >> arg2;
            string arg1_name = get_name(arg1);
            string arg2_name = get_name(arg2);

            for (int i = 1; i < reg_nos + 1; i++)
            {
                spill(i, 0);
            }

            map<string, string> op_map = {
                {"==", "JNE"},
                {"!=", "JEQ"},
                {"<", "JGE"},
                {">", "JLE"},
                {"<=", "JGT"},
                {">=", "JLT"}};

            string opr = op_map[operation];
            emit(opr, arg1_name, arg2_name, quads[i].result, 1);
        }
        else
        {
            string arg1_name = get_name(quads[i].arg1);
            string arg2_name = get_name(quads[i].arg2);

            symbol *result = symbol_search(quads[i].result, 1, 0);
            int result_reg = get_reg(result, 1);
            string result_name = "R" + to_string(result_reg);

            string operation = quads[i].op;

            map<string, string> op_map2 = {
                {"+", "ADD"},
                {"-", "SUB"},
                {"*", "MUL"},
                {"/", "DIV"},
                {"%", "MOD"}};

            string opr = op_map2[operation];
            emit(opr, arg1_name, arg2_name, result_name, 1);

            if (leaders[i + 1] != 0)
            {
                for (int i = 1; i < reg_nos + 1; i++)
                {
                    spill(i, 0);
                }
            }
        }
        quad_cnt++;
    }
}

// print the assembly code
void printAssembly()
{
    int block = 1;
    int lastIns = 0;
    for (int i = 1; i < ass_instr; i++)
    {
        if (lastIns != quads2[i].alt_ins)
        {
            lastIns = quads2[i].alt_ins;
            if (leaders[lastIns] == 1)
            {
                if (i != 1)
                {
                    cout << "\n";
                }
                cout << "Block " << block++ << endl;
            }
        }
        cout << "  ";
        if (quads2[i].op[0] == 'J')
        {
            int jmp = atoi(quads2[i].result.c_str());
            jmp = quads[jmp].alt_ins;
            jmp = (jmp == 0) ? ass_instr : jmp;
            quads2[i].result = jmp;
            if (quads2[i].arg2 == "")
            {
                if (quads2[i].arg1 == "")
                {
                    cout << i << "\t: " << quads2[i].op << " " << jmp << "\n";
                }
                else
                {
                    cout << i << "\t: " << quads2[i].op << " " << quads2[i].arg1 << " " << jmp << "\n";
                }
            }
            else
            {
                cout << i << "\t: " << quads2[i].op << " " << quads2[i].arg1 << " " << quads2[i].arg2 << " " << jmp << "\n";
            }
        }
        else
        {
            // cout << quads2[i].op << endl;
            cout << i << "\t: " << quads2[i].op << " " << quads2[i].result << " " << quads2[i].arg1 << " " << quads2[i].arg2 << " " << "\n";
        }
    }
    cout << "\n"
         << "  " << ass_instr << "\t:";
}

int main(int argc, char const *argv[])
{
    // custom number of token can be taken from command line as an argument, first run 'make all'
    // then run 'make run R=<number>', number = number of registers you want
    if (argc > 1)
    {
        istringstream ss(argv[1]);
        if (!(ss >> reg_nos) || reg_nos <= 0)
        {
            cerr << "Invalid Number of Registers. Setting default value 5";
            reg_nos = 5;
        }
    }
    else
    {
        reg_nos = 5;
    }
    SymbolTable = NULL;
    temp_count = 1;
    ass_instr = 1;
    quad_cnt = 1;
    next_instr = 1;

    ofstream file1("intermediate_code.txt");
    streambuf *old_buf = cout.rdbuf(file1.rdbuf());
    cout << "---------- Blocks of Intermediate Code ----------\n";
    for (int i = 0; i < 1000; i++)
    {
        leaders[i] = 0;
    }
    leaders[1] = 1;
    yyparse();
    for (int i = 1; i < 1000; i++)
    {
        size_t pos = quads[i].arg1.find("/=");
        if (pos != string::npos)
        {
            quads[i].arg1.replace(pos, 2, "!=");
        }

        size_t pos2 = quads[i].arg1.find("=");
        if (pos2 != string::npos && (pos2 == 0 || (quads[i].arg1[pos2 - 1] != '!' && quads[i].arg1[pos2 - 1] != '<' && quads[i].arg1[pos2 - 1] != '>')))
        {
            quads[i].arg1.replace(pos2, 1, "==");
        }
    }
    print_quads();
    cout.rdbuf(old_buf);
    file1.close();

    ofstream file2("target_code.txt");
    old_buf = cout.rdbuf(file2.rdbuf());
    cout << "---------- Target Code ----------\n";
    generateTargetCode();
    printAssembly();
    return 0;
}

void yyerror(string s)
{
    cout << s << endl;
}