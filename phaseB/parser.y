%{
    #include <iostream>
    int yyerror (char* yaccProvidedMessage);
    int yylex (void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    int flag_insert=1;

    int scope=0;
    
%}

%start program

%union {
	int intval;
    char* strval;
	double doubleval;
}

%token <intval> INTEGER
%token <doubleval> REAL
%token <strval> ID STRING 
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token ASSIGN PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL UMINUS
%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON PERIOD DOUBLE_PERIOD


%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL
%left PLUS MINUS
%left MUL DIV MOD
%right NOT PLUS_PLUS MINUS_MINUS UMINUS 
%left PERIOD DOUBLE_PERIOD
%left LEFT_BRACKET RIGHT_BRACKET 
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS


%%
program: stmt program
         |
         ;
 
stmt: expr SEMICOLON
      | ifstmt
      | whilestmt
      | forstmt
      | returnstmt
      | BREAK SEMICOLON
      | CONTINUE SEMICOLON
      | block
      | funcdef
      | SEMICOLON
      ;

expr: assignexpr
      | expr PLUS expr {$$ = $1 + $3;}
      | expr MINUS expr {$$ = $1 - $3;}
      | expr MUL expr {$$ = $1 * $3;}
      | expr DIV expr {$$ = $1 / $3;}
      | expr MOD expr {$$ = $1 % $3;}
      | expr EQUAL expr {if ($1 == $3) $$ = 1;
                         else $$ = 0;}
      | expr NOT_EQUAL expr {if ($1 != $3) $$ = 1;
                             else $$ = 0;}
      | expr GREATER expr {if ($1 > $3) $$ = 1;
                           else $$ = 0;}
      | expr LESS expr {if ($1 < $3) $$ = 1;
                         else $$ = 0;}
      | expr GREATER_EQUAL expr {if ($1 >= $3) $$ = 1;
                                 else $$ = 0;}
      | expr LESS_EQUAL expr {if ($1 <= $3) $$ = 1;
                              else $$ = 0;}
      | expr AND expr {if ($1 && $3) $$ = 1;
                       else $$ = 0;}
      | expr OR expr {if ($1 || $3) $$ = 1;
                      else $$ = 0;}
      | term
      ;

lvalue: ID {}
        |LOCAL ID {}
        |DOUBLE_COLON ID {}
        |member 
        ;

normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
          ;
        
methodcall: DOUBLE_PERIOD ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
            ;

elist: expr
       | elist COMMA expr
       |
       ;

objectdef: LEFT_BRACKET elist RIGHT_BRACKET
           | LEFT_BRACKET indexed RIGHT_BRACKET
           ;

indexed: indexedelem
       | indexed COMMA indexedelem
       |
       ;

indexedelem: LEFT_BRACE /*{ flag_insert=0; }*/ expr COLON /*{ flag_insert=1; }*/ expr RIGHT_BRACE
             ;

block: LEFT_BRACE{scope++;} stmtlist RIGHT_BRACE{scope--;}
       ;

stmtlist: stmt stmtlist
          |
          ;

funcdef: FUNCTION ID {/*code*/} LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
         | FUNCTION ID {/*other code*/} LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
         ;

funLEFT_PAR:    LEFT_PARENTHESIS{scope++;}
funRIGHT_PAR:   RIGHT_PARENTHESIS{scope--;}
<<<<<<< HEAD
=======

primary: lvalue
        |call
        |objectdef
		|LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
		|const
        ;

member: lvalue DOT ID
		|lvalue LEFT_BRACKET expr RIGHT_BRACKET
		|call DOT ID 
		|call LEFT_BRACKET expr RIGHT_BRACKET 
		;

call:	call LEFT_PARENTHESIS elist	RIGHT_PARENTHESIS 
		|lvalue callsuffix 
		|LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
		;

callsuffix:	normcall
			|methodcall
			;


const:	INTEGER
		| REAL
		| STRING
		| NIL 
		| TRUE 
		| FALSE 
		;

idlist:ID{/*code*/}
       | idlist COMMA ID
       ;

ifstmt:	IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
		|IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt
	    ;	 

whilestmt: WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
		 ;  	

	
forstmt: FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
		;	

returnstmt: RETURN expr SEMICOLON
			| RETURN SEMICOLON;

op: PLUS 
    | MINUS
    | MUL 
    | DIV 
    | MOD 
    | GREATER 
    | GREATER_EQUAL 
    | LESS 
    | LESS_EQUAL 
    | EQUAL 
    | NOT_EQUAL
    | AND 
    | OR
    ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS 
	    |MINUS expr
	    |NOT expr
		|PLUS_PLUS lvalue 
		|lvalue PLUS_PLUS 
		|MINUS_MINUS lvalue 
		|lvalue MINUS_MINUS 
		|primary
			;
	
>>>>>>> stelios
%%     

int yyerror (char* yaccProvidedMessage)
{
    fprintf(stderr, "%s: at line %d, before token: %s\n", yaccProvidedMessage, yylineno, yytext);
    fprintf(stderr, "INPUT NOT VALID\n");
}

//**********************************************************************************************************

int main (int argc, char** argv) {
    if (argv > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
           fprintf(stderr, "Cannot read file: %s\n", argv[1]);
           return 1;
        }
           
    }
    else 
        yyin = stdin;

    yyparse();
    return 0;
}