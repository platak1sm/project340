#include "icode.h"
#include <stack>

#define EXPAND_SIZE_I 1024
#define CURR_SIZE_I (total_i* sizeof(instruction))
#define NEW_SIZE_I (EXPAND_SIZE_I* sizeof(instruction)+ CURR_SIZE_I)

extern stack<int> funcstack;
unsigned nextinstructionlabel();

void add_incomplete_jump (unsigned instrNo, unsigned iaddress);
void patch_incomplete_jumps();

void userfunctions_add(unsigned address,unsigned localsize, char* id);

void generate(vmopcode op, quad* quad);
void generate_relational(vmopcode op, quad* quad);
void generate_ADD(quad* quad);
void generate_SUB(quad* quad);
void generate_MUL(quad* quad);
void generate_DIV(quad* quad);
void generate_MOD(quad* quad);
void generate_NEWTABLE(quad* quad);
void generate_TABLEGETELEM(quad* quad);
void generate_TABLESETELEM(quad* quad);
void generate_ASSIGN(quad* quad);
void generate_NOP(quad* quad);

void generate_JUMP(quad* quad);
void generate_IF_EQ(quad* quad);
void generate_IF_NOTEQ(quad* quad);
void generate_IF_LESS(quad* quad);
void generate_IF_LESSEQ(quad* quad);
void generate_IF_GREATER(quad* quad);
void generate_IF_GREATEREQ(quad* quad);

void generate_PARAM(quad* quad);
void generate_CALL(quad* quad);
void generate_GETRETVAL(quad* quad);

void generate_FUNCSTART(quad* quad);
void generate_FUNCEND(quad* quad);
void generate_RETURN(quad* quad);


typedef void (*generator_func_t) (quad *);

void match_op_i(vmopcode code);
void generate_all (void);
void print_instructions(void);


unsigned consts_newstring (char* s);
unsigned consts_newnumber (double n);
unsigned libfuncs_newused (char* s);
unsigned userfuncs_newfunc (SymbolTableEntry* sym);

void make_operand(expr* e, vmarg* arg);
void emit_i(instruction t);
void expand_i();
int currproccessdquad();