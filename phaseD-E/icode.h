#include "symbol_table.h"
#include "phase4.h"
#include <string.h>

/* #define EXPAND_SIZE 1024
#define CURR_SIZE (total * sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE * sizeof(quad) + CURR_SIZE) */

extern int yylineno, scope; /*,tmpc*/

typedef enum iopcode
{
    assign,
    add,
    sub,
    mul,
    divc,
    mod,
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
    jump,
    nop
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
    SymbolTableEntry sym;
    expr *index;
    double numConst;
    string strConst;
    bool boolConst;
    int truequad;
    int falsequad;
    expr *next;

    expr(){};
} expr;

typedef struct quad
{
    iopcode op;
    expr *result;
    expr *arg1;
    expr *arg2;
    unsigned taddress;
    unsigned label;
    unsigned line;
} quad;

extern vector<quad> quads;
extern unsigned int currQuad;

typedef struct elist_t
{
    vector<expr *>elist;
    elist_t(){
    };
} elist_t;

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
    SymbolTableEntry sym;
} stmt_t;

typedef struct indexedelements
{
    expr *index;
    expr *value;
    indexedelements *next;
} indexedelements;

typedef struct calls
{
    string name;
    bool method;
    elist_t *elist;
} calls;


typedef struct forpr
{
    unsigned test;
    unsigned enter;
    
} forpr;

/* void expand();*/

void emit(iopcode op, expr *arg1, expr *arg2, expr *result, unsigned label, unsigned line);

expr *emit_iftableitem(expr *e);

expr *newexpr(expr_t t);

SymbolTableEntry newtmp();

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

expr *make_call(expr *lv, elist_t *reversed_elist);

expr *newexpr_constnum(double i);

expr *newexpr_conststring(string s);

void check_arith(expr *e);

bool istempname(string s);

bool istempexpr(expr *e);

expr *member_item(expr *lvalue, string name);

void resettmpcounter();

void match_expr(expr* ex);

void match_op(iopcode code);

void print_quads();

expr *lvalue_exp(SymbolTableEntry sym);
