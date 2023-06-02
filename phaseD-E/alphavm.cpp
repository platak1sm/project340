#include "alphavm.h"
#include "parser.h"
unsigned char executionFinished=0;
unsigned pc=0;
unsigned currline=0;
unsigned codeSize=0;
vector<instruction> code;

#define AVM_ENDING_PC codeSize

avm_memcell *avm_translate_operand(vmarg *arg){
    if (arg==NULL) return NULL;

    switch (arg->type)
    {
    case global_a: //return &stack[AVM_STACKSIZE-1-arg->val];
    case local_a: //return &stack[topsp-arg->val];
    case formal_a: //return &stack[topsp+AVM_STACKENV_SIZE+1+arg->val];
    case retval_a: //return &retval;
    case number_a:{
        //reg->type=number_m; 
        //reg->data.numVal=
        //return reg;
    }
    case string_a:{
        //reg->type=string_m; 
        //reg->data.strVal=
        //return reg;
    }
    case bool_a:{
        //reg->type=bool_m; 
        //reg->data.boolVal=
        //return reg;
    }
    case nil_a: {
        //reg->type=nil_m; 
        //return reg;
    }
    case userfunc_a:{
        //reg->type=userfunc_m; 
        //reg->data.funcVal=
        //return reg;
    }
    case libfunc_a:{
        //reg->type=libfunc_m; 
        //reg->data.libfuncVal=
        //return reg;
    }
    break;
    
    default:
        break;
    }
}

void avm_calllibfunc(string id){
    libraru_func_t f = avm_getlibraryfunc(id);
    if(!f){
        avm_error("unsupported lib func");
        executionFinished=1;
    }else{
        topsp=top;
        totalActuals=0;
        (*f)();
        if(!executionFinished) execute_funcexit(vector<instruction>);
    }
}

