#include <string>
#include <vector>

class Variable {
    string name;
    unsigned int scope;
    unsigned int line;
};

class Function {
    string name;
    //List of arguments
    unsigned int scope;
    unsigned int line; 
    vector<Variable> arguments;
};

enum SymbolType {
 GLOBAL, LOCAL, FORMAL,  USERFUNC, LIBFUNC
}; 

class SymbolTableEntry { 
    bool isActive;
    union {
    Variable *varVal;
    Function *funcVal;
    } value;
    enum SymbolType type;
}; 

vector<SymbolTableEntry> SymbolTable;

void insert(SymbolTableEntry ste);
SymbolTableEntry lookup();
void hide();
