#include "header.h"

#define LARGE_BOUND 3000
#define SMALL_BOUND 150
#define AVG_MOBILE 3

#define MAX_INFRA 5
#define MAX_MOBILE (MAX_INFRA * AVG_MOBILE)

int main() //generateRandGraph();
{
	int numNodes, numMobile;
	int infraId, mobileId;
	long long randValue;
	char currType;
	int currX, currY, nowX, nowY;	
		
	FILE *fp;	
	
	currX = -100;
	currY = 100;
	
	srand((unsigned) time(NULL));
	
	fp = fopen("coordinateFile.txt","w+");
	
	numNodes = 0;	
	infraId = 0;
	mobileId = MAX_INFRA;

	while(numNodes <(MAX_INFRA+MAX_MOBILE))
	{
		currType = 'I';
		currX += 200; //rand() % LARGE_BOUND;		
		
		//currY = rand() % LARGE_BOUND;

		fprintf(fp,"(%d,%c,%d,%d) ",infraId,currType,currX,currY);		
		infraId++;
		
		numNodes++;
		
		numMobile = 1;
		
		while(numMobile <=AVG_MOBILE)
		{
			currType = 'M';
			if(rand() % 2 == 1)
				nowX = currX + rand() % SMALL_BOUND;
			else
			{
				randValue = rand() % SMALL_BOUND;
				if(randValue > currX)
					nowX = randValue - currX;
				else		
					nowX = currX - rand() % SMALL_BOUND;
			}	
				
			if(rand() % 2 == 1)		
				nowY = currY + rand() % SMALL_BOUND;			
			else
			{
				randValue = rand() % SMALL_BOUND;
				if(randValue > currY)
					nowY = randValue - currY;
				else
					nowY = currY - rand() % SMALL_BOUND;	
			}	
		
			fprintf(fp,"(%d,%c,%d,%d) ",mobileId,currType,nowX,nowY);		
			mobileId++;
			numNodes++;
			
			numMobile++;
		}
		fprintf(fp,"\n");
				
		if(currX > (LARGE_BOUND - 200))
		{
			currX = -100;
			currY += 225;
		}
								
	}
	
	fclose(fp);	
	return SUCCESS;
}
