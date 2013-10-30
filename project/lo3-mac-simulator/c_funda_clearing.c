#include<stdio.h>
#include<malloc.h>
#include<string.h>

/*int main()
{

int x;
double y;
y= 15.673487;
x=y;
printf("%d\n",x);
//int n1,n2;
//n1=32767;
//n2= n1 % 1000;
//printf("%d\n",n2);
return 0;
}*/

struct abc
{
int x;
float y;
};


int main()
{
	int i;
	struct abc temp1[5];
	struct abc temp2[5];

	for(i=0; i<5; i++)
	{
		temp1[i].x = i;
		temp1[i].y = i/(float)5;
	}

	for(i=0; i<5; i++)
	{
	printf("%d ", temp1[i].x);
	printf("%f ", temp1[i].y);
	printf("\n");
	}
	
	for(i=0;i<5;i++)
	temp2[i] = temp1[i] ;
	
	for(i=0; i<5; i++)
	{
	printf("%d ", temp2[i].x);
	printf("%f ", temp2[i].y);
	printf("\n");
	}

return 0;
}
