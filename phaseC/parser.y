%{
    #include <iostream>
    #include <string>
    #include "symbol_table.h"
    #include "icode.h"

    /* #define YY_DECL int alpha_yylex (void* yylval)*/
    extern int yylex(void);
    int yyerror (char* yaccProvidedMessage);
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
   /*  extern int tmpc; */

    /* int flag_insert=1; */
    int inloop=0;
    int infunction=0;
    int scope=0;
    int funcid=0;
    string lastidname;
    bool isMember=false;

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
    expr *exp;
}

%token <intVal> INTEGER
%token <doubleVal> REAL
%token <stringVal> ID STRING 
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token ASSIGN PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL UMINUS
%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON PERIOD DOUBLE_PERIOD

%type <exp> lvalue expr term assignexpr const primary member objectdef call
%type <stringVal> funcname

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
                                string name = $1;
                                if((is_sysfunc(name) || lookupactivefunc(name).isActive==true)){
                                    red();
                                    cout <<"Error: " <<name << " is defined as function \n";
                                    reset();
                                }else{
                                    int i = scope;
                                    SymbolTableEntry lookupent;
                                    while(i >=0 ){
                                        lookupent = lookupcurrentscope(name, i);
                                        if(lookupent.isActive)
                                            break;
                                        i--;
                                    }
                                    if(lookupent.type == USERFUNC && lookupcurrentscope(name, scope).type != LOCALV){
                                        red();
                                        cout <<"Error: " <<name << " is defined as function \n";
                                        reset();
                                    }
                                }
                                //if programfunc_s || libraryfunc_s
                                //Error
                                //code...
                            }
                        
            ;
	
primary: lvalue{ $$ = emit_iftableitem($1);}
        | call {$$=$1;}
        | objectdef {$$=$1;}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS{$$ = newexpr(programfunc_e);
                                                     $$->sym = $2;}
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
                ent.name = name;
                ent.scope = scope;
                ent.line = yylineno;
                inccurrscopeoffset()
                ent.offset = currscopeoffset();
                ent.scopespace = currscopespace();
                ent.symt = var_s;
                insert(ent);
                $$ = lvalue_exp(ent);
            }else{
                if(lookupactivevar(name).varVal.scope < infunction && lookupactivevar(name).varVal.scope > 0){
                    red(); cout << "Error: " <<" There is function between the uses of variable "<< name <<endl; reset();
                }
            }

            
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
                ent.name = name;
                ent.scope = scope;
                ent.line = yylineno;
                inccurrscopeoffset()
                ent.offset = currscopeoffset();
                ent.scopespace = currscopespace();
                ent.symt = var_s;
                insert(ent);
                $$ = lvalue_exp(ent);
                
                $$ = lvalue_exp(ent);
            }
           
        }
        | DOUBLE_COLON ID {
            string name($2);
            SymbolTableEntry ste = lookupcurrentscope(name,0);
            if(ste.isActive == false){
                red();
                cout << "Error: there is no global variable with name "<<name<<endl;
                reset();
            } else{
                $$=lvalue_exp(ste);
            }
            
        }
        | member{isMember=true;}
        ;

member: lvalue PERIOD ID {$$ = member_item($1,$3);}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET {$1 = emit_iftableitem($1);
                                                  $$ = newexpr(tableitem_e);
                                                  $$->sym = $1->sym;
                                                  $$->index=$3;
                                                  }
		| call PERIOD ID {}
		| call LEFT_BRACKET expr RIGHT_BRACKET {}
		;

call:	call LEFT_PARENTHESIS elist	RIGHT_PARENTHESIS {$$ = make_call($1,$3);}
		| lvalue callsuffix  {
                              if ($2->method){
                                expr *self = $1;
                                $1 = emit_iftableitem(member_item(self,$2->name)); 
                                self->next = $2;
                                $2 = self; /*pushing self in front*/
                              }
                              $$ = make_call($1,$2->elist);
                            }
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {expr *func = newexpr(programfunc_e);
                                                                                                func->sym=$2;
                                                                                                $$ = make_call(func,$5);}
		;

callsuffix:	normcall {$$=$1;}
			| methodcall {$$=$1;}
			;


normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {calls c;
                                                     c.name="nil";
                                                     c.method=false;
                                                     c.elist=$2;
                                                     $$=c;
                                                     }
          ;
        
methodcall: DOUBLE_PERIOD ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {calls c;
                                                                        c.name=$2;
                                                                        c.method=true;
                                                                        c.elist=$4;
                                                                        $$=c;
                                                                        }
            ;

elist: expr  {cout << "elist => expr\n";}
       | elist COMMA expr {cout << "elist => elist,expr\n";}
       | {cout << "elist => empty\n";}
       ;

objectdef: LEFT_BRACKET elist RIGHT_BRACKET {expr *tmp = newexpr(newtable_e);
                                             tmp->sym=newtmp();
                                             emit(tablecreate,NULL,NULL,tmp,0,yylineno);
                                             /* for(list <indexedelements>::iterator i = $2->getIndexedList()->begin() ;  i!=$2->getIndexedList()->end() ; i++){
                                                emit(tablesetelem,i->getIndexElement(),i->getValueElement(),tmp,0,yylineno);
                                             } */
                                             $$ = tmp;
                                             }
           | LEFT_BRACKET indexed RIGHT_BRACKET  {
                                                  int numc=0;
                                                  expr *tmp = newexpr(newtable_e);
                                                  tmp->sym=newtmp();
                                                  emit(tablecreate,NULL,NULL,tmp,0,yylineno);
                                                  /* for(list <expr>::iterator i = $2->getElist()->begin() ; i!=$2->getElist()->end() ; i++){
                                                  tmp2 = newexpr(costnum_e);
                                                  tmp2->numConst=numc;
                                                  emit(tablesetelem,tmp2,&*i,tmp,0,yylineno);
                                                  numc++;
                                                  } */
                                                  $$ = tmp;
                                                  }
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

funcdef: func_prefix func_args func_body { exitscopespace(); 
                                           /* $$->setTotalLocals(getfunctionLocalOffset());
                                           setfunctionLocalOffset(functionLocalStack.top());
                                           functionLocalStack.pop(); */
                                           $$=$1;
                                           emit(funcend,NULL,NULL,lvalue_exp($1),0,yylineno);
                                           }
                                           ;
func_prefix: FUNCTION ID{
            string name= $2;
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){red(); cout << "Error: " << name << " is declared in this scope already.\n"; reset();
            }else if(is_sysfunc(name)){red(); cout << "Error: "<< name <<" is a system function, it cannot be overriden.\n"; reset();
            }else {
                  ste.type=USERFUNC;
                  ste.name = name;
                  ste.scope = scope;
                  ste.line = yylineno;
                  ste.isActive=true;
                  inccurrscopeoffset()
                  ste.offset = currscopeoffset();
                  ste.scopespace = currscopespace();
                  ste.symt=programfunc_s;
                  insert(ste);
            }
            $$ = ste;
            emit(funcstart,NULL,NULL,lvalue_exp(ste),0,yylineno);
            /* functionLocalStack.push(getfunctionLocalOffset()); */
            enterscopespace();
            resetformalargoffset();
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
                    ste.type=USERFUNC;
                    ste.name = name;
                    ste.scope = scope;
                    ste.line = yylineno;
                    ste.isActive=true;
                    inccurrscopeoffset()
                    ste.offset = currscopeoffset();
                    ste.scopespace = currscopespace();
                    ste.symt=programfunc_s;
                    insert(ste);
                    break;
                }
            }
            $$ = ste;
            emit(funcstart,NULL,NULL,lvalue_exp(ste),0,yylineno);
            /* functionLocalStack.push(getfunctionLocalOffset()); */
            enterscopespace();
            resetformalargoffset();
         } 
         ;

func_bstart: {/* LoopCounterStack.push(loopcounter); loopcounter=0; */ };

func_bend:  { /* loopcounter = LoopCounterStack.top(); LoopCounterStack.pop(); */ };

func_args:  LEFT_PARENTHESIS {scope++;} idlist RIGHT_PARENTHESIS {enterscopespace(); resetfunctionlocaloffset();};
                                                                 
func_body: { scope--; infunction++;} func_bstart block func_bend {infunction--;cout <<"funcdef => function(idlist)block\n"; exitscopespace(); } ;


const:	INTEGER {$$ = newexpr_constnum($1);
                 /* $$->numConst=$1; */}
		| REAL {$$ = newexpr_constnum($1);
                 /* $$->numConst=$1; */}
		| STRING {$$ = newexpr_conststring($1);
                  /* $$->strConst=$1; */}
		| NIL {$$ = newexpr(nil_e);}
		| TRUE { $$ = newexpr_constbool($1);
                 /* $$->boolConst=true; */}
		| FALSE { $$ = newexpr_constbool($1);
                  /* $$->boolConst=false; */}
		;

idlist: ID{
            string name = $1; 
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){red(); cout << "Error: " << name << " is declared in this scope already.\n"; reset();
            }else if(is_sysfunc(name)){red(); cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n"; reset();
            }else {
                  ste.type=FORMAL;
                  ste.name = name;
                  ste.scope = scope;
                  ste.line = yylineno;
                  ste.isActive=true;
                  inccurrscopeoffset()
                  ste.offset = currscopeoffset();
                  ste.scopespace = currscopespace();
                  ste.symt=var_s;
                  insert(ste);
            }
          }
        | idlist COMMA ID{
            string name = $3; 
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){red(); cout << "Error: " << name << " is declared in this scope already.\n"; reset();
            }else if(is_sysfunc(name)){red(); cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n"; reset();
            }else {
                  ste.type=FORMAL;
                  ste.name = name;
                  ste.scope = scope;
                  ste.line = yylineno;
                  ste.isActive=true;
                  inccurrscopeoffset()
                  ste.offset = currscopeoffset();
                  ste.scopespace = currscopespace();
                  ste.symt=var_s;
                  insert(ste);
            }
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

whilestart: WHILE{$$=nextQuad()+1;};

whilecon: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {/*code..*/};
	
for_stmt: for_prefix N elist RIGHT_PARENTHESIS N loopstmt N {cout <<"forstmt => for(elist;expr;elist) stmt"<<endl;};
		
N: {/*unfinished jump*/ $$ = nextquad(); emit(jump,NULL,NULL,0);};

M:{/*nextquad*/ $$ = nextquad(); };

for_prefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON{};

returnstmt: RETURN expr SEMICOLON{
                    if(infunction==0) { red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                    /* cout <<"returnstmt => return expr;"<<endl; */
                    emit(ret,NULL,NULL,$2,0,yylineno);
         }
			| RETURN SEMICOLON { if(infunction==0) {red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                                cout <<"returnstmt => return;"<<endl;
                                emit(ret,NULL,NULL,NULL,0,yylineno);
                                }
            ;

%%     

int yyerror (char* yaccProvidedMessage)
{
    fprintf(stderr, "\033[1;31m %s: at line %d, before token: %s\n", yaccProvidedMessage, yylineno, yytext);
    fprintf(stderr, "INPUT NOT VALID \033[0m \n");
}

void insertLibFuncs(string name){
    SymbolTableEntry ste;
    ste.type=LIBFUNC;
    ste.name = name;
    ste.scope = 0;
    ste.line = 0;
    ste.isActive=true;
    ste.symt=libraryfunc_s;
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