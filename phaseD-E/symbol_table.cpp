#include <iostream>
#include "symbol_table.h"
#include <string>
vector<SymbolTableEntry> SymbolTable;
int hidden_var_count = 0;


/* string create_hiddenvar_name(){
    string name = "_tmp"+to_string(hidden_var_count);
    hidden_var_count++;
    return name;
}

void reset_hidden_count(){
    hidden_var_count = 0;
} */
void insert(SymbolTableEntry ste){
    SymbolTable.push_back(ste);
    return;
}

SymbolTableEntry lookupcurrentscope(string s, int scope) {
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == USERFUNC || SymbolTable[i].type == LIBFUNC) {
            if(SymbolTable[i].name == s) {
                if(SymbolTable[i].scope == scope)
                    return SymbolTable[i];
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].name == s) {
                if(SymbolTable[i].scope == scope)
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
            if(SymbolTable[i].name == s) {
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
            if(SymbolTable[i].name == s) {
                if(SymbolTable[i].scope > currentscope)
                    currentscope = SymbolTable[i].scope;
                    
                    ret = SymbolTable[i];
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].name == s) {
                if(SymbolTable[i].scope > currentscope){
                    currentscope = SymbolTable[i].scope;
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
            if(SymbolTable[i].name == s) {
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
            if(SymbolTable[i].scope >= scope) {
                SymbolTable[i].isActive = false;
            }
        }
        else if(SymbolTable[i].type == GLOBAL || SymbolTable[i].type == LOCALV  || SymbolTable[i].type == FORMAL) {
            if(SymbolTable[i].scope >= scope) {
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
            if(SymbolTable[i].type == USERFUNC && SymbolTable[i].scope == currentscope){
                name = SymbolTable[i].name;
                scope=SymbolTable[i].scope;
                cout<< " \"" << name << "\"\t" << "[user function]\t"<< "(line "<< SymbolTable[i].line << ")\t"<<"(scope "<<SymbolTable[i].scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == LIBFUNC && SymbolTable[i].scope == currentscope){
                name = SymbolTable[i].name;
                scope=SymbolTable[i].scope;
                cout << " \""<< name << "\"\t" << "[library function]\t"<< "(line "<< SymbolTable[i].line << ")\t"<<"(scope "<<SymbolTable[i].scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == GLOBAL && SymbolTable[i].scope == currentscope){
                name = SymbolTable[i].name;
                scope=SymbolTable[i].scope;
                cout << " \""<< name << "\"\t" << "[global variable]\t"<< "(line "<< SymbolTable[i].line<< ")\t"<<"(scope "<<SymbolTable[i].scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == LOCALV && SymbolTable[i].scope == currentscope){
                name = SymbolTable[i].name;
                scope=SymbolTable[i].scope;
                cout << " \""<< name << "\"\t" << "[local variable]\t"<< "(line "<< SymbolTable[i].line << ")\t"<<"(scope "<<SymbolTable[i].scope<< ")\t" <<endl;
                count++;
            }
            else if(SymbolTable[i].type == FORMAL && SymbolTable[i].scope == currentscope){
                name = SymbolTable[i].name;
                scope=SymbolTable[i].scope;
                cout << " \""<< name << "\"\t" << "[formal argument]\t"<< "(line "<< SymbolTable[i].line << ")\t"<<"(scope "<<SymbolTable[i].scope<< ")\t" <<endl;
                count++;
            }         

        }
        currentscope++;
    }
    
    
}
