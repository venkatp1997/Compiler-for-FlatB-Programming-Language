/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    declaration_list = 258,
    statement_list = 259,
    NUMBER = 260,
    IDENTIFIER = 261,
    ETOK = 262,
    INT = 263,
    FOR = 264,
    IF = 265,
    ELSE = 266,
    GOTO = 267,
    WHILE = 268,
    GT = 269,
    GTE = 270,
    LT = 271,
    LTE = 272,
    DE = 273,
    NE = 274,
    STRING = 275,
    PRINT = 276,
    PRINTLN = 277,
    READ = 278
  };
#endif
/* Tokens.  */
#define declaration_list 258
#define statement_list 259
#define NUMBER 260
#define IDENTIFIER 261
#define ETOK 262
#define INT 263
#define FOR 264
#define IF 265
#define ELSE 266
#define GOTO 267
#define WHILE 268
#define GT 269
#define GTE 270
#define LT 271
#define LTE 272
#define DE 273
#define NE 274
#define STRING 275
#define PRINT 276
#define PRINTLN 277
#define READ 278

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
