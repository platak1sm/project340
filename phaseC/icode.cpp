#include "icode.h"
#include <vector>
#include <assert.h>
vector<quad> quads;
unsigned total = 0, programVarOffset = 0, functionLocalOffset = 0, formalArgOffset = 0, scopeSpaceCounter = 1;
unsigned int currQuad = 0;
int tmpc = 0; // tmp counter

/* void expand(){
    assert(total == currQuad);
    quad * p = (quad*)malloc(NEW_SIZE);
    if(quads){
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }
    quads = p;
    total += EXPAND_SIZE;
} */

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

expr *emit_iftableitem(expr *e, int yylineno)
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

SymbolTableEntry *newtmp()
{ /*mporei na thelei diorthwsi*/
    string name = "$t" + to_string(tmpc++);
    SymbolTableEntry ste = lookupactivevar(name);
    if (lookupactivevar(name).isActive == false && lookupactivefunc(name).isActive == false)
    {
        ste.varVal.name = name;
        ste.varVal.scope = 0;
        ste.varVal.line = 0;
        ste.isActive = true;
        insert(ste);
    }
    return &ste;
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