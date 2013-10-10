	int i;
	int f;
D	[0-9]+
F	[0-9]+\.[0-9]+
%%
{D}	i++;
{F}	f++;
.	|
\n	;
%%
yywrap()
{
	printf("%d %d\n", i, f);
	return(1);
}
