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
#line 523 "1905113.y"

	SymbolInfo* symbolInfo; 

#line 153 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
