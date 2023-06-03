#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

/* class Variable {
    public:
        string name;
        unsigned int scope;
        unsigned int line;
};

class Function {
    public:
        string name;
        //List of arguments
        unsigned int scope;
        unsigned int line; 
        vector<Variable> arguments;
}; */

typedef enum SymbolType {
 GLOBAL, LOCALV, FORMAL,  USERFUNC, LIBFUNC
} stype; 

typedef enum scopespace_t
{
    programvar,
    functionlocal,
    formalarg
} scopespace_t;

typedef enum symbol_t
{
    var_s,
    programfunc_s,
    libraryfunc_s
} symbol_t;

class SymbolTableEntry { 
    public:
        string name;
        scopespace_t scopespace;
        symbol_t symt;
        bool isActive;
        //Variable varVal;
        //Function funcVal;
        unsigned int scope;
        unsigned int line;
        unsigned offset;
        unsigned int iaddress;
        unsigned int taddress;
        unsigned int totalloc;
        stype type;
}; 

extern vector<SymbolTableEntry> SymbolTable;
extern int hidden_var_count;
void insert(SymbolTableEntry ste);
SymbolTableEntry lookupcurrentscope(string s, int scope);
SymbolTableEntry lookupactivevar(string s);
SymbolTableEntry lookupactivefunc(string s);
SymbolTableEntry LookuplastRef(string s);
void hide(int scope);
void insertLibFuncs(string name);
void printsymbols();
int getNumOfGlobals();
/* string create_hiddenvar_name();
void reset_hidden_count(); */