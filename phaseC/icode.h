#include "symbol_table.h"

#define EXPAND_SIZE 1024
#define CURR_SIZE (total * sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE * sizeof(quad) + CURR_SIZE)

extern int yylineno, scope;

typedef enum iopcode
{
    assign,
    add,
    sub,
    mul,
    div,
    mod,
    uminus,
    andc,
    orc,
    notc,
    if_eq,
    if_noteq,
    if_lesseq,
    if_greatereq,
    if_less,
    if_greater,
    call,
    param,
    ret,
    getretval,
    funcstart,
    funcend,
    tablecreate,
    tablegetelem,
    tablesetelem,
    jump
} iopcode;

typedef enum expr_t
{
    var_e,
    tableitem_e,
    programfunc_e,
    libraryfunc_e,
    arithexp_e,
    boolexp_e,
    assignexp_e,
    newtable_e,
    costnum_e,
    constbool_e,
    conststring_e,
    nil_e
} expr_t;

typedef struct expr
{
    expr_t type;
    SymbolTableEntry *sym;
    expr *index;
    double numConst;
    string strConst;
    bool boolConst;
    expr *next;
} expr;

typedef struct quad
{
    iopcode op;
    expr *result;
    expr *arg1;
    expr *arg2;
    unsigned label;
    unsigned line;
} quad;

typedef enum scopespace_t
{
    programvar,
    functionlocal,
    formalarg
} scopespace_t;

typedef enum symbol_t
{
    var_s,
    programfunc_s,
    libraryfunc_s
} symbol_t;

struct symbol
{
    symbol_t type;
    string name;
    scopespace_t space;
    unsigned offset;
    unsigned scope;
    unsigned line;
};
/* void expand();*/

void emit(iopcode op, expr *arg1, expr *arg2, expr *result, unsigned label, unsigned line);

expr *emit_iftableitem(expr *e, int yylineno);

expr *newexpr(expr_t t);

SymbolTableEntry *newtmp();

scopespace_t currscopespace(void);

unsigned currscopeoffset(void);

void inccurrscopeoffset(void);

void enterscopespace(void);

void exitscopespace(void);