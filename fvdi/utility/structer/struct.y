/*
 * C structure parser
 *
 * Copyright 1997-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

%{
#include <stdio.h>
#include <stdarg.h>
#include "list.h"
#include "misc.h"
#include "expr.h"

int yylex(void);

List definitions;
%}

%union
{
   int        line;
   Numeral    num;
   Name       name;
   String     string;
   List       list;
   Expression expr;
}

%token <line> '{'  '}'  '['  ']'
%token <line> ','  ';'  '*'
%token <line> TK_TYPEDEF TK_STRUCT TK_UNION
%token <line> TK_CHAR TK_INT TK_SHORT TK_LONG TK_SIGNED TK_UNSIGNED TK_CONST TK_VOID
%token <name> TK_IDENTIFIER
%token <num>  TK_NUMERAL
%token <string> TK_STRING
%token <line> TK_LEXERROR

%type <list>          program  pdeflist  deflist
%type <expr>          pdef  def  struct  union  type  id  var

%start program

%expect 2

%%

program :  pdeflist
              { definitions = $1; }
;

pdeflist :  
                 { $$ = empty(_Deflist); }
        |  pdef pdeflist
                 { $$ = prepend($1, $2); }
;

pdef :     var ';'
                 { $$ = $1; }
	|  struct ';'
                 { $$ = $1; }
	|  union ';'
                 { $$ = $1; }
        |  TK_TYPEDEF var ';'
                 { $$ = mktypedef($2); }
;

deflist :  
                 { $$ = empty(_Deflist); }
        |  def deflist
                 { $$ = prepend($1, $2); }
;

def :      var ';'
                 { $$ = $1; }
	|  union ';'
                 { $$ = mkvar(mktype(_Uniondef, 0, $1), mkid(0, -1)); }
;

struct :   TK_STRUCT TK_IDENTIFIER '{' deflist '}'
                 { $$ = mkstruct($2, $4); }
        |  TK_STRUCT '{' deflist '}'
                 { $$ = mkstruct(0, $3); }
;

union :    TK_UNION TK_IDENTIFIER '{' deflist '}'
                 { $$ = mkunion($2, $4); }
        |  TK_UNION '{' deflist '}'
                 { $$ = mkunion(0, $3); }
;

type :     maybe_signed TK_CHAR
                 { $$ = mktype(_Char, 0, 0); }
        |  maybe_signed TK_SHORT
                 { $$ = mktype(_Short, 0, 0); }
        |  maybe_signed TK_INT
                 { $$ = mktype(_Int, 0, 0); }
        |  maybe_signed TK_LONG
                 { $$ = mktype(_Long, 0, 0); }
        |  maybe_signed TK_LONG TK_INT
                 { $$ = mktype(_Long, 0, 0); }
        |  maybe_signed TK_SHORT TK_INT
                 { $$ = mktype(_Short, 0, 0); }
        |  TK_VOID
                 { $$ = mktype(_Void, 0, 0); }
        |  struct
                 { $$ = mktype(_Structdef, 0, $1); }
        |  union
                 { $$ = mktype(_Uniondef, 0, $1); }
        |  TK_STRUCT TK_IDENTIFIER
                 { $$ = mktype(_Struct, $2, 0); }
        |  TK_UNION TK_IDENTIFIER
                 { $$ = mktype(_Union, $2, 0); }
        |  TK_IDENTIFIER
                 { $$ = mktype(_Typedef, $1, 0); }
        |  TK_CONST type
                 { $$ = $2; }
        |  type '*'
                 { $$ = mktype(_Pointer, 0, $1); }
;

maybe_signed :
		| TK_SIGNED
		| TK_UNSIGNED
		;

id :       TK_IDENTIFIER '[' TK_NUMERAL ']'
                 { $$ = mkid($1, $3); }
        |  TK_IDENTIFIER '[' ']'
                 { $$ = mkid($1, 0); }
        |  TK_IDENTIFIER
                 { $$ = mkid($1, -1); }
;

var :      type id
                 { $$ = mkvar($1, $2); }
        |  type '(' '*' id ')' '(' arglist ')'
                 { $$ = mkvar(mktype(_Pointer, 0, $1), $4); }
;

arglist :  
        |  arg
        |  arg ',' arglist
;

arg :      argtype
;

arg :      argtype argid
;

argtype :     maybe_signed TK_CHAR
        |  maybe_signed TK_SHORT
        |  maybe_signed TK_INT
        |  maybe_signed TK_LONG
        |  maybe_signed TK_LONG TK_INT
        |  maybe_signed TK_SHORT TK_INT
        |  TK_VOID
        |  TK_STRUCT TK_IDENTIFIER
        |  TK_UNION TK_IDENTIFIER
        |  TK_IDENTIFIER
        |  TK_CONST argtype
        |  argtype '*'
;

argid :       TK_IDENTIFIER '[' TK_NUMERAL ']'
        |  TK_IDENTIFIER '[' ']'
        |  TK_IDENTIFIER
;

%%

void yyerror(const char *str)
{
	error(lineno, "%s", str);
}

void error(int lineno, const char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	fprintf(stderr, "Error at line %d: ", lineno);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
}
