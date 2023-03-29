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
    for(unsigned int i = 0; i < SymbolTable.size(); i++) {
        if(SymbolTable[i].type == USERFUNC){
            name = SymbolTable[i].funcVal.name;
            scope=SymbolTable[i].funcVal.scope;
            cout<< " \"" << name << "\"\t" << "[user function]\t"<< "(line "<< SymbolTable[i].funcVal.line << ")\t"<<"(scope "<<SymbolTable[i].funcVal.scope<< ")\t" <<endl;
           }
        else if(SymbolTable[i].type == LIBFUNC){
            name = SymbolTable[i].funcVal.name;
            scope=SymbolTable[i].funcVal.scope;
            cout << " \""<< name << "\"\t" << "[library function]\t"<< "(line "<< SymbolTable[i].funcVal.line << ")\t"<<"(scope "<<SymbolTable[i].funcVal.scope<< ")\t" <<endl;
        }
        else if(SymbolTable[i].type == GLOBAL){
            name = SymbolTable[i].varVal.name;
            scope=SymbolTable[i].varVal.scope;
            cout << " \""<< name << "\"\t" << "[global variable]\t"<< "(line "<< SymbolTable[i].varVal.line<< ")\t"<<"(scope "<<SymbolTable[i].varVal.scope<< ")\t" <<endl;
        }
        else if(SymbolTable[i].type == LOCALV){
            name = SymbolTable[i].varVal.name;
            scope=SymbolTable[i].varVal.scope;
            cout << " \""<< name << "\"\t" << "[local variable]\t"<< "(line "<< SymbolTable[i].varVal.line << ")\t"<<"(scope "<<SymbolTable[i].varVal.scope<< ")\t" <<endl;
        }
        else if(SymbolTable[i].type == FORMAL){
            name = SymbolTable[i].varVal.name;
            scope=SymbolTable[i].varVal.scope;
            cout << " \""<< name << "\"\t" << "[formal argument]\t"<< "(line "<< SymbolTable[i].varVal.line << ")\t"<<"(scope "<<SymbolTable[i].varVal.scope<< ")\t" <<endl;
        }         
    }
    
    /*for (unsigned int i = 0; i < maxscope; i++){
        cout << "\n----------------------------- Scope   #" << i << "  -----------------------------" << endl;
          for(unsigned int j = 0; i < SymbolTable.size(); j++){
            if(SymbolTable[j].varVal.scope == i){
                if(SymbolTable[j].type==LIBFUNC) {
                    cout << "[library function]\t";
                    cout << "(line " << SymbolTable[j].funcVal.line << ")\t" ;
                    cout << "(scope " << SymbolTable[j].funcVal.scope << ")\t"<< endl ;
                }
                else if(SymbolTable[j].type==USERFUNC){
                    cout << "[user function]\t";
                    cout << "(line " << SymbolTable[j].funcVal.line << ")\t" ;
                    cout << "(scope " << SymbolTable[j].funcVal.scope << ")\t"<< endl ;
                }
                else if(SymbolTable[j].type==GLOBAL){
                    cout << "[global variable]\t";
                    cout << "(line " << SymbolTable[j].varVal.line << ")\t" ;
                    cout << "(line " << SymbolTable[j].varVal.scope << ")\t" ;
                }
                else if(SymbolTable[j].type==LOCALV){
                    cout << "[local variable]\t";
                    cout << "(line " << SymbolTable[j].varVal.line << ")\t" ;
                    cout << "(line " << SymbolTable[j].varVal.scope << ")\t" ;
                }
                else {
                    cout << "[formal argument]\t";
                    cout << "(line " << SymbolTable[j].varVal.line << ")\t" ;
                    cout << "(line " << SymbolTable[j].varVal.scope << ")\t" ;
                }
            }
           
         }
    } */
}
