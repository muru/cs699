	int l;
	int w;
	int c;
L	\n
W	[a-zA-Z0-9]+
C	.
%%
{W}	{w++; c += yyleng;}
{C}	c++;
{L}	{l++; c++;}
%%
yywrap()
{
	printf("%d %d %d\n", l, w, c);
	return(1);
}
