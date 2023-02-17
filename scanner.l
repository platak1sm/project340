%{
#if defined(WIN32)
#define YY_NO_UNISTD_H
static int isatty(int i){ return 0; }
#elif defined(_WIN32_WCE)
#define YY_NO_UNISTD_H
static int isatty(void *i){ return 0; }
#endif

#include <iostream.h>

#define YY_DECL int alpha_yylex(void* ylval)
%}

%option noyywrap
%option yylineno
%option nounistd

id          [a-zA-Z][a-zA-Z_0-9]*
integer     (0[xX][0-9A-Fa-f]+)|([0-9]+)
real        [0-9]*\.[0-9](e|E)?[0-9]*
space       [\r\n \t\v]
string      \"[^\n"]*\"
operators   (\=)|(\+)|(\-)|(\*)|(\/)|(\%)|(\==)|(\++)|(\--)|(\>)|(\<)|(\>=)|(<=)
keywords    if|else|while|for|function|return|break|continue|and|not|or|local|true|false|nil
pmarks      (\{)|(\})|(\[)|(\])|(\()|(\))|(\;)|(\,)|(\:)|(\::)|(\.)|(\..)
comment1    "//".*
comment2    "/*".*"*/"

%x COMMENT
%%

