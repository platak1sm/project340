#include <iostream>
#include "symbol_table.h"

vector<SymbolTableEntry> SymbolTable;

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
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
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
        if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
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

SymbolTableEntry LookuplastRef(string s) {
    int currentscope = -1;
    SymbolTableEntry ret;
    ret.isActive = false;
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == USERFUNC || SymbolTable[i].type == LIBFUNC) {
            if(SymbolTable[i].funcVal.name == s) {
                if(SymbolTable[i].funcVal.scope > currentscope)
                    currentscope = SymbolTable[i].funcVal.scope;
                    
                    ret = SymbolTable[i];
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].varVal.name == s) {
                if(SymbolTable[i].varVal.scope > currentscope){
                    currentscope = SymbolTable[i].varVal.scope;
                    ret = SymbolTable[i];
                }
            }
        }
    }

    return ret;
                
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
        if(SymbolTable[i].type == USERFUNC || SymbolTable[i].type == LIBFUNC || SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].funcVal.scope >= scope) {
                SymbolTable[i].isActive = false;
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].varVal.scope >= scope) {
                SymbolTable[i].isActive = false;
            }
        }
    }
}

void printsymbols(){
    string name;
    int scope;
    int currentscope = 0;
    int count = 0;
    while(count < SymbolTable.size()){
        cout <<"--------- Scope:"<<currentscope<<" ----------"<<endl;
        for(unsigned int i = 0; i < SymbolTable.size(); i++) {
            if(SymbolTable[i].type == USERFUNC && SymbolTable[i].funcVal.scope == currentscope){
                name = SymbolTable[i].funcVal.name;
                scope=SymbolTable[i].funcVal.scope;
                cout<< " \"" << name << "\"\t" << "[user function]\t"<< "(line "<< SymbolTable[i].funcVal.line << ")\t"<<"(scope "<<SymbolTable[i].funcVal.scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == LIBFUNC && SymbolTable[i].funcVal.scope == currentscope){
                name = SymbolTable[i].funcVal.name;
                scope=SymbolTable[i].funcVal.scope;
                cout << " \""<< name << "\"\t" << "[library function]\t"<< "(line "<< SymbolTable[i].funcVal.line << ")\t"<<"(scope "<<SymbolTable[i].funcVal.scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == GLOBAL && SymbolTable[i].varVal.scope == currentscope){
                name = SymbolTable[i].varVal.name;
                scope=SymbolTable[i].varVal.scope;
                cout << " \""<< name << "\"\t" << "[global variable]\t"<< "(line "<< SymbolTable[i].varVal.line<< ")\t"<<"(scope "<<SymbolTable[i].varVal.scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == LOCALV && SymbolTable[i].varVal.scope == currentscope){
                name = SymbolTable[i].varVal.name;
                scope=SymbolTable[i].varVal.scope;
                cout << " \""<< name << "\"\t" << "[local variable]\t"<< "(line "<< SymbolTable[i].varVal.line << ")\t"<<"(scope "<<SymbolTable[i].varVal.scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == FORMAL && SymbolTable[i].varVal.scope == currentscope){
                name = SymbolTable[i].varVal.name;
                scope=SymbolTable[i].varVal.scope;
                cout << " \""<< name << "\"\t" << "[formal argument]\t"<< "(line "<< SymbolTable[i].varVal.line << ")\t"<<"(scope "<<SymbolTable[i].varVal.scope<< ")\t" <<endl;
                count++;
            }         

        }
        currentscope++;
    }
    
    
}
