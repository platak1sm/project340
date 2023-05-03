#include "icode.h"
#include <vector>
#include <assert.h>
vector<quad> quads;
unsigned total = 0, programVarOffset = 0, functionLocalOffset = 0, formalArgOffset = 0, scopeSpaceCounter = 1;
unsigned int currQuad = 0;
bool isMember;
int tmpc = 0; // tmp counter


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
    return e;
}

SymbolTableEntry newtmp()
{ /*mporei na thelei diorthwsi*/
    string name = "$t" + to_string(tmpc++);
    SymbolTableEntry sym;
    if (lookupactivevar(name).isActive == false && lookupactivefunc(name).isActive==false)
    {
        sym.name = name;
        sym.scope = 0;
        sym.line = 0;
        sym.isActive = true;
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
    s->breakList = s->contList = 0;
}

int newlist(int i) // de kserw an xreiazetai
{
    quads[i].label = 0;
    return i;
}

int mergelist(int l1, int l2) // de kserw an xreiazetai
{
    if (!l1)
        return l2;
    else if (!l2)
        return l1;
    else
    {
        int i = l1;
        while (quads[i].label)
            i = quads[i].label;
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
    
    expr *e;
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

expr *make_call(expr *lv, expr *reversed_elist)
{
    expr *func = emit_iftableitem(lv);
    while (reversed_elist)
    {
        emit(param, reversed_elist, NULL, NULL, 0, yylineno);
        reversed_elist = reversed_elist->next;
    }
    emit(call, func, NULL, NULL, 0, yylineno);
    expr *result = newexpr(var_e);
    result->sym = newtmp();
    emit(getretval, NULL, NULL, result, 0, yylineno);
    return result;
}

expr* newexpr_constnum(double i) {
	expr *e = newexpr(constnum_e);
	e->numConst = i;
	return e;
}

void check_arith(expr* e, string context) {
	if (e->type == constbool_e ||
	e->type == conststring_e ||
	e->type == nil_e ||
	e->type == newtable_e ||
	e->type == programfunc_e ||
	e->type == libraryfunc_e ||
	e->type == boolexpr_e ){
		printf("Illegal expr used in %s!", context);
		//exit(0);
	}
}

bool istempname(string s) { return s[0] == '$'; }

//bool istempexpr(expr *e) { return e->sym && istempname(e->sym.name); }

expr *member_item(expr *lvalue,string name){
    lvalue = emit_iftableitem(lvalue);
    expr *item = newexpr(tableitem_e);
    item->sym = lvalue->sym;
    expr *temp = newexpr(conststring_e);
    temp->strConst=name;
    item->index = temp;
    return item;
}