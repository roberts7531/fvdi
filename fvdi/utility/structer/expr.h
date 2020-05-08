#ifndef _EXPR_H
#define _EXPR_H
/*
 * C structure expression declarations
 *
 * Copyright 1997-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "misc.h"
#include "list.h"

enum Exprs {
   _Idexpr, _Typedefexpr, _Structexpr, _Unionexpr, _Typeexpr, _Varexpr, _Listexpr
   } ;

struct _Expr {

   enum Exprs type;

   union _info {
      struct _Id_Expr {
         Identifier name;
         int count;
      } id;

      struct _Var_Expr {
         struct _Expr *id;
         struct _Expr *type;
      } var;

      struct _Struct_Expr {
         Identifier name;
         List defs;
      } strct;

      struct _Union_Expr {
         Identifier name;
         List defs;
      } unjon;

      struct _Type_Expr {
         Type sort;
         Identifier name;
         struct _Expr *type;
      } def;

      struct _Listexpr {
         struct _Expr *type;
      } list;
   } info;
} ;

typedef struct _Expr *Expression;

Expression mkid(Identifier, int);
Expression mktypedef(Expression);
Expression mkstruct(Identifier, List);
Expression mkunion(Identifier, List);
Expression mktype(int, Identifier, Expression);
Expression mkvar(Expression, Expression);
Expression mklist(Expression);

void printdefs(List);

#define EXPR(pointer)  VAL(Expression, pointer)

extern int lineno;

int yyparse(void);
void yyerror(const char *);
__attribute__((format(printf, 2, 3))) void error(int lineno, const char *format, ...);

void convert(char *, List);

#endif
