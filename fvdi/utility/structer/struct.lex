/*
 * C structure lexical analyzer
 *
 * Copyright 1997-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

%{
#include <strings.h>
#include <stdlib.h>

#include "misc.h"
#include "expr.h"
#include "list.h"
#include "hash.h"
#include "struct.tab.h"

int lineno = 1;
char tmpbuf[32];   /* Maximum identifier or number must fit! */

%}

%option noyywrap

D		[0-9]
identifier	[a-zA-Z_]+[a-zA-Z0-9_]*
%%

"*"	{ yylval.line = lineno; return '*'; }
"("	{ yylval.line = lineno; return '('; }
")"	{ yylval.line = lineno; return ')'; }
","	{ yylval.line = lineno; return ','; }
"{"	{ yylval.line = lineno; return '{'; }
"}"	{ yylval.line = lineno; return '}'; }
"["	{ yylval.line = lineno; return '['; }
"]"	{ yylval.line = lineno; return ']'; }
"struct"	{ yylval.line = lineno; return TK_STRUCT; }
"union"	{ yylval.line = lineno; return TK_UNION; }
"void"	{ yylval.line = lineno; return TK_VOID; }
"char"	{ yylval.line = lineno; return TK_CHAR; }
"short"	{ yylval.line = lineno; return TK_SHORT; }
"unsigned"	{ yylval.line = lineno; return TK_UNSIGNED; }
"signed"	{ yylval.line = lineno; return TK_SIGNED; }
"int"	{ yylval.line = lineno; return TK_INT; }
"long"	{ yylval.line = lineno; return TK_LONG; }
"typedef"	{ yylval.line = lineno; return TK_TYPEDEF; }
"const"	{ yylval.line = lineno; return TK_CONST; }
";"	{ yylval.line = lineno; return ';'; }

'.'	{ yylval.num = (long)yytext[0];
	  return TK_NUMERAL;
	}
{identifier}	{ yylval.name = new_string(yytext, yyleng);
		  return TK_IDENTIFIER;
		}
{D}+ 		{ strncpy(tmpbuf, yytext, yyleng);
                  tmpbuf[yyleng] = '\0';
		  yylval.num = atol(tmpbuf);
		  return TK_NUMERAL;
               	}
"/*".*"*/"  ;
"//".*	;
"#".*	;
[ \t\r] 	;
"\n"	{lineno++; }

.	{ error(lineno, "Unallowed Character '%c'", *yytext); return TK_LEXERROR; }
%%
