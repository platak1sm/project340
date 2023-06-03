%{
    // #include <iostream>
    // #include <string>
    // // #include "symbol_table.h"
    // #include "icode.h"
    // #include <list>
    // #include <stack>
    #include "target.hpp"

    /* #define YY_DECL int alpha_yylex (void* yylval)*/
    extern int yylex(void);
    int yyerror (char* yaccProvidedMessage);
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    stack <unsigned> funcLocalStack;
    stack <unsigned> loopCountStack;
    extern unsigned programVarOffset, functionLocalOffset, formalArgOffset;
   /*  extern int tmpc; */

    /* int flag_insert=1; */
    int inloop=0;
    int infunction=0;
    int scope=0;
    int funcid=0;
    string lastidname;
    bool isMember=false;
    int loopc=0;

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
    bool boolVal;
    struct expr *expVal;
    struct forpr *forVal;
    struct SymbolTableEntry *steVal;
    struct calls *callVal;
    struct indexedelements *indelVal;
    struct stmt_t *stmtVal;
    struct elist_t *elistVal;
}

%token <intVal> INTEGER
%token <doubleVal> REAL
%token <stringVal> ID STRING 
%token <boolVal> TRUE FALSE
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL NIL
%token ASSIGN PLUS MINUS MUL DIV MOD EQUAL NOT_EQUAL PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL UMINUS
%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON PERIOD DOUBLE_PERIOD

%type <expVal> lvalue expr term assignexpr const primary member objectdef call indexedelem 
%type <intVal> if_prefix else_prefix whilestart whilecon N M func_body func_bend
%type <steVal>  funcdef func_prefix
%type <forVal> for_prefix
%type <stmtVal> stmtlist stmt ifstmt for_stmt whilestmt block loopstmt returnstmt
%type <callVal> normcall methodcall callsuffix
%type <elistVal> elist indexed 





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
 
stmt: expr SEMICOLON {
    //make_stmt($$);
    if($1->type == boolexpr_e){
        if(istempname($1->sym.name)){
            $$->sym = $1->sym;
        }else{
            $$->sym = newtmp();
        }
        patchlist($1->truequad, nextquad());
        emit(assign, newexpr_constbool(1), NULL, $1, nextquad(), yylineno);
        emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
        patchlist($1->falsequad, nextquad());
        emit(assign, newexpr_constbool(0), NULL, $1, nextquad(), yylineno);
        resettmpcounter();
    }
    cout << "stmt => expr\n";
    }
      | ifstmt {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => ifstmt\n";
        }
      | {inloop++;}whilestmt{
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => whilestmt\n";inloop--;}
      | {inloop++;}for_stmt{
        //make_stmt($$);
        resettmpcounter();
        inloop--; cout << "stmt => for_stmt\n";}
      | returnstmt {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => returnstmt\n";}
      | BREAK SEMICOLON{
        //make_stmt($$);
        $$->breakList = newlist(nextquad());
        $$->contList = 0;
        emit(jump, NULL, NULL, NULL, 0, yylineno);
        
        if(inloop==0) { 
            red(); 
            cout << "Error: Cannot use BREAK when not in loop -> line " << yylineno << endl;
             cout << "stmt => break;\n"; reset();}
        resettmpcounter();

        }
      | CONTINUE SEMICOLON{
        //make_stmt($$);
        $$->contList = newlist(nextquad());
        $$->breakList = 0;
        emit(jump, NULL, NULL, NULL, 0, yylineno);
        if(inloop==0){ red(); cout << "Error: Cannot use CONTINUE when not in loop -> line " << yylineno << endl;cout << "stmt => continue;\n"; reset();}
        resettmpcounter(); }
      | block {
        $$ = $1;
        resettmpcounter();
        cout << "stmt => block\n";}
      | funcdef {
       // make_stmt($$);
        resettmpcounter();
        cout << "stmt => funcdef\n";}
      | SEMICOLON {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => ;\n";}
      ;

expr: assignexpr {$$=$1;
                 cout << "expr => assignexpr\n";}
      | expr PLUS expr {$$ =newexpr(arithexp_e);
                        if(istempname($1->sym.name)){
                            $$->sym = $1->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        cout << "expr => expr+expr\n";
                        emit(add,$1,$3,$$,0,yylineno);}
      | expr MINUS expr {$$ =newexpr(arithexp_e);
                        if(istempname($1->sym.name)){
                            $$->sym = $1->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        cout << "expr => expr-expr\n";
                        emit(sub,$1,$3,$$,0,yylineno);}
      | expr MUL expr   {$$ =newexpr(arithexp_e);
                         if(istempname($1->sym.name)){
                            $$->sym = $1->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        cout << "expr => expr*expr\n";
                         emit(mul,$1,$3,$$,0,yylineno);}
      | expr DIV expr  {$$ =newexpr(arithexp_e);
                        if(istempname($1->sym.name)){
                            $$->sym = $1->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        cout << "expr => expr div expr\n";
                        emit(divc,$1,$3,$$,0,yylineno);}
      | expr MOD expr  {$$ =newexpr(arithexp_e);
                        if(istempname($1->sym.name)){
                            $$->sym = $1->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        cout << "expr => expr mod expr\n";
                        emit(mod,$1,$3,$$,0,yylineno);}
      | expr EQUAL{
        if($1->type == boolexpr_e){
            if(istempname($1->sym.name)){
                $<expVal>$->sym = $1->sym;
            }else{
                $<expVal>$->sym = newtmp();
            }
            patchlist($1->truequad, nextquad());
            emit(assign, newexpr_constbool(1), NULL, $1, nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
            patchlist($1->falsequad, nextquad());
            emit(assign, newexpr_constbool(0), NULL, $1, nextquad(), yylineno);
        }
      } expr {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         $$->truequad = nextquad();
                         $$->falsequad = nextquad()+1;
                         emit(if_eq, $1, $4, NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr == expr\n";
                         }
      | expr NOT_EQUAL{
        if($1->type == boolexpr_e){
            if(istempname($1->sym.name)){
                $<expVal>$->sym = $1->sym;
            }else{
                $<expVal>$->sym = newtmp();
            }
            patchlist($1->truequad, nextquad());
            emit(assign, newexpr_constbool(1), NULL, $1, nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
            patchlist($1->falsequad, nextquad());
            emit(assign, newexpr_constbool(0), NULL, $1, nextquad(), yylineno);
        }
      } expr {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         $$->truequad = nextquad();
                         $$->falsequad = nextquad()+1;
                         emit(if_noteq, $1, $4, NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr!=expr\n";
                         }
      | expr GREATER expr   {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         $$->truequad = nextquad();
                         $$->falsequad = nextquad()+1;
                         emit(if_greater, $1, $3, NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr > expr\n";
                         }
      | expr LESS expr      {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         $$->truequad = nextquad();
                         $$->falsequad = nextquad()+1;
                         emit(if_less, $1, $3, NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr < expr\n";
                         }
      | expr GREATER_EQUAL expr {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         $$->truequad = nextquad();
                         $$->falsequad = nextquad()+1;
                         emit(if_greatereq, $1, $3, NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr >= expr\n";
                                }
      | expr LESS_EQUAL expr    {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         $$->truequad = nextquad();
                         $$->falsequad = nextquad()+1;
                         emit(if_lesseq, $1, $3, NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr <= expr\n";
                                 }
      | expr AND {
    
        if($1->type != boolexpr_e){
            $1->truequad = nextquad();
            $1->falsequad = nextquad()+1;
            emit(if_eq, $1, newexpr_constbool(1), NULL, 0 , yylineno);
            emit(jump,NULL,NULL,NULL,0, yylineno);
            patchlist($1->truequad, nextquad());
        }
        
        
      }M expr {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                        if($5->type != boolexpr_e){
                            $5->truequad = nextquad();
                            $5->falsequad = nextquad()+1;
                            emit(if_eq, $5, newexpr_constbool(1), NULL, 0 , yylineno);
                            //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
                            emit(jump,NULL,NULL,NULL,0, yylineno);
                            //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                        }
                        if($1->type == boolexpr_e){
                            patchlist($1->truequad, $4);
                        }
                        $$->falsequad = mergelist($1->falsequad, $5->falsequad);
                        $$->truequad = $5->truequad;
                         cout << "expr => expr && expr\n";
        
      }
      | expr OR{
        if($1->type != boolexpr_e){
            $1->truequad = nextquad();
            $1->falsequad = nextquad()+1;
            emit(if_eq, $1, newexpr_constbool(1), NULL, 0 , yylineno);
            emit(jump,NULL,NULL,NULL,0, yylineno);
            patchlist($1->falsequad, nextquad());
        }
      } M expr {$$ = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                        if($5->type != boolexpr_e){
                            $5->truequad = nextquad();
                            $5->falsequad = nextquad()+1;
                            emit(if_eq, $5, newexpr_constbool(1), NULL, 0 , yylineno);
                            //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
                            emit(jump,NULL,NULL,NULL,0, yylineno);
                            //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                        }
                        if($1->type == boolexpr_e){
                            patchlist($1->falsequad, $4);
                        }
                        $$->truequad = mergelist($1->truequad, $5->truequad);
                        $$->falsequad = $5->falsequad;
                        cout << "expr => expr || expr\n";
                      }
      | term {
        $$=$1;
        cout << "expr => term\n";}
      ;

term:   LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {$$=$2;
                                                cout << "term => (expr)\n";}
	    | MINUS expr %prec UMINUS  {check_arith($2);
                        $$ =newexpr(arithexp_e);
                        if(istempname($2->sym.name)){
                            $$->sym = $2->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        cout << "term => -expr\n";
                        emit(mul,$2,newexpr_constnum(-1),$$,-1,yylineno);}
	    | NOT expr {$$ =newexpr(boolexpr_e);
                        if(istempname($2->sym.name)){
                            $$->sym = $2->sym;
                        }else{
                            $$->sym = newtmp();
                        }
                        if($2->type != boolexpr_e){
                            $2->truequad = nextquad();
                            $2->falsequad = nextquad()+1;
                            emit(if_eq, $2,newexpr_constbool(1), NULL, 0, yylineno);
                            emit(jump, NULL, NULL, NULL, 0, yylineno);
                        }
                        int tmp = $2->truequad;
                        $$->truequad = $2->falsequad;
                        $$->falsequad = tmp;
                        cout << "term => !expr\n";
                        }
		| PLUS_PLUS lvalue {
                            string name = $2->sym.name;
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
                            check_arith($2);
                            if($2->type == tableitem_e){
                                $$ = emit_iftableitem($2);
                                emit(add,$$,newexpr_constnum(1), $$, -1, yylineno);
                                emit(tablesetelem, $2, $2->index, $$, -1, yylineno);
                            }else{
                                emit(add,$2,newexpr_constnum(1), $2, -1, yylineno);
                                $$ = newexpr(arithexp_e);
                                if(istempname($2->sym.name)){
                                    $$->sym = $2->sym;
                                }else{
                                    $$->sym = newtmp();
                                }
                                emit(assign, $2, NULL, $$, -1, yylineno);
                            }
                            cout << "term => ++lvalue\n";
                           }
		| lvalue PLUS_PLUS {
                            string name = $1->sym.name;
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
                            check_arith($1);
                            $$ = newexpr(var_e);
                            if(istempname($1->sym.name)){
                                $$->sym = $1->sym;
                            }else{
                                $$->sym = newtmp();
                            }
                            if($1->type == tableitem_e){
                                expr* val = emit_iftableitem($1);
                                
                                emit(assign,val, NULL,$$,-1,yylineno);
                                emit(add,val, newexpr_constnum(1),val,-1,yylineno);
                                emit(tablesetelem, $1, $1->index, $$, -1, yylineno);
                            }else{
                                emit(assign,$1, NULL,$$,-1,yylineno);
                                emit(add,$1, newexpr_constnum(1),$1,-1,yylineno);
                            }
                            cout << "term => lvalue++\n";
                           }
		| MINUS_MINUS lvalue {
                            string name = $2->sym.name;
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
                            check_arith($2);
                            if($2->type == tableitem_e){
                                $$ = emit_iftableitem($2);
                                emit(sub,$$,newexpr_constnum(1), $$, -1, yylineno);
                                emit(tablesetelem, $2, $2->index, $$, -1, yylineno);
                            }else{
                                emit(sub,$2,newexpr_constnum(1), $2, -1, yylineno);
                                $$ = newexpr(arithexp_e);
                                if(istempname($2->sym.name)){
                                    $$->sym = $2->sym;
                                }else{
                                    $$->sym = newtmp();
                                }
                                emit(assign, $2, NULL, $$, -1, yylineno);
                            }
                            cout << "term => --lvalue\n";
                           }
		| lvalue MINUS_MINUS {
                             string name = $1->sym.name;
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
                            check_arith($1);
                            $$ = newexpr(var_e);
                            if(istempname($1->sym.name)){
                                $$->sym = $1->sym;
                            }else{
                                $$->sym = newtmp();
                            }
                            if($1->type == tableitem_e){
                                expr* val = emit_iftableitem($1);
                                
                                emit(assign,val, NULL,$$,-1,yylineno);
                                emit(sub,val, newexpr_constnum(1),val,-1,yylineno);
                                emit(tablesetelem, $1, $1->index, $$, -1, yylineno);
                            }else{
                                emit(assign,$1, NULL,$$,-1,yylineno);
                                emit(sub,$1, newexpr_constnum(1),$1,-1,yylineno);
                            }
                            cout << "term => lvalue--\n";
                             }
		| primary {$$=$1;
                    cout << "term => primary\n";}
		;

assignexpr: lvalue ASSIGN expr{		
                                string name = $1->sym.name;
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
                                if($3->type == boolexpr_e){
                                    if(istempname($3->sym.name)){
                                        $3->sym = $3->sym;
                                    }else{
                                        $3->sym = newtmp();
                                    }
                                    patchlist($3->truequad, nextquad());
                                    emit(assign, newexpr_constbool(1), NULL, $3, nextquad(), yylineno);
                                    emit(jump, NULL, NULL, NULL, nextquad(), yylineno);
                                    patchlist($3->falsequad, nextquad());
                                    emit(assign, newexpr_constbool(0), NULL, $3, nextquad(), yylineno);
                                }

                                if($1!=NULL){
                                    if($1->type == tableitem_e){
                                        emit(tablesetelem, $1, $1->index, $3, -1, yylineno);
                                        $$ = emit_iftableitem($1);
                                        $$->type = assignexp_e;
                                    }else{
                                        emit(assign, $3, NULL, $1, -1, yylineno);
                                        $$ = newexpr(assignexp_e);
                                        if(istempname($1->sym.name)){
                                            $$->sym = $1->sym;
                                        }else{
                                            $$->sym = newtmp();
                                        }
                                        emit(assign, $1, NULL,$$,-1,yylineno);
                                    }
                                }
                                cout << "assignexpr => lvalue=expr\n";
                            }
                        
            ;
	
primary: lvalue{ $$ = emit_iftableitem($1);
                cout << "primary => lvalue\n";}
        | call {$$=$1;
                cout << "primary => call\n";}
        | objectdef {$$=$1;
                    cout << "primary => objectdef\n";}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS{$$ = newexpr(programfunc_e);
                                                     $$->sym = *($2);
                                                    cout << "primary => (funcdef)\n";}
		| const{$$=$1;
                cout << "primary => const\n";}
        ;

lvalue: ID { 
    
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
                inccurrscopeoffset();
                ent.offset = currscopeoffset();
                ent.scopespace = currscopespace();
                ent.symt = var_s;
                
                insert(ent);
                
                $$ = lvalue_exp(ent);
                
            }else{
                if(lookupactivevar(name).scope < infunction && lookupactivevar(name).scope > 0){
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
                ent.name = name;
                ent.scope = scope;
                ent.line = yylineno;
                inccurrscopeoffset();
                ent.offset = currscopeoffset();
                ent.scopespace = currscopespace();
                ent.symt = var_s;
                insert(ent);
                
                
                $$ = lvalue_exp(ent);
            }
           cout << "lvalue => local id:" << yylval.stringVal<<", "<<scope<<endl;
        }
        | DOUBLE_COLON ID {
            string name($2);
            SymbolTableEntry ste = lookupcurrentscope(name,0);
            if(ste.isActive == false){
                red();
                cout << "Error: there is no global variable with name "<<name<<endl;
                reset();
                $$ = newexpr(nil_e);
            } else{
                $$=lvalue_exp(ste);
            }
            cout << "lvalue => ::id:" << yylval.stringVal<<endl;
        }
        | member{$$ = $1;
            isMember=true;
            cout << "lvalue => member" <<endl;}
        ;

member: lvalue PERIOD ID {$$ = member_item($1,$3);
                            cout << "member => lvalue.id\n";}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET {$1 = emit_iftableitem($1);
                                                  $$ = newexpr(tableitem_e);
                                                  $$->sym = $1->sym;
                                                  $$->index=$3;
                                                  cout << "member => lvalue[expr]\n";
                                                  }
		| call PERIOD ID {cout << "member => call.id\n";}
		| call LEFT_BRACKET expr RIGHT_BRACKET {cout << "member => call[expr]\n";}
		;

call:	call LEFT_PARENTHESIS elist	RIGHT_PARENTHESIS {
    $$ = make_call($1,$3);
    cout << "call => (elist)\n";}
		| lvalue callsuffix  {
                  
                  if ($2 != NULL && $2->method != NULL) {
                     expr *self = $1;
                     if (self != NULL) {
                        $1 = emit_iftableitem(member_item(self, $2->name));
                        $2->elist->elist.push_back(self);
                    }
                    $$ = make_call($1, $2->elist);
}

                              cout << "call => lvalue callsuffix\n";
                            }
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {expr *func = newexpr(programfunc_e);
                                                                                                func->sym=*($2);
                                                                                                $$ = make_call(func,$5);
                                                                                                cout << "call => (funcdef)(elist)\n";}
		;

callsuffix:	normcall {$$=$1;
                     cout << "callsuffix => normcall\n";}
			| methodcall {$$=$1;
                         cout << "callsuffix => methodcall\n";}
			;


normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {
                                                     calls c;
                                                     c.name="nil";
                                                     c.method=false;
                                                     c.elist=$2;
                                                     $$=&c;
                                                     cout << "normcall => (elist)\n";
                                                     }
          ;
        
methodcall: DOUBLE_PERIOD ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {calls c;
                                                                        c.name=$2;
                                                                        c.method=true;
                                                                        c.elist=$4;
                                                                        $$=&c;
                                                                        cout << "methodcall => ..id(elist)\n";
                                                                        }
            ;

elist: expr  {cout << "elist => expr\n";
             $$ = new elist_t();  
             $$->elist.push_back($1);   
             }
       | elist COMMA expr {cout << "elist => elist,expr\n";
                        $$ = new elist_t();  
                        $$->elist.push_back($3);  }
       | {cout << "elist => empty\n";
          $$ = new elist_t();  
        }
       ;

objectdef: LEFT_BRACKET elist RIGHT_BRACKET {  expr* t = newexpr(newtable_e);
                                                t->sym = newtmp();
                                                emit(tablecreate,  NULL, NULL,t, -1, yylineno);
                                                int num = 0;
                                                if ($2!=NULL && $2->elist.size()>0){
                                                    for(int i=0; i <= $2->elist.size() - 1; i++)
                                                    emit(tablesetelem, newexpr_constnum(i), $2->elist[i],t,-1,yylineno);
                                                }
                                                
                                                $$ = t;
                                             cout << "objectdef => [elist]\n";
                                             }
           | LEFT_BRACKET indexed RIGHT_BRACKET  {
                                                expr* t = newexpr(newtable_e);
                                                t->sym = newtmp();
                                                emit(tablecreate,  NULL, NULL,t, -1, yylineno);
                                                int num = 0;
                                                if ($2!=NULL && $2->elist.size()>0){
                                                    for(int i=0; i <= $2->elist.size() - 1; i++)
                                                    emit(tablesetelem, newexpr_constnum(i), $2->elist[i],t,-1,yylineno);
                                                }
                                                
                                                $$ = t;
                                                  cout << "objectdef => [indexed]\n";
                                                  }
           ;

indexed: indexedelem  {cout << "indexed => indexedelem\n";
                      $$ = new elist_t();
                      $$->elist.push_back($1);}
       | indexed COMMA indexedelem       
                      {cout << "indexed => indexed, indexelem\n";
                      $$ = new elist_t();
                      $$->elist.push_back($3);
                      }
       ;

indexedelem: LEFT_BRACE expr COLON expr RIGHT_BRACE  
                        {cout << "indexedelem => {expr:expr}\n";
                        if($2==NULL || $4==NULL){
                            $$=NULL;
                        }else{
                            expr *temp=$4;
                            temp->index=$2;
                        $$=temp;
                        }
                        
                        }
             ;

block: LEFT_BRACE{scope++;} stmtlist RIGHT_BRACE{hide(scope--); cout << "block => {stmtlist}\n";}
       ;

stmtlist: stmt stmtlist  {cout << "stmtlist => stmt stmtlist\n";}
          |
          ;

funcdef: func_prefix func_args func_body { exitscopespace(); 
                                           $$->totalloc=functionLocalOffset;
                                           functionLocalOffset=funcLocalStack.top();
                                           funcLocalStack.pop(); 
                                           $$=$1;
                                           emit(funcend,NULL,NULL,lvalue_exp(*($1)),0,yylineno);
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
                  ste.offset = currscopeoffset();
                  inccurrscopeoffset();
                  ste.scopespace = currscopespace();
                  ste.symt=programfunc_s;
                  ste.taddress = nextquad();
                  insert(ste);
                  $$ = &ste;
                  emit(funcstart,NULL,NULL,lvalue_exp(ste),0,yylineno);
                  funcLocalStack.push(functionLocalOffset); 
                  enterscopespace();
                  resetformalargoffset(); 
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
                    ste.type=USERFUNC;
                    ste.name = name;
                    ste.scope = scope;
                    ste.line = yylineno;
                    ste.isActive=true;
                    ste.offset = currscopeoffset();
                    inccurrscopeoffset();
                    ste.scopespace = currscopespace();
                    ste.symt=programfunc_s;
                    insert(ste);
                    $$ = &ste;
                    emit(funcstart,NULL,NULL,lvalue_exp(ste),0,yylineno);
                    funcLocalStack.push(functionLocalOffset); 
                    enterscopespace();
                    resetformalargoffset(); 
                    break;
                }
            }
         } 
         ;

func_bstart: {loopCountStack.push(loopc); loopc=0;};

func_bend:  {loopc = loopCountStack.top(); loopCountStack.pop();};

func_args:  LEFT_PARENTHESIS {scope++;} idlist RIGHT_PARENTHESIS {enterscopespace(); resetfunctionlocaloffset();cout <<"funcdef => function id(idlist)block\n";};
                                                                 
func_body: { scope--; infunction++;} func_bstart block func_bend {infunction--; exitscopespace(); } ;


const:	INTEGER {$$ = newexpr_constnum($1);
                 /* $$->numConst=$1; */
                 cout <<"const => integer:"<<yylval.intVal<<endl;}
		| REAL {$$ = newexpr_constnum($1);
                 /* $$->numConst=$1; */
                 cout <<"const => real:"<<yylval.doubleVal<<endl;}
		| STRING {$$ = newexpr_conststring($1);
                  /* $$->strConst=$1; */
                  cout <<"const => string"<<yylval.stringVal<<endl;}
		| NIL {$$ = newexpr(nil_e);
              cout <<"const => nil"<<endl;}
		| TRUE { $$ = newexpr_constbool($1);
                 /* $$->boolConst=true; */
                 cout <<"const => true"<<endl;}
		| FALSE { $$ = newexpr_constbool($1);
                  /* $$->boolConst=false; */
                  cout <<"const => false"<<endl;}
		;

idlist: ID{
            string name = $1; 
            SymbolTableEntry ste= lookupcurrentscope(name,scope);
            if(ste.isActive){red(); cout << "Error: " << name << " is declared in this scope already.\n"; reset();
            }else if(is_sysfunc(name)){red(); cout << "Error: "<< name <<" is a system function, it cannot be a function argument.\n"; reset();
            }else {
                  ste.type = FORMAL;
                  ste.name = name;
                  ste.scope = scope;
                  ste.line = yylineno;
                  ste.isActive = true;
                  inccurrscopeoffset();
                  ste.offset = currscopeoffset();
                  ste.scopespace = currscopespace();
                  ste.symt = var_s;
                  insert(ste);
            }
            cout <<"idlist => id"<<endl;
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
                  inccurrscopeoffset();
                  ste.offset = currscopeoffset();
                  ste.scopespace = currscopespace();
                  ste.symt=var_s;
                  insert(ste);
            }
            cout <<"idlist => idlist, id"<<endl;
         }
        |
       ;

loopstart : {++inloop; ++loopc;};

loopend : {--inloop; --loopc;};

loopstmt : loopstart stmt loopend { $$ = $2; } ;



ifstmt:	if_prefix stmt {//patchlabel($1-2, $1+1);
                        patchlabel($1, nextquad());
                        cout <<"ifstmt => if(expr) stmt"<<endl;
                       }
		|if_prefix stmt else_prefix stmt   {patchlabel($1,$3+1); //if eq if_prefix
                                            patchlabel($3, nextquad()); //jmp if_prefix
                                            cout <<"ifstmt => if(expr) stmt else stmt"<<endl;
                                            
        }
	    ;	 
if_prefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{
                                                        if($3->type == boolexpr_e){
                                                            if(istempname($3->sym.name)){
                                                                $3->sym = $3->sym;
                                                            }else{
                                                                $3->sym = newtmp();
                                                            }
                                                            patchlist($3->truequad, nextquad());
                                                            emit(assign,newexpr_constbool(1), NULL, $3, nextquad(), yylineno);
                                                            emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
                                                            patchlist($3->falsequad, nextquad());
                                                            emit(assign,newexpr_constbool(0), NULL, $3, nextquad(), yylineno);
                                                        }
                                                        emit(if_eq, $3, newexpr_constbool(1), NULL, nextquad()+2, yylineno);
                                                        $$ = nextquad();
                                                        emit(jump, NULL, NULL, NULL,0,yylineno);
                                                      }

else_prefix: ELSE {$$ = nextquad();   
                   emit(jump,NULL,NULL,NULL,0,yylineno);}


whilestmt: whilestart whilecon loopstmt {
    cout <<"whilestmt => while(expr) stmt"<<endl;
    emit(jump,NULL,NULL,NULL,$1,yylineno); 
    patchlabel($2, nextquad()+1);
 } ; 

whilestart: WHILE{$$=nextquad()+1;};

whilecon: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {
        if($2->type == boolexpr_e){
            if(!istempname($2->sym.name)){
                $2->sym = newtmp();
            }
            patchlist($2->truequad, nextquad());
            emit(assign,newexpr_constbool(1), NULL, $2, nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+3, yylineno);
            patchlist($2->falsequad, nextquad());
            emit(assign,newexpr_constbool(0), NULL, $2, nextquad(), yylineno);
        }
        emit(if_eq, $2, newexpr_constbool(1), NULL, nextquad()+3, yylineno);
        $$ = nextquad();
        emit(jump, NULL, NULL, NULL,0,yylineno);
};
	
for_stmt: for_prefix N elist RIGHT_PARENTHESIS N loopstmt N {
                    forpr *pr=$1;
                    patchlabel(pr->enter,$5+2); // true jump
                    patchlabel($2,nextquad()+1); // false jump
                    patchlabel($5,pr->test+1); // loop jump
                    patchlabel($7,$2+2); // closure jump
                    cout <<"forstmt => for(elist;expr;elist) stmt"<<endl;
};
		
N: {/*unfinished jump*/ $$ = nextquad(); emit(jump,NULL,NULL,NULL,0,yylineno);};

M:{/*nextquad*/ $$ = nextquad(); };

for_prefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON{
            if($6->type == boolexpr_e){
            if(!istempname($6->sym.name)){
                $6->sym = newtmp();
            }
            patchlist($6->truequad, nextquad());
            emit(assign,newexpr_constbool(1), NULL, $6, nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+3, yylineno);
            patchlist($6->falsequad, nextquad());
            emit(assign,newexpr_constbool(0), NULL, $6, nextquad(), yylineno);
        }
        forpr pr;
        pr.enter=nextquad();
        pr.test=$5;
        $$=&pr;
        emit(if_eq,$6,newexpr_constbool(1),NULL,0,yylineno);
};

returnstmt: RETURN expr SEMICOLON{
                    if(infunction==0) { red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                    /* cout <<"returnstmt => return expr;"<<endl; */
                    emit(ret,NULL,NULL,$2,0,yylineno);
                    cout <<"returnstmt => return expr;"<<endl;
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
    ste.taddress = 0;
    ste.scopespace = currscopespace();
    ste.offset = 0;
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
    print_quads();
    cout <<"1\n";
    generate_all();
    cout <<"2\n";
    print_instructions();
    cout <<"3\n";
    return 0;
}