%{
#if defined(WIN32)
#define YY_NO_UNISTD_H
static int isatty(int i){ return 0; }
#elif defined(_WIN32_WcE)
#define YY_NO_UNISTD_H
static int isatty(void *i){ return 0; }
#endif


#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#define YY_DECL int alpha_yylex(void* yyval)
#ifdef __cplusplus
static int yyinput(void);
#else 
static int input(void);
#endif


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
} alpha_token_t; 

vector<alpha_token_t> tokens;
vector<alpha_token_t>::iterator it;

int yytokenno = 1; 
int openings = 0;
int endings = 0;
int openings_s = 0;
int endings_s = 0;
string operator2str(string s);

string punctuation2str(string s);

string bcomment(int isNestedc);

string findstr();

void inserttoken(string type, string value, string cpptype);

%}

%option noyywrap
%option yylineno

id          [a-zA-Z][a-zA-Z_0-9]*   
integer     ([+-])?((0[xX][0-9A-Fa-f]+)|([0-9]+))
real        ([+-])?([0-9]*\.[0-9](e|E)?[0-9]*)
space       [\r\n \t\v]
string      \"
onelinecomment    "//".*
multilinescomment    "\/\*"

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

{string}   {
             openings_s = 0;
             endings_s = 0;
             string ss = findstr();
             const char * ss2 = ss.c_str();
             alpha_token_t token;
             token.lineno= yylineno;
             token.tokenno= yytokenno++;
             token.tokenval= ss2;
             token.type="STRING";
             token.value= strdup(ss2);
             token.cpptype= "string";
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
{onelinecomment} {
                  alpha_token_t token;
                  token.lineno= yylineno;
                  token.tokenno= yytokenno++;
                  token.tokenval= "";
                  token.type="COMMENT";
                  token.value= "LINE_COMMENT";
                  token.cpptype= "enumerated";
                  tokens.push_back(token);
                 }

{multilinescomment}  {
                openings = 0;
                endings = 0;
                alpha_token_t token;
                token.lineno= yylineno;
                token.tokenno= yytokenno++;
                
                token.tokenval= bcomment(0);
                token.type="COMMENT";
                token.value= "BLOCK_COMMENT";
                token.cpptype= "enumerated";
                tokens.push_back(token);
            }		
            
{space} { }


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
            inserttoken("OPERATOR",operator2str(yytext), "enumerated");
            }

">"         {
             inserttoken("OPERATOR",operator2str(yytext), "enumerated");
            }

"<"         {
             inserttoken("OPERATOR",operator2str(yytext), "enumerated");
            }


">="        {
            inserttoken("OPERATOR",operator2str(yytext), "enumerated");
            }

"<="        {
             inserttoken("OPERATOR",operator2str(yytext), "enumerated");
            }  

"{"         {
             inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

"}"         {
            inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

"["         {
             inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

"]"         {
             inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

"("         {
            inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

")"         {
             inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

";"         {
             inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
            }

","         {
             inserttoken("PUNCTUATION", punctuation2str(yytext), "enumerated");
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
             token.tokenno= yytokenno++;
             token.tokenval= yytext;
             token.type="PUNCTUATION";
             token.value= punctuation2str(yytext);
             token.cpptype= "enumerated";
             tokens.push_back(token);
            }

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

.    { cout << "UNDEF INPUT: "<<yytext <<" AT LINE: "<<yylineno<<endl; 
       exit(0);
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

string bcomment(int isNestedc){
	int lineStart = yylineno ,lineEnd;
    char curr;
    curr = yyinput();
    string ret;
    if(isNestedc){
        ret = "";
    }else{
        ret = "\n";
    }
    
    while(curr != EOF && curr != '\0'){
        if(curr == '*'){
		    curr = yyinput();
            if(curr == '/'){
                endings++;
                lineEnd=yylineno;
                
                
            
                ret += to_string(yylineno)+": "+to_string(lineStart) + "-" + to_string(lineEnd);
                
                return ret;
                        
            }else{
                 unput(curr);
            }
		}
		else if(curr == '/'){
			curr = yyinput();
            if(curr == '*'){
                openings++;
                ret += bcomment(1)+" NESTED COMMENT\n";
                
                
            }else{
                unput(curr);
            }
		}
        
        
        curr = yyinput(); 
	}
    
    if(isNestedc){
		cout <<"Line "<<lineStart<<": error nested block comment reached EOF\n";
        exit(0);
		return "ERROR";
	}
	cout << lineStart<<": error block comment reached EOF without getting closed\n";
    exit(0);
	return "ERROR";
}

string findstr() {
    openings_s++;
    int lineStart = yylineno ,lineEnd;
    char next;
    string buf = "";
    next = yyinput();

    while(next != EOF && next!='\0'){
        
        if( next == '\\') {
            next = yyinput();
            if( next == 'n') {
                buf += '\n';
                next = yyinput();
                continue;
            }
            else if( next == 't') {
                buf += '\t';
                next = yyinput();
                continue;
            }
            else if( next == '"') {
                buf += '"';
                next = yyinput();
                continue;
            }
            else if(next == '\\'){
                buf += '\\';
                next = yyinput();
                continue;
            }else{
                cout<<("warning invalid escape character in string\n");
                buf += "\\";
                continue; 
            }
        }

        if(next == '"'){
            endings_s++;
            lineEnd=yylineno;
		    return buf;
		}
        
        buf += next;
        next = yyinput();
    }
    cout <<lineStart<<": error string reached EOF without getting closed\n";;
    exit(0);
    return buf;
}

void inserttoken(string type, string value, string cpptype){
    alpha_token_t token;
    token.lineno= yylineno;
    token.tokenno= yytokenno++;
    token.tokenval= yytext;
    token.type=type;
    token.value= value;
    token.cpptype= cpptype;
    tokens.push_back(token);
}

int main(int argc, char *argv[]){
    if(argc == 1) {
		yyin = stdin;
    }else if(argc > 1){
        if(!(yyin=fopen(argv[1],"r"))){
            fprintf(stderr,"cannot read file: %s\n",argv[1]);
            return -1;
        }
    }
    it = tokens.begin();

    alpha_yylex(&it);
 
    /* cout << tokens.size()<<"\n"; */
    for(int i = 0; i<tokens.size();i++){
        cout << tokens.at(i).lineno << ": #" << tokens.at(i).tokenno << " \"" << tokens.at(i).tokenval << "\" " << tokens.at(i).type << " " << tokens.at(i).value << " <-" << tokens.at(i).cpptype << endl;
    }
    return 0;
}
