%{
#include "y.tab.h"
#include "stdlib.h"
%}
D	[0-9]+
X	0x[0-9A-Fa-f]+
E	[0-9]+(\.[0-9]+)?([eE]-?[0-9]+)?
%%
{D}			{ yylval.ivalue = strtol(yytext, NULL, yytext[0] == 0 ? 8 : 10); return INT; }
{X}			{ yylval.ivalue = strtol(yytext, NULL, 16); return INT; }
{E}			{ yylval.fvalue = strtod(yytext, NULL); return FLOAT; }
[-+*/\n)(]	return *yytext;
[ \t]		;
.			yyerror("Invalid character.");
%%
