%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *str)
{
	fprintf(stderr, "Error: %s\n", str);
}

int yywrap()
{
	return 1;
}
%}
%union {
	long ivalue;
	double fvalue;
}
%token <fvalue> FLOAT
%token <ivalue> INT
%type <ivalue> iexp
%type <fvalue> fexp
%left '+' '-' 
%left '*' '/'

%%
prog : prog iexp '\n' { printf("%ld\n", $2); }
	 | prog fexp '\n' { printf("%lf\n", $2); }
	 |
	 ;

iexp : '(' iexp ')'		{ $$ = $2; }
	 | iexp '+' iexp	{ $$ = $1 + $3; }
	 | iexp '-' iexp	{ $$ = $1 - $3; }
	 | iexp '*' iexp	{ $$ = $1 * $3; }
	 | iexp '/' iexp	{ $$ = $1 / $3; }
	 | INT				{ $$ = $1; }
	 ;

fexp : '(' fexp ')'		{ $$ = $2; }
	 | fexp '+' fexp	{ $$ = $1 + $3; }
	 | fexp '+' iexp	{ $$ = $1 + (double) $3; }
	 | iexp '+' fexp	{ $$ = (double) $1 + $3; }
	 | fexp '-' fexp	{ $$ = $1 - $3; }
	 | fexp '-' iexp	{ $$ = $1 - (double) $3; }
	 | iexp '-' fexp	{ $$ = (double) $1 - $3; }
	 | fexp '*' fexp	{ $$ = $1 * $3; }
	 | fexp '*' iexp	{ $$ = $1 * (double) $3; }
	 | iexp '*' fexp	{ $$ = (double) $1 * $3; }
	 | fexp '/' fexp	{ $$ = $1 / $3; }
	 | fexp '/' iexp	{ $$ = $1 / (double) $3; }
	 | iexp '/' fexp	{ $$ = (double) $1 / $3; }
	 | FLOAT			{ $$ = $1; }
	 ;



%%

int main(void)
{
	yyparse();
	return 0;
}
