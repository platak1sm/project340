#include "icode.h"
#include <vector>
#include <assert.h>
#include <stack>
#include <list>
#include <string.h>

// using namespace std;

vector<quad> quads;
unsigned totalLocals = 0, programVarOffset = 0, functionLocalOffset = 0, formalArgOffset = 0, scopeSpaceCounter = 1;
unsigned int currQuad = 0;
extern bool isMember;
int tmpc = 0; // tmp counter
extern stack<unsigned> funcLocalStack;
extern stack<unsigned> loopCountStack;

void emit(iopcode op, expr *arg1, expr *arg2, expr *result, unsigned label, unsigned line)
{ /*mallon correct alla check it*/
    currQuad++;
    quad q;
    q.op = op;
    q.arg1 = arg1;
    q.arg2 = arg2;
    q.result = result;
    q.label = label;
    q.line = line;
    quads.push_back(q);
}

expr *emit_iftableitem(expr *e)
{
    if (e->type != tableitem_e)
        return e;
    else
    {
        expr *result = newexpr(var_e);
        result->sym = newtmp();
        emit(tablegetelem, result, e, e->index, -1, yylineno);
        return result;
    }
}

expr *newexpr(expr_t t)
{ /* thelei diorthwsi probably*/
    expr *e = new expr;
    // memset(e, 0, sizeof(expr));
    e->type = t;
    if (t == boolexpr_e)
    {
        e->truequad = 0;
        e->falsequad = 0;
    }
    return e;
}

SymbolTableEntry newtmp()
{ /*mporei na thelei diorthwsi*/
    string name = "$t" + to_string(tmpc++);
    SymbolTableEntry sym;
    if (lookupactivevar(name).isActive == false && lookupactivefunc(name).isActive == false)
    {
        sym.name = name;
        sym.scope = 0;
        sym.line = 0;
        sym.isActive = true;
        sym.scopespace = currscopespace();
        sym.offset = currscopeoffset();
        insert(sym);
    }
    return sym;
}

scopespace_t currscopespace(void)
{
    if (scopeSpaceCounter == 1)
        return programvar;
    else if (scopeSpaceCounter % 2 == 0)
        return formalarg;
    else
        return functionlocal;
}

unsigned currscopeoffset(void)
{
    switch (currscopespace())
    {
    case programvar:
        return programVarOffset;
    case functionlocal:
        return functionLocalOffset;
    case formalarg:
        return formalArgOffset;
    default:
        assert(0);
    }
}

void inccurrscopeoffset(void)
{
    switch (currscopespace())
    {
    case programvar:
        ++programVarOffset;
        break;
    case functionlocal:
        ++functionLocalOffset;
        break;
    case formalarg:
        ++formalArgOffset;
        break;
    default:
        assert(0);
    }
}

void enterscopespace(void) { ++scopeSpaceCounter; }

void exitscopespace(void)
{
    assert(scopeSpaceCounter > 1);
    --scopeSpaceCounter;
}

void patchlabel(unsigned quadNo, unsigned label)
{
    assert(quadNo < currQuad && !quads[quadNo].label);
    quads[quadNo].label = label;
}

expr *newexpr_constbool(unsigned int b)
{
    expr *e = newexpr(constbool_e);
    e->boolConst = !!b;
    return e;
}

unsigned nextquad(void) { return currQuad; }

void make_stmt(stmt_t *s)
{
    if (s != NULL)
    {
        if (s->breakList != 0)
        {
            s->breakList = 0;
        }
        if (s->contList != 0)
        {
            s->contList = 0;
        }
    }
}

int newlist(int i) // de kserw an xreiazetai
{
    quads[i].label = 0;
    return i;
}

int mergelist(int l1, int l2) // de kserw an xreiazetai
{
    if (!l1 || l1 < 0 || l1 >= quads.size())
        return l2;
    else if (!l2 || l2 < 0 || l2 > quads.size())
        return l1;
    else
    {
        int i = l1;
        cout << "quads:" << i << "," << quads.size() << endl;

        while ((i >= 0) && (i < quads.size()) && quads[i].label)
        {

            cout << "quads:" << quads[i].label << endl;
            i = quads[i].label;
        }
        quads[i].label = l2;
        return l1;
    }
}

void patchlist(int list, int label) // de kserw an xreiazetai
{
    while (list)
    {
        int next = quads[list].label;
        quads[list].label = label;
        list = next;
    }
}

void resetformalargoffset(void) { formalArgOffset = 0; }

void resetfunctionlocaloffset(void) { functionLocalOffset = 0; }

void restorecurrscopeoffset(unsigned n)
{
    switch (currscopespace())
    {
    case programvar:
        programVarOffset = n;
        break;
    case functionlocal:
        functionLocalOffset = n;
        break;
    case formalarg:
        formalArgOffset = n;
        break;
    default:
        assert(0);
    }
}

expr *lvalue_exp(SymbolTableEntry sym) // thelei diorthwsh
{

    expr *e = new expr;
    switch (sym.symt)
    {
    case var_s:
        e->type = var_e;
        break;
    case programfunc_s:
        e->type = programfunc_e;
        break;
    case libraryfunc_s:
        e->type = libraryfunc_e;
        break;
    default:
        assert(0);
    }
    e->sym = sym;
    return e;
}

expr *newexpr_conststring(string s)
{
    expr *e = newexpr(conststring_e);
    e->strConst = s;
    return e;
}

expr *make_call(expr *lv, elist_t *reversed_elist)
{
    expr *func = emit_iftableitem(lv);
    for (int i = reversed_elist->elist.size() - 1; i >= 0; --i)
    {
        emit(param, reversed_elist->elist[i], NULL, NULL, 0, yylineno);
    }
    emit(call, func, NULL, NULL, 0, yylineno);
    expr *result = newexpr(var_e);
    if (result != NULL)
    {
        result->sym = newtmp();
        emit(getretval, NULL, NULL, result, 0, yylineno);
        return result;
    }
}

expr *newexpr_constnum(double i)
{
    expr *e = newexpr(constnum_e);
    e->numConst = i;
    return e;
}

void check_arith(expr *e)
{
    if (e->type == constbool_e ||
        e->type == conststring_e ||
        e->type == nil_e ||
        e->type == newtable_e ||
        e->type == programfunc_e ||
        e->type == libraryfunc_e ||
        e->type == boolexpr_e)
    {
    }
}
void resettmpcounter()
{
    tmpc = 0;
}

bool istempname(string s) { return s[0] == '$'; }

// bool istempexpr(expr *e) { return e->sym && istempname(e->sym.name); }

expr *member_item(expr *lvalue, string name)
{
    lvalue = emit_iftableitem(lvalue);
    expr *item = newexpr(tableitem_e);
    item->sym = lvalue->sym;
    expr *temp = newexpr(conststring_e);
    temp->strConst = name;
    item->index = temp;
    return item;
}

void match_expr(expr *ex)
{
    if (ex == NULL)
    {
        cout << "-";
    }
    else if (ex->type == constnum_e)
    {
        cout << ex->numConst;
    }
    else if (ex->type == constbool_e)
    {
        if (ex->boolConst == 0)
            cout << "FALSE";
        else
            cout << "TRUE";
    }
    else if (ex->type == conststring_e)
    {
        cout << ex->strConst;
    }
    else if (ex->type == nil_e)
    {
        cout << "NILL";
    }
    else if (ex->type == tableitem_e)
    {
        cout << ex->sym.name;
    }
    else
    {
        cout << ex->sym.name;
    }
}

void match_op(iopcode code)
{
    char s_code[20] = "";
    switch (code)
    {
    case assign:
        strcpy(s_code, "assign");
        // printf("assign");
        break;
    case add:
        strcpy(s_code, "add");
        break;
    case sub:
        strcpy(s_code, "sub");
        break;
    case mul:
        strcpy(s_code, "mul");
        // printf("mul");
        break;
    case divc:
        // printf("a_div");
        strcpy(s_code, "div");
        break;
    case mod:
        strcpy(s_code, "mod");
        // printf("mul");
        break;
    case if_eq:
        // printf("if_eq");
        strcpy(s_code, "if_eq");
        break;
    case if_noteq:
        // printf("if_noteq");
        strcpy(s_code, "if_noteq");
        break;
    case if_lesseq:
        // printf("if_lesseq");
        strcpy(s_code, "if_lesseq");
        break;
    case if_greatereq:
        // printf("if_greatereq");
        strcpy(s_code, "if_greatereq");
        break;
    case if_less:
        // printf("if_less");
        strcpy(s_code, "if_less");
        break;
    case if_greater:
        // printf("if_greater");
        strcpy(s_code, "if_greater");
        break;
    case jump:
        strcpy(s_code, "jump");
        break;
    case call:
        // printf("call");
        strcpy(s_code, "call");
        break;
    case param:
        // printf("param");
        strcpy(s_code, "param");
        break;
    case ret:
        // printf("ret");
        strcpy(s_code, "return");
        break;
    case getretval:
        // printf("getretval");
        strcpy(s_code, "getretval");
        break;
    case funcstart:
        // printf("funcstart");
        strcpy(s_code, "funcstart");
        break;
    case funcend:
        // printf("funcend");
        strcpy(s_code, "funcend");
        break;
    case tablecreate:
        // printf("tablecreate");
        strcpy(s_code, "tablecreate");
        break;
    case tablegetelem:
        // printf("tablegetelem");
        strcpy(s_code, "tablegetelem");
        break;
    case tablesetelem:
        // printf("tablesetelem");
        strcpy(s_code, "tablesetelem");
        break;
    default:
        strcpy(s_code, "NO OPCODE");
        break;
    }
    cout << s_code;
}

void match_label(int val)
{
    if (val > 0)
    {
        cout << val;
    }
    else
    {
        cout << '-';
    }
}

void print_quads()
{
    int i = 0;
    printf("quad len:%d\n", quads.size());
    printf("%5s %20s %20s %20s %20s %20s", "quad#", "opcode", "result", "arg1", "arg2", "label");
    printf("\n------------------------------------------------------------");
    printf("----------------------------------------------------\n");
    while (i < currQuad)
    {
        
        printf("%4d: ", i);
        cout << "\t\t\t";
        match_op(quads[i].op);
        cout << "\t\t\t";
        match_expr(quads[i].result);
        cout << "\t\t";
        match_expr(quads[i].arg1);
        cout << "\t\t\t";
        match_expr(quads[i].arg2);
        if (quads[i].op == if_greatereq || quads[i].op == if_eq || quads[i].op == if_noteq || quads[i].op == if_lesseq || quads[i].op == if_greatereq || quads[i].op == if_less || quads[i].op == if_greater || quads[i].op == jump)
        {
            match_label(quads[i].label);
        }
        else
        {
            printf("%20s", "-");
        }

        printf("\n");
        i++;
    }
}