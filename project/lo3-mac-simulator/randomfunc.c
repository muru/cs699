#include "header.h"

void initrand(int value)
{
	srand((unsigned) value);
}
double randdouble(double window)
{
	double temp = (double) (window * (rand()/(double)(RAND_MAX))); 
	return temp;
} 

double rand_val()
{
	double temp = (double) (rand()/(double)(RAND_MAX));
	return temp;
}

double exponDistriValue(double x)
{
	double z;                     // Uniform random number (0 < z < 1)
	double exp_value;             // Computed exponential value to be returned

	// Pull a uniform random number (0 < z < 1)	
	do
	{
		z = rand_val();
  	}while ((z == 0) || (z == 1));

  	// Compute exponential random variable using inversion method
  	exp_value = -x * log(z);
  	
  	return exp_value;
}

//generate random number between 0.0 to 0.99

int IsPacketToBeDropped(double errorValue)
{
	unsigned int randValue; 
		
	randValue = rand();
	
	if(((randValue/(double)(RAND_MAX))) < errorValue)
		return TRUE;
	else
		return FALSE;
}

int contentionFrame(double probability)
{

	double randValue = (double) (rand()/(double)(RAND_MAX)) ;

	if( randValue < probability)
		return TRUE;
	else
		return FALSE;
}

//portability changes start

double generateTopologyUpdateTime(double time)
{

double temp_rand, temp_time;
int compare, i;
int uniq_flag; // flag for whether time is unique or not
//assuming the current depth of the tree to be 8
int depth = 3;

do
{
uniq_flag = TRUE;

temp_rand = (double)((rand() % (int) ((TOPOLOGY_UPDATE_PERIOD * SECOND)/FRAME_DURATION)) * FRAME_DURATION);

temp_time = time + temp_rand;
compare = (int) ((int)temp_time % (TOPOLOGY_UPDATE_PERIOD * SECOND));

//fprintf(stderr,"simtime passed: %lf  temp_rand: %lf  temp_time: %lf  compare: %d \n",time,temp_rand,temp_time,compare);

uniq_time[uniq_time_counter] = compare;

//VJ

return temp_time;

for(i=0; i<uniq_time_counter; i++)
{
	if((uniq_time[i] < (compare - (depth*FRAME_DURATION))) || (uniq_time[i] > (compare + (depth*FRAME_DURATION))))
	{
	//do nothing
	}
	else
	{
	uniq_flag = FALSE;
	break;
	}
}

}while(uniq_flag == FALSE);

uniq_time_counter++;

/*if(uniq_time_counter == numOfNodesInNetwork - 1)
{
	for(i=0;i<uniq_time_counter;i++)
	{
		fprintf(stderr,"%d\n",uniq_time[i]);
	}
}
*/

//fprintf(stderr,"temp_time returned is %lf \n",temp_time);
return temp_time;
}
//portability changes end
