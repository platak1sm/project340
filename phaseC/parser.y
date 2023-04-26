%{
    #include <iostream>
    #include <string>
    #include "symbol_table.h"

    /* #define YY_DECL int alpha_yylex (void* yylval)*/
    extern int yylex(void);
    int yyerror (char* yaccProvidedMessage);
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    /* int flag_insert=1; */
    int inloop=0;
    int infunction=0;
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
    
    void insertLibFuncs(string name);
    void red();
    void reset();

%}

%start program

%union {
	int intVal;
    char *stringVal;
	double doubleVal;
}

%token <intVal> INTEGER
%token <doubleVal> REAL
%token <stringVal> ID STRING 
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token ASSIGN PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL UMINUS
%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON PERIOD DOUBLE_PERIOD

%type <stringVal> lvalue

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
program: stmt program {cout << "program stmt\n";}
         | {cout << "program empty\n";}
         ;
 
stmt: expr SEMICOLON {cout << "stmt => expr\n";}
      | ifstmt {cout << "stmt => ifstm\n";}
      | {inloop++;}whilestmt{cout << "stmt => whilestmt\n";inloop--;}
      | {inloop++;}for_stmt{inloop--; cout << "stmt => forstmt\n";}
      | returnstmt {cout << "stmt => returnstmt\n";}
      | BREAK SEMICOLON{if(inloop==0) { red(); cout << "Error: Cannot use BREAK when not in loop -> line " << yylineno << endl; cout << "stmt => break;\n"; reset();}  }
      | CONTINUE SEMICOLON{if(inloop==0){ red(); cout << "Error: Cannot use CONTINUE when not in loop -> line " << yylineno << endl;cout << "stmt => continue;\n"; reset();} }
      | block {cout << "stmt => block\n";}
      | funcdef {cout << "stmt => funcdef\n";}
      | SEMICOLON {cout << "stmt => ;\n";}
      ;

expr: assignexpr {$$=$1;}
      | expr PLUS expr {$$ =newexpr(arithexp_e);
                        $$->sym=newtmp();
                        emit(add,$1,$3,$$,0,yylineno);}
      | expr MINUS expr {$$ =newexpr(arithexp_e);
                        $$->sym=newtmp();
                        emit(sub,$1,$3,$$,0,yylineno);}
      | expr MUL expr   {$$ =newexpr(arithexp_e);
                         $$->sym=newtmp();
                         emit(mul,$1,$3,$$,0,yylineno);}
      | expr DIV expr  {$$ =newexpr(arithexp_e);
                        $$->sym=newtmp();
                        emit(div,$1,$3,$$,0,yylineno);;}
      | expr MOD expr  {$$ =newexpr(arithexp_e);
                        $$->sym=newtmp();
                        emit(mod,$1,$3,$$,0,yylineno);}
      | expr EQUAL expr {$$ = newexpr(boolexpr_e);
		                 $$->sym = newtmp();
                         //emit(if_eq, $1, $3, $$, nextQuad()+3 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         }
      | expr NOT_EQUAL expr {$$ = newexpr(boolexpr_e);
		                     $$->sym = newtmp();
                             //emit(if_noteq, $1, $3, $$, nextQuad()+3 , yylineno);
		                     //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                     //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                             //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         }
      | expr GREATER expr   {$$ = newexpr(boolexpr_e);
		                     $$->sym = newtmp();
                             //emit(if_greater, $1, $3, $$, nextQuad()+3 , yylineno);
		                     //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                     //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                             //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         }
      | expr LESS expr      {$$ = newexpr(boolexpr_e);
		                     $$->sym = newtmp();
                             //emit(if_less, $1, $3, $$, nextQuad()+3 , yylineno);
		                     //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                     //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                             //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         }
      | expr GREATER_EQUAL expr {$$ = newexpr(boolexpr_e);
		                         $$->sym = newtmp();
                                 //emit(if_greatereq, $1, $3, $$, nextQuad()+3 , yylineno);
		                         //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                         //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                                 //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                                }
      | expr LESS_EQUAL expr    {$$ = newexpr(boolexpr_e);
		                         $$->sym = newtmp();
                                 //emit(if_lesseq, $1, $3, $$, nextQuad()+3 , yylineno);
		                         //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                         //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                                 //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                                 }
      | expr AND expr {$$ = newexpr(boolexpr_e);
		                         $$->sym = newtmp();
                                 //emit(andc, $1, $3, $$, nextQuad()+3 , yylineno);
		                         //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                         //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
                                 //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
        
      }
      | expr OR expr {$$ = newexpr(boolexpr_e);
		              $$->sym = newtmp();
		              //emit(or_c, $1, $3, $$, nextQuad()+3 , yylineno);
		              //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		              //emit(jump,NULL,NULL,NULL,nextQuad()+2, yylineno);
		              //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                      }
      | term {$$=$1;}
      ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {$$=$2;}
	    | UMINUS expr  {check_arith($2);
                        $$ =newexpr(arithexp_e);
                        $$->sym=newtmp();
                        emit(uminus,$2,NULL,$$,0,yylineno);}
	    | NOT expr {$$ =newexpr(arithexp_e);
                        $$->sym=newtmp();
                        emit(notc,$2,NULL,$$,0,yylineno);}
		| PLUS_PLUS lvalue {
                             //lookup
                             //if programfunc_s || libraryfunc_s
                             //Error
                             //if tableitem_e
                             //$$=emit_iftableitem($2);
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=1;
                             //emit(add,$$,tmp,$$,0,yylineno);
                             //emit(tablesetelem,$2->getIndex(),$$,$2,0,yylineno);
                             //else
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=1;
                             //emit(add,$2,tmp,$2,0,yylineno);
                             //$$ = newexpr(arithexp_e);
                             //$$->sym=newtmp();
                             //emit(assign,$2,NULL,$$,0,yylineno);
                           }
		| lvalue PLUS_PLUS {
                             //lookup
                             //if programfunc_s || libraryfunc_s
                             //check_arith($$);
                             //Error
                             //if tableitem_e
                             //expr *value = emit_iftableitem($1);
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=1;
                             //emit(assign,value,NULL,$$,0,yylineno);
                             //emit(add,value,tmp,value,0,yylineno);
                             //emit(tablesetelem,$1->getIndex(),value,$1,0,yylineno);
                             //else
                             //emit(assign,$1,NULL,$$,0,yylineno);
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=1;
                             //emit(add,$1,tmp,$1,0,yylineno);
                           }
		| MINUS_MINUS lvalue {
                             //lookup
                             //if programfunc_s || libraryfunc_s
                             //Error
                             //if tableitem_e
                             //$$=emit_iftableitem($2);
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=-1;
                             //emit(add,$$,tmp,$$,0,yylineno);
                             //emit(tablesetelem,$2->getIndex(),$$,$2,0,yylineno);
                             //else
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=-1;
                             //emit(add,$2,tmp,$2,0,yylineno);
                             //$$ = newexpr(arithexp_e);
                             //$$->sym=newtmp();
                             //emit(assign,$2,NULL,$$,0,yylineno);
                           }
		| lvalue MINUS_MINUS {
                             //lookup
                             //if programfunc_s || libraryfunc_s
                             //Error
                             //if tableitem_e
                             //expr *value = emit_iftableitem($1);
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=-1;
                             //emit(assign,value,NULL,$$,0,yylineno);
                             //emit(add,value,tmp,value,0,yylineno);
                             //emit(tablesetelem,$1->getIndex(),value,$1,0,yylineno);
                             //else
                             //emit(assign,$1,NULL,$$,0,yylineno);
                             //expr *tmp = newexpr(costnum_e);
                             //tmp->numConst=-1;
                             //emit(add,$1,tmp,$1,0,yylineno);
                             }
		| primary {$$=$1;}
		;

assignexpr: lvalue ASSIGN expr{		
                                //lookup
                                //if programfunc_s || libraryfunc_s
                                //Error
                                //code...
                            }
                        
            ;
	
primary: lvalue{ $$ = emit_iftableitem($1);}
        | call {$$=$1;}
        | objectdef {$$=$1;;}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS{$$ = newexpr(programfunc_e);
                                                    $$->insertsym($2);}
		| const{$$=$1;}
        ;

lvalue: ID { /*wait for irene to fix 2nd phase*/
            string name($1);

            if(lookupactivevar(name).isActive == false && lookupactivefunc(name).isActive == false ){
                SymbolTableEntry ent;
                ent.isActive = true;
                if(scope == 0)
                    ent.type = GLOBAL;
                else
                    ent.type = LOCALV;
                ent.varVal.name = name;
                ent.varVal.scope = scope;
                ent.varVal.line = yylineno;
                insert(ent);
            }else{
                if(lookupactivevar(name).varVal.scope < infunction && lookupactivevar(name).varVal.scope > 0){
                    red(); cout << "Error: " <<" There is function between the uses of variable "<< name <<endl; reset();
                }
            }
            cout << "lvalue => id:" << yylval.stringVal<<endl;
            }
        | LOCAL ID {
            string name($2);
            //if(is_sysfunc(name)) {red(); cout << "Error: "<< name <<" is a system function, it cannot be used as local variable.\n"; reset();
            //}else 
            if(lookupactivevar(name).isActive == false && lookupactivefunc(name).isActive == false){
                SymbolTableEntry ent;
                ent.isActive = true;
                if(scope == 0)
                    ent.type = GLOBAL;
                else
                    ent.type = LOCALV;
                ent.varVal.name = name;
                ent.varVal.scope = scope;
                ent.varVal.line = yylineno;
                insert(ent);
            }
            cout << "lvalue => local id:" << yylval.stringVal<<", "<<scope<<endl;
        }
        | DOUBLE_COLON ID {
            string name($2);
            if(lookupcurrentscope(name, 0).isActive == false){
                red();
                cout << "Error: there is no global variable with name "<<name<<endl;
                reset();
            }
            cout << "lvalue => ::id:" << yylval.stringVal<<endl;
        }
        | member{cout << "lvalue => member" <<endl;}
        ;

member: lvalue PERIOD ID {}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET {$1 = emit_iftableitem($1);
                                                  $$ = newexpr(tableitem_e);
                                                  //$$->insertsym($1->sym);
                                                  //$$->index=$3;
                                                  }
		| call PERIOD ID {}
		| call LEFT_BRACKET expr RIGHT_BRACKET {}
		;

call:	call LEFT_PARENTHESIS elist	RIGHT_PARENTHESIS {$$ = make_call($1,$3);}
		| lvalue callsuffix  {/**/}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {expr *func = newexpr(programfunc_e);
                                                                                                func->insertsym($2);
                                                                                                $$ = make_call(func,$5);}
		;

callsuffix:	normcall {$$=$1;}
			| methodcall {$$=$1;}
			;


normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {cout << "normcall => (elist)\n";}
          ;
        
methodcall: DOUBLE_PERIOD ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {cout << "methodcall => ..id(elist)\n";}
            ;

elist: expr  {cout << "elist => expr\n";}
       | elist COMMA expr {cout << "elist => elist,expr\n";}
       | {cout << "elist => empty\n";}
       ;

objectdef: LEFT_BRACKET elist RIGHT_BRACKET {cout << "objectdef => [elist]\n";}
           | LEFT_BRACKET indexed RIGHT_BRACKET  {cout << "objectdef => [indexed]\n";}
           ;

indexed: indexedelem  {cout << "indexed => indexedelem\n";}
       | indexed COMMA indexedelem       {cout << "indexed => indexed, indexelem\n";}
       ;

indexedelem: LEFT_BRACE { /* flag_insert=0; */ } expr COLON { /* flag_insert=1; */ } expr RIGHT_BRACE  {cout << "indexedelem => {expr:expr}\n";}
             ;

block: LEFT_BRACE{scope++;} stmtlist RIGHT_BRACE{hide(scope--); cout << "block => {stmtlist}\n";}
       ;

stmtlist: stmt stmtlist  {cout << "stmtlist => stmt stmtlist\n";}
          |
          ;

funcdef: funcprefix funcargs funcbody {}
            
         ;
funcprefix: FUNCTION funcname{
            string name= $2;
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){red(); cout << "Error: " << name << " is declared in this scope already.\n"; reset();
            }else if(is_sysfunc(name)){red(); cout << "Error: "<< name <<" is a system function, it cannot be overriden.\n"; reset();
            }else {
                  ste.type=USERFUNC;
                  ste.isActive=TRUE;
                  ste.funcVal.name=name;
                  ste.funcVal.scope=scope;
                  ste.funcVal.line=yylineno;
                  insert(ste);
            }
            
         } 
         | FUNCTION {
            string fid=to_string(funcid++);
            string name= "$f" + fid;
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            while(true){
                if(ste.isActive || is_sysfunc(name)){ 
                    fid= to_string(funcid++);
                    name= "$f" + fid;
                    ste= lookupcurrentscope(name,scope);
                }else {
                    ste.isActive=TRUE;
                    ste.type=USERFUNC;
                    ste.funcVal.name=name;
                    ste.funcVal.scope=scope;
                    ste.funcVal.line=yylineno;
                    insert(ste);
                    break;
                }
            }
         } 
funcname: ID { $$ = $1; }
funcblockstart: {}
funcblockend: {}
funcargs:  LEFT_PARENTHESIS {scope++;} idlist RIGHT_PARENTHESIS {}
funcbody: { scope--; infunction++;}funcblockstart block funcblockend {infunction--;cout <<"funcdef => function(idlist)block\n"; exitscopespace(); } ;


const:	INTEGER {$$ = newexpr(costnum_e);
                 $$->numConst=$1;}
		| REAL {$$ = newexpr(costnum_e);
                 $$->numConst=$1;}
		| STRING {$$ = newexpr(conststring_e);
                  $$->strConst=$1($1);}
		| NIL {$$ = newexpr(nil_e)}
		| TRUE { $$ = newexpr(constbool_e);
                 $$->boolConst=true;}
		| FALSE { $$ = new expr(constbool_e);
                  $$->boolConst=false;}
		;

idlist: ID{
            //lookup
            //if exists then throw error for redefinition
            //if libfunc
            //error
            //else
            //insert in table
           
          }
        | idlist COMMA ID{
            //lookup
            //if exists then throw error for redefinition
            //if libfunc
            //error
            //else
            //insert in table
         }
        |
       ;

loopstart : {++inloop;};

loopend : {--inloop;};

loopstmt : loopstart stmt loopend { $$ = $2; } ;



ifstmt:	if_prefix stmt {//patchlabel($1-2, $1+1);
                        //patchlabel($1-1, nextQuad()+1);
                       }
		|if_prefix stmt else_prefix stmt   {//patchlabel($1-2,$3-1); //if eq if_prefix
                                            //patchlabel($1-1,$3+2); //jmp if_prefix
                                            //patchlabel($3, nextQuad()+1); // jmp to end   
        }
	    ;	 
if_prefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{expr *tmp = newexpr(constbool_e);
                                                      tmp->boolConst=true;
                                                      //code..
                                                      //backpatching (truelist & falselist)
                                                      $$ = nextQuad();
                                                      }

else_prefix: ELSE {$$ = nextQuad();   
                   emit(jump,NULL,NULL,NULL,0,yylineno);}


whilestmt: whilestart whilecon loopstmt {
    cout <<"whilestmt => while(expr) stmt"<<endl;
    emit(jump,NULL,NULL,NULL,$1,yylineno); 
    patchlabel($2, nextQuad()+1); } ; 

whilestart: WHILE{$$=nextQuad()+1;}

whilecon: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {/*code..*/}
	
for_stmt: for_prefix N elist RIGHT_PARENTHESIS N loopstmt N {cout <<"forstmt => for(elist;expr;elist) stmt"<<endl;}
		;
N: {/*unfinished jump*/ $$ = nextquad(); emit(jump,NULL,NULL,0);}

Q:{/*nextquad*/ $$ = nextquad(); }

for_prefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON{}

M: {}

returnstmt: RETURN expr SEMICOLON{
                    if(infunction==0) { red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                    cout <<"returnstmt => return expr;"<<endl;
                    emit(ret,NULL,NULL,$2,0,yylineno);
         }
			| RETURN SEMICOLON { if(infunction==0) {red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                                cout <<"returnstmt => return;"<<endl;
                                emit(ret,NULL,NULL,NULL,0,yylineno);}
            ;

%%     

int yyerror (char* yaccProvidedMessage)
{
    fprintf(stderr, "\033[1;31m %s: at line %d, before token: %s\n", yaccProvidedMessage, yylineno, yytext);
    fprintf(stderr, "INPUT NOT VALID \033[0m \n");
}

void insertLibFuncs(string name){
    SymbolTableEntry ste;
    ste.isActive=true;
    ste.type=LIBFUNC;
    ste.funcVal.name=name;
    ste.funcVal.scope=0;
    ste.funcVal.line=0;
    insert(ste);
}

void red()
{
    cout<<"\033[1;31m";
}

void reset()
{
    cout<<"\033[0m";
}
//**********************************************************************************************************

int main (int argc, char** argv) {
    if (argc > 1) {
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
    printsymbols();
    return 0;
}