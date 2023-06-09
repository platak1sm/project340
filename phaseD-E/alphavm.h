// #include "icode.h"
#include "parser.hpp"
// #include "target.hpp"
#include "phase4.h"
#include <string>
#include <vector>
struct avm_table;
using namespace std;

typedef enum avm_memcell_t{
    number_m,
    string_m,
    bool_m,
    table_m,
    userfunc_m,
    libfunc_m,
    nil_m,
    undef_m
} avm_memcell_t;

typedef struct avm_memcell{
    avm_memcell_t type;
    
    union{
        double numVal;
        char* strVal;
        bool boolVal;
        avm_table *tableVal;
        unsigned userfuncVal;
        char* libfuncVal;
    } data;

} avm_memcell;

string typeStrings[] = {
	"number",
	"string",
	"bool",
    "table",
	"userfunc",
	"libfunc",
	"nil",
    "undef"
};


/* string avm_type2str (unsigned i);*/

typedef void (*library_func_t)();
library_func_t avm_getlibraryfunc(string id);
void avm_calllibfunc(string id);

avm_memcell *avm_translate_operand(vmarg *arg);

typedef void (*execute_func_t)(instruction*);

void execute_assign (instruction *instr);
void execute_arithmetic(instruction* instr);
void execute_add (instruction* instr);
void execute_sub (instruction* instr);
void execute_mul (instruction* instr);
void execute_div (instruction* instr);
void execute_mod (instruction* instr);
void execute_uminus (instruction* instr);
void execute_and (instruction* instr);
void execute_or (instruction* instr);
void execute_not (instruction* instr);
void execute_jeq (instruction* instr);
void execute_jne (instruction* instr);
void execute_jle (instruction* instr);
void execute_jge (instruction* instr);
void execute_jlt (instruction* instr);
void execute_jgt (instruction* instr);
void execute_call (instruction* instr);
void execute_pusharg (instruction* instr);
void execute_funcenter (instruction* instr);
void execute_funcexit (instruction* instr);
void execute_newtable (instruction* instr);
void execute_tablegetelem (instruction* instr);
void execute_tablesetelem (instruction* instr);
void execute_nop (instruction *instr);

execute_func_t executeFuncs[]={
    execute_assign,   
    execute_add,  
    execute_sub,
    execute_mul,  
    execute_div,  
    execute_mod,
    execute_uminus,   
    execute_and,  
    execute_or,
    execute_not,  
    execute_jeq,  
    execute_jne,
    execute_jle,  
    execute_jge,  
    execute_jlt,
    execute_jgt,  
    execute_call, 
    execute_pusharg,
    execute_funcenter,  
    execute_funcexit,   
    execute_newtable,
    execute_tablegetelem, 
    execute_tablesetelem, 
    execute_nop/* , 
    execute_jump */
};

void avm_error(string v){
	printf("%s",v.c_str());
	exit(0);
}
void avm_warning(string v){
	printf("%s",v.c_str());
}

double add_impl( double x, double y ){
	return x+y;
}

double sub_impl( double x, double y ){
	return x-y;
}

double mul_impl( double x, double y ){
	return x*y;
}

double div_impl( double x, double y ){
	if(y == 0){avm_error("Error:Division with zero attempted.\n");}
	return x/y;
}

double mod_impl( double x, double y ){
	if(y == 0){avm_error("Error:Division with zero attempted.\n");}
	return (( unsigned ) x ) % (( unsigned ) y );
}

typedef double (*arithmetic_func_t) (double x,double y);

arithmetic_func_t arithmeticFuncs[] ={
	add_impl,
	sub_impl,
	mul_impl,
	div_impl,
	mod_impl
};

void avm_callsaveenvironment();
unsigned avm_totalactuals();

avm_memcell *avm_getactual(unsigned i);
// void libfunc_print();
void avm_registerlibfunc(string id, library_func_t addr);
// void libfunc_typeof();
// void libfunc_totalarguments();
void libfunc_print();
void libfunc_typeof();
void libfunc_totalarguments();
void objectmemberkeys();
void objecttotalmembers();
void objectcopy();
void input();
void libfunc_argument();
void libfunc_strtonum(void);
void libfunc_sqrt(void);
void libfunc_cos(void);
void libfunc_sin(void);


typedef bool (*equality_func_t)(avm_memcell*, avm_memcell*);

void execute_jump(instruction *instr);

int avm_get_envvalue(int i);

void avm_push_envvalue(int val);

typedef void (*memclear_func_t)(avm_memcell*);

void memclear_string (avm_memcell *m);
void memclear_table (avm_memcell *m);

static memclear_func_t memclearFuncs[] = {
    NULL,
    memclear_string,
    NULL,
    memclear_table,
    NULL,
    NULL,
    NULL,
    NULL
};

void avm_assign (avm_memcell *lv, avm_memcell *rv);

typedef string (*tostring_func_t) (avm_memcell*);
string number_tostring (avm_memcell*);
string string_tostring (avm_memcell*);
string bool_tostring (avm_memcell*);
string table_tostring (avm_memcell*);
string userfunc_tostring (avm_memcell*);
string libfunc_tostring (avm_memcell*);
string nil_tostring (avm_memcell*);
string undef_tostring (avm_memcell*);

tostring_func_t tostringFuncs[] = {
    number_tostring,
    string_tostring,
    bool_tostring,
    table_tostring,
    userfunc_tostring,
    libfunc_tostring,
    nil_tostring,
    undef_tostring
};

userfunc* avm_getfuncinfo(int address);