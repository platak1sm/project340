#include "target.hpp"
#include <string.h>
#include <assert.h>
instruction* instructions = (instruction*) 0;
unsigned total_i = 0;
unsigned currinstruction = 0;
int currprocquad = 0;

double* numConsts = (double *) 0;
unsigned totalNumConsts = 0;
unsigned currNumConst = 0;
char** numStrings = (char **) 0;
unsigned totalStringConsts = 0;
unsigned currStringConst = 0;
char** namedLibfuncs = (char **) 0;
unsigned totalNamedLibfuncs = 0;
unsigned currNamedLibfunc = 0;
userfunc* userFuncs = (userfunc *) 0;
unsigned totalUserFuncs = 0;
unsigned currUserFunc = 0;

/* Declare funcitons */
void match_op_i(vmopcode code);
void reset_operand(vmarg* arg);
void generate_bfile(void);
void read_bfile(void);

incomplete_jump * ij_head = (incomplete_jump*)0;
unsigned ij_total = 0;

stack<int> funcstack;

generator_func_t generators[] = {
    generate_ASSIGN,
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_JUMP,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_CALL,
    generate_PARAM,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_RETURN,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_NOP
};

void expand_num_consts(){
    assert(totalNumConsts == currNumConst);
    double* p = (double*) malloc(512* sizeof(double) + totalNumConsts);
    if(numConsts){
        memcpy(p,numConsts, 512* sizeof(double));
        free(numConsts);
    }
    numConsts = p;
    totalNumConsts+= 512;
}

void expand_string_consts(){
    assert(totalStringConsts == currStringConst);
    char** p = (char**) malloc(512* sizeof(char *) + totalStringConsts);
    if(numStrings){
        memcpy(p,numStrings, 512* sizeof(char *));
        free(numStrings);
    }
    numStrings = p;
    totalStringConsts+= 512;
}

void expand_lib_funcs(){
    assert(totalNamedLibfuncs == currNamedLibfunc);
    char** p = (char**) malloc(512* sizeof(char *) + totalNamedLibfuncs);
    if(namedLibfuncs){
        memcpy(p,namedLibfuncs, 512* sizeof(char *));
        free(namedLibfuncs);
    }
    namedLibfuncs = p;
    totalNamedLibfuncs+= 512;
}

void expand_user_funcs(){
    assert(totalUserFuncs == currUserFunc);
    userfunc* p = (userfunc*) malloc(512* sizeof(userfunc) + totalUserFuncs);
    if(userFuncs){
        memcpy(p,userFuncs, 512* sizeof(char *));
        free(userFuncs);
    }
    userFuncs = p;
    totalUserFuncs+= 512;
}

void expand_i(){
    assert(total_i == currinstruction);
    instruction* p = (instruction*) malloc(NEW_SIZE_I);
    if(instructions){
        memcpy(p,instructions,CURR_SIZE_I);
        free(instructions);
    }
    instructions = p;
    total_i+= EXPAND_SIZE_I;
}

void emit_i(instruction t){
    if(currinstruction == total_i)expand_i();
    instruction* p = instructions+currinstruction++;
    p->arg1 = t.arg1;
    p->arg2 = t.arg2;
    p->result = t.result;
    p->opcode = t.opcode;
    p->srcLine = t.srcLine;
}

unsigned nextinstructionlabel(){
    return currinstruction;
}

unsigned consts_newstring (char* s) {
    if(currStringConst == totalStringConsts) expand_string_consts();
    int l;
    for(l=0;l<currStringConst;l++){ // check if print has already been inserted
        if(strcmp(*(numStrings+l),s)==0) return l;
    }
    char** p = numStrings+currStringConst++;
    *p = strdup(s);
    return currStringConst-1;
}

unsigned consts_newnumber (double d) {
    if(currNumConst == totalNumConsts) expand_num_consts();
    int l;
    for(l=0;l<currNumConst;l++){ // check if number has already been inserted
        if(*(numConsts+l) == d) return l;
    }
    double* p = numConsts+currNumConst++;
    *p = d;
    return currNumConst-1;
}

unsigned libfuncs_newused (char* s) {
    if(currNamedLibfunc == totalNamedLibfuncs) expand_lib_funcs();
    int l;
    for(l=0;l<currNamedLibfunc;l++){ // check if libfunc has already been inserted
        if(strcmp(*(namedLibfuncs+l),s)==0) return l;
    }
    char** p = namedLibfuncs+currNamedLibfunc++;
    *p = strdup(s);
    return currNamedLibfunc-1;
}

unsigned userfuncs_newfunc (SymbolTableEntry* sym) {
    printf("Symbol t: %d\n",sym->taddress);
    if(currUserFunc == totalUserFuncs) expand_user_funcs();
    int l;
    for(l=0;l<currUserFunc;l++){ // check if user func has already been inserted
        if((userFuncs+l)->address == sym->taddress) return l;
    }

    userfunc* p  = userFuncs+currUserFunc++;
    (*p).address =  sym->taddress;
    (*p).id = &sym->name[0];
    (*p).localSize = sym->totalloc;
    
    return currUserFunc-1;
}


void make_operand(expr* e, vmarg* arg){
    
    if(e == NULL) return;
    
    cout << "name:"<<e->sym.name << endl;
    cout <<"type:"<<e->type << endl;
    cout <<"scope:"<<e->sym.scopespace << endl;
    switch (e->type)
    {
    case var_e: 
    case assignexp_e:
    case tableitem_e:
    case arithexp_e:
    case boolexpr_e:
    case newtable_e: {
        // assert(e->sym);
        arg->val = e->sym.offset;
        switch (e->sym.scopespace)
        {
            case programvar: {arg->type = global_a; break;}
            case functionlocal: {arg->type = local_a; break;}
            case formalarg: {arg->type = formal_a; break;}
            default: {assert(0); break;}
        }
        break;
    }
    case constbool_e: {
        arg->val = e->boolConst;
        arg->type = bool_a;
        break;
    }
    case conststring_e: {
        arg->val = consts_newstring(&e->strConst[0]);
        arg->type = string_a;
        break;
    }
    case constnum_e: {
        
        arg->val = consts_newnumber(e->numConst);
        arg->type =  number_a;
        break;
    }
    case nil_e: {arg->type = nil_a;break;}

    /*FUNCTION*/
    case programfunc_e: {
        arg->type = userfunc_a;
        arg->val = userfuncs_newfunc(&e->sym);
        break;
    }

    case libraryfunc_e: {
        arg->type = libfunc_a;
        arg->val = libfuncs_newused(&e->sym.name[0]);
        break;
    }
    default: assert(0);
    }
    
}


void make_numberoperand(vmarg * arg, double val){ 
    arg->val = consts_newnumber(val);
    arg->type = number_a;
}
void make_booloperand(vmarg * arg, unsigned val){ 
    arg->val = val;
    arg->type = bool_a;
}
void make_retvaloperand(vmarg * arg){ 
    arg->type = retval_a;
}



void generate(vmopcode op, quad *quad){

    instruction t;
    t.opcode = op;
    t.srcLine = quad->line;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    reset_operand(&t.result);

    make_operand(quad->arg1, &t.arg1);
    make_operand(quad->arg2, &t.arg2);
    make_operand(quad->result, &t.result);
    quad->taddress = nextinstructionlabel();
    emit_i(t);
    currprocquad++;
}
void generate_ADD(quad* quad){
    generate(add_v,quad);
}
void generate_SUB(quad* quad){
    generate(sub_v,quad);
}
void generate_MUL(quad* quad){
    generate(mul_v,quad);
}
void generate_DIV(quad* quad){
    generate(div_v,quad);
}
void generate_MOD(quad* quad){
    generate(mod_v,quad);
}
void generate_NEWTABLE(quad* quad){
    generate(newtable_v,quad);
}
void generate_TABLEGETELEM(quad* quad){
    generate(tablegetelem_v,quad);
}
void generate_TABLESETELEM(quad* quad){
    generate(tablesetelem_v,quad);
}
void generate_ASSIGN(quad* quad){
    generate(assign_v,quad);
}
void generate_NOP(quad* quad){
    instruction t;
    t.opcode = nop_v;
    emit_i(t);
    currprocquad++;
}


int currproccessdquad(){
    return currprocquad;
}

void generate_relational(vmopcode op, quad* quad){

    instruction t;
    t.opcode = op;
    t.srcLine = quad->line;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    reset_operand(&t.result);

    make_operand(quad->arg1, &t.arg1);
    make_operand(quad->arg2, &t.arg2);
    t.result.type = label_a;

    if(quad->label < currproccessdquad()) {
        t.result.val = quads.at(quad->label).taddress;
    }else{
        add_incomplete_jump(nextinstructionlabel(),quad->label);
    }
    quad->taddress = nextinstructionlabel();
    emit_i(t);
    currprocquad++;
}

/* Relational */
void generate_JUMP(quad* quad){generate_relational(jump_v,quad);}
void generate_IF_EQ(quad* quad){generate_relational(jeq_v,quad);}
void generate_IF_NOTEQ(quad* quad){generate_relational(jne_v,quad);}
void generate_IF_LESS(quad* quad){generate_relational(jlt_v,quad);}
void generate_IF_LESSEQ(quad* quad){generate_relational(jle_v,quad);}
void generate_IF_GREATER(quad* quad){generate_relational(jgt_v,quad);}
void generate_IF_GREATEREQ(quad* quad){generate_relational(jge_v,quad);}
/*Function Call*/
void generate_PARAM(quad* quad){
    quad->taddress = nextinstructionlabel();
    instruction t;
    t.srcLine = quad->line;
    t.opcode = pusharg_v;
    t.arg1.type = None;
    t.arg1.val = -1;
    t.arg2.type = None;
    t.arg2.val = -1;
    t.result.type = None;
    t.result.val = -1;
    make_operand(quad->arg1, &t.arg1);
    emit_i(t);
    currprocquad++;
}
void generate_CALL(quad* quad){
    quad->taddress = nextinstructionlabel();
    instruction t;
    t.srcLine = quad->line;
    t.arg1.type = None;
    t.arg1.val = -1;
    t.arg2.type = None;
    t.arg2.val = -1;
    t.result.type = None;
    t.result.val = - 1;
    t.opcode = call_v;
    make_operand(quad->arg1, &t.arg1);
    emit_i(t);
    currprocquad++;
}
void generate_GETRETVAL(quad* quad){
    quad->taddress = nextinstructionlabel();
    instruction t;
    t.opcode = assign_v;
    t.srcLine = quad->line;
    t.arg1.type = None;
    t.arg1.val = -1;
    t.arg2.type = None;
    t.arg2.val = -1;
    t.result.type = None;
    t.result.val = - 1;
    make_operand(quad->result, &t.result);
    make_retvaloperand(&t.arg1);
    emit_i(t);
    currprocquad++;
}
/*Function Definition*/
void generate_FUNCSTART(quad* quad) {
    instruction t;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    reset_operand(&t.result);
    t.opcode = funcenter_v;
    t.srcLine = quad->line;
    make_operand(quad->arg1, &t.result);
    funcstack.push(t.result.val);
    emit_i(t);
    currprocquad++;
}


void generate_FUNCEND(quad* quad){

    quad->taddress = nextinstructionlabel();
    instruction t;
    t.opcode = funcexit_v;
    t.srcLine = quad->line;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    reset_operand(&t.result);

    t.result.type = userfunc_a; // instead of make operand
    t.result.val = funcstack.top();
    funcstack.pop();

    emit_i(t);
    currprocquad++;
}
void reset_operand(vmarg* arg){
    arg->type = None;
    arg->val = -1;
}

void generate_RETURN(quad* quad){
    quad->taddress = nextinstructionlabel();
    instruction t;
    t.opcode = assign_v;
    t.srcLine = quad->line;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    reset_operand(&t.result);
    make_retvaloperand(&t.result);
    make_operand(quad->arg1, &t.arg1);
    emit_i(t);

    currprocquad++;
}

void add_incomplete_jump(unsigned instrNo,unsigned iaddress){
    struct incomplete_jump* new_node = (struct incomplete_jump*) malloc(sizeof(struct incomplete_jump)); 
   
    /* 2. put in the data  */
    new_node->iaddress  = iaddress; 
    new_node->instrNo = instrNo;
    /* 3. Make next of new node as head */
    new_node->next = (ij_head); 
   
    /* 4. move the head to point to the new node */
    (ij_head)    = new_node; 
    ij_total++;
}

void patch_incomplete_jumps(){
    incomplete_jump* tmp = ij_head;
    
    for(int i = 0;i<ij_total-1;i++){
        if(tmp->iaddress == currinstruction){
            instructions[tmp->instrNo].result.val = currinstruction;
        }else{
            instructions[tmp->instrNo].result.val = quads[tmp->iaddress].taddress;
        }
        tmp = tmp->next;
    }
}


void generate_all () {
    unsigned int i;
    for(i = 0; i< currQuad; ++i){
        cout << "op:"<<quads.at(i).op <<endl;
        (*generators[quads.at(i).op])(&quads.at(i));
    }
    patch_incomplete_jumps();

    /*Genarating Binary File */
    generate_bfile();
    /*Reading Binary File */
    // read_bfile();
}


void match_op_i(vmopcode code){
    char s_code[20] = "";
    switch(code){
        case assign_v:
            strcpy(s_code,"assign");
            break;
        case add_v:
            strcpy(s_code,"add");
            break;
        case sub_v:
            strcpy(s_code,"sub");
            break;
        case mul_v:
            strcpy(s_code,"mul");
            break;
        case div_v:
            strcpy(s_code,"div");
            break;
        case mod_v:
            strcpy(s_code,"mod");
            break;
        case jeq_v:
            strcpy(s_code,"jeq");
            break;
        case jne_v:
            strcpy(s_code,"jne");
            break;
        case jle_v:
            strcpy(s_code,"jle");
            break;
        case jge_v:
            strcpy(s_code,"jge");
            break;
        case jlt_v:
            strcpy(s_code,"jlt");
            break;
        case jgt_v:
            strcpy(s_code,"jgt");
            break;
        case jump_v:
            strcpy(s_code,"jump");
            break;
        case call_v:
            strcpy(s_code,"call");
            break;
        case pusharg_v:
            strcpy(s_code,"pusharg");
            break;
        case return_v:
            strcpy(s_code,"return");
            break;
        case getretval_v:
            strcpy(s_code,"getretval");
            break;
        case funcenter_v:
            strcpy(s_code,"funcenter");
            break;
        case funcexit_v:
            strcpy(s_code,"funcexit");
            break;
        case newtable_v:
            strcpy(s_code,"newtable");
            break;
        case tablegetelem_v:
            strcpy(s_code,"tablegetelem");
            break;
        case tablesetelem_v:
            strcpy(s_code,"tablesetelem");
            break;
        default: 
            strcpy(s_code,"NO OPCODE");
            break;
    }
    printf("%20s",s_code);
}


char * match_type(vmarg_t type){
    char s_code[20] = "";
    switch(type){
        case label_a:
            return "label";
            break;
        case global_a:
            return "global";
            break;
        case formal_a:
            return "formal";
            break;
        case local_a:
            return "local";
            break;
        case number_a:
            return "number";
            break;
        case string_a:
            return "string";
            break;
        case bool_a:
            return "bool";
            break;
        case nil_a:
            return "nil";
            break;
        case userfunc_a:
            return "userfunc";
            break;
        case libfunc_a:
            return "libfunc";
            break;
        case retval_a:
            return "retval";
            break;
        default: assert(0);
    }
    assert(0);
}

void match_vmarg(vmarg * arg){

    
    if(arg == NULL){
        printf("%25s","-");
    }else if(arg->type == None && arg->val == -1){
        printf("%25s","-");
    }
    else{

        int len_start = snprintf( NULL, 0, "%d", arg->type);
        char* start_line_string = (char*)malloc( len_start + 1 );
        snprintf( start_line_string, len_start + 1, "%d", arg->type );

        int len_val = snprintf( NULL, 0, "%d", arg->val);
        char* val_str = (char*)malloc( len_start + 1 );
        snprintf( val_str, len_val + 1, "%d", arg->val );
        
        char *full = (char*)malloc(50);
        char *s = match_type(arg->type);
        if(arg->type !=None){
            strcat(full,"(");
            strcat(full,start_line_string);
            strcat(full," ");
            strcat(full,s);
        }
        strcat(full,")");
        if(arg->val!=-1){
            strcat(full," , (");
            strcat(full,val_str);
            strcat(full," offset)");
        }

        printf("%35s",full);
    }

}

void print_tables(){

    printf("--------Const Numbers ---------\n");
    printf("%5s %20s\n", "Index#", "number");
    int i;
    for (i = 0; i < currNumConst; i++)
    {
        printf("%4d: %20f\n",i,*(numConsts+i));
    }
    printf("\n");

    printf("--------Const Strings ---------\n");
    printf("%5s %20s\n", "Index#", "string");
    for (i = 0; i < currStringConst; i++)
    {
        printf("%4d: %20s\n",i,*(numStrings+i));
    }
    printf("\n");

    printf("--------Used Lib Funcs ---------\n");
    printf("%5s %20s\n", "Index#", "function");
    for (i = 0; i < currNamedLibfunc; i++)
    {
        printf("%4d: %20s\n",i,*(namedLibfuncs+i));
    }
    printf("\n");

    printf("-------- User Funcs ---------\n");
    printf("%5s %20s\n", "Index#", "function");
    for (i = 0; i < currUserFunc; i++)
    {
        printf("%4d: %20s\n",i,(*(userFuncs+i)).id);
    }
    printf("\n");
    

}

void print_instructions(){
    print_tables();
    int i = 1;
    printf("%5s %20s %30s %30s %30s", "instr#", "opcode","result","arg1","arg2");
    printf("\n------------------------------------------------------------");
    printf("----------------------------------------------------\n");
    while (i<currinstruction)
    {
        printf("%4d: ", i);
        match_op_i(instructions[i].opcode);
        match_vmarg(&instructions[i].result);
        match_vmarg(&instructions[i].arg1);
        match_vmarg(&instructions[i].arg2);
        printf("\n");
        i++;
    }
}

void generate_bfile(void){
    FILE *bin_file;
    int i;
    bin_file = fopen("xyz.abc","wb");
    if(!bin_file) {
        fprintf(stderr,"Unable to write binary file");
        return;
    }
    /* WRITE MAGIC NUMBER */
    unsigned int magic_number = 340200501;
    fwrite(&magic_number,sizeof(unsigned int),1,bin_file);

    /* WRITE STRINGS */
    fwrite(&currStringConst,sizeof(unsigned int),1,bin_file);
    for (i = 0; i < currStringConst; i++)
    {
        unsigned size = strlen(*(numStrings+i));
        fwrite(&size,sizeof(unsigned),1,bin_file);
        fwrite(*(numStrings+i),sizeof(char),size,bin_file);
    }

    /* WRITE NUMBERS */
    fwrite(&currNumConst,sizeof(unsigned int),1,bin_file);
    for (i = 0; i < currNumConst; i++)
    {
        fwrite(numConsts+i,sizeof(double),1,bin_file);
    }

    /* WRITE USER FUNCTIONS */
    fwrite(&currUserFunc,sizeof(unsigned int),1,bin_file);
    for (i = 0; i < currUserFunc; i++)
    {
        fwrite(&(userFuncs+i)->address,sizeof(unsigned),1,bin_file);
        fwrite(&(userFuncs+i)->localSize,sizeof(unsigned),1,bin_file);
        unsigned size = strlen((userFuncs+i)->id);
        fwrite(&size,sizeof(unsigned),1,bin_file);
        fwrite((userFuncs+i)->id,sizeof(char),size,bin_file);
    }

    /* WRITE USED LIBS */
    fwrite(&currNamedLibfunc,sizeof(unsigned int),1,bin_file);
    for (i = 0; i < currNamedLibfunc; i++)
    {   
        unsigned size = strlen(*(namedLibfuncs+i));
        fwrite(&size,sizeof(unsigned),1,bin_file);
        fwrite(*(namedLibfuncs+i),sizeof(char),size,bin_file);
    }

    /* WRITE INSTRUNCTIONS */
    fwrite(&currinstruction,sizeof(unsigned int),1,bin_file);
    for (i = 0; i < currinstruction; i++){
        struct binary_instr temp;
        temp.op = instructions[i].opcode;
        temp.res_val = instructions[i].result.val;
        temp.res_type = instructions[i].result.type;
        temp.arg1_val = instructions[i].arg1.val;
        temp.arg1_type = instructions[i].arg1.type;
        temp.arg2_val = instructions[i].arg2.val;
        temp.arg2_type = instructions[i].arg2.type;
        // printf("srcLINE %d\n",instructions[i].srcLine);
        temp.src_line = instructions[i].srcLine;
        fwrite(&temp,sizeof(struct binary_instr),1,bin_file);
    }

    int globalC = getNumOfGlobals();
    fwrite(&globalC,sizeof(int),1,bin_file);




    
    fclose(bin_file);
}