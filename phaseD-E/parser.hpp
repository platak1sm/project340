/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* INTEGER  */
    REAL = 259,                    /* REAL  */
    ID = 260,                      /* ID  */
    STRING = 261,                  /* STRING  */
    TRUE = 262,                    /* TRUE  */
    FALSE = 263,                   /* FALSE  */
    IF = 264,                      /* IF  */
    ELSE = 265,                    /* ELSE  */
    WHILE = 266,                   /* WHILE  */
    FOR = 267,                     /* FOR  */
    FUNCTION = 268,                /* FUNCTION  */
    RETURN = 269,                  /* RETURN  */
    BREAK = 270,                   /* BREAK  */
    CONTINUE = 271,                /* CONTINUE  */
    AND = 272,                     /* AND  */
    NOT = 273,                     /* NOT  */
    OR = 274,                      /* OR  */
    LOCAL = 275,                   /* LOCAL  */
    NIL = 276,                     /* NIL  */
    ASSIGN = 277,                  /* ASSIGN  */
    PLUS = 278,                    /* PLUS  */
    MINUS = 279,                   /* MINUS  */
    MUL = 280,                     /* MUL  */
    DIV = 281,                     /* DIV  */
    MOD = 282,                     /* MOD  */
    EQUAL = 283,                   /* EQUAL  */
    NOT_EQUAL = 284,               /* NOT_EQUAL  */
    PLUS_PLUS = 285,               /* PLUS_PLUS  */
    MINUS_MINUS = 286,             /* MINUS_MINUS  */
    GREATER = 287,                 /* GREATER  */
    LESS = 288,                    /* LESS  */
    GREATER_EQUAL = 289,           /* GREATER_EQUAL  */
    LESS_EQUAL = 290,              /* LESS_EQUAL  */
    UMINUS = 291,                  /* UMINUS  */
    LEFT_BRACE = 292,              /* LEFT_BRACE  */
    RIGHT_BRACE = 293,             /* RIGHT_BRACE  */
    LEFT_BRACKET = 294,            /* LEFT_BRACKET  */
    RIGHT_BRACKET = 295,           /* RIGHT_BRACKET  */
    LEFT_PARENTHESIS = 296,        /* LEFT_PARENTHESIS  */
    RIGHT_PARENTHESIS = 297,       /* RIGHT_PARENTHESIS  */
    SEMICOLON = 298,               /* SEMICOLON  */
    COMMA = 299,                   /* COMMA  */
    COLON = 300,                   /* COLON  */
    DOUBLE_COLON = 301,            /* DOUBLE_COLON  */
    PERIOD = 302,                  /* PERIOD  */
    DOUBLE_PERIOD = 303            /* DOUBLE_PERIOD  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INTEGER 258
#define REAL 259
#define ID 260
#define STRING 261
#define TRUE 262
#define FALSE 263
#define IF 264
#define ELSE 265
#define WHILE 266
#define FOR 267
#define FUNCTION 268
#define RETURN 269
#define BREAK 270
#define CONTINUE 271
#define AND 272
#define NOT 273
#define OR 274
#define LOCAL 275
#define NIL 276
#define ASSIGN 277
#define PLUS 278
#define MINUS 279
#define MUL 280
#define DIV 281
#define MOD 282
#define EQUAL 283
#define NOT_EQUAL 284
#define PLUS_PLUS 285
#define MINUS_MINUS 286
#define GREATER 287
#define LESS 288
#define GREATER_EQUAL 289
#define LESS_EQUAL 290
#define UMINUS 291
#define LEFT_BRACE 292
#define RIGHT_BRACE 293
#define LEFT_BRACKET 294
#define RIGHT_BRACKET 295
#define LEFT_PARENTHESIS 296
#define RIGHT_PARENTHESIS 297
#define SEMICOLON 298
#define COMMA 299
#define COLON 300
#define DOUBLE_COLON 301
#define PERIOD 302
#define DOUBLE_PERIOD 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 56 "parser.y"

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

#line 177 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
