%{
    #include <iostream>
    int yyerror (char* yaccProvidedMessage);
    int yylex (void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
%}

%start program

%token ID INTEGER 

%right      '='
%left       ','
%left       '+' '-'
%left       '*' '/'
%nonassoc   UMINUS
%left       '(' ')'

%%

program:        assignments expressions
                | /* empty */
                ;
expression:     INTEGER
                | ID
                | expression '+' expression
                | expression '-' expression
                | expression '*' expression
                | expression '/' expression
                | '(' expression ')'
                | '-' expression %prec UMINUS
                ;

expr:           expression '\n'

expressions:    expressions expr
                | expr
                ;

assignment:     ID '=' expression '\n'
                ;

assignments:    assignments assignment
                | /* empty */
                ;

%%   

int yyerror (char* yaccProvidedMessage)
{
    fprintf(stderr, "%s: at line %d, before token: %s\n", yaccProvidedMessage, yylineno, yytext);
    fprintf(stderr, "INPUT NPT VALID\n");
}

//***********************************************************************************************************

int main (int argc, char** argv) {
    if (argv > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
           fprintf(stderr, "Cannot reaf file: %s\n", argv[1]);
           return 1;
        }
           
    }
    else 
        yyin = stdin;

    yyparse();
    return 0;
}