%{
#include "config.h"
#include "y.tab.h"
#include "stdlib.h"

FILE* next_file;
YY_BUFFER_STATE next_state;
%}

%s STR_OPEN
%s FNAME
%s VALUE

KEY		[_a-zA-Z][_a-zA-Z0-9]*
INT		[0-9]+
DBL		[0-9]+\.[0-9]+
STR		[^"\n]+
CMT		#.*$
OPS		[-+*/)(=]
%%
<INITIAL>{KEY}				{ BEGIN VALUE; yylval.kval = strdup(yytext); return KEY; }
<VALUE>\"					{ BEGIN STR_OPEN; return *yytext; }
<STR_OPEN>{STR}				{ yylval.sval = strdup(yytext); return STRING; }
<STR_OPEN>\n+				{ yyerror("Unmatched \"!"); }
<STR_OPEN>\"[ \t]*/\n		{ BEGIN VALUE; return *yytext; }

<VALUE>{KEY}				{ yylval.kval = strdup(yytext); return KEY; }

<VALUE>{INT}				{ 
								yylval.nval.l = strtol(yytext, NULL, 0); 
								yylval.nval.d = yylval.nval.l;
								yylval.nval.type = 0;
							 	return NUMBER; 
							}
<VALUE>{DBL}				{ 
								yylval.nval.d = strtod(yytext, NULL);
								yylval.nval.type = 1;
							 	return NUMBER; 
							}
<VALUE>{OPS}				{ return *yytext; }
<VALUE>\n					{ BEGIN INITIAL; return *yytext; }
<INITIAL>\n					;
{CMT}						;
[ \t]+						;
<INITIAL>IncludeCfg[ \t]+\"	BEGIN FNAME;
<FNAME>{STR}/\"				{
								next_file = fopen(yytext, "r");
								if (next_file == NULL)
								{
									fprintf(stderr, "Couldn't open file: %s.\n", yytext);
									exit(-1);
								}
							}
<FNAME>\"					{
								BEGIN INITIAL;
								next_state = yy_create_buffer(next_file, YY_BUF_SIZE);
								yy_switch_to_buffer(next_state);
							}
.							yyerror(yytext);
<INITIAL><<EOF>>			yyterminate();
%%
