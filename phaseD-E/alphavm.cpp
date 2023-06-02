#include "alphavm.h"
#include "parser.hpp"

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
    /* library_func_t f = avm_getlibraryfunc(id);
    if(!f){
        avm_error("unsupported lib func");
        executionFinished=1;
    }else{
        topsp=top;
        totalActuals=0;
        (*f)();
        if(!executionFinished) execute_funcexit(vector<instruction>); 
    }*/
}

void avm_callsaveenvironment(){
    /* avm_push_envvalue(totalActuals);
    avm_push_envvalue(pc+1);
    avm_push_envvalue(top+totalActuals+2);
    avm_push_envvalue(topsp); */
}

void execute_funcexit(instruction t){
   /*  unsigned oldTop=top;
    top= avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc=avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp=avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    while  (oldTop++<=top) avm_memcellclear(&stack[oldTop]); */
}

void execute_funcenter(instruction t){
    /* totalActuals=0;
    userfunc *funcInfo= avm_getfuncinfo(pc);
    topsp=top;
    top=top-funcInfo->localSize; */
}

unsigned avm_totalactuals(){
    //return avm_get_envvalue(topsp+AVM_NUMACTUALS_OFFSET);
}

avm_memcell *avm_getactual(unsigned i){
    /* assert(i<avm_totalactuals());
    return &stack[topsp + AVM_STACKENV_SIZE]; */
}

void libfunc_print(){
    /* unsigned n= avm_totalactuals();
    for (unsigned i=0; i<n; i++){
        string s = avm_tostring(avm_getactual(i));
    } */
}

void avm_registerlibfunc(string id, library_func_t addr){
    
}

void libfunc_typeof(){
    /* unsigned n= avm_totalactuals();
    if(n!=1) avm_error("one argument expected in 'typeof'!");
    else{
        avm_memcellclear(&retval);
        retval.type=string_m;
        retval.data.strVal=strdup(typeStrings[avm_getactual(0)->type]);
    } */
}

void libfunc_totalarguments(){
    /* unsigned p_topsp=avm_get_envvalue(topsp+AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if(!p_topsp){
        avm_error("totalarguments called outside of a function");
        retval.type=nil_m;
    }else{
        retval.type=number_m;
        retval.data.numval=avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    } */
}

void execute_arithmetic(instruction* instr){
   /*  avm_memcell *lv=avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell *rv1=avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2=avm_translate_operand(&instr->arg2, &bx);
    assert(lv && (&stack[0] <= lv && &stack[top] > lv || lv==&retval));
    assert(rv1 && rv2);
    if(rv1->type !=number_m || rv2->type != number_m){
        avm_error("not a number in arithmetic");
        executionFinished=1;
    } else{
        arithmetic_func_t op = arithemticFuncs [instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m ;
        lv->data.numVal= (*op) (rv1->data.numVal, rv2->data.numVal);
    } */
}