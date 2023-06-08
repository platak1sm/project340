#include <stdint.h>
enum vmopcode {
    assign_v, add_v, sub_v,
    mul_v,  div_v, mod_v,
    jump_v, and_v, or_v,
    not_v,  jeq_v, jne_v,
    jle_v, jge_v, jlt_v,
    jgt_v, call_v, pusharg_v, getretval_v,
    funcenter_v, return_v, funcexit_v, newtable_v,
    tablegetelem_v, tablesetelem_v, nop_v, 
}typedef vmopcode;

enum vmarg_t {
    label_a = 0,
    global_a = 1,
    formal_a = 2,
    local_a = 3,
    number_a = 4,
    string_a = 5,
    bool_a = 6,
    nil_a = 7,
    userfunc_a = 8,
    libfunc_a = 9,
    retval_a = 10,
    None=11
}typedef vmarg_t;

struct vmarg {
    enum vmarg_t type;
    unsigned val;
} typedef vmarg ;

struct instruction {
    enum vmopcode opcode;
    vmarg result;
    vmarg arg1;
    vmarg arg2;
    unsigned srcLine;
    // unsigned label;
} typedef instruction;

struct userfunc {
    unsigned address;
    unsigned localSize;
    char* id;
} typedef userfunc;


struct incomplete_jump 
{
    unsigned instrNo;    //jump instruction number
    unsigned iaddress;   //i-code jump-target address
    struct incomplete_jump* next;
} typedef incomplete_jump;


struct binary_instr{
    uint8_t op;
    uint8_t res_type;
    unsigned res_val;
    uint8_t arg1_type;
    unsigned arg1_val;
    uint8_t arg2_type;
    unsigned arg2_val;
    unsigned src_line;
};