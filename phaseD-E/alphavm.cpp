#include "alphavm.h"
#include "math.h"
#include <stdio.h>
#include <stdarg.h>

#define AVM_STACKSIZE 4096
#define AVM_MAX_INSTRUCTIONS (unsigned)nop_v
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))
#define AVM_NUMACTUALS_OFFSET 4
#define AVM_SAVEDPC_OFFSET 3
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET 1
#define AVM_STACKENV_SIZE 4
#define PI 3.141592654

vector<double> numConst;
vector<string> stringConst;
vector<string> libFuncConst;
vector<userfunc *> avmuserFuncs;
vector<instruction *> code;
avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned codeSize;

avm_memcell stackavm[AVM_STACKSIZE];

int top, topsp;
int totalActuals = 0;
int currLine = 0;
bool executionFinished = false;
int pc = 0;

#define AVM_ENDING_PC codeSize

avm_memcell *avm_translate_operand(vmarg *arg, avm_memcell *reg)
{
    if (arg == NULL)
        return NULL;

    switch (arg->type)
    {
    case global_a:
        return &stackavm[AVM_STACKSIZE - 1 - arg->val];
    case local_a:
        return &stackavm[topsp - arg->val];
    case formal_a:
        return &stackavm[topsp + AVM_STACKENV_SIZE + 1 + arg->val];
    case retval_a:
        return &retval;
    case number_a:
    {
        reg->type = number_m;
        // reg->data.numVal=const_getnumber(arg->val);
        return reg;
    }
    case string_a:
    {
        reg->type = string_m;
        // reg->data.strVal=const_getstring(arg->val);
        return reg;
    }
    case bool_a:
    {
        reg->type = bool_m;
        // reg->data.boolVal=arg->val;
        return reg;
    }
    case nil_a:
    {
        // reg->type=nil_m;
        return reg;
    }
    case userfunc_a:
    {
        reg->type = userfunc_m;
        // reg->data.funcVal=
        return reg;
    }
    case libfunc_a:
    {
        reg->type = libfunc_m;
        // reg->data.libfuncVal=
        return reg;
    }
    break;

    default:
        break;
    }
    return NULL;
}

void avm_calllibfunc(string id)
{
    library_func_t f = avm_getlibraryfunc(id);
    if (!f)
    {
        avm_error("unsupported lib func");
        executionFinished = 1;
    }
    else
    {
        topsp = top;
        totalActuals = 0;
        (*f)();
        if (!executionFinished)
            execute_funcexit(new instruction());
    }
}

void avm_memcellclear(avm_memcell *m)
{
    if (m->type == undef_m)
    {
        return;
    }
    memclear_func_t f = memclearFuncs[m->type];
    if (f)
    {
        f(m);
    }
    m->type = undef_m;
}

void avm_callsaveenvironment()
{
    avm_push_envvalue(totalActuals);
    avm_push_envvalue(pc + 1);
    avm_push_envvalue(top + totalActuals + 2);
    avm_push_envvalue(topsp);
}

void execute_funcexit(instruction t)
{
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    while (oldTop++ <= top)
        avm_memcellclear(&stackavm[oldTop]);
}

void execute_funcenter(instruction t)
{
    avm_memcell *func = avm_translate_operand(&(t.result), &ax);
    totalActuals = 0;
    userfunc *funcInfo = avm_getfuncinfo(pc);
    topsp = top;
    top = top - funcInfo->localSize;
}

unsigned avm_totalactuals()
{
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell *avm_getactual(unsigned i)
{
    assert(i < avm_totalactuals());
    return &stackavm[topsp + AVM_STACKENV_SIZE];
}
string avm_tostring(avm_memcell *m)
{
    assert(m->type >= 0 and m->type <= undef_m);
    return tostringFuncs[m->type](m);
}

void libfunc_print()
{
    unsigned n = avm_totalactuals();
    for (unsigned i = 0; i < n; i++)
    {
        char *s = &avm_tostring(avm_getactual(i))[0];
        puts(s);
        free(s);
    }
}

void avm_error(char *format, ...)
{
    va_list args;
    fprintf(stderr, "\033[0;31mError at line %d: \033[0m", currLine);
    va_start(args, format);
    int rc = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    executionFinished = 1;
}

void avm_warning(char *format, ...)
{
    va_list args;
    fprintf(stderr,"\033[0;33mWarning at line %d: \033[0m",currLine);
    //va_start(args, format);
    int rc = vfprintf(stderr, format, args);
    fprintf(stderr,"\n");
    //va_end(args);
}

char *avm_memcell_type_to_string(avm_memcell_t t)
{
    switch (t)
    {
    case number_m:
        return "number";
    case string_m:
        return "string";
    case bool_m:
        return "boolean";
    case userfunc_m:
        return "userfunction";
    case libfunc_m:
        return "libraryfunction";
    case table_m:
        return "table";
    case nil_m:
        return "nil";
    case undef_m:
        return "undefined";
    default:
        assert(0);
    }
}

void libfunc_typeof()
{
    unsigned n = avm_totalactuals();
    if (n != 1)
    {
        avm_error("typeof takes 1 paramter, %d were given", n);
    }
    else
    {
        char *s = avm_memcell_type_to_string(avm_getactual(n - 1)->type);
        retval.type = string_m;
        retval.data.strVal = strdup(s);
    }
}

void libfunc_argument(void)
{
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    unsigned n = avm_totalactuals();
    if (n != 1)
    {
        avm_error("'argument' takes 1 paramter, %d were given", n);
    }
    avm_memcellclear(&retval);
    if (!p_topsp)
    {
        avm_warning("'argument' called outside a function!");
        retval.type = nil_m;
    }
    else if (avm_getactual(0)->type != number_m)
    {
        avm_warning("'argument' needs number as argument!");
        retval.type = nil_m;
    }
    else
    {
        int offset = avm_getactual(0)->data.numVal;
        printf("OFFSET as arg %d\n", offset);
        if (offset >= avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET))
        {
            retval.type = nil_m;
            return;
        }
        retval.type = stackavm[p_topsp + AVM_NUMACTUALS_OFFSET + 1 + offset].type;
        retval.data = stackavm[p_topsp + AVM_NUMACTUALS_OFFSET + 1 + offset].data;
    }
}

void libfunc_strtonum(void)
{
    unsigned n = avm_totalactuals();
    if (n != 1)
    {
        avm_error("'strtonum' takes 1 paramter, %d were given", n);
    }
    avm_memcellclear(&retval);
    if (avm_getactual(0)->type != string_m)
    {
        avm_warning("'strtonum' needs string as argument!");
        retval.type = nil_m;
    }
    else
    {
        char *str = avm_getactual(0)->data.strVal;
        char *other;
        double ret = 0;
        ret = strtod(str, &other);
        if (ret == 0)
        {
            retval.type = nil_m;
        }
        else
        {
            retval.type = number_m;
            retval.data.numVal = ret;
        }
    }
}

void libfunc_sqrt(void)
{
    unsigned n = avm_totalactuals();
    if (n != 1)
    {
        avm_error("'sqrt' takes 1 paramter, %d were given", n);
    }
    avm_memcellclear(&retval);
    if (avm_getactual(0)->type != number_m)
    {
        avm_warning("'sqrt' needs number as argument!");
        retval.type = nil_m;
    }
    else
    {
        double num = avm_getactual(0)->data.numVal;
        if (num < 0)
        {
            retval.type = nil_m;
        }
        else
        {
            retval.type = number_m;
            retval.data.numVal = sqrt(num);
        }
    }
}

void libfunc_cos(void)
{
    unsigned n = avm_totalactuals();
    if (n != 1)
    {
        avm_error("'cos' takes 1 paramter, %d were given", n);
    }
    avm_memcellclear(&retval);
    if (avm_getactual(0)->type != number_m)
    {
        avm_warning("'cos' needs number as argument!");
        retval.type = nil_m;
    }
    else
    {
        double angle = avm_getactual(0)->data.numVal;
        angle = (angle * PI) / 180;
        retval.type = number_m;
        retval.data.numVal = cos(angle);
    }
}

void libfunc_sin(void)
{
    unsigned n = avm_totalactuals();
    if (n != 1)
    {
        avm_error("'cos' takes 1 paramter, %d were given", n);
    }
    avm_memcellclear(&retval);
    if (avm_getactual(0)->type != number_m)
    {
        avm_warning("'cos' needs number as argument!");
        retval.type = nil_m;
    }
    else
    {
        double angle = avm_getactual(0)->data.numVal;
        angle = (angle * PI) / 180;
        retval.type = number_m;
        retval.data.numVal = sin(angle);
    }
}
void objectmemberkeys()
{
}

void objecttotalmembers()
{
}

void objectcopy()
{
}
int is_valid_int(const char *str)
{

    if (*str == '-')
        ++str;

    if (!*str)
        return 0;

    while (*str)
    {
        if (!isdigit(*str) && (*str) != '.')
            return 0;
        else
            ++str;
    }

    return 1;
}

void input()
{
    avm_memcellclear(&retval);
    unsigned n = avm_totalactuals();
    char c;
    c = getchar();
    int size = 1000;
    char *buf = (char *)malloc(sizeof(char) * size);
    int i = 0;
    while (c != '\n' && c != EOF)
    {
        buf[i] = c;
        i++;
        if (i >= size)
        {
            buf = (char *)realloc(buf, sizeof(char) * (size + 100));
            size += 100;
        }
        c = getchar();
    }
    if (buf[0] == '"')
    {
        retval.type = string_m;
        retval.data.strVal = strdup(buf);
    }
    else if (is_valid_int(buf) == 1)
    {
        retval.type = number_m;
        retval.data.numVal = atof(buf);
    }
    else if (strcmp(buf, "true") == 0 || strcmp(buf, "false") == 0)
    {
        retval.type = bool_m;
        if (strcmp(buf, "true") == 0)
            retval.data.boolVal = 1;
        if (strcmp(buf, "false") == 0)
            retval.data.boolVal = 0;
    }
    else if (strcmp(buf, "nil") == 0)
    {
        retval.type = nil_m;
    }
    else
    {
        retval.type = string_m;
        retval.data.strVal = strdup(buf);
    }
}

void avm_registerlibfunc(string id, library_func_t addr)
{
}

void memclear_string(avm_memcell *m) {}
void memclear_table(avm_memcell *m) {}
void execute_funcexit(instruction *instr) {}
void execute_or(instruction *instr) {}
void execute_add(instruction *instr) {}
void execute_and(instruction *instr) {}
void execute_div(instruction *instr) {}
void execute_mod(instruction *instr) {}
void execute_mul(instruction *instr) {}
void execute_nop(instruction *instr) {}
void execute_not(instruction *instr) {}
void execute_sub(instruction *instr) {}
void execute_uminus(instruction *instr) {}
void execute_pusharg(instruction *instr) {}
void execute_newtable(instruction *instr) {}
void execute_tablegetelem(instruction *instr) {}
void execute_tablesetelem(instruction *instr) {}
void execute_funcenter(instruction *instr) {}
library_func_t avm_getlibraryfunc(string id)
{
    if (strcmp(id.c_str(), "print") == 0)
    {
        return &libfunc_print;
    }
    else if (strcmp(id.c_str(), "typeof") == 0)
    {
        return &libfunc_typeof;
    }
    else if (strcmp(id.c_str(), "totalarguments") == 0)
    {
        return &libfunc_totalarguments;
    }
    else if (strcmp(id.c_str(), "objectmemberkeys") == 0)
    {
        return &objectmemberkeys;
    }
    else if (strcmp(id.c_str(), "objecttotalmembers") == 0)
    {
        return &objecttotalmembers;
    }
    else if (strcmp(id.c_str(), "objectcopy") == 0)
    {
        return &objectcopy;
    }
    else if (strcmp(id.c_str(), "input") == 0)
    {
        return &input;
    }
    else if (strcmp(id.c_str(), "argument") == 0)
    {
        return &libfunc_argument;
    }
    else if (strcmp(id.c_str(), "strtonum") == 0)
    {
        return &libfunc_strtonum;
    }
    else if (strcmp(id.c_str(), "sqrt") == 0)
    {
        return &libfunc_sqrt;
    }
    else if (strcmp(id.c_str(), "cos") == 0)
    {
        return &libfunc_cos;
    }
    else if (strcmp(id.c_str(), "sin") == 0)
    {
        return &libfunc_sin;
    }
    return 0;
}

void libfunc_totalarguments()
{
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if (!p_topsp)
    {
        avm_error("totalarguments called outside of a function");
        retval.type = nil_m;
    }
    else
    {
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void execute_cycle(void)
{
    if (executionFinished)
        return;
    if (pc >= codeSize)
    {
        executionFinished = 1;
        return;
    }
    else
    {
        if (pc == AVM_ENDING_PC)
        {
            executionFinished = true;
            return;
        }
        else
        {
            assert(pc < AVM_ENDING_PC);
            assert(code[pc]->opcode >= 0 && code[pc]->opcode <= AVM_MAX_INSTRUCTIONS);
            if (code[pc]->srcLine)
                currLine = code[pc]->srcLine;
            unsigned oldpc = pc;
            (*executeFuncs[code[pc]->opcode])(code[pc]);
            if (pc == oldpc)
                ++pc;
        }
    }
}

void execute_arithmetic(instruction *instr)
{
    avm_memcell *lv = avm_translate_operand(&instr->result, (avm_memcell *)0);
    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);
    assert(lv && (&stackavm[0] <= lv && &stackavm[top] > lv || lv == &retval));
    assert(rv1 && rv2);
    if (rv1->type != number_m || rv2->type != number_m)
    {
        avm_error("not a number in arithmetic");
        executionFinished = 1;
    }
    else
    {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}

void execute_assign(instruction *instr)
{
    avm_memcell *lv = avm_translate_operand(&instr->result, NULL);
    avm_memcell *rv = avm_translate_operand(&instr->arg1, &ax);
    avm_assign(lv, rv);
}

void execute_jump(instruction *instr)
{
    assert(instr->result.type == label_a);
    if (!executionFinished)
    {
        pc = instr->result.val;
    }
}

int avm_get_envvalue(int i)
{
    int val = (int)stackavm[i].data.numVal;
    return val;
}

void avm_push_envvalue(int val)
{
    stackavm[top].type = number_m;
    stackavm[top].data.numVal = val;
    // avm_dec_top();
}

void avm_assign(avm_memcell *lv, avm_memcell *rv)
{
    if (lv == rv)
    {
        return;
    }
    if (lv->type == table_m and
        rv->type == table_m and
        lv->data.tableVal == rv->data.tableVal)
    {
        return;
    }

    if (rv->type == undef_m)
    {
        avm_warning("Assigning from 'undefined' variable\n");
    }

    avm_memcellclear(lv);
    memcpy(lv, rv, sizeof(avm_memcell));

    if (lv->type == string_m)
    {
        // lv->data.strVal = strdup(rv->data.strVal);
    }
    else if (lv->type == table_m)
    {
        // avm_tablerefinc(lv->data.tableVal);
    }
}

string number_tostring(avm_memcell *m)
{
    assert(m->type == number_m);
    return to_string(m->data.numVal).c_str();
}
string string_tostring(avm_memcell *m)
{
    return m->data.strVal;
}

string bool_tostring(avm_memcell *m)
{
    assert(m->type == bool_m);
    if (m->data.boolVal)
        return strdup("false");
    else
        return strdup("true");
}

string table_tostring(avm_memcell *m)
{
    //
}

string userfunc_tostring(avm_memcell *m)
{
    assert(m->type == userfunc_m);
    return strdup("func");
}

string libfunc_tostring(avm_memcell *m)
{
    return m->data.libfuncVal;
}

string nil_tostring(avm_memcell *m)
{
    return strdup("nil");
}

string undef_tostring(avm_memcell *m)
{
    return strdup("undef");
}

userfunc *avm_getfuncinfo(int address)
{
    for (auto i : avmuserFuncs)
        if (i->address == address)
            return i;
    printf("Not found\n");
    return NULL;
}


void execute_call(instruction *instr)
{
    avm_memcell *func = avm_translate_operand(&instr->arg1, &ax);
    printf("%d\n", func->type);
    assert(func);
    switch (func->type)
    {
    case userfunc_m:
    {
        avm_callsaveenvironment();
        pc = func->data.userfuncVal;
        assert(pc < AVM_ENDING_PC);
        assert(code[pc]->opcode == funcenter_v);
        break;
    }
    case string_m:
        avm_callsaveenvironment();
        avm_calllibfunc(func->data.strVal);
        break;
    case libfunc_m:
    {
        avm_callsaveenvironment();
        avm_calllibfunc(func->data.libfuncVal);
        break;
    }
    case table_m:
    {
    }
    default:
    {
        avm_callsaveenvironment();
        char *s = &avm_tostring(func)[0];
        avm_error("call: cannot bind '%s' to function!", s);
        free(s);
        executionFinished = 1;
    }
    }
}

unsigned char number_tobool(avm_memcell *m) { return m->data.numVal != 0; }
unsigned char string_tobool(avm_memcell *m) { return m->data.strVal[0] != 0; }
unsigned char bool_tobool(avm_memcell *m) { return m->data.boolVal; }
unsigned char table_tobool(avm_memcell *m) { return 1; }
unsigned char userfunc_tobool(avm_memcell *m) { return 1; }
unsigned char libfunc_tobool(avm_memcell *m) { return 1; }
unsigned char nil_tobool(avm_memcell *m) { return 0; }
unsigned char undef_tobool(avm_memcell *m)
{
    assert(0);
    return 0;
}
typedef unsigned char (*tobool_func_t)(avm_memcell *);

tobool_func_t toboolFuncs[] = {
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool,
};
unsigned char avm_tobool(avm_memcell *m)
{
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}

void execute_jeq(instruction *instr)
{
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
    {
        avm_error("'undef' involved in equality!");
    }
    else if (rv1->type == nil_m || rv2->type == nil_m)
    {
        result = (rv1->type == nil_m && rv2->type == nil_m);
    }
    else if (rv1->type == bool_m || rv2->type == bool_m)
    {
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    }
    else if (rv1->type != rv2->type)
    {
        avm_error(
            "%s == %s is illegal!",
            typeStrings[rv1->type].c_str(),
            typeStrings[rv2->type].c_str());
    }
    else
    {
        switch (rv1->type)
        {
        case number_m:
        {
            result = (rv1->data.numVal == rv2->data.numVal);
            break;
        }
        case string_m:
        {
            result = strcmp(rv1->data.strVal, rv2->data.strVal);
            if (result == 0)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
            break;
        }
        case table_m:
        {
            result = (rv1->data.tableVal == rv2->data.tableVal);
            break;
        }
        case userfunc_m:
        {
            result = (rv1->data.userfuncVal == rv2->data.userfuncVal);
            break;
        }
        case libfunc_m:
        {
            result = (rv1->data.libfuncVal == rv1->data.libfuncVal);
            break;
        }
        default:
            assert(0);
        }
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jne(instruction *instr)
{
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
    {
        avm_error("'undef' involved in not equal!");
    }
    else if (rv1->type == nil_m || rv2->type == nil_m)
    {
        result = !(rv1->type == nil_m && rv2->type == nil_m);
    }
    else if (rv1->type == bool_m || rv2->type == bool_m)
    {
        result = (avm_tobool(rv1) != avm_tobool(rv2));
    }
    else if (rv1->type != rv2->type)
    {
        avm_error(
            "%s == %s is illegal!",
            typeStrings[rv1->type].c_str(),
            typeStrings[rv2->type].c_str());
    }
    else
    {
        switch (rv1->type)
        {
        case number_m:
        {
            result = (rv1->data.numVal != rv2->data.numVal);
            break;
        }
        case string_m:
        {
            result = strcmp(rv1->data.strVal, rv2->data.strVal);
            if (result != 0)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
            break;
        }
        case table_m:
        {
            result = (rv1->data.tableVal != rv2->data.tableVal);
            break;
        }
        case userfunc_m:
        {
            result = (rv1->data.userfuncVal != rv2->data.userfuncVal);
            break;
        }
        case libfunc_m:
        {
            result = (rv1->data.libfuncVal != rv1->data.libfuncVal);
            break;
        }
        default:
            assert(0);
        }
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jle(instruction *instr)
{
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type != number_m || rv2->type != number_m)
    {
        avm_error("not a number in less equal");
    }
    else
    {
        result = (rv1->data.numVal <= rv2->data.numVal);
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jge(instruction *instr)
{
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;
    if (rv1->type != number_m || rv2->type != number_m)
    {
        avm_error("not a number in greater equal");
    }
    else
    {
        result = (rv1->data.numVal >= rv2->data.numVal);
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jlt(instruction *instr)
{
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;
    if (rv1->type != number_m || rv2->type != number_m)
    {
        avm_error("not a number in less than");
    }
    else
    {
        result = (rv1->data.numVal < rv2->data.numVal);
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jgt(instruction *instr)
{
    assert(instr->result.type == label_a);

    avm_memcell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;
    if(rv1->type!=number_m || rv2->type!= number_m){
        avm_error("not a number in less than");
    }else{
        result = (rv1->data.numVal < rv2->data.numVal);
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

int i;
void match_op_i(vmopcode code)
{
    char s_code[20] = "";
    switch (code)
    {
    case assign_v:
        strcpy(s_code, "assign");
        break;
    case add_v:
        strcpy(s_code, "add");
        break;
    case sub_v:
        strcpy(s_code, "sub");
        break;
    case mul_v:
        strcpy(s_code, "mul");
        break;
    case div_v:
        strcpy(s_code, "div");
        break;
    case mod_v:
        strcpy(s_code, "mod");
        break;
    case jeq_v:
        strcpy(s_code, "jeq");
        break;
    case jne_v:
        strcpy(s_code, "jne");
        break;
    case jle_v:
        strcpy(s_code, "jle");
        break;
    case jge_v:
        strcpy(s_code, "jge");
        break;
    case jlt_v:
        strcpy(s_code, "jlt");
        break;
    case jgt_v:
        strcpy(s_code, "jgt");
        break;
    case jump_v:
        strcpy(s_code, "jump");
        break;
    case call_v:
        strcpy(s_code, "call");
        break;
    case pusharg_v:
        strcpy(s_code, "pusharg");
        break;
    case return_v:
        strcpy(s_code, "return");
        break;
    case getretval_v:
        strcpy(s_code, "getretval");
        break;
    case funcenter_v:
        strcpy(s_code, "funcenter");
        break;
    case funcexit_v:
        strcpy(s_code, "funcexit");
        break;
    case newtable_v:
        strcpy(s_code, "newtable");
        break;
    case tablegetelem_v:
        strcpy(s_code, "tablegetelem");
        break;
    case tablesetelem_v:
        strcpy(s_code, "tablesetelem");
        break;
    default:
        strcpy(s_code, "NO OPCODE");
        break;
    }
    printf("%20s", s_code);
}

unsigned char is_alpha_binfile(FILE *bin_file)
{
    unsigned int magic_number;
    fread(&magic_number, sizeof(unsigned int), 1, bin_file);
    printf("MAGIC NUMBER %d\n", magic_number);
    if (magic_number != 340200501)
    {
        return 0;
    }
    return 1;
}

unsigned load_const_strings(FILE *bin_file, char ***const_strings){
    /* READ STRINGS */
    unsigned int csc;
    fread(&csc, sizeof(unsigned int), 1, bin_file);
    *const_strings = (char **)malloc(sizeof(char **) * csc);
    for (i = 0; i < csc; i++)
    {
        unsigned size;
        fread(&size, sizeof(unsigned), 1, bin_file);
        char *str = (char*)malloc(size +1);
        fread(str, sizeof(char), size, bin_file);
        str[size] = '\0';
        (*const_strings)[i] = str;
        printf("str %d: %s\n", i, str);
        // str = NULL;
    }
    return csc;
}

unsigned load_const_nums(FILE *bin_file, double **const_nums)
{
    /* READ NUMBERS */
    unsigned int csc;
    fread(&csc, sizeof(unsigned int), 1, bin_file);
    *const_nums = (double*)malloc(sizeof(double *) * csc);
    for (i = 0; i < csc; i++)
    {
        double p;
        fread(&p, sizeof(double), 1, bin_file);
        (*const_nums)[i] = p;
        printf("double %d: %f\n", i, (*const_nums)[i]);
    }
    return csc;
}

unsigned load_user_funcs(FILE *bin_file, userfunc** userfuncs){
    unsigned int csc;
    /* READ USER FUNCTIONS */
    fread(&csc, sizeof(unsigned int), 1, bin_file);
    *userfuncs = (userfunc *) malloc(sizeof(userfunc) * csc);
    for (i = 0; i < csc; i++)
    {
        unsigned p1, p2;
        fread(&(*userfuncs)[i].address, sizeof(unsigned), 1, bin_file);
        fread(&(*userfuncs)[i].localSize, sizeof(unsigned), 1, bin_file);
        unsigned size;
        fread(&size, sizeof(unsigned), 1, bin_file);
        (*userfuncs)[i].id = (char*)malloc(size);
        fread((*userfuncs)[i].id, sizeof(char), size, bin_file);
        printf("function addr:%d locals:%d name:%s\n", (*userfuncs)[i].address, (*userfuncs)[i].localSize, (*userfuncs)[i].id);
    }
    return csc;
}

unsigned char load_machine_code(FILE *bin_file, instruction **instrTable, int *instrSize, char ***libfuncs_used, unsigned int *libfuncs_usedSize)
{
    int i;


    unsigned int csc;
    /* READ USER FUNCTIONS */
    // fread(&csc, sizeof(unsigned int), 1, bin_file);
    // for (i = 0; i < csc; i++)
    // {
    //     unsigned p1, p2;
    //     fread(&p1, sizeof(unsigned), 1, bin_file);
    //     fread(&p2, sizeof(unsigned), 1, bin_file);
    //     unsigned size;
    //     fread(&size, sizeof(unsigned), 1, bin_file);
    //     char *str = malloc(size);
    //     fread(str, sizeof(char), size, bin_file);
    //     printf("function addr:%d locals:%d name:%s\n", p1, p2, str);
    // }

    /* READ LIBS */
    fread(&csc, sizeof(unsigned int), 1, bin_file);
    *libfuncs_usedSize = csc;
    *libfuncs_used = (char **)malloc(sizeof(char **) * csc);
    for (i = 0; i < csc; i++)
    {
        unsigned size;
        fread(&size, sizeof(unsigned), 1, bin_file);
        char *str = NULL;
        str =  (char*)malloc(size+1);
        fread(str, sizeof(char), size, bin_file);
        str[size] = '\0';
        (*libfuncs_used)[i] = str;
        printf("lib %d: %s\n", i, (*libfuncs_used)[i]);
        // str = NULL;
    }

    /* READING INSTRUNCTIONS */
    fread(&csc, sizeof(unsigned int), 1, bin_file);
    *instrSize = csc;
    *instrTable = (instruction *)malloc(sizeof(instruction) * csc);
    for (i = 0; i < csc; i++)
    {
        struct binary_instr temp;
        fread(&temp, sizeof(struct binary_instr), 1, bin_file);
        if (i == 0)
            continue;
        // printf("i: %d op %d", i,temp.op);
        match_op_i((vmopcode)temp.op);
        printf(" %d %d %d %d %d %d %d", temp.res_type, temp.res_val, temp.arg1_type, temp.arg1_val, temp.arg2_type, temp.arg2_val, temp.src_line);

        (*instrTable)[i].opcode = (vmopcode)temp.op;
        // printf("i: %d op %d", i,(*instrTable)[i].opcode);
        (*instrTable)[i].srcLine = temp.src_line;

        (*instrTable)[i].result.type = (vmarg_t)temp.res_type;
        (*instrTable)[i].result.val = temp.res_val;

        (*instrTable)[i].arg1.type = (vmarg_t)temp.arg1_type;
        (*instrTable)[i].arg1.val = temp.arg1_val;

        (*instrTable)[i].arg2.type = (vmarg_t)temp.arg2_type;
        (*instrTable)[i].arg2.val = temp.arg2_val;

        printf("\n");
    }
    return 1;
}

unsigned load_global_size(FILE * bin_file){
    unsigned csc;
    fread(&csc, sizeof(unsigned int), 1, bin_file);
    return csc;
}
void avm_initstack(void){
    unsigned i;
    for(i = 0;i<AVM_STACKSIZE;i++){
        AVM_WIPEOUT(stackavm[i]);
        stackavm[i].type = undef_m;
    }
}

void execute_cycle(instruction* instructions,unsigned size){
    instruction* code = instructions;
    codeSize = size;
    if(executionFinished)return;
    else if(pc == AVM_ENDING_PC){
        // printf("PC 1:%d\n",pc);
        executionFinished = 1;
        return;
    }
    else{
        // printf("PC 2:%d\n",pc);
        assert(pc<AVM_ENDING_PC);
        printf("pc :%d srcline %d\n",pc,currLine);
        instruction* instr = instructions+pc;
        // printf("opcode: %d\n",instr->opcode);
        assert(instr->opcode >=0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
        if(instr->srcLine)currLine = instr->srcLine;
        unsigned oldpc = pc;
        // printf("OLDPC %d \n",oldpc);
        (*executeFuncs[instr->opcode])(instr);
        // printf("newpc %d \n",pc);
        if(pc == oldpc)pc++;
    }
}

char** string_consts;
double* number_consts;
char** libfuncs_used;
userfunc* userfuncs;
unsigned strings_const_size;
unsigned numbers_const_size;
unsigned libfuncs_used_size;
unsigned userfuncs_size;

int main(int argc, char **argv)
{
    FILE *bin_file;
    instruction *instructions;
    int instructionsSize;
    int globalSize;
    avm_initstack();
    if (argc == 1)
    {
        fprintf(stderr, "Too few arguments\n");
        exit(-1);
    }
    bin_file = fopen(argv[1], "rb");
    if (!bin_file)
    {
        fprintf(stderr, "Unable to read binary file %s\n", argv[1]);
        exit(-1);
    }
    printf("READING %s\n", argv[1]);
    if (!is_alpha_binfile(bin_file))
    {
        printf("It's not an alpha binary file");
        exit(-1);
    }
    strings_const_size = load_const_strings(bin_file, &string_consts);
    numbers_const_size = load_const_nums(bin_file, &number_consts);
    userfuncs_size = load_user_funcs(bin_file,&userfuncs);

    load_machine_code(bin_file, &instructions, &instructionsSize,&libfuncs_used,&libfuncs_used_size);
    globalSize = load_global_size(bin_file);
    printf("GLOBAL SIZE: %d\n",globalSize);
    top = topsp =  AVM_STACKSIZE - globalSize-1;
    
    unsigned i;
    while(pc < instructionsSize)
    {
        if(executionFinished) exit(-1);
        execute_cycle(instructions, instructionsSize);
    }

    
}

