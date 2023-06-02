/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

    #include <iostream>
    #include <string>
    // #include "symbol_table.h"
    #include "icode.h"
    #include <list>
    #include <stack>

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


#line 124 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_REAL = 4,                       /* REAL  */
  YYSYMBOL_ID = 5,                         /* ID  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_TRUE = 7,                       /* TRUE  */
  YYSYMBOL_FALSE = 8,                      /* FALSE  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_FOR = 12,                       /* FOR  */
  YYSYMBOL_FUNCTION = 13,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_BREAK = 15,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 16,                  /* CONTINUE  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_NOT = 18,                       /* NOT  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_LOCAL = 20,                     /* LOCAL  */
  YYSYMBOL_NIL = 21,                       /* NIL  */
  YYSYMBOL_ASSIGN = 22,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 23,                      /* PLUS  */
  YYSYMBOL_MINUS = 24,                     /* MINUS  */
  YYSYMBOL_MUL = 25,                       /* MUL  */
  YYSYMBOL_DIV = 26,                       /* DIV  */
  YYSYMBOL_MOD = 27,                       /* MOD  */
  YYSYMBOL_EQUAL = 28,                     /* EQUAL  */
  YYSYMBOL_NOT_EQUAL = 29,                 /* NOT_EQUAL  */
  YYSYMBOL_PLUS_PLUS = 30,                 /* PLUS_PLUS  */
  YYSYMBOL_MINUS_MINUS = 31,               /* MINUS_MINUS  */
  YYSYMBOL_GREATER = 32,                   /* GREATER  */
  YYSYMBOL_LESS = 33,                      /* LESS  */
  YYSYMBOL_GREATER_EQUAL = 34,             /* GREATER_EQUAL  */
  YYSYMBOL_LESS_EQUAL = 35,                /* LESS_EQUAL  */
  YYSYMBOL_UMINUS = 36,                    /* UMINUS  */
  YYSYMBOL_LEFT_BRACE = 37,                /* LEFT_BRACE  */
  YYSYMBOL_RIGHT_BRACE = 38,               /* RIGHT_BRACE  */
  YYSYMBOL_LEFT_BRACKET = 39,              /* LEFT_BRACKET  */
  YYSYMBOL_RIGHT_BRACKET = 40,             /* RIGHT_BRACKET  */
  YYSYMBOL_LEFT_PARENTHESIS = 41,          /* LEFT_PARENTHESIS  */
  YYSYMBOL_RIGHT_PARENTHESIS = 42,         /* RIGHT_PARENTHESIS  */
  YYSYMBOL_SEMICOLON = 43,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 44,                     /* COMMA  */
  YYSYMBOL_COLON = 45,                     /* COLON  */
  YYSYMBOL_DOUBLE_COLON = 46,              /* DOUBLE_COLON  */
  YYSYMBOL_PERIOD = 47,                    /* PERIOD  */
  YYSYMBOL_DOUBLE_PERIOD = 48,             /* DOUBLE_PERIOD  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_program = 50,                   /* program  */
  YYSYMBOL_stmt = 51,                      /* stmt  */
  YYSYMBOL_52_1 = 52,                      /* $@1  */
  YYSYMBOL_53_2 = 53,                      /* $@2  */
  YYSYMBOL_expr = 54,                      /* expr  */
  YYSYMBOL_55_3 = 55,                      /* @3  */
  YYSYMBOL_56_4 = 56,                      /* @4  */
  YYSYMBOL_57_5 = 57,                      /* $@5  */
  YYSYMBOL_58_6 = 58,                      /* $@6  */
  YYSYMBOL_term = 59,                      /* term  */
  YYSYMBOL_assignexpr = 60,                /* assignexpr  */
  YYSYMBOL_primary = 61,                   /* primary  */
  YYSYMBOL_lvalue = 62,                    /* lvalue  */
  YYSYMBOL_member = 63,                    /* member  */
  YYSYMBOL_call = 64,                      /* call  */
  YYSYMBOL_callsuffix = 65,                /* callsuffix  */
  YYSYMBOL_normcall = 66,                  /* normcall  */
  YYSYMBOL_methodcall = 67,                /* methodcall  */
  YYSYMBOL_elist = 68,                     /* elist  */
  YYSYMBOL_objectdef = 69,                 /* objectdef  */
  YYSYMBOL_indexed = 70,                   /* indexed  */
  YYSYMBOL_indexedelem = 71,               /* indexedelem  */
  YYSYMBOL_block = 72,                     /* block  */
  YYSYMBOL_73_7 = 73,                      /* $@7  */
  YYSYMBOL_stmtlist = 74,                  /* stmtlist  */
  YYSYMBOL_funcdef = 75,                   /* funcdef  */
  YYSYMBOL_func_prefix = 76,               /* func_prefix  */
  YYSYMBOL_func_bstart = 77,               /* func_bstart  */
  YYSYMBOL_func_bend = 78,                 /* func_bend  */
  YYSYMBOL_func_args = 79,                 /* func_args  */
  YYSYMBOL_80_8 = 80,                      /* $@8  */
  YYSYMBOL_func_body = 81,                 /* func_body  */
  YYSYMBOL_82_9 = 82,                      /* $@9  */
  YYSYMBOL_const = 83,                     /* const  */
  YYSYMBOL_idlist = 84,                    /* idlist  */
  YYSYMBOL_loopstart = 85,                 /* loopstart  */
  YYSYMBOL_loopend = 86,                   /* loopend  */
  YYSYMBOL_loopstmt = 87,                  /* loopstmt  */
  YYSYMBOL_ifstmt = 88,                    /* ifstmt  */
  YYSYMBOL_if_prefix = 89,                 /* if_prefix  */
  YYSYMBOL_else_prefix = 90,               /* else_prefix  */
  YYSYMBOL_whilestmt = 91,                 /* whilestmt  */
  YYSYMBOL_whilestart = 92,                /* whilestart  */
  YYSYMBOL_whilecon = 93,                  /* whilecon  */
  YYSYMBOL_for_stmt = 94,                  /* for_stmt  */
  YYSYMBOL_N = 95,                         /* N  */
  YYSYMBOL_M = 96,                         /* M  */
  YYSYMBOL_for_prefix = 97,                /* for_prefix  */
  YYSYMBOL_returnstmt = 98                 /* returnstmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  64
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   645

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  194

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   104,   104,   105,   108,   125,   130,   130,   134,   134,
     138,   142,   155,   162,   166,   170,   176,   178,   186,   194,
     202,   210,   218,   218,   241,   241,   264,   274,   284,   294,
     304,   304,   333,   333,   358,   363,   365,   374,   391,   423,
     457,   489,   523,   527,   582,   584,   586,   588,   591,   595,
     628,   653,   666,   671,   673,   679,   680,   683,   686,   699,
     705,   707,   712,   722,   731,   735,   738,   743,   755,   770,
     773,   780,   793,   793,   796,   797,   800,   808,   832,   863,
     865,   867,   867,   869,   869,   872,   875,   878,   881,   883,
     886,   891,   910,   929,   932,   934,   936,   940,   944,   950,
     968,   972,   978,   980,   996,  1005,  1007,  1009,  1027,  1033
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "REAL",
  "ID", "STRING", "TRUE", "FALSE", "IF", "ELSE", "WHILE", "FOR",
  "FUNCTION", "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL",
  "NIL", "ASSIGN", "PLUS", "MINUS", "MUL", "DIV", "MOD", "EQUAL",
  "NOT_EQUAL", "PLUS_PLUS", "MINUS_MINUS", "GREATER", "LESS",
  "GREATER_EQUAL", "LESS_EQUAL", "UMINUS", "LEFT_BRACE", "RIGHT_BRACE",
  "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS",
  "SEMICOLON", "COMMA", "COLON", "DOUBLE_COLON", "PERIOD", "DOUBLE_PERIOD",
  "$accept", "program", "stmt", "$@1", "$@2", "expr", "@3", "@4", "$@5",
  "$@6", "term", "assignexpr", "primary", "lvalue", "member", "call",
  "callsuffix", "normcall", "methodcall", "elist", "objectdef", "indexed",
  "indexedelem", "block", "$@7", "stmtlist", "funcdef", "func_prefix",
  "func_bstart", "func_bend", "func_args", "$@8", "func_body", "$@9",
  "const", "idlist", "loopstart", "loopend", "loopstmt", "ifstmt",
  "if_prefix", "else_prefix", "whilestmt", "whilestart", "whilecon",
  "for_stmt", "N", "M", "for_prefix", "returnstmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-169)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-76)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     120,  -169,  -169,  -169,  -169,  -169,  -169,   -39,    14,   253,
     -34,    -3,   355,    79,  -169,   355,     0,     0,  -169,   282,
     326,  -169,    84,    90,   120,    88,    94,   403,  -169,  -169,
    -169,    12,  -169,    11,  -169,  -169,  -169,    72,  -169,  -169,
     209,  -169,   355,  -169,  -169,   424,  -169,  -169,  -169,  -169,
    -169,   101,   -26,    11,   -26,   165,   355,   584,   -32,    17,
    -169,   466,    73,  -169,  -169,  -169,  -169,  -169,    77,    78,
    -169,  -169,  -169,  -169,   355,   355,   355,   355,   355,  -169,
    -169,   355,   355,   355,   355,  -169,   355,  -169,  -169,   355,
     355,   116,   117,  -169,  -169,  -169,   355,   355,   132,  -169,
    -169,   133,   486,  -169,   103,   165,   108,   380,  -169,   355,
    -169,   110,  -169,   111,   355,  -169,   355,   355,  -169,  -169,
      37,    37,  -169,  -169,  -169,   355,   355,     4,     4,     4,
       4,   584,   526,   -28,  -169,   112,   545,   -18,  -169,   150,
    -169,  -169,  -169,   209,  -169,   111,  -169,  -169,   355,   584,
    -169,   355,   506,   209,  -169,     5,    34,   355,   355,   597,
     597,  -169,  -169,   355,  -169,  -169,  -169,    41,   119,  -169,
     564,    67,  -169,  -169,  -169,  -169,   610,    69,    68,  -169,
     153,  -169,  -169,  -169,  -169,   355,  -169,  -169,  -169,  -169,
     445,  -169,  -169,  -169
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    85,    86,    49,    87,    89,    90,     0,    78,     0,
       0,     0,     0,     0,    88,     0,     0,     0,    72,    66,
       0,    15,     0,     0,     3,     0,     0,     0,    34,    16,
      42,    44,    52,    45,    46,    13,    14,     0,    48,     5,
       6,    10,     0,    77,   109,     0,    11,    12,    37,    50,
      36,     0,    38,     0,    40,     6,     0,    64,     0,     0,
      69,     0,     0,    51,     1,     2,   102,     7,     0,     0,
       9,   105,    30,    32,     0,     0,     0,     0,     0,    22,
      24,     0,     0,     0,     0,     4,     0,    39,    41,     0,
      66,     0,     0,    58,    60,    61,     0,    66,     0,    81,
      83,    97,     0,   108,     0,     6,     0,     0,    67,     0,
      68,     0,    35,    47,     0,    94,    66,    66,   106,   106,
      17,    18,    19,    20,    21,     0,     0,    26,    27,    28,
      29,    43,     0,     0,    53,     0,     0,     0,    55,    93,
      76,    79,   100,     6,    99,     0,    74,    73,     0,    65,
      70,    66,     0,     6,   101,     0,     0,     0,     0,    23,
      25,    54,    62,    66,    56,    57,    91,     0,     0,    98,
       0,     0,   103,    95,   106,   105,    31,    33,     0,    82,
       0,    80,    71,    59,    96,     0,    94,    63,    92,    84,
       0,   105,   107,   104
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,   136,   -23,  -169,  -169,    -9,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,    38,  -169,    54,  -169,  -169,  -169,   -72,
    -169,  -169,    51,    -4,  -169,    60,   -16,  -169,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,   -19,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -168,  -118,  -169,  -169
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    23,    24,    25,    26,    27,   125,   126,   118,   119,
      28,    29,    30,    31,    32,    33,    93,    94,    95,    58,
      34,    59,    60,    35,    55,   106,    36,    37,   168,   189,
     100,   139,   140,   141,    38,   167,   153,   184,   154,    39,
      40,   143,    67,    68,   115,    70,   117,   157,    71,    41
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   158,    42,    48,    62,     3,    50,   186,   108,    46,
      57,    61,   109,    89,   162,    90,   109,   101,   133,    43,
      13,    91,    92,   193,   165,   137,   109,    74,    75,    76,
      77,    78,   105,   102,    86,   104,   -76,   -76,   -76,   -76,
      47,    51,    87,    88,   155,   156,    22,   107,   174,   109,
      96,    89,    97,    90,    52,    54,   185,   110,    98,    91,
      92,   111,    76,    77,    78,   120,   121,   122,   123,   124,
      53,    53,   127,   128,   129,   130,   175,   131,   109,   171,
     132,    57,   105,   179,    49,   180,    72,   136,    57,    63,
      64,   178,    74,    75,    76,    77,    78,    79,    80,    66,
     149,    81,    82,    83,    84,   152,    69,    57,    57,   183,
     187,   109,   109,    99,     8,   113,   159,   160,   114,   116,
     169,   134,   135,     1,     2,     3,     4,     5,     6,     7,
     173,    -6,    -8,     8,     9,    10,    11,   138,    12,   170,
      13,    14,    57,   142,    15,   145,   147,    56,   176,   177,
      16,    17,   151,   163,    57,   166,    18,    18,   188,    19,
      65,    20,   150,    21,   181,   146,    22,   191,     1,     2,
       3,     4,     5,     6,     7,     0,   190,    -8,     8,     9,
      10,    11,     0,    12,     0,    13,    14,     0,     0,    15,
       0,     0,     0,     0,     0,    16,    17,     0,     0,     0,
       0,     0,    18,   -75,    19,     0,    20,     0,    21,     0,
       0,    22,     1,     2,     3,     4,     5,     6,     7,     0,
       0,    -8,     8,     9,    10,    11,     0,    12,     0,    13,
      14,     0,     0,    15,     0,     0,     0,     0,     0,    16,
      17,     0,     0,     0,     0,     0,    18,     0,    19,     0,
      20,     0,    21,     0,     0,    22,     1,     2,     3,     4,
       5,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,     0,    13,    14,     0,     0,    15,     0,     0,
       0,     0,     0,    16,    17,     1,     2,     3,     4,     5,
       6,     0,    19,     0,    20,     0,    44,     0,     0,    22,
      12,     0,    13,    14,     0,     0,    15,     0,     0,     0,
       0,     0,    16,    17,     0,     0,     0,     0,     0,    56,
       0,    19,     0,    20,     0,     0,     0,     0,    22,     1,
       2,     3,     4,     5,     6,     0,     0,     0,     0,     8,
       0,     0,     0,     0,    12,     0,    13,    14,     0,     0,
      15,     0,     0,     0,     0,     0,    16,    17,     1,     2,
       3,     4,     5,     6,     0,    19,     0,    20,     0,     0,
       0,     0,    22,    12,     0,    13,    14,     0,     0,    15,
       0,     0,     0,     0,     0,    16,    17,     0,     0,     0,
       0,     0,     0,     0,    19,     0,    20,    72,     0,    73,
       0,    22,     0,    74,    75,    76,    77,    78,    79,    80,
       0,     0,    81,    82,    83,    84,     0,     0,     0,     0,
      72,     0,    73,     0,     0,   148,    74,    75,    76,    77,
      78,    79,    80,     0,     0,    81,    82,    83,    84,     0,
       0,    72,     0,    73,     0,     0,    85,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
       0,     0,    72,     0,    73,     0,     0,   103,    74,    75,
      76,    77,    78,    79,    80,     0,     0,    81,    82,    83,
      84,     0,     0,    72,     0,    73,     0,     0,   192,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,    82,
      83,    84,     0,    72,     0,    73,     0,     0,   112,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,    82,
      83,    84,     0,    72,     0,    73,     0,     0,   144,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,    82,
      83,    84,     0,    72,     0,    73,     0,     0,   172,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,    82,
      83,    84,    72,     0,    73,     0,   161,     0,    74,    75,
      76,    77,    78,    79,    80,     0,     0,    81,    82,    83,
      84,    72,     0,    73,     0,   164,     0,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
       0,    72,   182,    73,     0,     0,     0,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
      74,    75,    76,    77,    78,   -76,   -76,     0,     0,    81,
      82,    83,    84,    74,    75,    76,    77,    78,    79,    80,
       0,     0,    81,    82,    83,    84
};

static const yytype_int16 yycheck[] =
{
       9,   119,    41,    12,    20,     5,    15,   175,    40,    43,
      19,    20,    44,    39,    42,    41,    44,    40,    90,     5,
      20,    47,    48,   191,    42,    97,    44,    23,    24,    25,
      26,    27,    55,    42,    22,    51,    32,    33,    34,    35,
      43,    41,    30,    31,   116,   117,    46,    56,    43,    44,
      39,    39,    41,    41,    16,    17,   174,    40,    47,    47,
      48,    44,    25,    26,    27,    74,    75,    76,    77,    78,
      16,    17,    81,    82,    83,    84,    42,    86,    44,   151,
      89,    90,   105,    42,     5,    44,    17,    96,    97,     5,
       0,   163,    23,    24,    25,    26,    27,    28,    29,    11,
     109,    32,    33,    34,    35,   114,    12,   116,   117,    42,
      42,    44,    44,    41,    13,    42,   125,   126,    41,    41,
     143,     5,     5,     3,     4,     5,     6,     7,     8,     9,
     153,    11,    12,    13,    14,    15,    16,     5,    18,   148,
      20,    21,   151,    10,    24,    42,    38,    37,   157,   158,
      30,    31,    41,    41,   163,     5,    37,    37,     5,    39,
      24,    41,   111,    43,   168,   105,    46,   186,     3,     4,
       5,     6,     7,     8,     9,    -1,   185,    12,    13,    14,
      15,    16,    -1,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    41,    -1,    43,    -1,
      -1,    46,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    18,    -1,    20,
      21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,
      41,    -1,    43,    -1,    -1,    46,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,     3,     4,     5,     6,     7,
       8,    -1,    39,    -1,    41,    -1,    43,    -1,    -1,    46,
      18,    -1,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    39,    -1,    41,    -1,    -1,    -1,    -1,    46,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    18,    -1,    20,    21,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,     3,     4,
       5,     6,     7,     8,    -1,    39,    -1,    41,    -1,    -1,
      -1,    -1,    46,    18,    -1,    20,    21,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    41,    17,    -1,    19,
      -1,    46,    -1,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      17,    -1,    19,    -1,    -1,    45,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    33,    34,    35,    -1,
      -1,    17,    -1,    19,    -1,    -1,    43,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
      -1,    -1,    17,    -1,    19,    -1,    -1,    43,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35,    -1,    -1,    17,    -1,    19,    -1,    -1,    43,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    -1,    17,    -1,    19,    -1,    -1,    42,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    -1,    17,    -1,    19,    -1,    -1,    42,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    -1,    17,    -1,    19,    -1,    -1,    42,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    17,    -1,    19,    -1,    40,    -1,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35,    17,    -1,    19,    -1,    40,    -1,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
      -1,    17,    38,    19,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      33,    34,    35,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    13,    14,
      15,    16,    18,    20,    21,    24,    30,    31,    37,    39,
      41,    43,    46,    50,    51,    52,    53,    54,    59,    60,
      61,    62,    63,    64,    69,    72,    75,    76,    83,    88,
      89,    98,    41,     5,    43,    54,    43,    43,    54,     5,
      54,    41,    62,    64,    62,    73,    37,    54,    68,    70,
      71,    54,    75,     5,     0,    50,    11,    91,    92,    12,
      94,    97,    17,    19,    23,    24,    25,    26,    27,    28,
      29,    32,    33,    34,    35,    43,    22,    30,    31,    39,
      41,    47,    48,    65,    66,    67,    39,    41,    47,    41,
      79,    51,    54,    43,    75,    51,    74,    54,    40,    44,
      40,    44,    42,    42,    41,    93,    41,    95,    57,    58,
      54,    54,    54,    54,    54,    55,    56,    54,    54,    54,
      54,    54,    54,    68,     5,     5,    54,    68,     5,    80,
      81,    82,    10,    90,    42,    42,    74,    38,    45,    54,
      71,    41,    54,    85,    87,    68,    68,    96,    96,    54,
      54,    40,    42,    41,    40,    42,     5,    84,    77,    51,
      54,    68,    42,    51,    43,    42,    54,    54,    68,    42,
      44,    72,    38,    42,    86,    96,    95,    42,     5,    78,
      54,    87,    43,    95
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    51,    53,    51,
      51,    51,    51,    51,    51,    51,    54,    54,    54,    54,
      54,    54,    55,    54,    56,    54,    54,    54,    54,    54,
      57,    54,    58,    54,    54,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    61,    61,    61,    61,    61,    62,
      62,    62,    62,    63,    63,    63,    63,    64,    64,    64,
      65,    65,    66,    67,    68,    68,    68,    69,    69,    70,
      70,    71,    73,    72,    74,    74,    75,    76,    76,    77,
      78,    80,    79,    82,    81,    83,    83,    83,    83,    83,
      83,    84,    84,    84,    85,    86,    87,    88,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    98
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     1,     0,     2,     0,     2,
       1,     2,     2,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     0,     4,     0,     4,     3,     3,     3,     3,
       0,     5,     0,     5,     1,     3,     2,     2,     2,     2,
       2,     2,     1,     3,     1,     1,     1,     3,     1,     1,
       2,     2,     1,     3,     4,     3,     4,     4,     2,     6,
       1,     1,     3,     5,     1,     3,     0,     3,     3,     1,
       3,     5,     0,     4,     2,     0,     3,     2,     1,     0,
       0,     0,     4,     0,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     0,     0,     3,     2,     4,     4,
       1,     3,     1,     3,     7,     0,     0,     7,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt program  */
#line 104 "parser.y"
                      {cout << "program stmt\n";}
#line 1437 "parser.cpp"
    break;

  case 3: /* program: %empty  */
#line 105 "parser.y"
           {cout << "program empty\n";}
#line 1443 "parser.cpp"
    break;

  case 4: /* stmt: expr SEMICOLON  */
#line 108 "parser.y"
                     {
    //make_stmt($$);
    if((yyvsp[-1].expVal)->type == boolexpr_e){
        if(istempname((yyvsp[-1].expVal)->sym.name)){
            (yyval.stmtVal)->sym = (yyvsp[-1].expVal)->sym;
        }else{
            (yyval.stmtVal)->sym = newtmp();
        }
        patchlist((yyvsp[-1].expVal)->truequad, nextquad());
        emit(assign, newexpr_constbool(1), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
        emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
        patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
        emit(assign, newexpr_constbool(0), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
        resettmpcounter();
    }
    cout << "stmt => expr\n";
    }
#line 1465 "parser.cpp"
    break;

  case 5: /* stmt: ifstmt  */
#line 125 "parser.y"
               {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => ifstmt\n";
        }
#line 1475 "parser.cpp"
    break;

  case 6: /* $@1: %empty  */
#line 130 "parser.y"
        {inloop++;}
#line 1481 "parser.cpp"
    break;

  case 7: /* stmt: $@1 whilestmt  */
#line 130 "parser.y"
                            {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => whilestmt\n";inloop--;}
#line 1490 "parser.cpp"
    break;

  case 8: /* $@2: %empty  */
#line 134 "parser.y"
        {inloop++;}
#line 1496 "parser.cpp"
    break;

  case 9: /* stmt: $@2 for_stmt  */
#line 134 "parser.y"
                           {
        //make_stmt($$);
        resettmpcounter();
        inloop--; cout << "stmt => for_stmt\n";}
#line 1505 "parser.cpp"
    break;

  case 10: /* stmt: returnstmt  */
#line 138 "parser.y"
                   {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => returnstmt\n";}
#line 1514 "parser.cpp"
    break;

  case 11: /* stmt: BREAK SEMICOLON  */
#line 142 "parser.y"
                       {
        //make_stmt($$);
        (yyval.stmtVal)->breakList = newlist(nextquad());
        (yyval.stmtVal)->contList = 0;
        emit(jump, NULL, NULL, NULL, 0, yylineno);
        
        if(inloop==0) { 
            red(); 
            cout << "Error: Cannot use BREAK when not in loop -> line " << yylineno << endl;
             cout << "stmt => break;\n"; reset();}
        resettmpcounter();

        }
#line 1532 "parser.cpp"
    break;

  case 12: /* stmt: CONTINUE SEMICOLON  */
#line 155 "parser.y"
                          {
        //make_stmt($$);
        (yyval.stmtVal)->contList = newlist(nextquad());
        (yyval.stmtVal)->breakList = 0;
        emit(jump, NULL, NULL, NULL, 0, yylineno);
        if(inloop==0){ red(); cout << "Error: Cannot use CONTINUE when not in loop -> line " << yylineno << endl;cout << "stmt => continue;\n"; reset();}
        resettmpcounter(); }
#line 1544 "parser.cpp"
    break;

  case 13: /* stmt: block  */
#line 162 "parser.y"
              {
        (yyval.stmtVal) = (yyvsp[0].stmtVal);
        resettmpcounter();
        cout << "stmt => block\n";}
#line 1553 "parser.cpp"
    break;

  case 14: /* stmt: funcdef  */
#line 166 "parser.y"
                {
       // make_stmt($$);
        resettmpcounter();
        cout << "stmt => funcdef\n";}
#line 1562 "parser.cpp"
    break;

  case 15: /* stmt: SEMICOLON  */
#line 170 "parser.y"
                  {
        //make_stmt($$);
        resettmpcounter();
        cout << "stmt => ;\n";}
#line 1571 "parser.cpp"
    break;

  case 16: /* expr: assignexpr  */
#line 176 "parser.y"
                 {(yyval.expVal)=(yyvsp[0].expVal);
                 cout << "expr => assignexpr\n";}
#line 1578 "parser.cpp"
    break;

  case 17: /* expr: expr PLUS expr  */
#line 178 "parser.y"
                       {(yyval.expVal) =newexpr(arithexp_e);
                        if(istempname((yyvsp[-2].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[-2].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        cout << "expr => expr+expr\n";
                        emit(add,(yyvsp[-2].expVal),(yyvsp[0].expVal),(yyval.expVal),0,yylineno);}
#line 1591 "parser.cpp"
    break;

  case 18: /* expr: expr MINUS expr  */
#line 186 "parser.y"
                        {(yyval.expVal) =newexpr(arithexp_e);
                        if(istempname((yyvsp[-2].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[-2].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        cout << "expr => expr-expr\n";
                        emit(sub,(yyvsp[-2].expVal),(yyvsp[0].expVal),(yyval.expVal),0,yylineno);}
#line 1604 "parser.cpp"
    break;

  case 19: /* expr: expr MUL expr  */
#line 194 "parser.y"
                        {(yyval.expVal) =newexpr(arithexp_e);
                         if(istempname((yyvsp[-2].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[-2].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        cout << "expr => expr*expr\n";
                         emit(mul,(yyvsp[-2].expVal),(yyvsp[0].expVal),(yyval.expVal),0,yylineno);}
#line 1617 "parser.cpp"
    break;

  case 20: /* expr: expr DIV expr  */
#line 202 "parser.y"
                       {(yyval.expVal) =newexpr(arithexp_e);
                        if(istempname((yyvsp[-2].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[-2].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        cout << "expr => expr div expr\n";
                        emit(divc,(yyvsp[-2].expVal),(yyvsp[0].expVal),(yyval.expVal),0,yylineno);}
#line 1630 "parser.cpp"
    break;

  case 21: /* expr: expr MOD expr  */
#line 210 "parser.y"
                       {(yyval.expVal) =newexpr(arithexp_e);
                        if(istempname((yyvsp[-2].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[-2].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        cout << "expr => expr mod expr\n";
                        emit(mod,(yyvsp[-2].expVal),(yyvsp[0].expVal),(yyval.expVal),0,yylineno);}
#line 1643 "parser.cpp"
    break;

  case 22: /* @3: %empty  */
#line 218 "parser.y"
                  {
        if((yyvsp[-1].expVal)->type == boolexpr_e){
            if(istempname((yyvsp[-1].expVal)->sym.name)){
                (yyval.expVal)->sym = (yyvsp[-1].expVal)->sym;
            }else{
                (yyval.expVal)->sym = newtmp();
            }
            patchlist((yyvsp[-1].expVal)->truequad, nextquad());
            emit(assign, newexpr_constbool(1), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
            patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
            emit(assign, newexpr_constbool(0), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
        }
      }
#line 1662 "parser.cpp"
    break;

  case 23: /* expr: expr EQUAL @3 expr  */
#line 231 "parser.y"
             {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         (yyval.expVal)->truequad = nextquad();
                         (yyval.expVal)->falsequad = nextquad()+1;
                         emit(if_eq, (yyvsp[-3].expVal), (yyvsp[0].expVal), NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr == expr\n";
                         }
#line 1677 "parser.cpp"
    break;

  case 24: /* @4: %empty  */
#line 241 "parser.y"
                      {
        if((yyvsp[-1].expVal)->type == boolexpr_e){
            if(istempname((yyvsp[-1].expVal)->sym.name)){
                (yyval.expVal)->sym = (yyvsp[-1].expVal)->sym;
            }else{
                (yyval.expVal)->sym = newtmp();
            }
            patchlist((yyvsp[-1].expVal)->truequad, nextquad());
            emit(assign, newexpr_constbool(1), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
            patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
            emit(assign, newexpr_constbool(0), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
        }
      }
#line 1696 "parser.cpp"
    break;

  case 25: /* expr: expr NOT_EQUAL @4 expr  */
#line 254 "parser.y"
             {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         (yyval.expVal)->truequad = nextquad();
                         (yyval.expVal)->falsequad = nextquad()+1;
                         emit(if_noteq, (yyvsp[-3].expVal), (yyvsp[0].expVal), NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr!=expr\n";
                         }
#line 1711 "parser.cpp"
    break;

  case 26: /* expr: expr GREATER expr  */
#line 264 "parser.y"
                            {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         (yyval.expVal)->truequad = nextquad();
                         (yyval.expVal)->falsequad = nextquad()+1;
                         emit(if_greater, (yyvsp[-2].expVal), (yyvsp[0].expVal), NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr > expr\n";
                         }
#line 1726 "parser.cpp"
    break;

  case 27: /* expr: expr LESS expr  */
#line 274 "parser.y"
                            {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         (yyval.expVal)->truequad = nextquad();
                         (yyval.expVal)->falsequad = nextquad()+1;
                         emit(if_less, (yyvsp[-2].expVal), (yyvsp[0].expVal), NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr < expr\n";
                         }
#line 1741 "parser.cpp"
    break;

  case 28: /* expr: expr GREATER_EQUAL expr  */
#line 284 "parser.y"
                                {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         (yyval.expVal)->truequad = nextquad();
                         (yyval.expVal)->falsequad = nextquad()+1;
                         emit(if_greatereq, (yyvsp[-2].expVal), (yyvsp[0].expVal), NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr >= expr\n";
                                }
#line 1756 "parser.cpp"
    break;

  case 29: /* expr: expr LESS_EQUAL expr  */
#line 294 "parser.y"
                                {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                         (yyval.expVal)->truequad = nextquad();
                         (yyval.expVal)->falsequad = nextquad()+1;
                         emit(if_lesseq, (yyvsp[-2].expVal), (yyvsp[0].expVal), NULL, 0 , yylineno);
		                 //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
		                 emit(jump,NULL,NULL,NULL,0, yylineno);
                         //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                         cout << "expr => expr <= expr\n";
                                 }
#line 1771 "parser.cpp"
    break;

  case 30: /* $@5: %empty  */
#line 304 "parser.y"
                 {
    
        if((yyvsp[-1].expVal)->type != boolexpr_e){
            (yyvsp[-1].expVal)->truequad = nextquad();
            (yyvsp[-1].expVal)->falsequad = nextquad()+1;
            emit(if_eq, (yyvsp[-1].expVal), newexpr_constbool(1), NULL, 0 , yylineno);
            emit(jump,NULL,NULL,NULL,0, yylineno);
            patchlist((yyvsp[-1].expVal)->truequad, nextquad());
        }
        
        
      }
#line 1788 "parser.cpp"
    break;

  case 31: /* expr: expr AND $@5 M expr  */
#line 315 "parser.y"
              {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                        if((yyvsp[0].expVal)->type != boolexpr_e){
                            (yyvsp[0].expVal)->truequad = nextquad();
                            (yyvsp[0].expVal)->falsequad = nextquad()+1;
                            emit(if_eq, (yyvsp[0].expVal), newexpr_constbool(1), NULL, 0 , yylineno);
                            //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
                            emit(jump,NULL,NULL,NULL,0, yylineno);
                            //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                        }
                        if((yyvsp[-4].expVal)->type == boolexpr_e){
                            patchlist((yyvsp[-4].expVal)->truequad, (yyvsp[-1].intVal));
                        }
                        (yyval.expVal)->falsequad = mergelist((yyvsp[-4].expVal)->falsequad, (yyvsp[0].expVal)->falsequad);
                        (yyval.expVal)->truequad = (yyvsp[0].expVal)->truequad;
                         cout << "expr => expr && expr\n";
        
      }
#line 1811 "parser.cpp"
    break;

  case 32: /* $@6: %empty  */
#line 333 "parser.y"
               {
        if((yyvsp[-1].expVal)->type != boolexpr_e){
            (yyvsp[-1].expVal)->truequad = nextquad();
            (yyvsp[-1].expVal)->falsequad = nextquad()+1;
            emit(if_eq, (yyvsp[-1].expVal), newexpr_constbool(1), NULL, 0 , yylineno);
            emit(jump,NULL,NULL,NULL,0, yylineno);
            patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
        }
      }
#line 1825 "parser.cpp"
    break;

  case 33: /* expr: expr OR $@6 M expr  */
#line 341 "parser.y"
               {(yyval.expVal) = newexpr(boolexpr_e);
		                //  $$->sym = newtmp();
                        if((yyvsp[0].expVal)->type != boolexpr_e){
                            (yyvsp[0].expVal)->truequad = nextquad();
                            (yyvsp[0].expVal)->falsequad = nextquad()+1;
                            emit(if_eq, (yyvsp[0].expVal), newexpr_constbool(1), NULL, 0 , yylineno);
                            //emit(assign, newexpr_constbool(false), NULL, $$, -1 , yylineno);
                            emit(jump,NULL,NULL,NULL,0, yylineno);
                            //emit(assign, newexpr_constbool(true), NULL, $$, -1 , yylineno);
                        }
                        if((yyvsp[-4].expVal)->type == boolexpr_e){
                            patchlist((yyvsp[-4].expVal)->falsequad, (yyvsp[-1].intVal));
                        }
                        (yyval.expVal)->truequad = mergelist((yyvsp[-4].expVal)->truequad, (yyvsp[0].expVal)->truequad);
                        (yyval.expVal)->falsequad = (yyvsp[0].expVal)->falsequad;
                        cout << "expr => expr || expr\n";
                      }
#line 1847 "parser.cpp"
    break;

  case 34: /* expr: term  */
#line 358 "parser.y"
             {
        (yyval.expVal)=(yyvsp[0].expVal);
        cout << "expr => term\n";}
#line 1855 "parser.cpp"
    break;

  case 35: /* term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 363 "parser.y"
                                                {(yyval.expVal)=(yyvsp[-1].expVal);
                                                cout << "term => (expr)\n";}
#line 1862 "parser.cpp"
    break;

  case 36: /* term: MINUS expr  */
#line 365 "parser.y"
                                       {check_arith((yyvsp[0].expVal));
                        (yyval.expVal) =newexpr(arithexp_e);
                        if(istempname((yyvsp[0].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[0].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        cout << "term => -expr\n";
                        emit(uminus,(yyvsp[0].expVal),NULL,(yyval.expVal),-1,yylineno);}
#line 1876 "parser.cpp"
    break;

  case 37: /* term: NOT expr  */
#line 374 "parser.y"
                       {(yyval.expVal) =newexpr(boolexpr_e);
                        if(istempname((yyvsp[0].expVal)->sym.name)){
                            (yyval.expVal)->sym = (yyvsp[0].expVal)->sym;
                        }else{
                            (yyval.expVal)->sym = newtmp();
                        }
                        if((yyvsp[0].expVal)->type != boolexpr_e){
                            (yyvsp[0].expVal)->truequad = nextquad();
                            (yyvsp[0].expVal)->falsequad = nextquad()+1;
                            emit(if_eq, (yyvsp[0].expVal),newexpr_constbool(1), NULL, 0, yylineno);
                            emit(jump, NULL, NULL, NULL, 0, yylineno);
                        }
                        int tmp = (yyvsp[0].expVal)->truequad;
                        (yyval.expVal)->truequad = (yyvsp[0].expVal)->falsequad;
                        (yyval.expVal)->falsequad = tmp;
                        cout << "term => !expr\n";
                        emit(notc,(yyvsp[0].expVal),NULL,(yyval.expVal),0,yylineno);}
#line 1898 "parser.cpp"
    break;

  case 38: /* term: PLUS_PLUS lvalue  */
#line 391 "parser.y"
                                   {
                            string name = (yyvsp[0].expVal)->sym.name;
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
                            check_arith((yyvsp[0].expVal));
                            if((yyvsp[0].expVal)->type == tableitem_e){
                                (yyval.expVal) = emit_iftableitem((yyvsp[0].expVal));
                                emit(add,(yyval.expVal),newexpr_constnum(1), (yyval.expVal), -1, yylineno);
                                emit(tablesetelem, (yyvsp[0].expVal), (yyvsp[0].expVal)->index, (yyval.expVal), -1, yylineno);
                            }else{
                                emit(add,(yyvsp[0].expVal),newexpr_constnum(1), (yyvsp[0].expVal), -1, yylineno);
                                (yyval.expVal) = newexpr(arithexp_e);
                                if(istempname((yyvsp[0].expVal)->sym.name)){
                                    (yyval.expVal)->sym = (yyvsp[0].expVal)->sym;
                                }else{
                                    (yyval.expVal)->sym = newtmp();
                                }
                                emit(assign, (yyvsp[0].expVal), NULL, (yyval.expVal), -1, yylineno);
                            }
                            cout << "term => ++lvalue\n";
                           }
#line 1935 "parser.cpp"
    break;

  case 39: /* term: lvalue PLUS_PLUS  */
#line 423 "parser.y"
                                   {
                            string name = (yyvsp[-1].expVal)->sym.name;
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
                            check_arith((yyvsp[-1].expVal));
                            (yyval.expVal) = newexpr(var_e);
                            if(istempname((yyvsp[-1].expVal)->sym.name)){
                                (yyval.expVal)->sym = (yyvsp[-1].expVal)->sym;
                            }else{
                                (yyval.expVal)->sym = newtmp();
                            }
                            if((yyvsp[-1].expVal)->type == tableitem_e){
                                expr* val = emit_iftableitem((yyvsp[-1].expVal));
                                
                                emit(assign,val, NULL,(yyval.expVal),-1,yylineno);
                                emit(add,val, newexpr_constnum(1),val,-1,yylineno);
                                emit(tablesetelem, (yyvsp[-1].expVal), (yyvsp[-1].expVal)->index, (yyval.expVal), -1, yylineno);
                            }else{
                                emit(assign,(yyvsp[-1].expVal), NULL,(yyval.expVal),-1,yylineno);
                                emit(add,(yyvsp[-1].expVal), newexpr_constnum(1),(yyvsp[-1].expVal),-1,yylineno);
                            }
                            cout << "term => lvalue++\n";
                           }
#line 1974 "parser.cpp"
    break;

  case 40: /* term: MINUS_MINUS lvalue  */
#line 457 "parser.y"
                                     {
                            string name = (yyvsp[0].expVal)->sym.name;
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
                            check_arith((yyvsp[0].expVal));
                            if((yyvsp[0].expVal)->type == tableitem_e){
                                (yyval.expVal) = emit_iftableitem((yyvsp[0].expVal));
                                emit(sub,(yyval.expVal),newexpr_constnum(1), (yyval.expVal), -1, yylineno);
                                emit(tablesetelem, (yyvsp[0].expVal), (yyvsp[0].expVal)->index, (yyval.expVal), -1, yylineno);
                            }else{
                                emit(sub,(yyvsp[0].expVal),newexpr_constnum(1), (yyvsp[0].expVal), -1, yylineno);
                                (yyval.expVal) = newexpr(arithexp_e);
                                if(istempname((yyvsp[0].expVal)->sym.name)){
                                    (yyval.expVal)->sym = (yyvsp[0].expVal)->sym;
                                }else{
                                    (yyval.expVal)->sym = newtmp();
                                }
                                emit(assign, (yyvsp[0].expVal), NULL, (yyval.expVal), -1, yylineno);
                            }
                            cout << "term => --lvalue\n";
                           }
#line 2011 "parser.cpp"
    break;

  case 41: /* term: lvalue MINUS_MINUS  */
#line 489 "parser.y"
                                     {
                             string name = (yyvsp[-1].expVal)->sym.name;
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
                            check_arith((yyvsp[-1].expVal));
                            (yyval.expVal) = newexpr(var_e);
                            if(istempname((yyvsp[-1].expVal)->sym.name)){
                                (yyval.expVal)->sym = (yyvsp[-1].expVal)->sym;
                            }else{
                                (yyval.expVal)->sym = newtmp();
                            }
                            if((yyvsp[-1].expVal)->type == tableitem_e){
                                expr* val = emit_iftableitem((yyvsp[-1].expVal));
                                
                                emit(assign,val, NULL,(yyval.expVal),-1,yylineno);
                                emit(sub,val, newexpr_constnum(1),val,-1,yylineno);
                                emit(tablesetelem, (yyvsp[-1].expVal), (yyvsp[-1].expVal)->index, (yyval.expVal), -1, yylineno);
                            }else{
                                emit(assign,(yyvsp[-1].expVal), NULL,(yyval.expVal),-1,yylineno);
                                emit(sub,(yyvsp[-1].expVal), newexpr_constnum(1),(yyvsp[-1].expVal),-1,yylineno);
                            }
                            cout << "term => lvalue--\n";
                             }
#line 2050 "parser.cpp"
    break;

  case 42: /* term: primary  */
#line 523 "parser.y"
                          {(yyval.expVal)=(yyvsp[0].expVal);
                    cout << "term => primary\n";}
#line 2057 "parser.cpp"
    break;

  case 43: /* assignexpr: lvalue ASSIGN expr  */
#line 527 "parser.y"
                              {		
                                string name = (yyvsp[-2].expVal)->sym.name;
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
                                if((yyvsp[0].expVal)->type == boolexpr_e){
                                    if(istempname((yyvsp[0].expVal)->sym.name)){
                                        (yyvsp[0].expVal)->sym = (yyvsp[0].expVal)->sym;
                                    }else{
                                        (yyvsp[0].expVal)->sym = newtmp();
                                    }
                                    patchlist((yyvsp[0].expVal)->truequad, nextquad());
                                    emit(assign, newexpr_constbool(1), NULL, (yyvsp[0].expVal), nextquad(), yylineno);
                                    emit(jump, NULL, NULL, NULL, nextquad(), yylineno);
                                    patchlist((yyvsp[0].expVal)->falsequad, nextquad());
                                    emit(assign, newexpr_constbool(0), NULL, (yyvsp[0].expVal), nextquad(), yylineno);
                                }

                                if((yyvsp[-2].expVal)!=NULL){
                                    if((yyvsp[-2].expVal)->type == tableitem_e){
                                        emit(tablesetelem, (yyvsp[-2].expVal), (yyvsp[-2].expVal)->index, (yyvsp[0].expVal), -1, yylineno);
                                        (yyval.expVal) = emit_iftableitem((yyvsp[-2].expVal));
                                        (yyval.expVal)->type = assignexp_e;
                                    }else{
                                        emit(assign, (yyvsp[0].expVal), NULL, (yyvsp[-2].expVal), -1, yylineno);
                                        (yyval.expVal) = newexpr(assignexp_e);
                                        if(istempname((yyvsp[-2].expVal)->sym.name)){
                                            (yyval.expVal)->sym = (yyvsp[-2].expVal)->sym;
                                        }else{
                                            (yyval.expVal)->sym = newtmp();
                                        }
                                        emit(assign, (yyvsp[-2].expVal), NULL,(yyval.expVal),-1,yylineno);
                                    }
                                }
                                cout << "assignexpr => lvalue=expr\n";
                            }
#line 2114 "parser.cpp"
    break;

  case 44: /* primary: lvalue  */
#line 582 "parser.y"
               { (yyval.expVal) = emit_iftableitem((yyvsp[0].expVal));
                cout << "primary => lvalue\n";}
#line 2121 "parser.cpp"
    break;

  case 45: /* primary: call  */
#line 584 "parser.y"
               {(yyval.expVal)=(yyvsp[0].expVal);
                cout << "primary => call\n";}
#line 2128 "parser.cpp"
    break;

  case 46: /* primary: objectdef  */
#line 586 "parser.y"
                    {(yyval.expVal)=(yyvsp[0].expVal);
                    cout << "primary => objectdef\n";}
#line 2135 "parser.cpp"
    break;

  case 47: /* primary: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS  */
#line 588 "parser.y"
                                                            {(yyval.expVal) = newexpr(programfunc_e);
                                                     (yyval.expVal)->sym = *((yyvsp[-1].steVal));
                                                    cout << "primary => (funcdef)\n";}
#line 2143 "parser.cpp"
    break;

  case 48: /* primary: const  */
#line 591 "parser.y"
                       {(yyval.expVal)=(yyvsp[0].expVal);
                cout << "primary => const\n";}
#line 2150 "parser.cpp"
    break;

  case 49: /* lvalue: ID  */
#line 595 "parser.y"
           { 
    
            string name((yyvsp[0].stringVal));
    
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
                
                (yyval.expVal) = lvalue_exp(ent);
                
            }else{
                if(lookupactivevar(name).scope < infunction && lookupactivevar(name).scope > 0){
                    red(); cout << "Error: " <<" There is function between the uses of variable "<< name <<endl; reset();
                }
            }
            
            cout << "lvalue => id:" << yylval.stringVal<<endl;
            }
#line 2188 "parser.cpp"
    break;

  case 50: /* lvalue: LOCAL ID  */
#line 628 "parser.y"
                   {
            string name((yyvsp[0].stringVal));
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
                
                
                (yyval.expVal) = lvalue_exp(ent);
            }
           cout << "lvalue => local id:" << yylval.stringVal<<", "<<scope<<endl;
        }
#line 2218 "parser.cpp"
    break;

  case 51: /* lvalue: DOUBLE_COLON ID  */
#line 653 "parser.y"
                          {
            string name((yyvsp[0].stringVal));
            SymbolTableEntry ste = lookupcurrentscope(name,0);
            if(ste.isActive == false){
                red();
                cout << "Error: there is no global variable with name "<<name<<endl;
                reset();
                (yyval.expVal) = newexpr(nil_e);
            } else{
                (yyval.expVal)=lvalue_exp(ste);
            }
            cout << "lvalue => ::id:" << yylval.stringVal<<endl;
        }
#line 2236 "parser.cpp"
    break;

  case 52: /* lvalue: member  */
#line 666 "parser.y"
                {(yyval.expVal) = (yyvsp[0].expVal);
            isMember=true;
            cout << "lvalue => member" <<endl;}
#line 2244 "parser.cpp"
    break;

  case 53: /* member: lvalue PERIOD ID  */
#line 671 "parser.y"
                         {(yyval.expVal) = member_item((yyvsp[-2].expVal),(yyvsp[0].stringVal));
                            cout << "member => lvalue.id\n";}
#line 2251 "parser.cpp"
    break;

  case 54: /* member: lvalue LEFT_BRACKET expr RIGHT_BRACKET  */
#line 673 "parser.y"
                                                         {(yyvsp[-3].expVal) = emit_iftableitem((yyvsp[-3].expVal));
                                                  (yyval.expVal) = newexpr(tableitem_e);
                                                  (yyval.expVal)->sym = (yyvsp[-3].expVal)->sym;
                                                  (yyval.expVal)->index=(yyvsp[-1].expVal);
                                                  cout << "member => lvalue[expr]\n";
                                                  }
#line 2262 "parser.cpp"
    break;

  case 55: /* member: call PERIOD ID  */
#line 679 "parser.y"
                                 {cout << "member => call.id\n";}
#line 2268 "parser.cpp"
    break;

  case 56: /* member: call LEFT_BRACKET expr RIGHT_BRACKET  */
#line 680 "parser.y"
                                                       {cout << "member => call[expr]\n";}
#line 2274 "parser.cpp"
    break;

  case 57: /* call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 683 "parser.y"
                                                          {
    (yyval.expVal) = make_call((yyvsp[-3].expVal),(yyvsp[-1].elistVal));
    cout << "call => (elist)\n";}
#line 2282 "parser.cpp"
    break;

  case 58: /* call: lvalue callsuffix  */
#line 686 "parser.y"
                                     {
                  
                  if ((yyvsp[0].callVal) != NULL && (yyvsp[0].callVal)->method != NULL) {
                     expr *self = (yyvsp[-1].expVal);
                     if (self != NULL) {
                        (yyvsp[-1].expVal) = emit_iftableitem(member_item(self, (yyvsp[0].callVal)->name));
                        (yyvsp[0].callVal)->elist->elist.push_back(self);
                    }
                    (yyval.expVal) = make_call((yyvsp[-1].expVal), (yyvsp[0].callVal)->elist);
}

                              cout << "call => lvalue callsuffix\n";
                            }
#line 2300 "parser.cpp"
    break;

  case 59: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 699 "parser.y"
                                                                                                       {expr *func = newexpr(programfunc_e);
                                                                                                func->sym=*((yyvsp[-4].steVal));
                                                                                                (yyval.expVal) = make_call(func,(yyvsp[-1].elistVal));
                                                                                                cout << "call => (funcdef)(elist)\n";}
#line 2309 "parser.cpp"
    break;

  case 60: /* callsuffix: normcall  */
#line 705 "parser.y"
                         {(yyval.callVal)=(yyvsp[0].callVal);
                     cout << "callsuffix => normcall\n";}
#line 2316 "parser.cpp"
    break;

  case 61: /* callsuffix: methodcall  */
#line 707 "parser.y"
                                     {(yyval.callVal)=(yyvsp[0].callVal);
                         cout << "callsuffix => methodcall\n";}
#line 2323 "parser.cpp"
    break;

  case 62: /* normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 712 "parser.y"
                                                    {
                                                     calls c;
                                                     c.name="nil";
                                                     c.method=false;
                                                     c.elist=(yyvsp[-1].elistVal);
                                                     (yyval.callVal)=&c;
                                                     cout << "normcall => (elist)\n";
                                                     }
#line 2336 "parser.cpp"
    break;

  case 63: /* methodcall: DOUBLE_PERIOD ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 722 "parser.y"
                                                                       {calls c;
                                                                        c.name=(yyvsp[-3].stringVal);
                                                                        c.method=true;
                                                                        c.elist=(yyvsp[-1].elistVal);
                                                                        (yyval.callVal)=&c;
                                                                        cout << "methodcall => ..id(elist)\n";
                                                                        }
#line 2348 "parser.cpp"
    break;

  case 64: /* elist: expr  */
#line 731 "parser.y"
             {cout << "elist => expr\n";
             (yyval.elistVal) = new elist_t();  
             (yyval.elistVal)->elist.push_back((yyvsp[0].expVal));   
             }
#line 2357 "parser.cpp"
    break;

  case 65: /* elist: elist COMMA expr  */
#line 735 "parser.y"
                          {cout << "elist => elist,expr\n";
                        (yyval.elistVal) = new elist_t();  
                        (yyval.elistVal)->elist.push_back((yyvsp[0].expVal));  }
#line 2365 "parser.cpp"
    break;

  case 66: /* elist: %empty  */
#line 738 "parser.y"
         {cout << "elist => empty\n";
          (yyval.elistVal) = new elist_t();  
        }
#line 2373 "parser.cpp"
    break;

  case 67: /* objectdef: LEFT_BRACKET elist RIGHT_BRACKET  */
#line 743 "parser.y"
                                            {  expr* t = newexpr(newtable_e);
                                                t->sym = newtmp();
                                                emit(tablecreate,  NULL, NULL,t, -1, yylineno);
                                                int num = 0;
                                                if ((yyvsp[-1].elistVal)!=NULL && (yyvsp[-1].elistVal)->elist.size()>0){
                                                    for(int i=0; i <= (yyvsp[-1].elistVal)->elist.size() - 1; i++)
                                                    emit(tablesetelem, newexpr_constnum(i), (yyvsp[-1].elistVal)->elist[i],t,-1,yylineno);
                                                }
                                                
                                                (yyval.expVal) = t;
                                             cout << "objectdef => [elist]\n";
                                             }
#line 2390 "parser.cpp"
    break;

  case 68: /* objectdef: LEFT_BRACKET indexed RIGHT_BRACKET  */
#line 755 "parser.y"
                                                 {
                                                expr* t = newexpr(newtable_e);
                                                t->sym = newtmp();
                                                emit(tablecreate,  NULL, NULL,t, -1, yylineno);
                                                int num = 0;
                                                if ((yyvsp[-1].elistVal)!=NULL && (yyvsp[-1].elistVal)->elist.size()>0){
                                                    for(int i=0; i <= (yyvsp[-1].elistVal)->elist.size() - 1; i++)
                                                    emit(tablesetelem, newexpr_constnum(i), (yyvsp[-1].elistVal)->elist[i],t,-1,yylineno);
                                                }
                                                
                                                (yyval.expVal) = t;
                                                  cout << "objectdef => [indexed]\n";
                                                  }
#line 2408 "parser.cpp"
    break;

  case 69: /* indexed: indexedelem  */
#line 770 "parser.y"
                      {cout << "indexed => indexedelem\n";
                      (yyval.elistVal) = new elist_t();
                      (yyval.elistVal)->elist.push_back((yyvsp[0].expVal));}
#line 2416 "parser.cpp"
    break;

  case 70: /* indexed: indexed COMMA indexedelem  */
#line 774 "parser.y"
                      {cout << "indexed => indexed, indexelem\n";
                      (yyval.elistVal) = new elist_t();
                      (yyval.elistVal)->elist.push_back((yyvsp[0].expVal));
                      }
#line 2425 "parser.cpp"
    break;

  case 71: /* indexedelem: LEFT_BRACE expr COLON expr RIGHT_BRACE  */
#line 781 "parser.y"
                        {cout << "indexedelem => {expr:expr}\n";
                        if((yyvsp[-3].expVal)==NULL || (yyvsp[-1].expVal)==NULL){
                            (yyval.expVal)=NULL;
                        }else{
                            expr *temp=(yyvsp[-1].expVal);
                            temp->index=(yyvsp[-3].expVal);
                        (yyval.expVal)=temp;
                        }
                        
                        }
#line 2440 "parser.cpp"
    break;

  case 72: /* $@7: %empty  */
#line 793 "parser.y"
                 {scope++;}
#line 2446 "parser.cpp"
    break;

  case 73: /* block: LEFT_BRACE $@7 stmtlist RIGHT_BRACE  */
#line 793 "parser.y"
                                                {hide(scope--); cout << "block => {stmtlist}\n";}
#line 2452 "parser.cpp"
    break;

  case 74: /* stmtlist: stmt stmtlist  */
#line 796 "parser.y"
                         {cout << "stmtlist => stmt stmtlist\n";}
#line 2458 "parser.cpp"
    break;

  case 76: /* funcdef: func_prefix func_args func_body  */
#line 800 "parser.y"
                                         { exitscopespace(); 
                                           (yyval.steVal)->totalloc=functionLocalOffset;
                                           functionLocalOffset=funcLocalStack.top();
                                           funcLocalStack.pop(); 
                                           (yyval.steVal)=(yyvsp[-2].steVal);
                                           emit(funcend,NULL,NULL,lvalue_exp(*((yyvsp[-2].steVal))),0,yylineno);
                                           }
#line 2470 "parser.cpp"
    break;

  case 77: /* func_prefix: FUNCTION ID  */
#line 808 "parser.y"
                        {
            string name= (yyvsp[0].stringVal);
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
                  insert(ste);
                  (yyval.steVal) = &ste;
                  emit(funcstart,NULL,NULL,lvalue_exp(ste),0,yylineno);
                  funcLocalStack.push(functionLocalOffset); 
                  enterscopespace();
                  resetformalargoffset(); 
            }
           
         }
#line 2499 "parser.cpp"
    break;

  case 78: /* func_prefix: FUNCTION  */
#line 832 "parser.y"
                    {
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
                    (yyval.steVal) = &ste;
                    emit(funcstart,NULL,NULL,lvalue_exp(ste),0,yylineno);
                    funcLocalStack.push(functionLocalOffset); 
                    enterscopespace();
                    resetformalargoffset(); 
                    break;
                }
            }
         }
#line 2533 "parser.cpp"
    break;

  case 79: /* func_bstart: %empty  */
#line 863 "parser.y"
             {loopCountStack.push(loopc); loopc=0;}
#line 2539 "parser.cpp"
    break;

  case 80: /* func_bend: %empty  */
#line 865 "parser.y"
            {loopc = loopCountStack.top(); loopCountStack.pop();}
#line 2545 "parser.cpp"
    break;

  case 81: /* $@8: %empty  */
#line 867 "parser.y"
                             {scope++;}
#line 2551 "parser.cpp"
    break;

  case 82: /* func_args: LEFT_PARENTHESIS $@8 idlist RIGHT_PARENTHESIS  */
#line 867 "parser.y"
                                                                 {enterscopespace(); resetfunctionlocaloffset();cout <<"funcdef => function id(idlist)block\n";}
#line 2557 "parser.cpp"
    break;

  case 83: /* $@9: %empty  */
#line 869 "parser.y"
           { scope--; infunction++;}
#line 2563 "parser.cpp"
    break;

  case 84: /* func_body: $@9 func_bstart block func_bend  */
#line 869 "parser.y"
                                                                 {infunction--; exitscopespace(); }
#line 2569 "parser.cpp"
    break;

  case 85: /* const: INTEGER  */
#line 872 "parser.y"
                {(yyval.expVal) = newexpr_constnum((yyvsp[0].intVal));
                 /* $$->numConst=$1; */
                 cout <<"const => integer:"<<yylval.intVal<<endl;}
#line 2577 "parser.cpp"
    break;

  case 86: /* const: REAL  */
#line 875 "parser.y"
                       {(yyval.expVal) = newexpr_constnum((yyvsp[0].doubleVal));
                 /* $$->numConst=$1; */
                 cout <<"const => real:"<<yylval.doubleVal<<endl;}
#line 2585 "parser.cpp"
    break;

  case 87: /* const: STRING  */
#line 878 "parser.y"
                         {(yyval.expVal) = newexpr_conststring((yyvsp[0].stringVal));
                  /* $$->strConst=$1; */
                  cout <<"const => string"<<yylval.stringVal<<endl;}
#line 2593 "parser.cpp"
    break;

  case 88: /* const: NIL  */
#line 881 "parser.y"
                      {(yyval.expVal) = newexpr(nil_e);
              cout <<"const => nil"<<endl;}
#line 2600 "parser.cpp"
    break;

  case 89: /* const: TRUE  */
#line 883 "parser.y"
                       { (yyval.expVal) = newexpr_constbool((yyvsp[0].boolVal));
                 /* $$->boolConst=true; */
                 cout <<"const => true"<<endl;}
#line 2608 "parser.cpp"
    break;

  case 90: /* const: FALSE  */
#line 886 "parser.y"
                        { (yyval.expVal) = newexpr_constbool((yyvsp[0].boolVal));
                  /* $$->boolConst=false; */
                  cout <<"const => false"<<endl;}
#line 2616 "parser.cpp"
    break;

  case 91: /* idlist: ID  */
#line 891 "parser.y"
          {
            string name = (yyvsp[0].stringVal); 
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
#line 2640 "parser.cpp"
    break;

  case 92: /* idlist: idlist COMMA ID  */
#line 910 "parser.y"
                         {
            string name = (yyvsp[0].stringVal); 
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
#line 2664 "parser.cpp"
    break;

  case 94: /* loopstart: %empty  */
#line 932 "parser.y"
            {++inloop; ++loopc;}
#line 2670 "parser.cpp"
    break;

  case 95: /* loopend: %empty  */
#line 934 "parser.y"
          {--inloop; --loopc;}
#line 2676 "parser.cpp"
    break;

  case 96: /* loopstmt: loopstart stmt loopend  */
#line 936 "parser.y"
                                  { (yyval.stmtVal) = (yyvsp[-1].stmtVal); }
#line 2682 "parser.cpp"
    break;

  case 97: /* ifstmt: if_prefix stmt  */
#line 940 "parser.y"
                       {//patchlabel($1-2, $1+1);
                        patchlabel((yyvsp[-1].intVal), nextquad());
                        cout <<"ifstmt => if(expr) stmt"<<endl;
                       }
#line 2691 "parser.cpp"
    break;

  case 98: /* ifstmt: if_prefix stmt else_prefix stmt  */
#line 944 "parser.y"
                                                   {patchlabel((yyvsp[-3].intVal),(yyvsp[-1].intVal)+1); //if eq if_prefix
                                            patchlabel((yyvsp[-1].intVal), nextquad()); //jmp if_prefix
                                            cout <<"ifstmt => if(expr) stmt else stmt"<<endl;
                                            
        }
#line 2701 "parser.cpp"
    break;

  case 99: /* if_prefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 950 "parser.y"
                                                     {
                                                        if((yyvsp[-1].expVal)->type == boolexpr_e){
                                                            if(istempname((yyvsp[-1].expVal)->sym.name)){
                                                                (yyvsp[-1].expVal)->sym = (yyvsp[-1].expVal)->sym;
                                                            }else{
                                                                (yyvsp[-1].expVal)->sym = newtmp();
                                                            }
                                                            patchlist((yyvsp[-1].expVal)->truequad, nextquad());
                                                            emit(assign,newexpr_constbool(1), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
                                                            emit(jump, NULL, NULL, NULL, nextquad()+2, yylineno);
                                                            patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
                                                            emit(assign,newexpr_constbool(0), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
                                                        }
                                                        emit(if_eq, (yyvsp[-1].expVal), newexpr_constbool(1), NULL, nextquad()+2, yylineno);
                                                        (yyval.intVal) = nextquad();
                                                        emit(jump, NULL, NULL, NULL,0,yylineno);
                                                      }
#line 2723 "parser.cpp"
    break;

  case 100: /* else_prefix: ELSE  */
#line 968 "parser.y"
                  {(yyval.intVal) = nextquad();   
                   emit(jump,NULL,NULL,NULL,0,yylineno);}
#line 2730 "parser.cpp"
    break;

  case 101: /* whilestmt: whilestart whilecon loopstmt  */
#line 972 "parser.y"
                                        {
    cout <<"whilestmt => while(expr) stmt"<<endl;
    emit(jump,NULL,NULL,NULL,(yyvsp[-2].intVal),yylineno); 
    patchlabel((yyvsp[-1].intVal), nextquad()+1);
 }
#line 2740 "parser.cpp"
    break;

  case 102: /* whilestart: WHILE  */
#line 978 "parser.y"
                 {(yyval.intVal)=nextquad()+1;}
#line 2746 "parser.cpp"
    break;

  case 103: /* whilecon: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 980 "parser.y"
                                                  {
        if((yyvsp[-1].expVal)->type == boolexpr_e){
            if(!istempname((yyvsp[-1].expVal)->sym.name)){
                (yyvsp[-1].expVal)->sym = newtmp();
            }
            patchlist((yyvsp[-1].expVal)->truequad, nextquad());
            emit(assign,newexpr_constbool(1), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+3, yylineno);
            patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
            emit(assign,newexpr_constbool(0), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
        }
        emit(if_eq, (yyvsp[-1].expVal), newexpr_constbool(1), NULL, nextquad()+3, yylineno);
        (yyval.intVal) = nextquad();
        emit(jump, NULL, NULL, NULL,0,yylineno);
}
#line 2766 "parser.cpp"
    break;

  case 104: /* for_stmt: for_prefix N elist RIGHT_PARENTHESIS N loopstmt N  */
#line 996 "parser.y"
                                                            {
                    forpr *pr=(yyvsp[-6].forVal);
                    patchlabel(pr->enter,(yyvsp[-2].intVal)+2); // true jump
                    patchlabel((yyvsp[-5].intVal),nextquad()+1); // false jump
                    patchlabel((yyvsp[-2].intVal),pr->test+1); // loop jump
                    patchlabel((yyvsp[0].intVal),(yyvsp[-5].intVal)+2); // closure jump
                    cout <<"forstmt => for(elist;expr;elist) stmt"<<endl;
}
#line 2779 "parser.cpp"
    break;

  case 105: /* N: %empty  */
#line 1005 "parser.y"
   {/*unfinished jump*/ (yyval.intVal) = nextquad(); emit(jump,NULL,NULL,NULL,0,yylineno);}
#line 2785 "parser.cpp"
    break;

  case 106: /* M: %empty  */
#line 1007 "parser.y"
  {/*nextquad*/ (yyval.intVal) = nextquad(); }
#line 2791 "parser.cpp"
    break;

  case 107: /* for_prefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON  */
#line 1009 "parser.y"
                                                                 {
            if((yyvsp[-1].expVal)->type == boolexpr_e){
            if(!istempname((yyvsp[-1].expVal)->sym.name)){
                (yyvsp[-1].expVal)->sym = newtmp();
            }
            patchlist((yyvsp[-1].expVal)->truequad, nextquad());
            emit(assign,newexpr_constbool(1), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
            emit(jump, NULL, NULL, NULL, nextquad()+3, yylineno);
            patchlist((yyvsp[-1].expVal)->falsequad, nextquad());
            emit(assign,newexpr_constbool(0), NULL, (yyvsp[-1].expVal), nextquad(), yylineno);
        }
        forpr pr;
        pr.enter=nextquad();
        pr.test=(yyvsp[-2].intVal);
        (yyval.forVal)=&pr;
        emit(if_eq,(yyvsp[-1].expVal),newexpr_constbool(1),NULL,0,yylineno);
}
#line 2813 "parser.cpp"
    break;

  case 108: /* returnstmt: RETURN expr SEMICOLON  */
#line 1027 "parser.y"
                                 {
                    if(infunction==0) { red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                    /* cout <<"returnstmt => return expr;"<<endl; */
                    emit(ret,NULL,NULL,(yyvsp[-1].expVal),0,yylineno);
                    cout <<"returnstmt => return expr;"<<endl;
         }
#line 2824 "parser.cpp"
    break;

  case 109: /* returnstmt: RETURN SEMICOLON  */
#line 1033 "parser.y"
                                           { if(infunction==0) {red(); cout << "Error: Cannot use RETURN when not in function, in line " << yylineno << endl; reset();}
                                cout <<"returnstmt => return;"<<endl;
                                emit(ret,NULL,NULL,NULL,0,yylineno);
                                }
#line 2833 "parser.cpp"
    break;


#line 2837 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1039 "parser.y"
     

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
    print_quads();
    return 0;
}
