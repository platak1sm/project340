#include "icode.h"

struct avm_table;

typedef enum avm_memcell_t{
    number_m    = 0,
    string_m    = 1,
    bool_m      = 2,
    table_m     = 3,
    userfunc_m  = 4,
    libfunc_m   = 5,
    nil_m       = 6,
    undef_m     = 7
} avm_memcell_t;

typedef struct avm_memcell {
    avm_memcell_t type;
    union{
        double numVal;
        string strVal;
        bool boolVal;
        avm_table *tableVal;
        unsigned funcVal;
        string libfuncVal;
    } data;
} avm_memcell;

avm_memcell *avm_translate_operand(vmarg *arg);

void execute_assign(instruction t); 
void execute_add(instruction t);
void execute_sub(instruction t);
void execute_mul(instruction t);
void execute_div(instruction t); 
void execute_mod(instruction t);
void execute_uminus(instruction t);  
void execute_and(instruction t);
void execute_or(instruction t);
void execute_not(instruction t);
void execute_jeq(instruction t);
void execute_jne(instruction t);
void execute_jle(instruction t);
void execute_jge(instruction t);
void execute_jlt(instruction t);
void execute_jgt(instruction t); 
void execute_call(instruction t);
void execute_pusharg(instruction t);
void execute_funcenter(instruction t); 
void execute_funcexit(instruction t);  
void execute_newtable(instruction t);
void execute_tablegetelem(instruction t); 
void execute_tablesetelem(instruction t);
void execute_nop(instruction t);
void execute_jump(instruction t);

execute_func_t executeFuncs[]{
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
    execute_nop, 
    execute_jump
};