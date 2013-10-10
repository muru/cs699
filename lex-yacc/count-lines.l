	int s;
%%
.	;
\n	s++;
%%
yywrap()
{
	printf("%d\n", s);
	return(1);
}

