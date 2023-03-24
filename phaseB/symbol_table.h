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
 GLOBAL, LOCAL, FORMAL,  USERFUNC, LIBFUNC
}; 

class SymbolTableEntry { 
    public:
        bool isActive;
        Variable varVal;
        Function funcVal;
        enum SymbolType type;
}; 

vector<SymbolTableEntry> SymbolTable;

void insert(SymbolTableEntry ste);
SymbolTableEntry lookup(string s);
void hide();
