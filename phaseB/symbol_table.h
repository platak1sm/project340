#include <string>
#include <vector>

using namespace std;

class Variable {
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
};

enum SymbolType {
 GLOBAL, LOCALV, FORMAL,  USERFUNC, LIBFUNC
}; 

class SymbolTableEntry { 
    public:
        bool isActive;
        Variable varVal;
        Function funcVal;
        enum SymbolType type;
}; 

extern vector<SymbolTableEntry> SymbolTable;

void insert(SymbolTableEntry ste);
SymbolTableEntry lookupcurrentscope(string s, int scope);
SymbolTableEntry lookupactivevar(string s);
SymbolTableEntry lookupactivefunc(string s);
void hide(int scope);
void insertLibFuncs(string name);