/* Program to implement Single source shortest Path algorithm using 
			Dynamic Programming	*/

#include "header.h"

#define DELAY_THRESHOLD_HOPS 10

struct routingPath * getRoutingPath(int source, int destination)
{
	int i, j, k;
	struct routingPath *aPath;	
	int dist[MAX_NO_NODES];
	int previousNode[MAX_NO_NODES];

	//printf("\n source %d destination %d",source, destination);		
	
	//manan change
	if(source < 0 || source > numOfNodesInNetwork)
	{
	fprintf(stderr,"error in routing path, source invalid %d\n",source);
	exit(0);
	}
	if(destination < 0 || destination > numOfNodesInNetwork)
	{
	fprintf(stderr,"error in routing path, destination invalid %d\n",destination);
	exit(0);
	}	
	//manan end

	aPath = (struct routingPath *)malloc(sizeof(struct routingPath));
	for(j=0;j<MAX_NO_NODES;j++)
	{
	   dist[j] = networkGraph[source][j];
	   previousNode[j] = source;
	}
	
	for(k=1;k<numOfNodesInNetwork;k++)
	{
	 for(j=0;j<numOfNodesInNetwork;j++)  // for each j
	 {
	  if(j!=source && incomingEdge[j]!=0) // j has atleast one incoming edge
	  {
	   for(i=0;i<numOfNodesInNetwork;i++) // for each i & j in graph
	   {
	    if(networkGraph[i][j]!=0 && networkGraph[i][j]!= UNDEFINED)
	    {
	     //if(nodeID[i].type == INFRA || j == destination)
	     if(nodeID[i].type == INFRA && ( j == destination || nodeID[j].type == INFRA )) //do not consider the nodes in the pool of bottleneck nodes
	     {
	      if(dist[i]+networkGraph[i][j] < dist[j])
	      {
	     	dist[j] = dist[i]+networkGraph[i][j];
	     	previousNode[j] = i;
		//this means that if the direct path (source to j, say) is longer than the two hop path (source to i, and i to j), then choose to reach source from j via i and update the distance metric for j accordingly and set the previous node of j to i 
	      }
             }	
	    }
	   }
	  }
	 }
	}		
	aPath->numOfNodesInPath = 0;
	
	//*************
	if(previousNode[destination] == source)
	{
		//return NULL;
	}
	//*************
	
	for(i=destination; i != source;)
	{		
		aPath->numOfNodesInPath++;
		//printf("\n current node %d previous node %d num of nodes in path %d", i, previousNode[i],aPath->numOfNodesInPath);
		i=previousNode[i];
	}
	aPath->numOfNodesInPath++;
	
	//**************
	if(aPath->numOfNodesInPath >= DELAY_THRESHOLD_HOPS)
	{
		//return NULL;
	}
	//**************
			
	i = destination;
	//printf("\n numOfNodesInPath %d",aPath->numOfNodesInPath);
	aPath->path = (int *)malloc(aPath->numOfNodesInPath * sizeof(int ));
			
	for(j=aPath->numOfNodesInPath; j > 0; j--)
	{		
		aPath->path[j-1] = i;
		i = previousNode[i];		
	}
	
	//printf("\n");
	
	for(i = 0; i <aPath->numOfNodesInPath; i++)
	{
		if(aPath->path[i] > numOfNodesInNetwork)
		{
			fprintf(stderr,"\n Errorneous path node %d numOfNodesInNetwork %d",aPath->path[i], numOfNodesInNetwork);
			exit(0);
		}
		//printf(" -> %d",aPath->path[i]);
	}
	return aPath;
}
