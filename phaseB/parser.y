%{
    #include <iostream>
    int yyerror (char* yaccProvidedMessage);
    int yylex (void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    int flag_insert=1;

    int scope=0;
    int funcid=0;
    string lastidname;

    bool is_sysfunc(string name) {
        if( name == "print" ||
            name == "input" ||
            name == "objectmemberkeys" ||
            name == "objecttotalmembers" ||
            name == "objectcopy" ||
            name == "argument" ||
            name == "typeof" ||
            name == "strtonum" ||
            name == "sqrt" ||
            name == "cos" ||
            name == "sin") {
            return true;
        }
        else {
            return false;
        }
    }
    
%}

%start program

%union {
	int intval;
    char* strval;
	double doubleval;
    SymbolTableEntry node;
}

%token <intval> INTEGER
%token <doubleval> REAL
%token <strval> ID STRING 
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token ASSIGN PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL UMINUS
%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON PERIOD DOUBLE_PERIOD

%type <node> lvalue

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
      | expr PLUS expr {/*$$ = $1 + $3;*/}
      | expr MINUS expr {/*$$ = $1 - $3;*/}
      | expr MUL expr {/*$$ = $1 * $3;*/}
      | expr DIV expr {/*$$ = $1 / $3;*/}
      | expr MOD expr {/*$$ = $1 % $3;*/}
      | expr EQUAL expr {/*if ($1 == $3) $$ = 1; else $$ = 0;*/}
      | expr NOT_EQUAL expr {/*if ($1 != $3) $$ = 1; else $$ = 0;*/}
      | expr GREATER expr {/*if ($1 > $3) $$ = 1; else $$ = 0;*/}
      | expr LESS expr {/*if ($1 < $3) $$ = 1; else $$ = 0;*/}
      | expr GREATER_EQUAL expr {/*if ($1 >= $3) $$ = 1; else $$ = 0;*/}
      | expr LESS_EQUAL expr {/*if ($1 <= $3) $$ = 1; else $$ = 0;*/}
      | expr AND expr {/*if ($1 && $3) $$ = 1; else $$ = 0;*/}
      | expr OR expr {/*if ($1 || $3) $$ = 1; else $$ = 0;*/}
      | term
      ;

term:   LEFT_PARENTHESIS{/* scope++; */} expr RIGHT_PARENTHESIS {/* scope--; */}
	    | UMINUS expr {/* $$ = $2 * (-1); */}
	    | NOT expr {/* if ($2) $$=0; else $$=1; */}
		| PLUS_PLUS lvalue {
                             string name=$2;
                             SymbolTableEntry ste= lookupactivevar(name,scope);
                             if(is_sysfunc(name)) cout << "Error: "<< name <<" is a system function, it cannot be used for decrement.\n";
                             else if (!ste.isActive) cout << "Error: There is no variable " << name << endl;
                             else{ 
                                if (ste.varVal.scope!=0 || ste.varVal.scope!=scope) cout << "Error: Variable " << name << " is not accessible in this scope.\n"; 
                                /* else $$ = ++$2; */
                             }
                           }
		| lvalue PLUS_PLUS {
                             string name=$1;
                             SymbolTableEntry ste= lookupactivevar(name,scope);
                             if(is_sysfunc(name)) cout << "Error: "<< name <<" is a system function, it cannot be used for decrement.\n";
                             else if (!ste.isActive) cout << "Error: There is no variable " << name << endl;
                             else{ 
                                if (ste.varVal.scope!=0 || ste.varVal.scope!=scope) cout << "Error: Variable " << name << " is not accessible in this scope.\n"; 
                                /* else $$ = $2++; */
                             }
                           }
		| MINUS_MINUS lvalue {
                             string name=$2;
                             SymbolTableEntry ste= lookupactivevar(name,scope);
                             if(is_sysfunc(name)) cout << "Error: "<< name <<" is a system function, it cannot be used for decrement.\n";
                             else if (!ste.isActive) cout << "Error: There is no variable " << name << endl;
                             else{ 
                                if (ste.varVal.scope!=0 || ste.varVal.scope!=scope) cout << "Error: Variable " << name << " is not accessible in this scope.\n"; 
                                /* else $$ = --$2; */
                             }
                           }
		| lvalue MINUS_MINUS {
                             string name=$1;
                             SymbolTableEntry ste= lookupactivevar(name,scope);
                             if(is_sysfunc(name)) cout << "Error: "<< name <<" is a system function, it cannot be used for decrement.\n";
                             else if (!ste.isActive) cout << "Error: There is no variable " << name << endl;
                             else{ 
                                if (ste.varVal.scope!=0 || ste.varVal.scope!=scope) cout << "Error: Variable " << name << " is not accessible in this scope.\n";
                                /* else $$ = $2--; */
                             }
                           }
		| primary {/*$$=$1;*/}
		;

assignexpr: lvalue ASSIGN expr{		
                                string name = $1;
                                if(lookupactivefunc(name).isActive==true)
                                        cout <<"Error: " <<name << "is defined as function \n";           
                            }
            ;
	
primary: lvalue{/* 
                    string name = $1;
                    SymbolTableEntry ent = lookupactivevar(name)
                    if(ent.isActive == false || ent.type == GLOBAL){
                        ent.isActive = true;
                        if(scope == 0)
                            ent.type = GLOBAL;
                        else
                            ent.type = LOCAL;
                        ent.varVal.name = name;
                        ent.varVal.scope = scope;
                        ent.varVal.line = yylineno;
                        insert(ent);
                    }   */ 
                 }
        | call {/* $$=$1; */}
        | objectdef {/* $$=$1; */}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS{
                    /* string name = $2;
                    SymbolTableEntry ent = lookupactivevar(name)
                    if(ent.isActive == false || ent.type == GLOBAL){
                        ent.isActive = true;
                        if(scope == 0)
                            ent.type = GLOBAL;
                        else
                            ent.type = LOCAL;
                        ent.varVal.name = name;
                        ent.varVal.scope = scope;
                        ent.varVal.line = yylineno;
                        insert(ent);
                    }    */
        }
		| const {/* $$=$1; */}
        ;

lvalue: ID {
            string name($1);
            if(lookupactivevar(name).isActive == false && lookupactivefunc(name).isActive == false){
                SymbolTableEntry ent;
                ent.isActive = true;
                if(scope == 0)
                    ent.type = GLOBAL;
                else
                    ent.type = LOCAL;
                ent.varVal.name = name;
                ent.varVal.scope = scope;
                ent.varVal.line = yylineno;
                insert(ent);
            }
            }
        | LOCAL ID {
            string name($1);
            SymbolTableEntry ret = lookupactivevar(name)
            if(ret.isActive == false || ret.type == GLOBAL){
                SymbolTableEntry ent;
                ent.isActive = true;
                if(scope == 0)
                    ent.type = GLOBAL;
                else
                    ent.type = LOCAL;
                ent.varVal.name = name;
                ent.varVal.scope = scope;
                ent.varVal.line = yylineno;
                insert(ent);
            }
        }
        | DOUBLE_COLON ID {
            string name($1);
            if(lookupcurrentscope(name, 0).isActive == false){
                cout << "error\n";
            }
        }
        | member{}
        ;

member: lvalue PERIOD ID
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET
		| call PERIOD ID 
		| call LEFT_BRACKET expr RIGHT_BRACKET 
		;

call:	call LEFT_PARENTHESIS elist	RIGHT_PARENTHESIS 
		| lvalue callsuffix 
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
		;

callsuffix:	normcall
			| methodcall
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

indexedelem: LEFT_BRACE { flag_insert=0; } expr COLON { flag_insert=1; } expr RIGHT_BRACE
             ;

block: LEFT_BRACE{scope++;} stmtlist RIGHT_BRACE{hide(scope--);}
       ;

stmtlist: stmt stmtlist
          |
          ;

funcdef: FUNCTION ID {
            string name= $2;
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){ cout << "Error: " << name << " is declared in this scope already.\n";
            }else if(is_sysfunc()){ cout << "Error: "<< name <<" is a system function, it cannot be overriden.\n";
            }else {
                  ste.type=USERFUNC;
                  ste.funcVal.name=name;
                  ste.funcVal.scope=scope;
                  ste.funcVal.line=yylineno;
                  insert(ste);
            }
         } funLEFT_PAR idlist funRIGHT_PAR block
         | FUNCTION {
            string fid=to_string(funcid++);
            string name= "$f" + fid;
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            while(true){
                if(ste.isActive || is_sysfunc()){ 
                    fid= to_string(funcid++);
                    name= "$f" + fid;
                    ste= lookupcurrentscope(name,scope);
                }else {
                    ste.type=USERFUNC;
                    ste.funcVal.name=name;
                    ste.funcVal.scope=scope;
                    ste.funcVal.line=yylineno;
                    insert(ste);
                    break;
                }
            }
         } funLEFT_PAR idlist funRIGHT_PAR block
         ;

funLEFT_PAR:    LEFT_PARENTHESIS{scope++;}
funRIGHT_PAR:   RIGHT_PARENTHESIS{scope--;}

const:	INTEGER {/*$$=$1;*/}
		| REAL {/*$$=$1;*/}
		| STRING {/*$$=$1;*/}
		| NIL {/*$$=$1;*/}
		| TRUE {/*$$=$1;*/}
		| FALSE {/*$$=$1;*/}
		;

idlist: ID{
            string name = $1; 
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){ cout << "Error: " << name << " is declared in this scope already.\n";
            }else if(is_sysfunc()){ cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n";
            }else {
                  ste.type=FORMAL;
                  ste.varVal.name=name;
                  ste.varVal.scope=scope;
                  ste.varVal.line=yylineno;
                  insert(ste);
            }
          }
        | idlist COMMA ID{
            string name = $3; 
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){ cout << "Error: " << name << " is declared in this scope already.\n";
            }else if(is_sysfunc(name)){ cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n";
            }else {
                  ste.type=FORMAL;
                  ste.varVal.name=name;
                  ste.varVal.scope=scope;
                  ste.varVal.line=yylineno;
                  ste.funcVal.name=name; /*de kserw an xreiazontai kai var kai funcval*/
                  ste.funcVal.scope=scope;
                  ste.funcVal.line=yylineno;
                  insert(ste);
            }
         }
        |
       ;

ifstmt:	IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{
                    /* string name = $3;
                    SymbolTableEntry ent = lookupactivevar(name);
                    if(ste.isActive)
                        cout << "Error: " << name << " is declared in this scope already.\n";
                     else if(is_sysfunc(name)) 
                        cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n";
                    if(ent.isActive == false || ent.type == GLOBAL){
                        ent.isActive = true;
                        if(scope == 0)
                            ent.type = GLOBAL;
                        else
                            ent.type = LOCAL;
                        ent.varVal.name = name;
                        ent.varVal.scope = scope;
                        ent.varVal.line = yylineno;
                        insert(ent);
                    }    */
         } stmt 
		| IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{
                   /*  string name = $3;
                    SymbolTableEntry ent = lookupactivevar(name)
                    if(ent.isActive == false || ent.type == GLOBAL){
                        ent.isActive = true;
                        if(scope == 0)
                            ent.type = GLOBAL;
                        else
                            ent.type = LOCAL;
                        ent.varVal.name = name;
                        ent.varVal.scope = scope;
                        ent.varVal.line = yylineno;
                        insert(ent);
                    }  */  
         } stmt ELSE stmt 
	    ;	 

whilestmt: WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{
                    /* string name = $3;
                    SymbolTableEntry ent = lookupactivevar(name);
                    if(ste.isActive)
                        cout << "Error: " << name << " is declared in this scope already.\n";
                     else if(is_sysfunc(name)) 
                        cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n";
                    if(ent.isActive == false || ent.type == GLOBAL){
                        ent.isActive = true;
                        if(scope == 0)
                            ent.type = GLOBAL;
                        else
                            ent.type = LOCAL;
                        ent.varVal.name = name;
                        ent.varVal.scope = scope;
                        ent.varVal.line = yylineno;
                        insert(ent);
                    }    */
         } stmt
		 ;  	

	
forstmt: FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS{
                        /* string name = $3;
                        SymbolTableEntry ent = lookupactivevar(name);
                        if(ste.isActive)
                            cout << "Error: " << name << " is declared in this scope already.\n";
                        else if(is_sysfunc(name)) 
                            cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n";
                        if(ent.isActive == false || ent.type == GLOBAL){
                            ent.isActive = true;
                            if(scope == 0)
                                ent.type = GLOBAL;
                            else
                                ent.type = LOCAL;
                            ent.varVal.name = name;
                            ent.varVal.scope = scope;
                            ent.varVal.line = yylineno;
                            insert(ent);
                        }  */
                    }stmt
		;	

returnstmt: RETURN expr SEMICOLON{
                    /* string name = $2;
                    SymbolTableEntry ent = lookupactivevar(name);
                    if(ste.isActive)
                        cout << "Error: " << name << " is declared in this scope already.\n";
                     else if(is_sysfunc(name)) 
                        cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n";
                    if(ent.isActive == false || ent.type == GLOBAL){
                        ent.isActive = true;
                        if(scope == 0)
                            ent.type = GLOBAL;
                        else
                            ent.type = LOCAL;
                        ent.varVal.name = name;
                        ent.varVal.scope = scope;
                        ent.varVal.line = yylineno;
                        insert(ent);
                        //$$=$2;
                    }    */
         }
			| RETURN SEMICOLON;


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

    insertLibFuncs("print");
    insertLibFuncs("input");
    insertLibFuncs("objectmemberkeys");
    insertLibFuncs("objecttotalmembers");
    insertLibFuncs("objectcopy");
    insertLibFuncs("totalarguments");
    insertLibFuncs("argument");
    insertLibFuncs("typeof");
    insertLibFuncs("strtonum");
    insertLibFuncs("sqrt");
    insertLibFuncs("cos");
    insertLibFuncs("sin");
    yyparse();
    return 0;
}