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
    boolexpr_e,
    assignexp_e,
    newtable_e,
    constnum_e,
    constbool_e,
    conststring_e,
    nil_e
} expr_t;

typedef struct expr
{
    expr_t type;
    symbol *sym;
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

typedef struct symbol
{
    symbol_t type;
    string name;
    scopespace_t space;
    unsigned offset;
    unsigned scope;
    unsigned line;
    bool isActive;
} symbol;

typedef struct stmt_t
{
    int breakList, contList;
} stmt_t;

/* void expand();*/

void emit(iopcode op, expr *arg1, expr *arg2, expr *result, unsigned label, unsigned line);

expr *emit_iftableitem(expr *e);

expr *newexpr(expr_t t);

symbol *newtmp();

scopespace_t currscopespace(void);

unsigned currscopeoffset(void);

void inccurrscopeoffset(void);

void enterscopespace(void);

void exitscopespace(void);

void patchlabel(unsigned quadNo, unsigned label);

expr *newexpr_constbool(unsigned int b);

unsigned nextquad(void);

void make_stmt(stmt_t *s);

int newlist(int i); // de kserw an xreiazetai

int mergelist(int l1, int l2); // de kserw an xreiazetai

void patchlist(int list, int label); // de kserw an xreiazetai

void resetformalargoffset(void);

void resetfunctionlocaloffset(void);

void restorecurrscopeoffset(unsigned n);

symbol *lookup(string name);

void insertsym(symbol *sym);

expr *make_call(expr *lv, expr *reversed_elist);

expr *newexpr_constnum(double i);

void check_arith(expr *e, string context);

bool istempname(string s);

bool istempexpr(expr *e);