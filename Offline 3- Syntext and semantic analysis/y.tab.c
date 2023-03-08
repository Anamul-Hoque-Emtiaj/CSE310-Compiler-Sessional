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
#line 1 "1905113.y"

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include "1905113.h"
#define num_buckets 11


using namespace std;

int yyparse(void);
int yylex(void);

extern FILE *yyin;
extern int line_count;
extern int error_count;

SymbolTable *symbolTable = new SymbolTable(num_buckets);
vector<SymbolInfo*> paramList;
ofstream errFile;
ofstream treeFile;

string typeCasting(string x, string y){
		if(x == y)
			return x;
		if(x == "INT" && y == "FLOAT"){
			return "FLOAT";
		}else if(x == "FLOAT" && y == "INT"){
			return "FLOAT";
		}
		return "VOID";
	}


void declareFuncParam(string dataType, string name, int it){
		if(dataType == "VOID"){
			errFile<<"Line# "<<line_count<<": Function parameter cannot be void\n";
			error_count++;
			return;
		}
		if(!symbolTable->Insert(name, "ID",dataType) && name!=""){
			errFile<<"Line# "<<line_count<<": Redefinition of parameter '"<<name<<"'\n";
			error_count++;
			return;
		}

		SymbolInfo* info = symbolTable->LookUp(name);
		info->setInfoType(it);
	}

void declareFuncParamList(){
	if(paramList.empty()){
		return;
	}
	for(SymbolInfo* info: paramList){

		declareFuncParam(info->getDataType(), info->getName(), info->getInfoType());
	}
	paramList.clear();
}

void deleteInfo(SymbolInfo* info)
{
	for(int i=0; i<info->getChild().size(); i++){
		deleteInfo(info->getChild()[i]);
	}
	delete info;
}

void genTree(SymbolInfo* symInfo, int sc){
	for(int i=1; i<=sc; i++)
		treeFile<<" ";
	
	if(symInfo->getIsleaf())
	{
		if(symInfo->getType()=="ARRAY")
			treeFile<<"ID : "<<symInfo->getName()<<"\t<Line: "<<symInfo->getSl()<<">\n";
		else
			treeFile<<symInfo->getType()<<" : "<<symInfo->getName()<<"\t<Line: "<<symInfo->getSl()<<">\n";
	}
	else
		treeFile<<symInfo->getType()<<" : "<<symInfo->getName()<<" \t<Line: "<<symInfo->getSl()<<"-"<<symInfo->getEl()<<">\n";

	for(int i=0; i<symInfo->getChild().size(); i++){
		genTree(symInfo->getChild()[i], sc+1);
	}

}

string findLeafVal(SymbolInfo* info)
{
	if(info->getIsleaf())
		return info->getName();
	return findLeafVal(info->getChild()[0]);
}

void yyerror(char *s)
{
	//write your code
}



#line 177 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    DO = 261,                      /* DO  */
    WHILE = 262,                   /* WHILE  */
    SWITCH = 263,                  /* SWITCH  */
    CASE = 264,                    /* CASE  */
    DEFAULT = 265,                 /* DEFAULT  */
    INT = 266,                     /* INT  */
    FLOAT = 267,                   /* FLOAT  */
    DOUBLE = 268,                  /* DOUBLE  */
    CHAR = 269,                    /* CHAR  */
    VOID = 270,                    /* VOID  */
    BREAK = 271,                   /* BREAK  */
    CONTINUE = 272,                /* CONTINUE  */
    RETURN = 273,                  /* RETURN  */
    PRINTLN = 274,                 /* PRINTLN  */
    LPAREN = 275,                  /* LPAREN  */
    RPAREN = 276,                  /* RPAREN  */
    LCURL = 277,                   /* LCURL  */
    RCURL = 278,                   /* RCURL  */
    LSQUARE = 279,                 /* LSQUARE  */
    RSQUARE = 280,                 /* RSQUARE  */
    COMMA = 281,                   /* COMMA  */
    SEMICOLON = 282,               /* SEMICOLON  */
    NOT = 283,                     /* NOT  */
    ADDOP = 284,                   /* ADDOP  */
    MULOP = 285,                   /* MULOP  */
    INCOP = 286,                   /* INCOP  */
    DECOP = 287,                   /* DECOP  */
    RELOP = 288,                   /* RELOP  */
    ASSIGNOP = 289,                /* ASSIGNOP  */
    LOGICOP = 290,                 /* LOGICOP  */
    BITOP = 291,                   /* BITOP  */
    CONST_INT = 292,               /* CONST_INT  */
    CONST_FLOAT = 293,             /* CONST_FLOAT  */
    CONST_CHAR = 294,              /* CONST_CHAR  */
    ID = 295,                      /* ID  */
    LOWER_THAN_ELSE = 296          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define DO 261
#define WHILE 262
#define SWITCH 263
#define CASE 264
#define DEFAULT 265
#define INT 266
#define FLOAT 267
#define DOUBLE 268
#define CHAR 269
#define VOID 270
#define BREAK 271
#define CONTINUE 272
#define RETURN 273
#define PRINTLN 274
#define LPAREN 275
#define RPAREN 276
#define LCURL 277
#define RCURL 278
#define LSQUARE 279
#define RSQUARE 280
#define COMMA 281
#define SEMICOLON 282
#define NOT 283
#define ADDOP 284
#define MULOP 285
#define INCOP 286
#define DECOP 287
#define RELOP 288
#define ASSIGNOP 289
#define LOGICOP 290
#define BITOP 291
#define CONST_INT 292
#define CONST_FLOAT 293
#define CONST_CHAR 294
#define ID 295
#define LOWER_THAN_ELSE 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 119 "1905113.y"

	SymbolInfo* symbolInfo; 

#line 316 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_FOR = 5,                        /* FOR  */
  YYSYMBOL_DO = 6,                         /* DO  */
  YYSYMBOL_WHILE = 7,                      /* WHILE  */
  YYSYMBOL_SWITCH = 8,                     /* SWITCH  */
  YYSYMBOL_CASE = 9,                       /* CASE  */
  YYSYMBOL_DEFAULT = 10,                   /* DEFAULT  */
  YYSYMBOL_INT = 11,                       /* INT  */
  YYSYMBOL_FLOAT = 12,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 13,                    /* DOUBLE  */
  YYSYMBOL_CHAR = 14,                      /* CHAR  */
  YYSYMBOL_VOID = 15,                      /* VOID  */
  YYSYMBOL_BREAK = 16,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 17,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 18,                    /* RETURN  */
  YYSYMBOL_PRINTLN = 19,                   /* PRINTLN  */
  YYSYMBOL_LPAREN = 20,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 21,                    /* RPAREN  */
  YYSYMBOL_LCURL = 22,                     /* LCURL  */
  YYSYMBOL_RCURL = 23,                     /* RCURL  */
  YYSYMBOL_LSQUARE = 24,                   /* LSQUARE  */
  YYSYMBOL_RSQUARE = 25,                   /* RSQUARE  */
  YYSYMBOL_COMMA = 26,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 27,                 /* SEMICOLON  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_ADDOP = 29,                     /* ADDOP  */
  YYSYMBOL_MULOP = 30,                     /* MULOP  */
  YYSYMBOL_INCOP = 31,                     /* INCOP  */
  YYSYMBOL_DECOP = 32,                     /* DECOP  */
  YYSYMBOL_RELOP = 33,                     /* RELOP  */
  YYSYMBOL_ASSIGNOP = 34,                  /* ASSIGNOP  */
  YYSYMBOL_LOGICOP = 35,                   /* LOGICOP  */
  YYSYMBOL_BITOP = 36,                     /* BITOP  */
  YYSYMBOL_CONST_INT = 37,                 /* CONST_INT  */
  YYSYMBOL_CONST_FLOAT = 38,               /* CONST_FLOAT  */
  YYSYMBOL_CONST_CHAR = 39,                /* CONST_CHAR  */
  YYSYMBOL_ID = 40,                        /* ID  */
  YYSYMBOL_LOWER_THAN_ELSE = 41,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_start = 43,                     /* start  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_unit = 45,                      /* unit  */
  YYSYMBOL_func_declaration = 46,          /* func_declaration  */
  YYSYMBOL_func_definition = 47,           /* func_definition  */
  YYSYMBOL_48_1 = 48,                      /* $@1  */
  YYSYMBOL_49_2 = 49,                      /* $@2  */
  YYSYMBOL_parameter_list = 50,            /* parameter_list  */
  YYSYMBOL_compound_statement = 51,        /* compound_statement  */
  YYSYMBOL_var_declaration = 52,           /* var_declaration  */
  YYSYMBOL_type_specifier = 53,            /* type_specifier  */
  YYSYMBOL_declaration_list = 54,          /* declaration_list  */
  YYSYMBOL_statements = 55,                /* statements  */
  YYSYMBOL_statement = 56,                 /* statement  */
  YYSYMBOL_expression_statement = 57,      /* expression_statement  */
  YYSYMBOL_variable = 58,                  /* variable  */
  YYSYMBOL_expression = 59,                /* expression  */
  YYSYMBOL_logic_expression = 60,          /* logic_expression  */
  YYSYMBOL_rel_expression = 61,            /* rel_expression  */
  YYSYMBOL_simple_expression = 62,         /* simple_expression  */
  YYSYMBOL_term = 63,                      /* term  */
  YYSYMBOL_unary_expression = 64,          /* unary_expression  */
  YYSYMBOL_factor = 65,                    /* factor  */
  YYSYMBOL_argument_list = 66,             /* argument_list  */
  YYSYMBOL_arguments = 67,                 /* arguments  */
  YYSYMBOL__lcurl = 68                     /* _lcurl  */
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
typedef yytype_int8 yy_state_t;

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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   171

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  121

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   296


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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   125,   125,   142,   150,   159,   167,   175,   184,   210,
     234,   234,   302,   302,   354,   374,   390,   405,   421,   434,
     448,   484,   491,   498,   507,   522,   542,   552,   570,   577,
     587,   594,   601,   608,   622,   633,   646,   657,   673,   684,
     692,   702,   720,   749,   758,   782,   791,   804,   813,   826,
     835,   848,   857,   891,   901,   911,   922,   931,   993,  1004,
    1013,  1022,  1031,  1043,  1053,  1061,  1074,  1085
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
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "FOR",
  "DO", "WHILE", "SWITCH", "CASE", "DEFAULT", "INT", "FLOAT", "DOUBLE",
  "CHAR", "VOID", "BREAK", "CONTINUE", "RETURN", "PRINTLN", "LPAREN",
  "RPAREN", "LCURL", "RCURL", "LSQUARE", "RSQUARE", "COMMA", "SEMICOLON",
  "NOT", "ADDOP", "MULOP", "INCOP", "DECOP", "RELOP", "ASSIGNOP",
  "LOGICOP", "BITOP", "CONST_INT", "CONST_FLOAT", "CONST_CHAR", "ID",
  "LOWER_THAN_ELSE", "$accept", "start", "program", "unit",
  "func_declaration", "func_definition", "$@1", "$@2", "parameter_list",
  "compound_statement", "var_declaration", "type_specifier",
  "declaration_list", "statements", "statement", "expression_statement",
  "variable", "expression", "logic_expression", "rel_expression",
  "simple_expression", "term", "unary_expression", "factor",
  "argument_list", "arguments", "_lcurl", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-69)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      40,   -69,   -69,   -69,     1,    40,   -69,   -69,   -69,   -69,
     -14,   -69,   -69,    16,    -8,     0,    -9,    -7,   -69,     4,
      -4,    14,    51,    56,   -69,    61,    57,    40,   -69,   -69,
      48,   -69,   -69,    59,   -69,    61,    49,    65,    71,    72,
      73,    10,    74,    10,   -69,   -69,    10,    10,   -69,   -69,
      33,   -69,   -69,    55,    95,   -69,   -69,    41,    76,   -69,
      66,    30,    75,   -69,   -69,   -69,   -69,   -69,    10,   127,
      10,    77,    68,    88,    29,   -69,   -69,    10,    10,    87,
     -69,   -69,   -69,   -69,    10,   -69,    10,    10,    10,    10,
      91,   127,    98,   -69,    99,   -69,   -69,   100,    90,   101,
     -69,   -69,    75,    96,   -69,   131,    10,   131,   102,   -69,
      10,   -69,   123,   107,   -69,   -69,   -69,   131,   131,   -69,
     -69
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    21,    22,    23,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    26,     0,     0,     0,     0,    20,    12,
       0,    17,     0,    24,     9,     0,    10,     0,    16,    27,
       0,    67,    13,     0,     8,     0,    15,     0,     0,     0,
       0,     0,     0,     0,    19,    39,     0,     0,    59,    60,
      41,    32,    30,     0,     0,    28,    31,    56,     0,    43,
      45,    47,    49,    51,    55,    11,    14,    25,     0,     0,
       0,     0,     0,     0,    56,    54,    53,    64,     0,    26,
      18,    29,    61,    62,     0,    40,     0,     0,     0,     0,
       0,     0,     0,    38,     0,    58,    66,     0,    63,     0,
      44,    46,    50,    48,    52,     0,     0,     0,     0,    57,
       0,    42,    34,     0,    36,    37,    65,     0,     0,    35,
      33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -69,   -69,   -69,   125,   -69,   -69,   -69,   -69,   -69,   -11,
      20,     8,   -69,   -69,   -49,   -59,   -43,   -41,   -68,    45,
      52,    50,   -40,   -69,   -69,   -69,   -69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,    35,    25,    20,    51,
      52,    53,    14,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    97,    98,    33
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      71,    11,    73,    74,    74,    81,    75,    76,    10,    96,
      91,     1,     2,    10,    32,     3,   100,    26,    17,    18,
       9,    19,    27,    21,    65,     9,    13,    90,    22,    92,
      43,    24,   106,    23,    74,    36,    15,    99,    46,    47,
      16,    74,   116,    74,    74,    74,    74,    48,    49,   104,
      50,     1,     2,    77,    28,     3,   112,    78,   114,    87,
      82,    83,    38,    88,    39,   113,    40,    74,   119,   120,
       1,     2,    82,    83,     3,    84,    29,    41,    42,    43,
      30,    31,    44,    31,    34,    37,    45,    46,    47,    66,
      67,    68,    69,    70,    72,    79,    48,    49,    38,    50,
      39,    86,    40,    85,    93,    89,     1,     2,    94,    95,
       3,    16,   105,    41,    42,    43,   110,    31,    80,   107,
     108,   109,    45,    46,    47,    87,   111,   117,   118,   115,
      12,   101,    48,    49,    38,    50,    39,   102,    40,     0,
     103,     0,     1,     2,     0,     0,     3,    43,     0,    41,
      42,    43,     0,    31,    45,    46,    47,     0,    45,    46,
      47,     0,     0,     0,    48,    49,     0,    50,    48,    49,
       0,    50
};

static const yytype_int8 yycheck[] =
{
      41,     0,    43,    46,    47,    54,    46,    47,     0,    77,
      69,    11,    12,     5,    25,    15,    84,    21,    26,    27,
       0,    21,    26,    15,    35,     5,    40,    68,    37,    70,
      20,    27,    91,    40,    77,    27,    20,    78,    28,    29,
      24,    84,   110,    86,    87,    88,    89,    37,    38,    89,
      40,    11,    12,    20,    40,    15,   105,    24,   107,    29,
      31,    32,     3,    33,     5,   106,     7,   110,   117,   118,
      11,    12,    31,    32,    15,    34,    25,    18,    19,    20,
      24,    22,    23,    22,    27,    37,    27,    28,    29,    40,
      25,    20,    20,    20,    20,    40,    37,    38,     3,    40,
       5,    35,     7,    27,    27,    30,    11,    12,    40,    21,
      15,    24,    21,    18,    19,    20,    26,    22,    23,    21,
      21,    21,    27,    28,    29,    29,    25,     4,    21,    27,
       5,    86,    37,    38,     3,    40,     5,    87,     7,    -1,
      88,    -1,    11,    12,    -1,    -1,    15,    20,    -1,    18,
      19,    20,    -1,    22,    27,    28,    29,    -1,    27,    28,
      29,    -1,    -1,    -1,    37,    38,    -1,    40,    37,    38,
      -1,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    11,    12,    15,    43,    44,    45,    46,    47,    52,
      53,     0,    45,    40,    54,    20,    24,    26,    27,    21,
      50,    53,    37,    40,    27,    49,    21,    26,    40,    25,
      24,    22,    51,    68,    27,    48,    53,    37,     3,     5,
       7,    18,    19,    20,    23,    27,    28,    29,    37,    38,
      40,    51,    52,    53,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    51,    40,    25,    20,    20,
      20,    59,    20,    59,    58,    64,    64,    20,    24,    40,
      23,    56,    31,    32,    34,    27,    35,    29,    33,    30,
      59,    57,    59,    27,    40,    21,    60,    66,    67,    59,
      60,    61,    63,    62,    64,    21,    57,    21,    21,    21,
      26,    25,    56,    59,    56,    27,    60,     4,    21,    56,
      56
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    45,    46,    46,
      48,    47,    49,    47,    50,    50,    50,    50,    51,    51,
      52,    53,    53,    53,    54,    54,    54,    54,    55,    55,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    57,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    64,    65,    65,    65,    65,
      65,    65,    65,    66,    66,    67,    67,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     7,     0,     6,     4,     3,     2,     1,     3,     2,
       3,     1,     1,     1,     3,     6,     1,     4,     1,     2,
       1,     1,     1,     7,     5,     7,     5,     5,     3,     1,
       2,     1,     4,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     2,     2,     1,     1,     4,     3,     1,
       1,     2,     2,     1,     0,     3,     1,     1
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
  case 2: /* start: program  */
#line 126 "1905113.y"
        {
		(yyval.symbolInfo) = new SymbolInfo("program", "start");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		cout<<"Total Lines: "<<line_count<<endl;
		cout<<"Total Errors: "<<error_count<<endl;

		genTree((yyval.symbolInfo),0);
		deleteInfo((yyval.symbolInfo));
		delete symbolTable;
	}
#line 1469 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 142 "1905113.y"
                       {
		(yyval.symbolInfo) = new SymbolInfo("program unit", "program");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 1482 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 150 "1905113.y"
              {
		(yyval.symbolInfo) = new SymbolInfo("unit", "program");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 1494 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 159 "1905113.y"
                      {
		(yyval.symbolInfo) = new SymbolInfo("var_declaration", "unit");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

	 }
#line 1507 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 167 "1905113.y"
                       {
		(yyval.symbolInfo) = new SymbolInfo("func_declaration", "unit");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

	 }
#line 1520 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 175 "1905113.y"
                      {
		(yyval.symbolInfo) = new SymbolInfo("func_definition", "unit");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	 }
#line 1532 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 184 "1905113.y"
                                                                           {
		string name = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
		(yyval.symbolInfo) = new SymbolInfo(name, "func_declaration");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-5].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-5].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));


		bool inserted = symbolTable->Insert((yyvsp[-4].symbolInfo)->getName(),"FUNCTION",(yyvsp[-5].symbolInfo)->getName());
		if(!inserted){
			errFile<<"Line# "<<(yyval.symbolInfo)->getSl()<<": Redeclaration of function '"<<(yyvsp[-4].symbolInfo)->getName()<<"'\n";
			error_count++;
		}
		else{
			SymbolInfo* info = symbolTable->LookUp((yyvsp[-4].symbolInfo)->getName());
			info->setInfoType(3);
			
			info->setParam((yyvsp[-2].symbolInfo)->getParam());
		}
	 }
#line 1563 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 210 "1905113.y"
                                                   {
		string name = "type_specifier ID LPAREN RPAREN SEMICOLON";
		(yyval.symbolInfo) = new SymbolInfo(name, "func_declaration");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-4].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		bool inserted = symbolTable->Insert((yyvsp[-3].symbolInfo)->getName(),"FUNCTION",(yyvsp[-4].symbolInfo)->getName());
		if(!inserted){
			errFile<<"Line# "<<(yyval.symbolInfo)->getSl()<<": Redeclaration of function '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
			error_count++;
		}
		else{
			SymbolInfo* info = symbolTable->LookUp((yyvsp[-3].symbolInfo)->getName());
			info->setInfoType(3);
		}
	}
#line 1590 "y.tab.c"
    break;

  case 10: /* $@1: %empty  */
#line 234 "1905113.y"
                                                                 {

	SymbolInfo* info = symbolTable->LookUp((yyvsp[-3].symbolInfo)->getName());

		if(info!=NULL)
		{
			if(info->getInfoType()==4)
			{
				errFile<<"Line# "<<(yyvsp[-4].symbolInfo)->getSl()<<": Redefinition of function '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
				error_count++;
			}
			else if(info->getInfoType()==3)
			{
				if(info->getDataType()!= (yyvsp[-4].symbolInfo)->getName()){
					errFile<<"Line# "<<(yyvsp[-4].symbolInfo)->getSl()<<": Conflicting types for '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
					error_count++;
				}
				else if(info->getParamSize()!= (yyvsp[-1].symbolInfo)->getParamSize()){
					errFile<<"Line# "<<(yyvsp[-4].symbolInfo)->getSl()<<": Conflicting types for '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
					error_count++;
				}
				else if(info->getParamSize()>0)
				{
					vector<SymbolInfo*> paramList1 = info->getParam();
					vector<SymbolInfo*> paramList2 = (yyvsp[-1].symbolInfo)->getParam();

					for(int i=0; i<info->getParamSize(); i++)
					{
						if(paramList1[i]->getDataType() != paramList2[i]->getDataType())
						{
							errFile<<"Line# "<<(yyvsp[-4].symbolInfo)->getSl()<<": conflicting types for '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
							error_count++;
							break;
						}
					}
				}
				
			}
			else{
				errFile<<"Line# "<<(yyvsp[-4].symbolInfo)->getSl()<<": '"<<(yyvsp[-3].symbolInfo)->getName()<<"' redeclared as different kind of symbol\n";
				error_count++;
			}

			info->setInfoType(4);
		}
		else
		{
			symbolTable->Insert((yyvsp[-3].symbolInfo)->getName(),"FUNCTION",(yyvsp[-4].symbolInfo)->getName());
			SymbolInfo* info = symbolTable->LookUp((yyvsp[-3].symbolInfo)->getName());
			info->setInfoType(4);
			info->setParam((yyvsp[-1].symbolInfo)->getParam());
		}
}
#line 1648 "y.tab.c"
    break;

  case 11: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN $@1 compound_statement  */
#line 287 "1905113.y"
                  {
		string name = "type_specifier ID LPAREN parameter_list RPAREN compound_statement";
		(yyval.symbolInfo) = new SymbolInfo(name, "func_definition");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-6].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-6].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-5].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		
	}
#line 1668 "y.tab.c"
    break;

  case 12: /* $@2: %empty  */
#line 302 "1905113.y"
                                          {

		SymbolInfo* info = symbolTable->LookUp((yyvsp[-2].symbolInfo)->getName());
		
		if(info!=NULL)
		{
			if(info->getInfoType()==4)
			{
				errFile<<"Line# "<<(yyvsp[-3].symbolInfo)->getSl()<<": Redefinition of function '"<<(yyvsp[-2].symbolInfo)->getName()<<"'\n";
				error_count++;
			}
			else if(info->getInfoType()==3)
			{
				if(info->getDataType()!= (yyvsp[-3].symbolInfo)->getName()){
					errFile<<"Line# "<<(yyvsp[-3].symbolInfo)->getSl()<<": Conflicting types for '"<<(yyvsp[-2].symbolInfo)->getName()<<"'\n";
					error_count++;
				}
				else if(info->getParamSize()!= 0){
					errFile<<"Line# "<<(yyvsp[-3].symbolInfo)->getSl()<<": Conflicting types for '"<<(yyvsp[-2].symbolInfo)->getName()<<"'\n";
					error_count++;
				}
				
			}
			else{
				errFile<<"Line# "<<(yyvsp[-3].symbolInfo)->getSl()<<": '"<<(yyvsp[-2].symbolInfo)->getName()<<"' redeclared as different kind of symbol\n";
				error_count++;
			}

			info->setInfoType(4);
		}
		else
		{
			symbolTable->Insert((yyvsp[-2].symbolInfo)->getName(),"FUNCTION",(yyvsp[-3].symbolInfo)->getName());
			SymbolInfo* info = symbolTable->LookUp((yyvsp[-2].symbolInfo)->getName());
			info->setInfoType(4);
		}
	}
#line 1710 "y.tab.c"
    break;

  case 13: /* func_definition: type_specifier ID LPAREN RPAREN $@2 compound_statement  */
#line 339 "1905113.y"
                          {
		string name = "type_specifier ID LPAREN RPAREN compound_statement";
		(yyval.symbolInfo) = new SymbolInfo(name, "func_definition");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-5].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-5].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 1727 "y.tab.c"
    break;

  case 14: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 354 "1905113.y"
                                                        {
		string name = "parameter_list COMMA type_specifier ID";
		(yyval.symbolInfo) = new SymbolInfo(name, "parameter_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-3].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setParam((yyvsp[-3].symbolInfo)->getParam());

		(yyvsp[0].symbolInfo)->setDataType((yyvsp[-1].symbolInfo)->getName());
		(yyvsp[0].symbolInfo)->setInfoType(1);

		(yyval.symbolInfo)->addParam((yyvsp[0].symbolInfo));

		paramList = (yyval.symbolInfo)->getParam();
	}
#line 1752 "y.tab.c"
    break;

  case 15: /* parameter_list: parameter_list COMMA type_specifier  */
#line 374 "1905113.y"
                                             {  
		string name = "parameter_list COMMA type_specifier";
		(yyval.symbolInfo) = new SymbolInfo(name, "parameter_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setParam((yyvsp[-2].symbolInfo)->getParam());

		(yyval.symbolInfo)->addParam(new SymbolInfo("","ID",(yyvsp[0].symbolInfo)->getName()));

		paramList = (yyval.symbolInfo)->getParam();
	}
#line 1773 "y.tab.c"
    break;

  case 16: /* parameter_list: type_specifier ID  */
#line 390 "1905113.y"
                           {
		string name = "type_specifier ID";
		(yyval.symbolInfo) = new SymbolInfo(name, "parameter_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyvsp[0].symbolInfo)->setDataType((yyvsp[-1].symbolInfo)->getName());
		(yyvsp[0].symbolInfo)->setInfoType(1);
		(yyval.symbolInfo)->addParam((yyvsp[0].symbolInfo));

		paramList = (yyval.symbolInfo)->getParam();
	}
#line 1793 "y.tab.c"
    break;

  case 17: /* parameter_list: type_specifier  */
#line 405 "1905113.y"
                        { 
		string name = "type_specifier";
		(yyval.symbolInfo) = new SymbolInfo(name, "parameter_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));


		(yyval.symbolInfo)->addParam(new SymbolInfo("","ID",(yyvsp[0].symbolInfo)->getName()));

		paramList = (yyval.symbolInfo)->getParam();
	}
#line 1811 "y.tab.c"
    break;

  case 18: /* compound_statement: _lcurl statements RCURL  */
#line 421 "1905113.y"
                                            {
		string name = "LCURL statements RCURL";
		(yyval.symbolInfo) = new SymbolInfo(name, "compound_statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		symbolTable->printAll();
		symbolTable->exitScope();
	}
#line 1829 "y.tab.c"
    break;

  case 19: /* compound_statement: _lcurl RCURL  */
#line 434 "1905113.y"
                      {
		string name = "LCURL RCURL";
		(yyval.symbolInfo) = new SymbolInfo(name, "compound_statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		symbolTable->printAll();
		symbolTable->exitScope();
	}
#line 1846 "y.tab.c"
    break;

  case 20: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 448 "1905113.y"
                                                           {
		string name = "type_specifier declaration_list SEMICOLON";
		(yyval.symbolInfo) = new SymbolInfo(name, "var_declaration");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		for(SymbolInfo* info : (yyvsp[-1].symbolInfo)->getParam())
		{
			if((yyvsp[-2].symbolInfo)->getName() == "VOID"){
				errFile<<"Line# "<<line_count<<": Variable or field '"<<info->getName()<<"' declared void\n";
				error_count++;
				break;
			}
			bool success = symbolTable->Insert(info->getName(), info->getType());
			if(success)
			{
				SymbolInfo* newVar = symbolTable->LookUp(info->getName());
				newVar->setDataType((yyvsp[-2].symbolInfo)->getName());
				newVar->setInfoType(info->getInfoType());

				if(info->getInfoType()==2)
					newVar->setArraySize(info->getArraySize());
			}
			else
			{
				errFile<<"Line# "<<line_count<<": Conflicting types for'"<<info->getName()<<"'\n";
				error_count++;
			}
		}
	}
#line 1885 "y.tab.c"
    break;

  case 21: /* type_specifier: INT  */
#line 484 "1905113.y"
                     {
		(yyval.symbolInfo) = new SymbolInfo("INT", "type_specifier");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 1897 "y.tab.c"
    break;

  case 22: /* type_specifier: FLOAT  */
#line 491 "1905113.y"
               {
		(yyval.symbolInfo) = new SymbolInfo("FLOAT", "type_specifier");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 1909 "y.tab.c"
    break;

  case 23: /* type_specifier: VOID  */
#line 498 "1905113.y"
              {
		(yyval.symbolInfo) = new SymbolInfo("VOID", "type_specifier");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 1921 "y.tab.c"
    break;

  case 24: /* declaration_list: declaration_list COMMA ID  */
#line 507 "1905113.y"
                                            {
		string name = "declaration_list COMMA ID";
		(yyval.symbolInfo) = new SymbolInfo(name, "declaration_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setParam((yyvsp[-2].symbolInfo)->getParam());

		(yyvsp[0].symbolInfo)->setInfoType(1);
		(yyval.symbolInfo)->addParam((yyvsp[0].symbolInfo));
	}
#line 1941 "y.tab.c"
    break;

  case 25: /* declaration_list: declaration_list COMMA ID LSQUARE CONST_INT RSQUARE  */
#line 522 "1905113.y"
                                                             {
		string name = "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE";
		(yyval.symbolInfo) = new SymbolInfo(name, "declaration_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-5].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-5].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setParam((yyvsp[-5].symbolInfo)->getParam());

		(yyvsp[-3].symbolInfo)->setInfoType(2);
		(yyvsp[-3].symbolInfo)->setType("ARRAY");
		(yyvsp[-3].symbolInfo)->setArraySize((yyvsp[-1].symbolInfo)->getName());
		(yyval.symbolInfo)->addParam((yyvsp[-3].symbolInfo));
	}
#line 1966 "y.tab.c"
    break;

  case 26: /* declaration_list: ID  */
#line 542 "1905113.y"
            {
		(yyval.symbolInfo) = new SymbolInfo("ID", "declaration_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyvsp[0].symbolInfo)->setInfoType(1);
		(yyval.symbolInfo)->addParam((yyvsp[0].symbolInfo));
	}
#line 1981 "y.tab.c"
    break;

  case 27: /* declaration_list: ID LSQUARE CONST_INT RSQUARE  */
#line 552 "1905113.y"
                                      {
		string name = "ID LSQUARE CONST_INT RSQUARE";
		(yyval.symbolInfo) = new SymbolInfo(name, "declaration_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-3].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyvsp[-3].symbolInfo)->setInfoType(2);
		(yyvsp[-3].symbolInfo)->setType("ARRAY");
		(yyvsp[-3].symbolInfo)->setArraySize((yyvsp[-1].symbolInfo)->getName());
		(yyval.symbolInfo)->addParam((yyvsp[-3].symbolInfo));
	}
#line 2002 "y.tab.c"
    break;

  case 28: /* statements: statement  */
#line 570 "1905113.y"
                      {
		(yyval.symbolInfo) = new SymbolInfo("statement", "statements");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2014 "y.tab.c"
    break;

  case 29: /* statements: statements statement  */
#line 577 "1905113.y"
                              {
		(yyval.symbolInfo) = new SymbolInfo("statements statement", "statements");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2027 "y.tab.c"
    break;

  case 30: /* statement: var_declaration  */
#line 587 "1905113.y"
                           {
		(yyval.symbolInfo) = new SymbolInfo("var_declaration", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2039 "y.tab.c"
    break;

  case 31: /* statement: expression_statement  */
#line 594 "1905113.y"
                                {
		(yyval.symbolInfo) = new SymbolInfo("expression_statement", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2051 "y.tab.c"
    break;

  case 32: /* statement: compound_statement  */
#line 601 "1905113.y"
                              {
		(yyval.symbolInfo) = new SymbolInfo("compound_statement", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2063 "y.tab.c"
    break;

  case 33: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 608 "1905113.y"
                                                                                            {
		(yyval.symbolInfo) = new SymbolInfo("FOR LPAREN expression_statement expression_statement expression RPAREN statement", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-6].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-6].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-5].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

	}
#line 2082 "y.tab.c"
    break;

  case 34: /* statement: IF LPAREN expression RPAREN statement  */
#line 622 "1905113.y"
                                                                       {
		(yyval.symbolInfo) = new SymbolInfo("IF LPAREN expression RPAREN statement", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-4].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2098 "y.tab.c"
    break;

  case 35: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 633 "1905113.y"
                                                                {
		(yyval.symbolInfo) = new SymbolInfo("IF LPAREN expression RPAREN statement ELSE statement", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-6].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-6].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-5].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2116 "y.tab.c"
    break;

  case 36: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 646 "1905113.y"
                                                    {
		(yyval.symbolInfo) = new SymbolInfo("WHILE LPAREN expression RPAREN statement", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-4].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2132 "y.tab.c"
    break;

  case 37: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 657 "1905113.y"
                                              {
		(yyval.symbolInfo) = new SymbolInfo("PRINTLN LPAREN ID RPAREN SEMICOLON", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-4].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-4].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		if(!symbolTable->LookUp((yyvsp[-2].symbolInfo)->getName())){
			errFile<<"Line# "<<line_count<<" : Undeclared variable '"<<(yyvsp[-2].symbolInfo)->getName()<<"'\n";
			error_count++;
		}
	}
#line 2153 "y.tab.c"
    break;

  case 38: /* statement: RETURN expression SEMICOLON  */
#line 673 "1905113.y"
                                       {
		(yyval.symbolInfo) = new SymbolInfo("RETURN expression SEMICOLON", "statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2167 "y.tab.c"
    break;

  case 39: /* expression_statement: SEMICOLON  */
#line 684 "1905113.y"
                                   {
		(yyval.symbolInfo) = new SymbolInfo("SEMICOLON", "expression_statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2179 "y.tab.c"
    break;

  case 40: /* expression_statement: expression SEMICOLON  */
#line 692 "1905113.y"
                              {
		(yyval.symbolInfo) = new SymbolInfo("expression SEMICOLON", "expression_statement");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
	}
#line 2192 "y.tab.c"
    break;

  case 41: /* variable: ID  */
#line 702 "1905113.y"
             {
		(yyval.symbolInfo) = new SymbolInfo("ID", "variable");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		SymbolInfo *info = symbolTable->LookUp((yyvsp[0].symbolInfo)->getName());
		if(info!=NULL){
			if(info->getInfoType() ==2){
				//errFile<<"Line# "<<line_count<<" :Type mismatch'"<<info->getName()<<" is an array\n";
			}
			(yyval.symbolInfo)->setDataType(info->getDataType());
		}else{
			errFile<<"Line# "<<line_count<<": Undeclared variable '"<<(yyvsp[0].symbolInfo)->getName()<<"'\n";
			error_count++;
		}
	}
#line 2215 "y.tab.c"
    break;

  case 42: /* variable: ID LSQUARE expression RSQUARE  */
#line 720 "1905113.y"
                                       {
		(yyval.symbolInfo) = new SymbolInfo("ID LSQUARE expression RSQUARE", "variable");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-3].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		SymbolInfo *info = symbolTable->LookUp((yyvsp[-3].symbolInfo)->getName());
		if(info!=NULL){
			if(info->getInfoType()!=2){
				errFile<<"Line# "<<line_count<<": '"<<info->getName()<<"' is not an array\n";
				error_count++;
			}
			else if((yyvsp[-1].symbolInfo)->getDataType()!="INT"){
				errFile<<"Line# "<<line_count<<": "<<"Array subscript is not an integer\n";
				error_count++;
			}
			(yyval.symbolInfo)->setDataType(info->getDataType());
			
		}else{
			errFile<<"Line# "<<line_count<<": Undeclared variable '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
			error_count++;
		}
	}
#line 2247 "y.tab.c"
    break;

  case 43: /* expression: logic_expression  */
#line 749 "1905113.y"
                              {
		(yyval.symbolInfo) = new SymbolInfo("logic_expression", "expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2261 "y.tab.c"
    break;

  case 44: /* expression: variable ASSIGNOP logic_expression  */
#line 758 "1905113.y"
                                            {
		(yyval.symbolInfo) = new SymbolInfo("variable ASSIGNOP logic_expression", "expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));



		if((yyvsp[-2].symbolInfo)->getDataType()== "VOID" || (yyvsp[0].symbolInfo)->getDataType()== "VOID")
		{
			errFile<<"Line# "<<line_count<<": Void cannot be used in expression\n";
			error_count++;
		}
		else if((yyvsp[-2].symbolInfo)->getDataType()== "INT" && (yyvsp[0].symbolInfo)->getDataType()=="FLOAT"){
			errFile<<"Line# "<<line_count<<": Warning: possible loss of data in assignment of FLOAT to INT\n";
			error_count++;
		}
		(yyval.symbolInfo)->setDataType((yyvsp[-2].symbolInfo)->getDataType());
	}
#line 2288 "y.tab.c"
    break;

  case 45: /* logic_expression: rel_expression  */
#line 782 "1905113.y"
                                 {
		(yyval.symbolInfo) = new SymbolInfo("rel_expression", "logic_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2302 "y.tab.c"
    break;

  case 46: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 791 "1905113.y"
                                               {
		(yyval.symbolInfo) = new SymbolInfo("rel_expression LOGICOP rel_expression", "logic_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType("INT");
	}
#line 2318 "y.tab.c"
    break;

  case 47: /* rel_expression: simple_expression  */
#line 804 "1905113.y"
                                   {
		(yyval.symbolInfo) = new SymbolInfo("simple_expression", "rel_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2332 "y.tab.c"
    break;

  case 48: /* rel_expression: simple_expression RELOP simple_expression  */
#line 813 "1905113.y"
                                                   {
		(yyval.symbolInfo) = new SymbolInfo("simple_expression RELOP simple_expression", "rel_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType("INT");
	}
#line 2348 "y.tab.c"
    break;

  case 49: /* simple_expression: term  */
#line 826 "1905113.y"
                        {
		(yyval.symbolInfo) = new SymbolInfo("term", "simple_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2362 "y.tab.c"
    break;

  case 50: /* simple_expression: simple_expression ADDOP term  */
#line 835 "1905113.y"
                                      {
		(yyval.symbolInfo) = new SymbolInfo("simple_expression ADDOP term", "simple_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType(typeCasting((yyvsp[-2].symbolInfo)->getDataType(),(yyvsp[0].symbolInfo)->getDataType()));
	}
#line 2378 "y.tab.c"
    break;

  case 51: /* term: unary_expression  */
#line 848 "1905113.y"
                        {
		(yyval.symbolInfo) = new SymbolInfo("unary_expression", "term");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2392 "y.tab.c"
    break;

  case 52: /* term: term MULOP unary_expression  */
#line 857 "1905113.y"
                                      {
		(yyval.symbolInfo) = new SymbolInfo("term MULOP unary_expression", "term");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		if((yyvsp[-1].symbolInfo)->getName()=="%")
		{
			(yyval.symbolInfo)->setDataType("INT");
			if(!((yyvsp[-2].symbolInfo)->getDataType()=="INT" && (yyvsp[0].symbolInfo)->getDataType()=="INT")){
				errFile<<"Line# "<<line_count<<": Operands of modulus must be integers\n";
				error_count++;
			}
			if(findLeafVal((yyvsp[0].symbolInfo))=="0"){
				errFile<<"Line# "<<line_count<<": Warning: division by zero\n";
				error_count++;
			}
		}
		else if((yyvsp[-1].symbolInfo)->getName()=="/")
		{
			if(findLeafVal((yyvsp[0].symbolInfo))=="0"){
				errFile<<"Line# "<<line_count<<": Warning: division by zero\n";
				error_count++;
			}
			(yyval.symbolInfo)->setDataType(typeCasting((yyvsp[-2].symbolInfo)->getDataType(),(yyvsp[0].symbolInfo)->getDataType()));
		}
		else
			(yyval.symbolInfo)->setDataType(typeCasting((yyvsp[-2].symbolInfo)->getDataType(),(yyvsp[0].symbolInfo)->getDataType()));
	}
#line 2429 "y.tab.c"
    break;

  case 53: /* unary_expression: ADDOP unary_expression  */
#line 891 "1905113.y"
                                         {
		(yyval.symbolInfo) = new SymbolInfo("ADDOP unary_expression", "unary_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2444 "y.tab.c"
    break;

  case 54: /* unary_expression: NOT unary_expression  */
#line 901 "1905113.y"
                              {
		(yyval.symbolInfo) = new SymbolInfo("NOT unary_expression", "unary_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType("INT");
	}
#line 2459 "y.tab.c"
    break;

  case 55: /* unary_expression: factor  */
#line 911 "1905113.y"
                {
		(yyval.symbolInfo) = new SymbolInfo("factor", "unary_expression");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2473 "y.tab.c"
    break;

  case 56: /* factor: variable  */
#line 922 "1905113.y"
                  {
		(yyval.symbolInfo) = new SymbolInfo("variable", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[0].symbolInfo)->getDataType());
	}
#line 2487 "y.tab.c"
    break;

  case 57: /* factor: ID LPAREN argument_list RPAREN  */
#line 931 "1905113.y"
                                        {
		(yyval.symbolInfo) = new SymbolInfo("ID LPAREN argument_list RPAREN", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-3].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-3].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
		SymbolInfo* info = symbolTable->LookUp((yyvsp[-3].symbolInfo)->getName());
		
		if(info==NULL)
		{
			errFile<<"Line# "<<line_count<<": "<<"Undeclared function '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
			error_count++;
		}
		else if(info->getInfoType() != 4)
		{
			if(info->getInfoType() == 3){
				//errFile<<"Line# "<<line_count<<": "<<"Function '"<<$1->getName()<<"' not defined\n";
				//error_count++;
			}
			else{
				errFile<<"Line# "<<line_count<<": '"<<(yyvsp[-3].symbolInfo)->getName()<<"' is not a function\n";
				error_count++;
			}

			(yyval.symbolInfo)->setDataType(info->getDataType());	
		}
		else
		{
			if(info->getParamSize() != (yyvsp[-1].symbolInfo)->getParamSize())
			{
				if((yyvsp[-1].symbolInfo)->getParamSize() < info->getParamSize()){
					errFile<<"Line# "<<line_count<<": Too few arguments to function '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
					error_count++;
				}
				else{
					errFile<<"Line# "<<line_count<<": Too many arguments to function '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
					error_count++;
				}

				(yyval.symbolInfo)->setDataType(info->getDataType());	
			}
			else
			{
				vector<SymbolInfo*> paramList1 = info->getParam();
				vector<SymbolInfo*> paramList2 = (yyvsp[-1].symbolInfo)->getParam();
				for(int i=0; i<(yyvsp[-1].symbolInfo)->getParamSize(); i++)
				{
					if(paramList1[i]->getDataType() != paramList2[i]->getDataType())
					{
						errFile<<"Line# "<<line_count<<": Type mismatch for argument "<<i+1<<" of '"<<(yyvsp[-3].symbolInfo)->getName()<<"'\n";
						error_count++;
					}
				}
				(yyval.symbolInfo)->setDataType(info->getDataType());	
			}
		}
		//*/
		
	}
#line 2554 "y.tab.c"
    break;

  case 58: /* factor: LPAREN expression RPAREN  */
#line 993 "1905113.y"
                                  {
		(yyval.symbolInfo) = new SymbolInfo("LPAREN expression RPAREN", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[-1].symbolInfo)->getDataType());
	}
#line 2570 "y.tab.c"
    break;

  case 59: /* factor: CONST_INT  */
#line 1004 "1905113.y"
                   {
		(yyval.symbolInfo) = new SymbolInfo("CONST_INT", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType("INT");
	}
#line 2584 "y.tab.c"
    break;

  case 60: /* factor: CONST_FLOAT  */
#line 1013 "1905113.y"
                     {
		(yyval.symbolInfo) = new SymbolInfo("CONST_FLOAT", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType("FLOAT");
	}
#line 2598 "y.tab.c"
    break;

  case 61: /* factor: variable INCOP  */
#line 1022 "1905113.y"
                        {
		(yyval.symbolInfo) = new SymbolInfo("variable INCOP", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));
		(yyval.symbolInfo)->setDataType((yyvsp[-1].symbolInfo)->getDataType());
	}
#line 2612 "y.tab.c"
    break;

  case 62: /* factor: variable DECOP  */
#line 1031 "1905113.y"
                        {
		(yyval.symbolInfo) = new SymbolInfo("variable DECOP", "factor");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-1].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setDataType((yyvsp[-1].symbolInfo)->getDataType());
	}
#line 2627 "y.tab.c"
    break;

  case 63: /* argument_list: arguments  */
#line 1043 "1905113.y"
                         {
		(yyval.symbolInfo) = new SymbolInfo("arguments", "argument_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->setParam((yyvsp[0].symbolInfo)->getParam());
	}
#line 2641 "y.tab.c"
    break;

  case 64: /* argument_list: %empty  */
#line 1053 "1905113.y"
        {
		(yyval.symbolInfo) = new SymbolInfo("", "argument_list");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl(line_count);
		(yyval.symbolInfo)->setEl(line_count);
	}
#line 2652 "y.tab.c"
    break;

  case 65: /* arguments: arguments COMMA logic_expression  */
#line 1061 "1905113.y"
                                            {
		(yyval.symbolInfo) = new SymbolInfo("arguments COMMA logic_expression", "arguments");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[-2].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[-2].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[-1].symbolInfo));
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		
		(yyval.symbolInfo)->setParam((yyvsp[-2].symbolInfo)->getParam());
		(yyval.symbolInfo)->addParam((yyvsp[0].symbolInfo));
	}
#line 2670 "y.tab.c"
    break;

  case 66: /* arguments: logic_expression  */
#line 1074 "1905113.y"
                          {
		(yyval.symbolInfo) = new SymbolInfo("logic_expression", "arguments");
		cout<<(yyval.symbolInfo)->getType()<<" : "<<(yyval.symbolInfo)->getName()<<" "<<endl;
		(yyval.symbolInfo)->setSl((yyvsp[0].symbolInfo)->getSl());
		(yyval.symbolInfo)->setEl((yyvsp[0].symbolInfo)->getEl());
		(yyval.symbolInfo)->addChild((yyvsp[0].symbolInfo));

		(yyval.symbolInfo)->addParam((yyvsp[0].symbolInfo));
	}
#line 2684 "y.tab.c"
    break;

  case 67: /* _lcurl: LCURL  */
#line 1085 "1905113.y"
              {
		symbolTable->enterScope();
		if(!paramList.empty())
			declareFuncParamList();
		(yyval.symbolInfo) = (yyvsp[0].symbolInfo);
	}
#line 2695 "y.tab.c"
    break;


#line 2699 "y.tab.c"

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

#line 1093 "1905113.y"

int main(int argc,char *argv[])
{
    FILE *fp;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	freopen("1905113_log.txt", "w", stdout);

    errFile.open("1905113_error.txt");
    treeFile.open("1905113_ parsetree.txt");
	

	yyin=fp;
	yyparse();
	

	fclose(yyin);
    errFile.close();
    treeFile.close();
	
	return 0;
}

