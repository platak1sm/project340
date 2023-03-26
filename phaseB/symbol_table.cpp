#include <iostream>
#include "symbol_table.h"

void insert(SymbolTableEntry ste){
    SymbolTable.push_back(ste);
    return;
}

SymbolTableEntry lookupcurrentscope(string s, int scope) {
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == USERFUNC || SymbolTable[i].type == LIBFUNC) {
            if(SymbolTable[i].funcVal.name == s) {
                if(SymbolTable[i].funcVal.scope == scope)
                    return SymbolTable[i];
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCAL  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].varVal.name == s) {
                if(SymbolTable[i].varVal.scope == scope)
                    return SymbolTable[i];
            }
        }
    }

    SymbolTableEntry unknown;
    unknown.isActive = false;
    return unknown;
                
}


SymbolTableEntry lookupactivevar(string s) {
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCAL  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].varVal.name == s) {
                if(SymbolTable[i].isActive)
                    return SymbolTable[i];
            }
        }
    }

    SymbolTableEntry unknown;
    unknown.isActive = false;
    return unknown;
                
}

SymbolTableEntry lookupactivefunc(string s) {
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == USERFUNC || SymbolTable[i].type == LIBFUNC) {
            if(SymbolTable[i].funcVal.name == s) {
                if(SymbolTable[i].isActive)
                    return SymbolTable[i];
            }
        }
        
    }

    SymbolTableEntry unknown;
    unknown.isActive = false;
    return unknown;
                
}

void hide(int scope) {
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == USERFUNC || SymbolTable[i].type == LIBFUNC || SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCAL  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].funcVal.scope >= scope) {
                SymbolTable[i].isActive = false;
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCAL  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].varVal.scope >= scope) {
                SymbolTable[i].isActive = false;
            }
        }
    }
}

void insertLibFuncs(string name){
    SymbolTableEntry ste;
    ste.isActive=1;
    ste.type=LIBFUNC;
    ste.funcVal.name=name;
    ste.funcVal.scope=0;
    ste.funcVal.line=0;
    insert(ste);
}