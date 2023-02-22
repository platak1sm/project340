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
#include <vector>
#include <string>
#include <iterator>
using namespace std;

#define YY_DECL int alpha_yylex(void* yyval)

typedef enum kword {IF,ELSE,WHILE,FOR,FUNCTION,RETURN,BREAK,CONTINUE,AND,NOT,OR,LOCAL,TRUE,FALSE,NIL} kwords;
typedef enum op {ASGN,ADD,SUB,MUL,DIV,PC,EQ,NE,INC,DEC,GT,LT,GE,LE} ops; 
typedef enum punc{LEFT_BRACE,RIGHT_BRACE,LEFT_BRACKET,RIGHT_BRACKET,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,SEMICOLON,COMMA,COLON,DOUBLE_COLON,PERIOD,DOUBLE_PERIOD} puncs;

typedef struct node{
    int lineno;
    int tokenno;
    string tokenval;
    string type;
    string value;
    string cpptype; 
} alpha_token_t; /*mporei na thelei prosthikes*/

vector<alpha_token_t> tokens;
vector<alpha_token_t>::iterator it;

int yytokenno = 1; /*kratame to token number gia na to auskanoume kai na to vazoume sto kathe node*/

string operator2str(string s);

string punctuation2str(string s);

%}

%option noyywrap
%option yylineno

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
{integer}   {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="INTEGER";
             token.value= strdup(yytext);
             token.cpptype= "int";
             tokens.push_back(token);
            }

{real}      {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="REAL";
             token.value= strdup(yytext);
             token.cpptype= "double";
             tokens.push_back(token);
            }


{space} { }

{id}        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="ID";
             token.value= strdup(yytext);
             token.cpptype= "string";
             tokens.push_back(token);
            }

"if"        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "IF";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"else"      {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "ELSE";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"while"     {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "WHILE";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"for"       {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "FOR";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"function"  {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "FUNCTION";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"return"    {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "RETURN";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"break"     {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "BREAK";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"continue"  {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "CONTINUE";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }
            
"and"       {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "AND";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }       

"not"       {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "NOT";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"or"        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "OR";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }
            
"local"     {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "LOCAL";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"true"      {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "TRUE";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"false"     {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "FALSE";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"nil"       {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="KEYWORD";
             token.value= "NIL";
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

        
"="         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"+"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"-"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"*"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"/"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }


"%"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"=="        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }                                    

"!="        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"++"        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"--"        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

">"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"<"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }


">="        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"<="        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="OPERATOR";
             token.value= operator2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }  

"{"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"}"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"["         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"]"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"("         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

")"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

";"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

","         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

":"         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"::"        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

"."         {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

".."        {
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

%%

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
    else return "";
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
    else if(s=="::") return "DOUBLE_COLON";
    else if(s==".") return "PERIOD";
    else if(s=="..") return "DOUBLE_PERIOD";
    else return "";
}


int main(int argc, char *argv[]){
    if(argc == 1) {
		yyin = stdin;
    }else if(argc > 1){
        if(!(yyin=fopen(argv[1],"r"))){
            fprintf(stderr,"Cannot read file: %s\n",argv[1]);
            return -1;
        }
    }
    it = tokens.begin();

    alpha_yylex(&it);
 
    cout << tokens.size()<<"\n";
    for(int i = 0; i<tokens.size();i++){
        cout << tokens.at(i).lineno << ": #" << tokens.at(i).tokenno << " \"" << tokens.at(i).tokenval << "\" " << tokens.at(i).type << " " << tokens.at(i).value << " <-" << tokens.at(i).cpptype << endl;
    }
    return 0;
}
