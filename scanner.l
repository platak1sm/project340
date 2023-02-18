%{
#if defined(WIN32)
#define YY_NO_UNISTD_H
static int isatty(int i){ return 0; }
#elif defined(_WIN32_WCE)
#define YY_NO_UNISTD_H
static int isatty(void *i){ return 0; }
#endif


#include <iostream>
#include <list>
#include <string>
using namespace std;

#define YY_DECL int alpha_yylex(void* yyval)

typedef enum kword {IF,ELSE,WHILE,FOR,FUNCTION,RETURN,BREAK,CONTINUE,AND,NOT,OR,LOCAL,TRUE,FALSE,NIL} kwords;
typedef enum op {ASSIGN,ADD,SUB,MUL,DIV,PC,EQ,NE,INC,DEC,GT,LT,GE,LE} ops; 
typedef enum punc{} puncs;

typedef struct node{
    int lineno;
    int tokenno;
    string tokenval;
    string type;
    string value;
    string cpptype;
} alpha_token_t; /*mporei na thelei prosthikes*/

list<alpha_token_t> tokens;

int yytokenno = 1; /*kratame to token number gia na to auskanoume kai na to vazoume sto kathe node*/

string operator2str(string s){
    if(s=="=") return "ASSIGNMENT";
	else if(s=="+") return "PLUS";
	else if(s=="-") return "MINUS";
    else if(s=="*") return "MULTIPLICATION";
    else if(s=="/") return "DIVISION";
    else if(s=="%") return "PERCENT";
    else if(s=="==") return "EQUAL";
    else if(s=="!=") return "NOT_EQUAL";
    else if(s=="++") return "PLUS_PLUS";
    else if(s=="--") return "MINUS_MINUS";
    else if(s==">") return "GREATER_THAN";
    else if(s=="<") return "LESS_THAN";
    else if(s==">=") return "GREATER_EQUAL";
    else if(s=="<=") return "LESS_EQUAL";
}

string punctuation2str(string s){
    if(s=="{") return "LEFT_BRACE";
	else if(s=="}") return "RIGHT_BRACE";
	else if(s=="[") return "LEFT_BRACKET";
    else if(s=="]") return "RIGHT_BRACKET";
    else if(s=="(") return "LEFT_PARENTHESIS";
    else if(s==")") return "RIGHT_PARENTHESIS";
    else if(s==";") return "SEMICOLON";
    else if(s==",") return "COMMA";
    else if(s==":") return "COLON";
    else if(s=="::") return "DOUBLE COLON";
    else if(s==".") return "PERIOD";
    else if(s=="..") return "DOUBLE PERIOD";
}

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

%x COMMENTS
%x STRINGS

%%

{space} { }

{keywords}  {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++
             token.tokenval= yytext;
             token.type="KEYWORD"
             token.value= toupper(yyext);
             list.push_back(token);
            }


{operators} {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++
             token.tokenval= yytext;
             token.type="OPERATOR"
             token.value= operator2str(yytext);
             list.push_back(token);
            }


{pmarks}    {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++
             token.tokenval= yytext;
             token.type="PUNCTUATION"
             token.value= punctuation2str(yytext);
             list.push_back(token);
            } 