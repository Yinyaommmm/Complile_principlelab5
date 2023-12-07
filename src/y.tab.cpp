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
#line 1 "parser.yacc"

#include <stdio.h>
#include "TeaplAst.h"

extern A_pos pos;
extern A_program root;

extern int yylex(void);
extern "C"{
extern void yyerror(char *s); 
extern int  yywrap();
}


#line 86 "y.tab.cpp"

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

#include "y.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ADD = 3,                        /* ADD  */
  YYSYMBOL_SUB = 4,                        /* SUB  */
  YYSYMBOL_MUL = 5,                        /* MUL  */
  YYSYMBOL_DIV = 6,                        /* DIV  */
  YYSYMBOL_SEMICOLON = 7,                  /* SEMICOLON  */
  YYSYMBOL_DOT = 8,                        /* DOT  */
  YYSYMBOL_LBRACE = 9,                     /* LBRACE  */
  YYSYMBOL_RBRACE = 10,                    /* RBRACE  */
  YYSYMBOL_LPAR = 11,                      /* LPAR  */
  YYSYMBOL_RPAR = 12,                      /* RPAR  */
  YYSYMBOL_LBRACKET = 13,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 14,                  /* RBRACKET  */
  YYSYMBOL_UNUM = 15,                      /* UNUM  */
  YYSYMBOL_ID = 16,                        /* ID  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_NOT = 19,                       /* NOT  */
  YYSYMBOL_GTR = 20,                       /* GTR  */
  YYSYMBOL_LES = 21,                       /* LES  */
  YYSYMBOL_GEQ = 22,                       /* GEQ  */
  YYSYMBOL_LEQ = 23,                       /* LEQ  */
  YYSYMBOL_EQL = 24,                       /* EQL  */
  YYSYMBOL_NEQ = 25,                       /* NEQ  */
  YYSYMBOL_INT = 26,                       /* INT  */
  YYSYMBOL_LET = 27,                       /* LET  */
  YYSYMBOL_COLON = 28,                     /* COLON  */
  YYSYMBOL_ASSIGN = 29,                    /* ASSIGN  */
  YYSYMBOL_STRUCT = 30,                    /* STRUCT  */
  YYSYMBOL_COMMA = 31,                     /* COMMA  */
  YYSYMBOL_RARROW = 32,                    /* RARROW  */
  YYSYMBOL_IF = 33,                        /* IF  */
  YYSYMBOL_ELSE = 34,                      /* ELSE  */
  YYSYMBOL_RETURN = 35,                    /* RETURN  */
  YYSYMBOL_WHILE = 36,                     /* WHILE  */
  YYSYMBOL_CONTINUE = 37,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 38,                     /* BREAK  */
  YYSYMBOL_FN = 39,                        /* FN  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_Program = 41,                   /* Program  */
  YYSYMBOL_ProgramElementList = 42,        /* ProgramElementList  */
  YYSYMBOL_ProgramElement = 43,            /* ProgramElement  */
  YYSYMBOL_ArithExpr = 44,                 /* ArithExpr  */
  YYSYMBOL_ExprUnit = 45,                  /* ExprUnit  */
  YYSYMBOL_BoolExpr = 46,                  /* BoolExpr  */
  YYSYMBOL_BoolUnit = 47,                  /* BoolUnit  */
  YYSYMBOL_AssignStmt = 48,                /* AssignStmt  */
  YYSYMBOL_RightVal = 49,                  /* RightVal  */
  YYSYMBOL_LeftVal = 50,                   /* LeftVal  */
  YYSYMBOL_FnCall = 51,                    /* FnCall  */
  YYSYMBOL_RightValList = 52,              /* RightValList  */
  YYSYMBOL_VarDeclStmt = 53,               /* VarDeclStmt  */
  YYSYMBOL_VarDecl = 54,                   /* VarDecl  */
  YYSYMBOL_VarDeclList = 55,               /* VarDeclList  */
  YYSYMBOL_VarDeclScalar = 56,             /* VarDeclScalar  */
  YYSYMBOL_VarDeclArray = 57,              /* VarDeclArray  */
  YYSYMBOL_VarDef = 58,                    /* VarDef  */
  YYSYMBOL_VarDefScalar = 59,              /* VarDefScalar  */
  YYSYMBOL_VarDefArray = 60,               /* VarDefArray  */
  YYSYMBOL_Type = 61,                      /* Type  */
  YYSYMBOL_StructDef = 62,                 /* StructDef  */
  YYSYMBOL_FnDeclStmt = 63,                /* FnDeclStmt  */
  YYSYMBOL_FnDecl = 64,                    /* FnDecl  */
  YYSYMBOL_ParamDecl = 65,                 /* ParamDecl  */
  YYSYMBOL_FnDef = 66,                     /* FnDef  */
  YYSYMBOL_CodeBlockStmtList = 67,         /* CodeBlockStmtList  */
  YYSYMBOL_CodeBlockStmt = 68,             /* CodeBlockStmt  */
  YYSYMBOL_ReturnStmt = 69,                /* ReturnStmt  */
  YYSYMBOL_CallStmt = 70,                  /* CallStmt  */
  YYSYMBOL_IfStmt = 71,                    /* IfStmt  */
  YYSYMBOL_WhileStmt = 72,                 /* WhileStmt  */
  YYSYMBOL_Num = 73,                       /* Num  */
  YYSYMBOL_Id = 74                         /* Id  */
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
#define YYFINAL  23
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   209

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  179

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


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
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   153,   153,   160,   165,   171,   175,   179,   183,   187,
     194,   198,   202,   206,   210,   216,   220,   225,   229,   233,
     238,   242,   246,   252,   256,   260,   265,   269,   273,   277,
     281,   285,   289,   293,   300,   305,   309,   314,   318,   323,
     327,   333,   338,   342,   347,   353,   357,   362,   366,   371,
     375,   380,   384,   389,   396,   400,   405,   409,   414,   418,
     423,   427,   433,   437,   446,   453,   458,   462,   467,   473,
     477,   481,   486,   490,   494,   498,   502,   506,   510,   514,
     518,   522,   527,   531,   536,   543,   547,   552,   558,   563
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
  "\"end of file\"", "error", "\"invalid token\"", "ADD", "SUB", "MUL",
  "DIV", "SEMICOLON", "DOT", "LBRACE", "RBRACE", "LPAR", "RPAR",
  "LBRACKET", "RBRACKET", "UNUM", "ID", "AND", "OR", "NOT", "GTR", "LES",
  "GEQ", "LEQ", "EQL", "NEQ", "INT", "LET", "COLON", "ASSIGN", "STRUCT",
  "COMMA", "RARROW", "IF", "ELSE", "RETURN", "WHILE", "CONTINUE", "BREAK",
  "FN", "$accept", "Program", "ProgramElementList", "ProgramElement",
  "ArithExpr", "ExprUnit", "BoolExpr", "BoolUnit", "AssignStmt",
  "RightVal", "LeftVal", "FnCall", "RightValList", "VarDeclStmt",
  "VarDecl", "VarDeclList", "VarDeclScalar", "VarDeclArray", "VarDef",
  "VarDefScalar", "VarDefArray", "Type", "StructDef", "FnDeclStmt",
  "FnDecl", "ParamDecl", "FnDef", "CodeBlockStmtList", "CodeBlockStmt",
  "ReturnStmt", "CallStmt", "IfStmt", "WhileStmt", "Num", "Id", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-136)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-41)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       7,  -136,    20,    20,    20,    15,  -136,     7,  -136,  -136,
    -136,    41,  -136,  -136,    42,  -136,  -136,    55,  -136,  -136,
      10,    72,    90,  -136,  -136,  -136,    81,  -136,  -136,    87,
      -7,   117,    20,    20,  -136,    99,    96,   102,   119,   136,
    -136,     3,   141,  -136,   142,    81,  -136,  -136,  -136,  -136,
     150,  -136,   163,  -136,   151,  -136,   131,   117,   117,   167,
     144,    66,  -136,  -136,     5,  -136,  -136,   149,   147,   169,
      13,  -136,   171,   117,  -136,   174,   117,  -136,  -136,    20,
      76,   117,  -136,  -136,  -136,   117,    70,   117,   131,  -136,
      48,   113,   144,  -136,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   117,   117,    20,    76,    20,  -136,
      87,    -7,   152,   133,  -136,   137,  -136,   168,   172,   178,
     156,   176,    -7,   180,  -136,  -136,  -136,  -136,   170,   170,
    -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,    66,    66,
      27,   177,   179,  -136,   181,  -136,    -7,   183,   185,  -136,
    -136,  -136,   117,  -136,   161,   117,   114,   121,   173,  -136,
      81,    81,  -136,   187,   188,    -7,   189,   190,   117,  -136,
    -136,   175,  -136,   192,   194,  -136,    81,   195,  -136
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     9,     0,     0,     0,     0,     2,     4,     5,     6,
       7,     0,     8,    89,     0,    47,    48,     0,    56,    57,
      53,     0,     0,     1,     3,    65,    72,    45,    46,     0,
       0,     0,    51,    51,    81,     0,     0,     0,     0,     0,
      74,     0,     0,    73,     0,    70,    78,    75,    76,    77,
      37,    88,     0,    62,    52,    63,     0,     0,     0,    35,
      14,    36,    25,    59,     0,    18,    15,    16,    49,     0,
      53,    68,     0,     0,    83,     0,     0,    79,    80,     0,
       0,     0,    84,    69,    71,    44,    54,     0,     0,    22,
       0,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    64,
       0,     0,    67,     0,    82,     0,    40,     0,     0,     0,
      42,     0,     0,     0,    58,    14,    17,    32,    10,    11,
      12,    13,    26,    27,    28,    29,    30,    31,    23,    24,
      21,     0,     0,    50,     0,    52,     0,     0,     0,    39,
      38,    34,    44,    41,    55,    44,    20,    19,    54,    66,
      72,    72,    43,     0,     0,     0,     0,     0,    44,    61,
      55,    85,    87,     0,     0,    60,    72,     0,    86
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -136,  -136,   197,  -136,    -1,   -29,   -15,   139,  -136,   -24,
     -23,   -20,  -135,    79,   204,   -28,  -136,  -136,  -136,  -136,
    -136,  -101,  -136,  -136,  -136,  -136,  -136,   -37,  -136,  -136,
    -136,  -136,  -136,   -25,    -2
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     5,     6,     7,    59,    60,    61,    62,    40,   120,
      64,    65,   121,    43,    68,    69,    15,    16,    17,    18,
      19,    54,     9,    10,    11,    72,    12,    44,    45,    46,
      47,    48,    49,    66,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      20,    21,    22,    41,    52,    71,    42,    63,    84,    13,
     145,    79,    75,   106,     1,    23,    80,   162,   107,    53,
     164,   154,    41,    29,    50,    42,   110,    89,    55,    92,
      70,    70,    81,   173,     2,   -40,    13,     3,    30,    31,
     -40,   111,    91,    50,    92,   159,     4,    92,    25,    27,
      26,    94,    95,    96,    97,   117,    90,   119,   113,   125,
     126,   115,    28,   124,   170,   125,   125,   125,   125,   132,
     133,   134,   135,   136,   137,    92,    92,   116,   118,     8,
     143,    32,   141,   104,   105,   144,     8,    90,    34,   138,
     139,    51,    13,   128,   129,   130,   131,    13,   122,   123,
      56,    33,    51,    74,   140,   142,    70,    57,     2,    55,
      73,    51,    13,    76,    35,    58,    36,    37,    38,    39,
      55,    56,   -39,   166,   167,   127,    77,   -39,    57,   -38,
     104,   105,    51,    13,   -38,    56,    58,    41,    41,   177,
      42,    42,    88,    78,    55,   147,    51,    13,    82,   148,
     104,   105,    83,    41,   104,   105,    42,   -37,    50,    50,
      85,    85,   -37,    55,    98,    99,   100,   101,   102,   103,
      94,    95,    96,    97,    50,    96,    97,    86,   108,   109,
      87,   114,   149,   112,   146,   151,   150,   152,   153,   155,
     163,   156,   160,   157,   161,   158,   168,    93,   169,   171,
     172,   165,   175,   176,    24,   178,    14,     0,     0,   174
};

static const yytype_int16 yycheck[] =
{
       2,     3,     4,    26,    29,    33,    26,    31,    45,    16,
     111,     8,    36,     8,     7,     0,    13,   152,    13,    26,
     155,   122,    45,    13,    26,    45,    13,    56,    30,    58,
      32,    33,    29,   168,    27,     8,    16,    30,    28,    29,
      13,    28,    57,    45,    73,   146,    39,    76,     7,     7,
       9,     3,     4,     5,     6,    80,    57,    81,    73,    88,
      12,    76,     7,    87,   165,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    79,    80,     0,
     108,     9,   107,    17,    18,   110,     7,    88,     7,   104,
     105,    15,    16,    94,    95,    96,    97,    16,    28,    29,
       4,    11,    15,     7,   106,   107,   108,    11,    27,   111,
      11,    15,    16,    11,    33,    19,    35,    36,    37,    38,
     122,     4,     8,   160,   161,    12,     7,    13,    11,     8,
      17,    18,    15,    16,    13,     4,    19,   160,   161,   176,
     160,   161,    11,     7,   146,    12,    15,    16,     7,    12,
      17,    18,    10,   176,    17,    18,   176,     8,   160,   161,
      11,    11,    13,   165,    20,    21,    22,    23,    24,    25,
       3,     4,     5,     6,   176,     5,     6,    14,    31,    10,
      29,     7,    14,    12,    32,     7,    14,    31,    12,     9,
      29,    14,     9,    14,     9,    14,     9,    58,    10,    10,
      10,    28,    10,     9,     7,    10,     2,    -1,    -1,    34
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    27,    30,    39,    41,    42,    43,    53,    62,
      63,    64,    66,    16,    54,    56,    57,    58,    59,    60,
      74,    74,    74,     0,    42,     7,     9,     7,     7,    13,
      28,    29,     9,    11,     7,    33,    35,    36,    37,    38,
      48,    50,    51,    53,    67,    68,    69,    70,    71,    72,
      74,    15,    73,    26,    61,    74,     4,    11,    19,    44,
      45,    46,    47,    49,    50,    51,    73,    74,    54,    55,
      74,    55,    65,    11,     7,    49,    11,     7,     7,     8,
      13,    29,     7,    10,    67,    11,    14,    29,    11,    45,
      44,    46,    45,    47,     3,     4,     5,     6,    20,    21,
      22,    23,    24,    25,    17,    18,     8,    13,    31,    10,
      13,    28,    12,    46,     7,    46,    74,    73,    74,    49,
      49,    52,    28,    29,    49,    45,    12,    12,    44,    44,
      44,    44,    45,    45,    45,    45,    45,    45,    46,    46,
      74,    73,    74,    55,    73,    61,    32,    12,    12,    14,
      14,     7,    31,    12,    61,     9,    14,    14,    14,    61,
       9,     9,    52,    29,    52,    28,    67,    67,     9,    10,
      61,    10,    10,    52,    34,    10,     9,    67,    10
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    43,    43,
      44,    44,    44,    44,    44,    45,    45,    45,    45,    45,
      45,    45,    45,    46,    46,    46,    47,    47,    47,    47,
      47,    47,    47,    47,    48,    49,    49,    50,    50,    50,
      50,    51,    52,    52,    52,    53,    53,    54,    54,    55,
      55,    55,    56,    56,    57,    57,    58,    58,    59,    59,
      60,    60,    61,    61,    62,    63,    64,    64,    65,    66,
      67,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    69,    69,    70,    71,    71,    72,    73,    74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     1,     1,     1,     3,     1,     4,
       4,     3,     2,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     4,     1,     1,     1,     4,     4,
       3,     4,     1,     3,     0,     3,     3,     1,     1,     1,
       3,     0,     3,     1,     4,     6,     1,     1,     5,     3,
      10,     8,     1,     1,     5,     2,     7,     5,     1,     4,
       1,     2,     0,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     3,     2,     2,     7,    11,     7,     1,     1
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
  case 2: /* Program: ProgramElementList  */
#line 154 "parser.yacc"
{  
  root = A_Program((yyvsp[0].programElementList));
  (yyval.program) = A_Program((yyvsp[0].programElementList));
}
#line 1271 "y.tab.cpp"
    break;

  case 3: /* ProgramElementList: ProgramElement ProgramElementList  */
#line 161 "parser.yacc"
{
  (yyval.programElementList) = A_ProgramElementList((yyvsp[-1].programElement), (yyvsp[0].programElementList));
}
#line 1279 "y.tab.cpp"
    break;

  case 4: /* ProgramElementList: %empty  */
#line 165 "parser.yacc"
{
  (yyval.programElementList) = NULL;
}
#line 1287 "y.tab.cpp"
    break;

  case 5: /* ProgramElement: VarDeclStmt  */
#line 172 "parser.yacc"
{
  (yyval.programElement) = A_ProgramVarDeclStmt((yyvsp[0].varDeclStmt)->pos, (yyvsp[0].varDeclStmt));
}
#line 1295 "y.tab.cpp"
    break;

  case 6: /* ProgramElement: StructDef  */
#line 176 "parser.yacc"
{
  (yyval.programElement) = A_ProgramStructDef((yyvsp[0].structDef)->pos, (yyvsp[0].structDef));
}
#line 1303 "y.tab.cpp"
    break;

  case 7: /* ProgramElement: FnDeclStmt  */
#line 180 "parser.yacc"
{
  (yyval.programElement) = A_ProgramFnDeclStmt((yyvsp[0].fnDeclStmt)->pos, (yyvsp[0].fnDeclStmt));
}
#line 1311 "y.tab.cpp"
    break;

  case 8: /* ProgramElement: FnDef  */
#line 184 "parser.yacc"
{
  (yyval.programElement) = A_ProgramFnDef((yyvsp[0].fnDef)->pos, (yyvsp[0].fnDef));
}
#line 1319 "y.tab.cpp"
    break;

  case 9: /* ProgramElement: SEMICOLON  */
#line 188 "parser.yacc"
{
  (yyval.programElement) = A_ProgramNullStmt((yyvsp[0].pos));
}
#line 1327 "y.tab.cpp"
    break;

  case 10: /* ArithExpr: ArithExpr ADD ArithExpr  */
#line 195 "parser.yacc"
{
  (yyval.arithExpr) = A_ArithBiOp_Expr((yyvsp[-2].arithExpr)->pos, A_ArithBiOpExpr((yyvsp[-2].arithExpr)->pos, A_add, (yyvsp[-2].arithExpr), (yyvsp[0].arithExpr)));
}
#line 1335 "y.tab.cpp"
    break;

  case 11: /* ArithExpr: ArithExpr SUB ArithExpr  */
#line 199 "parser.yacc"
{
  (yyval.arithExpr) = A_ArithBiOp_Expr((yyvsp[-2].arithExpr)->pos, A_ArithBiOpExpr((yyvsp[-2].arithExpr)->pos, A_sub, (yyvsp[-2].arithExpr), (yyvsp[0].arithExpr)));
}
#line 1343 "y.tab.cpp"
    break;

  case 12: /* ArithExpr: ArithExpr MUL ArithExpr  */
#line 203 "parser.yacc"
{
  (yyval.arithExpr) = A_ArithBiOp_Expr((yyvsp[-2].arithExpr)->pos, A_ArithBiOpExpr((yyvsp[-2].arithExpr)->pos, A_mul, (yyvsp[-2].arithExpr), (yyvsp[0].arithExpr)));
}
#line 1351 "y.tab.cpp"
    break;

  case 13: /* ArithExpr: ArithExpr DIV ArithExpr  */
#line 207 "parser.yacc"
{
  (yyval.arithExpr) = A_ArithBiOp_Expr((yyvsp[-2].arithExpr)->pos, A_ArithBiOpExpr((yyvsp[-2].arithExpr)->pos, A_div, (yyvsp[-2].arithExpr), (yyvsp[0].arithExpr)));
}
#line 1359 "y.tab.cpp"
    break;

  case 14: /* ArithExpr: ExprUnit  */
#line 211 "parser.yacc"
{
  (yyval.arithExpr) = A_ExprUnit((yyvsp[0].exprUnit)->pos, (yyvsp[0].exprUnit));
}
#line 1367 "y.tab.cpp"
    break;

  case 15: /* ExprUnit: Num  */
#line 217 "parser.yacc"
{
  (yyval.exprUnit) = A_NumExprUnit((yyvsp[0].tokenNum)->pos,(yyvsp[0].tokenNum)->num);
}
#line 1375 "y.tab.cpp"
    break;

  case 16: /* ExprUnit: Id  */
#line 221 "parser.yacc"
{

  (yyval.exprUnit) = A_IdExprUnit((yyvsp[0].tokenId)->pos, (yyvsp[0].tokenId)->id);
}
#line 1384 "y.tab.cpp"
    break;

  case 17: /* ExprUnit: LPAR ArithExpr RPAR  */
#line 226 "parser.yacc"
{
  (yyval.exprUnit) = A_ArithExprUnit((yyvsp[-2].pos), (yyvsp[-1].arithExpr));
}
#line 1392 "y.tab.cpp"
    break;

  case 18: /* ExprUnit: FnCall  */
#line 230 "parser.yacc"
{
  (yyval.exprUnit) = A_CallExprUnit((yyvsp[0].fnCall)->pos ,(yyvsp[0].fnCall));
}
#line 1400 "y.tab.cpp"
    break;

  case 19: /* ExprUnit: LeftVal LBRACKET Id RBRACKET  */
#line 234 "parser.yacc"
{
  
  (yyval.exprUnit) = A_ArrayExprUnit((yyvsp[-3].leftVal)->pos , A_ArrayExpr((yyvsp[-3].leftVal)->pos, (yyvsp[-3].leftVal), A_IdIndexExpr((yyvsp[-1].tokenId)->pos,(yyvsp[-1].tokenId)->id) ));
}
#line 1409 "y.tab.cpp"
    break;

  case 20: /* ExprUnit: LeftVal LBRACKET Num RBRACKET  */
#line 239 "parser.yacc"
{
  (yyval.exprUnit) = A_ArrayExprUnit((yyvsp[-3].leftVal)->pos , A_ArrayExpr((yyvsp[-3].leftVal)->pos, (yyvsp[-3].leftVal), A_NumIndexExpr((yyvsp[-1].tokenNum)->pos,(yyvsp[-1].tokenNum)->num) ));
}
#line 1417 "y.tab.cpp"
    break;

  case 21: /* ExprUnit: LeftVal DOT Id  */
#line 243 "parser.yacc"
{
  (yyval.exprUnit) = A_MemberExprUnit((yyvsp[-2].leftVal)->pos, A_MemberExpr((yyvsp[-2].leftVal)->pos,(yyvsp[-2].leftVal),(yyvsp[0].tokenId)->id) );
}
#line 1425 "y.tab.cpp"
    break;

  case 22: /* ExprUnit: SUB ExprUnit  */
#line 247 "parser.yacc"
{
  (yyval.exprUnit) = A_ArithUExprUnit((yyvsp[-1].pos), A_ArithUExpr((yyvsp[0].exprUnit)->pos,A_neg,(yyvsp[0].exprUnit)));
}
#line 1433 "y.tab.cpp"
    break;

  case 23: /* BoolExpr: BoolExpr AND BoolExpr  */
#line 253 "parser.yacc"
{
  (yyval.boolExpr) = A_BoolBiOp_Expr((yyvsp[-2].boolExpr)->pos, A_BoolBiOpExpr((yyvsp[0].boolExpr)->pos,A_and,(yyvsp[-2].boolExpr),(yyvsp[0].boolExpr)));
}
#line 1441 "y.tab.cpp"
    break;

  case 24: /* BoolExpr: BoolExpr OR BoolExpr  */
#line 257 "parser.yacc"
{
  (yyval.boolExpr) = A_BoolBiOp_Expr((yyvsp[-2].boolExpr)->pos, A_BoolBiOpExpr((yyvsp[0].boolExpr)->pos,A_or,(yyvsp[-2].boolExpr),(yyvsp[0].boolExpr)));
}
#line 1449 "y.tab.cpp"
    break;

  case 25: /* BoolExpr: BoolUnit  */
#line 261 "parser.yacc"
{
  (yyval.boolExpr) = A_BoolExpr((yyvsp[0].boolUnit)->pos,(yyvsp[0].boolUnit));
}
#line 1457 "y.tab.cpp"
    break;

  case 26: /* BoolUnit: ExprUnit GTR ExprUnit  */
#line 266 "parser.yacc"
{
  (yyval.boolUnit) = A_ComExprUnit((yyvsp[-2].exprUnit)->pos ,A_ComExpr((yyvsp[0].exprUnit)->pos,A_gt,(yyvsp[-2].exprUnit),(yyvsp[0].exprUnit)));
}
#line 1465 "y.tab.cpp"
    break;

  case 27: /* BoolUnit: ExprUnit LES ExprUnit  */
#line 270 "parser.yacc"
{
  (yyval.boolUnit) = A_ComExprUnit((yyvsp[-2].exprUnit)->pos ,A_ComExpr((yyvsp[0].exprUnit)->pos,A_lt,(yyvsp[-2].exprUnit),(yyvsp[0].exprUnit)));
}
#line 1473 "y.tab.cpp"
    break;

  case 28: /* BoolUnit: ExprUnit GEQ ExprUnit  */
#line 274 "parser.yacc"
{
  (yyval.boolUnit) = A_ComExprUnit((yyvsp[-2].exprUnit)->pos ,A_ComExpr((yyvsp[0].exprUnit)->pos,A_ge,(yyvsp[-2].exprUnit),(yyvsp[0].exprUnit)));
}
#line 1481 "y.tab.cpp"
    break;

  case 29: /* BoolUnit: ExprUnit LEQ ExprUnit  */
#line 278 "parser.yacc"
{
  (yyval.boolUnit) = A_ComExprUnit((yyvsp[-2].exprUnit)->pos ,A_ComExpr((yyvsp[0].exprUnit)->pos,A_le,(yyvsp[-2].exprUnit),(yyvsp[0].exprUnit)));
}
#line 1489 "y.tab.cpp"
    break;

  case 30: /* BoolUnit: ExprUnit EQL ExprUnit  */
#line 282 "parser.yacc"
{
  (yyval.boolUnit) = A_ComExprUnit((yyvsp[-2].exprUnit)->pos ,A_ComExpr((yyvsp[0].exprUnit)->pos,A_eq,(yyvsp[-2].exprUnit),(yyvsp[0].exprUnit)));
}
#line 1497 "y.tab.cpp"
    break;

  case 31: /* BoolUnit: ExprUnit NEQ ExprUnit  */
#line 286 "parser.yacc"
{
  (yyval.boolUnit) = A_ComExprUnit((yyvsp[-2].exprUnit)->pos ,A_ComExpr((yyvsp[0].exprUnit)->pos,A_ne,(yyvsp[-2].exprUnit),(yyvsp[0].exprUnit)));
}
#line 1505 "y.tab.cpp"
    break;

  case 32: /* BoolUnit: LPAR BoolExpr RPAR  */
#line 290 "parser.yacc"
{
  (yyval.boolUnit) = A_BoolExprUnit((yyvsp[-2].pos), (yyvsp[-1].boolExpr));
}
#line 1513 "y.tab.cpp"
    break;

  case 33: /* BoolUnit: NOT BoolUnit  */
#line 294 "parser.yacc"
{
  (yyval.boolUnit) = A_BoolUOpExprUnit((yyvsp[-1].pos),A_BoolUOpExpr((yyvsp[-1].pos),A_not,(yyvsp[0].boolUnit)));
}
#line 1521 "y.tab.cpp"
    break;

  case 34: /* AssignStmt: LeftVal ASSIGN RightVal SEMICOLON  */
#line 301 "parser.yacc"
{
  (yyval.assignStmt) = A_AssignStmt((yyvsp[-3].leftVal)->pos,(yyvsp[-3].leftVal),(yyvsp[-1].rightVal));
}
#line 1529 "y.tab.cpp"
    break;

  case 35: /* RightVal: ArithExpr  */
#line 306 "parser.yacc"
{
  (yyval.rightVal) = A_ArithExprRVal((yyvsp[0].arithExpr)->pos,(yyvsp[0].arithExpr));
}
#line 1537 "y.tab.cpp"
    break;

  case 36: /* RightVal: BoolExpr  */
#line 310 "parser.yacc"
{
  (yyval.rightVal) = A_BoolExprRVal((yyvsp[0].boolExpr)->pos,(yyvsp[0].boolExpr));
}
#line 1545 "y.tab.cpp"
    break;

  case 37: /* LeftVal: Id  */
#line 315 "parser.yacc"
{
  (yyval.leftVal) = A_IdExprLVal((yyvsp[0].tokenId)->pos,(yyvsp[0].tokenId)->id);
}
#line 1553 "y.tab.cpp"
    break;

  case 38: /* LeftVal: LeftVal LBRACKET Id RBRACKET  */
#line 319 "parser.yacc"
{
  
  (yyval.leftVal) = A_ArrExprLVal((yyvsp[-3].leftVal)->pos,A_ArrayExpr((yyvsp[-3].leftVal)->pos,(yyvsp[-3].leftVal),A_IdIndexExpr((yyvsp[-1].tokenId)->pos,(yyvsp[-1].tokenId)->id)));
}
#line 1562 "y.tab.cpp"
    break;

  case 39: /* LeftVal: LeftVal LBRACKET Num RBRACKET  */
#line 324 "parser.yacc"
{
  (yyval.leftVal) = A_ArrExprLVal((yyvsp[-3].leftVal)->pos,A_ArrayExpr((yyvsp[-3].leftVal)->pos,(yyvsp[-3].leftVal) ,A_NumIndexExpr((yyvsp[-1].tokenNum)->pos,(yyvsp[-1].tokenNum)->num)));
}
#line 1570 "y.tab.cpp"
    break;

  case 40: /* LeftVal: LeftVal DOT Id  */
#line 328 "parser.yacc"
{
  (yyval.leftVal) = A_MemberExprLVal((yyvsp[-2].leftVal)->pos, A_MemberExpr((yyvsp[-2].leftVal)->pos,(yyvsp[-2].leftVal),(yyvsp[0].tokenId)->id));
}
#line 1578 "y.tab.cpp"
    break;

  case 41: /* FnCall: Id LPAR RightValList RPAR  */
#line 334 "parser.yacc"
{
  (yyval.fnCall) = A_FnCall((yyvsp[-3].tokenId)->pos, (yyvsp[-3].tokenId)->id, (yyvsp[-1].rightValList));
}
#line 1586 "y.tab.cpp"
    break;

  case 42: /* RightValList: RightVal  */
#line 339 "parser.yacc"
{
  (yyval.rightValList) = A_RightValList((yyvsp[0].rightVal),NULL);
}
#line 1594 "y.tab.cpp"
    break;

  case 43: /* RightValList: RightVal COMMA RightValList  */
#line 343 "parser.yacc"
{
  (yyval.rightValList) = A_RightValList((yyvsp[-2].rightVal),(yyvsp[0].rightValList));
}
#line 1602 "y.tab.cpp"
    break;

  case 44: /* RightValList: %empty  */
#line 347 "parser.yacc"
{
  (yyval.rightValList) = NULL;
}
#line 1610 "y.tab.cpp"
    break;

  case 45: /* VarDeclStmt: LET VarDecl SEMICOLON  */
#line 354 "parser.yacc"
{
  (yyval.varDeclStmt) = A_VarDeclStmt((yyvsp[-2].pos),(yyvsp[-1].varDecl));
}
#line 1618 "y.tab.cpp"
    break;

  case 46: /* VarDeclStmt: LET VarDef SEMICOLON  */
#line 358 "parser.yacc"
{
  (yyval.varDeclStmt) = A_VarDefStmt((yyvsp[-2].pos) , (yyvsp[-1].varDef));
}
#line 1626 "y.tab.cpp"
    break;

  case 47: /* VarDecl: VarDeclScalar  */
#line 363 "parser.yacc"
{
  (yyval.varDecl) =  A_VarDecl_Scalar((yyvsp[0].varDeclScalar)->pos,(yyvsp[0].varDeclScalar));
}
#line 1634 "y.tab.cpp"
    break;

  case 48: /* VarDecl: VarDeclArray  */
#line 367 "parser.yacc"
{
  (yyval.varDecl) = A_VarDecl_Array((yyvsp[0].varDeclArray)->pos,(yyvsp[0].varDeclArray));
}
#line 1642 "y.tab.cpp"
    break;

  case 49: /* VarDeclList: VarDecl  */
#line 372 "parser.yacc"
{
  (yyval.varDeclList) = A_VarDeclList((yyvsp[0].varDecl),NULL);
}
#line 1650 "y.tab.cpp"
    break;

  case 50: /* VarDeclList: VarDecl COMMA VarDeclList  */
#line 376 "parser.yacc"
{
  (yyval.varDeclList) = A_VarDeclList((yyvsp[-2].varDecl) ,(yyvsp[0].varDeclList));
}
#line 1658 "y.tab.cpp"
    break;

  case 51: /* VarDeclList: %empty  */
#line 380 "parser.yacc"
{
  (yyval.varDeclList) = NULL;
}
#line 1666 "y.tab.cpp"
    break;

  case 52: /* VarDeclScalar: Id COLON Type  */
#line 385 "parser.yacc"
{
  //printf("match long\n");
  (yyval.varDeclScalar) = A_VarDeclScalar((yyvsp[-2].tokenId)->pos , (yyvsp[-2].tokenId)->id ,(yyvsp[0].type));
}
#line 1675 "y.tab.cpp"
    break;

  case 53: /* VarDeclScalar: Id  */
#line 390 "parser.yacc"
{
  //printf("match id\n");
  (yyval.varDeclScalar) = A_VarDeclScalar((yyvsp[0].tokenId)->pos,(yyvsp[0].tokenId)->id, NULL);
}
#line 1684 "y.tab.cpp"
    break;

  case 54: /* VarDeclArray: Id LBRACKET Num RBRACKET  */
#line 397 "parser.yacc"
{
  (yyval.varDeclArray) = A_VarDeclArray((yyvsp[-3].tokenId)->pos , (yyvsp[-3].tokenId)->id, (yyvsp[-1].tokenNum)->num, NULL);
}
#line 1692 "y.tab.cpp"
    break;

  case 55: /* VarDeclArray: Id LBRACKET Num RBRACKET COLON Type  */
#line 401 "parser.yacc"
{
  (yyval.varDeclArray) = A_VarDeclArray((yyvsp[-5].tokenId)->pos , (yyvsp[-5].tokenId)->id, (yyvsp[-3].tokenNum)->num,(yyvsp[0].type) );
}
#line 1700 "y.tab.cpp"
    break;

  case 56: /* VarDef: VarDefScalar  */
#line 406 "parser.yacc"
{
  (yyval.varDef) = A_VarDef_Scalar((yyvsp[0].varDefScalar)->pos, (yyvsp[0].varDefScalar));
}
#line 1708 "y.tab.cpp"
    break;

  case 57: /* VarDef: VarDefArray  */
#line 410 "parser.yacc"
{
  (yyval.varDef) = A_VarDef_Array((yyvsp[0].varDefArray)->pos,(yyvsp[0].varDefArray));
}
#line 1716 "y.tab.cpp"
    break;

  case 58: /* VarDefScalar: Id COLON Type ASSIGN RightVal  */
#line 415 "parser.yacc"
{
  (yyval.varDefScalar) = A_VarDefScalar((yyvsp[-4].tokenId)->pos,(yyvsp[-4].tokenId)->id,(yyvsp[-2].type),(yyvsp[0].rightVal));
}
#line 1724 "y.tab.cpp"
    break;

  case 59: /* VarDefScalar: Id ASSIGN RightVal  */
#line 419 "parser.yacc"
{
  (yyval.varDefScalar) = A_VarDefScalar((yyvsp[-2].tokenId)->pos,(yyvsp[-2].tokenId)->id,NULL,(yyvsp[0].rightVal));
}
#line 1732 "y.tab.cpp"
    break;

  case 60: /* VarDefArray: Id LBRACKET Num RBRACKET COLON Type ASSIGN LBRACE RightValList RBRACE  */
#line 424 "parser.yacc"
{
  (yyval.varDefArray) = A_VarDefArray((yyvsp[-9].tokenId)->pos,(yyvsp[-9].tokenId)->id,(yyvsp[-7].tokenNum)->num,(yyvsp[-4].type),(yyvsp[-1].rightValList));
}
#line 1740 "y.tab.cpp"
    break;

  case 61: /* VarDefArray: Id LBRACKET Num RBRACKET ASSIGN LBRACE RightValList RBRACE  */
#line 428 "parser.yacc"
{
  (yyval.varDefArray) = A_VarDefArray((yyvsp[-7].tokenId)->pos,(yyvsp[-7].tokenId)->id,(yyvsp[-5].tokenNum)->num,NULL,(yyvsp[-1].rightValList));
}
#line 1748 "y.tab.cpp"
    break;

  case 62: /* Type: INT  */
#line 434 "parser.yacc"
{
  (yyval.type) = A_NativeType((yyvsp[0].pos), A_intTypeKind);
}
#line 1756 "y.tab.cpp"
    break;

  case 63: /* Type: Id  */
#line 438 "parser.yacc"
{
  (yyval.type) = A_StructType((yyvsp[0].tokenId)->pos, (yyvsp[0].tokenId)->id);
}
#line 1764 "y.tab.cpp"
    break;

  case 64: /* StructDef: STRUCT Id LBRACE VarDeclList RBRACE  */
#line 447 "parser.yacc"
{
  (yyval.structDef) = A_StructDef((yyvsp[-4].pos), (yyvsp[-3].tokenId)->id, (yyvsp[-1].varDeclList));
}
#line 1772 "y.tab.cpp"
    break;

  case 65: /* FnDeclStmt: FnDecl SEMICOLON  */
#line 454 "parser.yacc"
{
  (yyval.fnDeclStmt) = A_FnDeclStmt((yyvsp[-1].fnDecl)->pos,(yyvsp[-1].fnDecl));
}
#line 1780 "y.tab.cpp"
    break;

  case 66: /* FnDecl: FN Id LPAR ParamDecl RPAR RARROW Type  */
#line 459 "parser.yacc"
{
  (yyval.fnDecl) = A_FnDecl((yyvsp[-6].pos),(yyvsp[-5].tokenId)->id,(yyvsp[-3].paramDecl),(yyvsp[0].type));
}
#line 1788 "y.tab.cpp"
    break;

  case 67: /* FnDecl: FN Id LPAR ParamDecl RPAR  */
#line 463 "parser.yacc"
{
  (yyval.fnDecl) = A_FnDecl((yyvsp[-4].pos),(yyvsp[-3].tokenId)->id,(yyvsp[-1].paramDecl),NULL);
}
#line 1796 "y.tab.cpp"
    break;

  case 68: /* ParamDecl: VarDeclList  */
#line 468 "parser.yacc"
{
  (yyval.paramDecl) = A_ParamDecl((yyvsp[0].varDeclList));
}
#line 1804 "y.tab.cpp"
    break;

  case 69: /* FnDef: FnDecl LBRACE CodeBlockStmtList RBRACE  */
#line 474 "parser.yacc"
{
  (yyval.fnDef) = A_FnDef((yyvsp[-3].fnDecl)->pos,(yyvsp[-3].fnDecl), (yyvsp[-1].codeBlockStmtList));
}
#line 1812 "y.tab.cpp"
    break;

  case 70: /* CodeBlockStmtList: CodeBlockStmt  */
#line 478 "parser.yacc"
{
  (yyval.codeBlockStmtList) = A_CodeBlockStmtList((yyvsp[0].codeBlockStmt), NULL);
}
#line 1820 "y.tab.cpp"
    break;

  case 71: /* CodeBlockStmtList: CodeBlockStmt CodeBlockStmtList  */
#line 482 "parser.yacc"
{
  (yyval.codeBlockStmtList) = A_CodeBlockStmtList((yyvsp[-1].codeBlockStmt), (yyvsp[0].codeBlockStmtList));
}
#line 1828 "y.tab.cpp"
    break;

  case 72: /* CodeBlockStmtList: %empty  */
#line 486 "parser.yacc"
{
  (yyval.codeBlockStmtList) = NULL;
}
#line 1836 "y.tab.cpp"
    break;

  case 73: /* CodeBlockStmt: VarDeclStmt  */
#line 491 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockVarDeclStmt((yyvsp[0].varDeclStmt)->pos,(yyvsp[0].varDeclStmt));
}
#line 1844 "y.tab.cpp"
    break;

  case 74: /* CodeBlockStmt: AssignStmt  */
#line 495 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockAssignStmt((yyvsp[0].assignStmt)->pos ,(yyvsp[0].assignStmt));
}
#line 1852 "y.tab.cpp"
    break;

  case 75: /* CodeBlockStmt: CallStmt  */
#line 499 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockCallStmt((yyvsp[0].callStmt)->pos ,(yyvsp[0].callStmt));
}
#line 1860 "y.tab.cpp"
    break;

  case 76: /* CodeBlockStmt: IfStmt  */
#line 503 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockIfStmt((yyvsp[0].ifStmt)->pos ,(yyvsp[0].ifStmt));
}
#line 1868 "y.tab.cpp"
    break;

  case 77: /* CodeBlockStmt: WhileStmt  */
#line 507 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockWhileStmt((yyvsp[0].whileStmt)->pos ,(yyvsp[0].whileStmt));
}
#line 1876 "y.tab.cpp"
    break;

  case 78: /* CodeBlockStmt: ReturnStmt  */
#line 511 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockReturnStmt((yyvsp[0].returnStmt)->pos ,(yyvsp[0].returnStmt));
}
#line 1884 "y.tab.cpp"
    break;

  case 79: /* CodeBlockStmt: CONTINUE SEMICOLON  */
#line 515 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockContinueStmt((yyvsp[-1].pos));
}
#line 1892 "y.tab.cpp"
    break;

  case 80: /* CodeBlockStmt: BREAK SEMICOLON  */
#line 519 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockBreakStmt((yyvsp[-1].pos));
}
#line 1900 "y.tab.cpp"
    break;

  case 81: /* CodeBlockStmt: SEMICOLON  */
#line 523 "parser.yacc"
{
  (yyval.codeBlockStmt) = A_BlockNullStmt((yyvsp[0].pos));
}
#line 1908 "y.tab.cpp"
    break;

  case 82: /* ReturnStmt: RETURN RightVal SEMICOLON  */
#line 528 "parser.yacc"
{
  (yyval.returnStmt) = A_ReturnStmt((yyvsp[-2].pos) , (yyvsp[-1].rightVal));
}
#line 1916 "y.tab.cpp"
    break;

  case 83: /* ReturnStmt: RETURN SEMICOLON  */
#line 532 "parser.yacc"
{
  (yyval.returnStmt) = A_ReturnStmt((yyvsp[-1].pos) , NULL);
}
#line 1924 "y.tab.cpp"
    break;

  case 84: /* CallStmt: FnCall SEMICOLON  */
#line 537 "parser.yacc"
{
  (yyval.callStmt) = A_CallStmt((yyvsp[-1].fnCall)->pos,(yyvsp[-1].fnCall));
}
#line 1932 "y.tab.cpp"
    break;

  case 85: /* IfStmt: IF LPAR BoolExpr RPAR LBRACE CodeBlockStmtList RBRACE  */
#line 544 "parser.yacc"
{
  (yyval.ifStmt) = A_IfStmt((yyvsp[-6].pos), (yyvsp[-4].boolExpr), (yyvsp[-1].codeBlockStmtList),NULL);
}
#line 1940 "y.tab.cpp"
    break;

  case 86: /* IfStmt: IF LPAR BoolExpr RPAR LBRACE CodeBlockStmtList RBRACE ELSE LBRACE CodeBlockStmtList RBRACE  */
#line 548 "parser.yacc"
{
  (yyval.ifStmt) = A_IfStmt((yyvsp[-10].pos), (yyvsp[-8].boolExpr), (yyvsp[-5].codeBlockStmtList), (yyvsp[-1].codeBlockStmtList));
}
#line 1948 "y.tab.cpp"
    break;

  case 87: /* WhileStmt: WHILE LPAR BoolExpr RPAR LBRACE CodeBlockStmtList RBRACE  */
#line 553 "parser.yacc"
{
  (yyval.whileStmt) = A_WhileStmt((yyvsp[-6].pos), (yyvsp[-4].boolExpr), (yyvsp[-1].codeBlockStmtList));
}
#line 1956 "y.tab.cpp"
    break;

  case 88: /* Num: UNUM  */
#line 559 "parser.yacc"
{
  (yyval.tokenNum) = (yyvsp[0].tokenNum);
}
#line 1964 "y.tab.cpp"
    break;

  case 89: /* Id: ID  */
#line 564 "parser.yacc"
{
  (yyval.tokenId) = (yyvsp[0].tokenId);
}
#line 1972 "y.tab.cpp"
    break;


#line 1976 "y.tab.cpp"

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

#line 567 "parser.yacc"


extern "C"{
void yyerror(char * s)
{
  fprintf(stderr, "%s\n",s);
}
int yywrap()
{
  return(1);
}
}


