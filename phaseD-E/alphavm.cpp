#include "alphavm.h"


#define AVM_STACKSIZE 4096
#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))
#define AVM_NUMACTUALS_OFFSET 4
#define AVM_SAVEDPC_OFFSET 3
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET 1
#define AVM_STACKENV_SIZE 4


vector<double> numConst;
vector<string> stringConst;
vector<string> libFuncConst;
vector<userfunc*> avmuserFuncs;
vector<instruction*> code;
avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned    codeSize;
//instruction* code=(instruction*) 0;

avm_memcell stackavm[AVM_STACKSIZE];

int top, topsp;
int totalActuals = 0;
int currLine = 0;
bool executionFinished = false;
int pc = 0;

#define AVM_ENDING_PC codeSize


avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell* reg){
    if (arg==NULL) return NULL;

    switch (arg->type)
    {
    case global_a: return &stackavm[AVM_STACKSIZE-1-arg->val];
    case local_a: return &stackavm[topsp-arg->val];
    case formal_a: return &stackavm[topsp+AVM_STACKENV_SIZE+1+arg->val];
    case retval_a: return &retval;
    case number_a:{
        reg->type=number_m; 
        //reg->data.numVal=const_getnumber(arg->val);
        return reg;
    }
    case string_a:{
        reg->type=string_m; 
        //reg->data.strVal=const_getstring(arg->val);
        return reg;
    }
    case bool_a:{
        reg->type=bool_m; 
        //reg->data.boolVal=arg->val;
        return reg;
    }
    case nil_a: {
        //reg->type=nil_m; 
        return reg;
    }
    case userfunc_a:{
        reg->type=userfunc_m; 
        //reg->data.funcVal=
        return reg;
    }
    case libfunc_a:{
        reg->type=libfunc_m; 
        //reg->data.libfuncVal=
        return reg;
    }
    break;
    
    default:
        break;
    }
    return NULL;
}

void avm_calllibfunc(string id){
    library_func_t f = avm_getlibraryfunc(id);
    if(!f){
        avm_error("unsupported lib func");
        executionFinished=1;
    }else{
        topsp=top;
        totalActuals=0;
        (*f)();
        if(!executionFinished) execute_funcexit(new instruction()); 
    }
}

void avm_memcellclear (avm_memcell *m) {
    if (m->type == undef_m) {
        return;
    }
    memclear_func_t f = memclearFuncs[m->type];
    if (f) {
        f(m);
    }
    m->type = undef_m;
}

void avm_callsaveenvironment(){
    avm_push_envvalue(totalActuals);
    avm_push_envvalue(pc+1);
    avm_push_envvalue(top+totalActuals+2);
    avm_push_envvalue(topsp);
}

void execute_funcexit(instruction t){
    unsigned oldTop=top;
    top= avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc=avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp=avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    while  (oldTop++<=top) avm_memcellclear(&stackavm[oldTop]);
}

void execute_funcenter(instruction t){
    totalActuals=0;
    userfunc *funcInfo= avm_getfuncinfo(pc);
    topsp=top;
    top=top-funcInfo->localSize;
}

unsigned avm_totalactuals(){
    return avm_get_envvalue(topsp+AVM_NUMACTUALS_OFFSET);
}

avm_memcell *avm_getactual(unsigned i){
    assert(i<avm_totalactuals());
    return &stackavm[topsp + AVM_STACKENV_SIZE];
}
string avm_tostring(avm_memcell* m){
	assert(m->type >= 0 and m->type <= undef_m);
	return tostringFuncs[m->type](m);
}
void libfunc_print(){
    unsigned n= avm_totalactuals();
    for (unsigned i=0; i<n; i++){
        string s = avm_tostring(avm_getactual(i));
    }
}



void avm_registerlibfunc(string id, library_func_t addr){
    
}

void memclear_string (avm_memcell *m){}
void memclear_table (avm_memcell *m){}
void execute_funcexit (instruction* instr){}
void execute_or (instruction* instr){}
void execute_add (instruction* instr){}
void execute_and (instruction* instr){}
void execute_div (instruction* instr){}
void execute_jeq (instruction* instr){}
void execute_jne (instruction* instr){}
void execute_jle (instruction* instr){}
void execute_jge (instruction* instr){}
void execute_jlt (instruction* instr){}
void execute_jgt (instruction* instr){}
void execute_mod (instruction* instr){}
void execute_mul (instruction* instr){}
void execute_nop (instruction *instr){}
void execute_not (instruction* instr){}
void execute_sub (instruction* instr){}
void execute_call (instruction* instr){}
void execute_uminus (instruction* instr){}
void execute_pusharg (instruction* instr){}
void execute_newtable (instruction* instr){}
void execute_tablegetelem (instruction* instr){}
void execute_tablesetelem (instruction* instr){}
void execute_funcenter (instruction* instr){}
library_func_t avm_getlibraryfunc(string id){}
void libfunc_typeof(){
    unsigned n= avm_totalactuals();
    if(n!=1) avm_error("one argument expected in 'typeof'!");
    else{
        avm_memcellclear(&retval);
        retval.type=string_m;
        //retval.data.strVal=strdup(typeStrings[avm_getactual(0)->type]);
    }
}

void libfunc_totalarguments(){
    unsigned p_topsp=avm_get_envvalue(topsp+AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if(!p_topsp){
        avm_error("totalarguments called outside of a function");
        retval.type=nil_m;
    }else{
        retval.type=number_m;
        retval.data.numVal=avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void execute_cycle (void) {
    if(executionFinished)
    return;
    if (pc >= codeSize) {
        executionFinished = 1;
        return;
    }
    else{
        if(pc==AVM_ENDING_PC){
            executionFinished=true;
            return;
        }
        else{
            assert(pc<AVM_ENDING_PC);
            assert(code[pc]->opcode>=0 && code[pc]->opcode <=AVM_MAX_INSTRUCTIONS);
            if(code[pc]->srcLine)
            currLine=code[pc]->srcLine;
            unsigned oldpc=pc;
            (*executeFuncs[code[pc]->opcode])(code[pc]);
            if(pc==oldpc)
            ++pc;
        }
    }
}

void execute_arithmetic(instruction* instr){
    avm_memcell *lv=avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell *rv1=avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2=avm_translate_operand(&instr->arg2, &bx);
    assert(lv && (&stackavm[0] <= lv && &stackavm[top] > lv || lv==&retval));
    assert(rv1 && rv2);
    if(rv1->type !=number_m || rv2->type != number_m){
        avm_error("not a number in arithmetic");
        executionFinished=1;
    } else{
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m ;
        lv->data.numVal= (*op) (rv1->data.numVal, rv2->data.numVal);
    }
}

void execute_assign (instruction *instr) {
    avm_memcell *lv = avm_translate_operand(&instr->result, NULL);
    avm_memcell *rv = avm_translate_operand(&instr->arg1, &ax);
    avm_assign(lv, rv);
}

void execute_jump(instruction *instr) {
    assert(instr->result.type == label_a);
    if (!executionFinished) {
        pc = instr->result.val;
    }
}

int avm_get_envvalue(int i){
    int val = (int) stackavm[i].data.numVal;
    return val;
}

void avm_push_envvalue(int val){
	stackavm[top].type = number_m;
	stackavm[top].data.numVal = val;
	//avm_dec_top();
}



void avm_assign (avm_memcell *lv, avm_memcell *rv) {
    if (lv == rv) {
        return;
    }
    if (lv->type == table_m and
        rv->type == table_m and
        lv->data.tableVal == rv->data.tableVal) {
            return;
    }

    if (rv->type == undef_m) {
        avm_warning("Assigning from 'undefined' variable\n");
    }

    avm_memcellclear(lv);
    memcpy(lv, rv, sizeof(avm_memcell));

    if (lv->type == string_m) {
        //lv->data.strVal = strdup(rv->data.strVal);
    }
    else if (lv->type == table_m) {
        //avm_tablerefinc(lv->data.tableVal);
    }
}


string number_tostring(avm_memcell* m){
	assert(m->type == number_m);
	return to_string(m->data.numVal).c_str();

}
string string_tostring(avm_memcell* m){
	return m->data.strVal;
}

string bool_tostring(avm_memcell* m){
	assert(m->type == bool_m);
	if(m->data.boolVal) return strdup("false");
	else return strdup("true");
}

string table_tostring(avm_memcell* m){
	//
}

string userfunc_tostring(avm_memcell* m){
	assert(m->type == userfunc_m);
	return strdup("func");

}

string libfunc_tostring(avm_memcell* m){
	 return m->data.libfuncVal;
}

string nil_tostring(avm_memcell* m){
	return strdup("nil");
}

string undef_tostring(avm_memcell* m){
	return strdup("undef");

}

/* string avm_tostring(avm_memcell* m){
	assert(m->type >= 0 and m->type <= undef_m);
	return tostringFuncs[m->type](m);
} */

userfunc* avm_getfuncinfo(int address) {
	for(auto i : avmuserFuncs) if(i->address == address) return i;
	printf("Not found\n");
	return NULL;
} 

/* string avm_type2str (unsigned i) {
    assert(number_m <= i and i <= undef_m);
    return typeStrings[i];
}  */

/* void execute_jeq (instruction *instr) {
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    bool res = false;

    if (rv1->type == undef_m or rv2->type == undef_m) {
        avm_error("Cannot perform equality comparison with 'undef' operands");
    }
    else if (rv1->type == nil_m or rv2->type == nil_m) {
        res = rv1->type == nil_m and rv2->type == nil_m;
    }
    else if (rv1->type == bool_m or rv2->type == bool_m) {
        res = (avm_tobool(rv1) == avm_tobool(rv2));
    }
    else if (rv1->type != rv2->type) {
        avm_error(  "Cannot perform equality comparison between two different types");
    }
    else { // type can be: num, str, table, userFunc, libFunc
        res = equalityFuncs[rv1->type](rv1, rv2);
    }

    if (!executionFinished and res) {
        pc = instr->result.val;
    }
}
bool avm_tobool(avm_memcell* m){
	assert(m->type >= 0 and m->type <= undef_m);
	return toboolFuncs[m->type](m);
}
static bool number_tobool  (avm_memcell *m) {
    return m->data.numVal != 0.0;
}

static bool string_tobool  (avm_memcell *m) {
    return m->data.strVal[0] != 0;
}

static bool bool_tobool    (avm_memcell *m) {
    return m->data.boolVal;
}

static bool table_tobool   (avm_memcell *m) {
    return true;
    // return  m->data.tableVal->numIndexed.size() +
    //         m->data.tableVal->strIndexed.size() > 0;
}

static bool userFunc_tobool(avm_memcell *m) {
    return true;
}

static bool libFunc_tobool (avm_memcell *m) {
    return true;
}

static bool nil_tobool (avm_memcell *m) {
    return false;
}

static bool undef_tobool   (avm_memcell *m) {
    avm_error("Cannot convert 'undef' to bool\n");
    return false;
}
bool avm_tobool(avm_memcell* m){
	assert(m->type >= 0 and m->type <= undef_m);
	return toboolFuncs[m->type](m);
} */