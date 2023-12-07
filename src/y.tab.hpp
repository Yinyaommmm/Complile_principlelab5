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

#ifndef YY_YY_Y_TAB_HPP_INCLUDED
# define YY_YY_Y_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
    ADD = 258,                     /* ADD  */
    SUB = 259,                     /* SUB  */
    MUL = 260,                     /* MUL  */
    DIV = 261,                     /* DIV  */
    SEMICOLON = 262,               /* SEMICOLON  */
    DOT = 263,                     /* DOT  */
    LBRACE = 264,                  /* LBRACE  */
    RBRACE = 265,                  /* RBRACE  */
    LPAR = 266,                    /* LPAR  */
    RPAR = 267,                    /* RPAR  */
    LBRACKET = 268,                /* LBRACKET  */
    RBRACKET = 269,                /* RBRACKET  */
    UNUM = 270,                    /* UNUM  */
    ID = 271,                      /* ID  */
    AND = 272,                     /* AND  */
    OR = 273,                      /* OR  */
    NOT = 274,                     /* NOT  */
    GTR = 275,                     /* GTR  */
    LES = 276,                     /* LES  */
    GEQ = 277,                     /* GEQ  */
    LEQ = 278,                     /* LEQ  */
    EQL = 279,                     /* EQL  */
    NEQ = 280,                     /* NEQ  */
    INT = 281,                     /* INT  */
    LET = 282,                     /* LET  */
    COLON = 283,                   /* COLON  */
    ASSIGN = 284,                  /* ASSIGN  */
    STRUCT = 285,                  /* STRUCT  */
    COMMA = 286,                   /* COMMA  */
    RARROW = 287,                  /* RARROW  */
    IF = 288,                      /* IF  */
    ELSE = 289,                    /* ELSE  */
    RETURN = 290,                  /* RETURN  */
    WHILE = 291,                   /* WHILE  */
    CONTINUE = 292,                /* CONTINUE  */
    BREAK = 293,                   /* BREAK  */
    FN = 294                       /* FN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ADD 258
#define SUB 259
#define MUL 260
#define DIV 261
#define SEMICOLON 262
#define DOT 263
#define LBRACE 264
#define RBRACE 265
#define LPAR 266
#define RPAR 267
#define LBRACKET 268
#define RBRACKET 269
#define UNUM 270
#define ID 271
#define AND 272
#define OR 273
#define NOT 274
#define GTR 275
#define LES 276
#define GEQ 277
#define LEQ 278
#define EQL 279
#define NEQ 280
#define INT 281
#define LET 282
#define COLON 283
#define ASSIGN 284
#define STRUCT 285
#define COMMA 286
#define RARROW 287
#define IF 288
#define ELSE 289
#define RETURN 290
#define WHILE 291
#define CONTINUE 292
#define BREAK 293
#define FN 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "parser.yacc"

  A_pos pos;
  A_program program;
  A_programElementList programElementList;
  A_programElement programElement;
  A_arithExpr arithExpr;
  A_exprUnit exprUnit;
  A_structDef structDef;
  A_varDeclStmt varDeclStmt;
  A_fnDeclStmt fnDeclStmt;
  A_fnDef fnDef;
  
  // newly add
  A_tokenNum tokenNum;
  A_tokenId tokenId;
  A_fnCall fnCall;
  A_boolExpr boolExpr;
  A_boolUnit boolUnit;
  A_assignStmt assignStmt;
  A_rightVal rightVal;
  A_leftVal leftVal;
  A_rightValList rightValList;
  A_varDecl varDecl;
  A_varDeclList varDeclList;
  A_varDef varDef;
  A_varDeclScalar varDeclScalar;
  A_varDeclArray varDeclArray;
  A_varDefScalar varDefScalar;
  A_varDefArray varDefArray;
  A_type type;
  // A_nativeType nativeType;
  A_fnDecl fnDecl;
  A_paramDecl paramDecl;
  // FnDef
  A_codeBlockStmtList codeBlockStmtList;
  A_codeBlockStmt codeBlockStmt;
  A_callStmt callStmt;
  A_ifStmt ifStmt;
  A_whileStmt whileStmt;
  A_returnStmt returnStmt;

#line 187 "y.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_HPP_INCLUDED  */
