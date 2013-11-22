#include "header.h"

//declarations for scheduling algorithm

#define MAX_NODES_SCHED_ALGO 128
#define MAX_SLOTS_SCHED_ALGO 8 //16
#define REAL_SLOTS 8
#define MAX_CHANNELS_SCHED_ALGO 4 //16
#define MAX_HOPS_SCHED_ALGO 8 //16

#define MAX_VERTICES_SCHED_ALGO (MAX_NODES_SCHED_ALGO * MAX_SLOTS_SCHED_ALGO * MAX_CHANNELS_SCHED_ALGO * MAX_HOPS_SCHED_ALGO)

#define SOURCE_SCHED_ALGO (-2) //artifact of the graph construction, source vertex is single dimensional
#define UNDEFINED_SCHED_ALGO (99)
#define DELAY_BOUND_SCHED_ALGO (24) //8 data slots per frame of 80ms, 24 data slots = 240ms

#define constructVertex(node, slot, channel, hops) ((uint32_t) ((((uint32_t) channel & 0x0003) << 13)|(((uint32_t) hops & 0x0007) << 10) | (((uint32_t) slot & 0x0007) << 7) | (((uint32_t) node & 0x007f))))

#define getNode(vertex) ((uint8_t)(vertex & 0x007f))
#define getSlot(vertex) ((uint8_t)((vertex >> 7) & 0x0007))
#define getChannel(vertex) ((uint8_t)((vertex >> 13) & 0x0003))
#define getHops(vertex) ((uint8_t)((vertex >> 10) & 0x0007))


//Dijkstra's algorithm storage space
int dist[MAX_VERTICES_SCHED_ALGO]; //stores delay
uint32_t previousNode[MAX_VERTICES_SCHED_ALGO]; //used for back tracing
int nodeVisited[MAX_VERTICES_SCHED_ALGO]; //used during selection of a vertex for an iteration in Dijkstra's algorithm
uint32_t minVertex;
int minDistance, tempDelay;	



int IsNodeReceiver(int slot, int channel, int nodeToCheck)
{
	int k;
	for(k=0; k<globalNumSchedElem; k++)
	{
		if(globalScheduleElements[k].receiver == nodeToCheck && globalScheduleElements[k].slotToStart == slot && EXTRACT_CHANNEL(globalScheduleElements[k].flowId_channel) == channel)
		{
			return TRUE;
		}		
	}
	
	return FALSE;
}

int IsNodeTransmitter(int slot, int channel, int nodeToCheck)
{
	int k;
	for(k=0; k<globalNumSchedElem; k++)
	{
		if(globalScheduleElements[k].transmitter == nodeToCheck && globalScheduleElements[k].slotToStart == slot && EXTRACT_CHANNEL(globalScheduleElements[k].flowId_channel) == channel)
		{
			return TRUE;
		}		
	}
	
	return FALSE;
}

int IsChannelAvailableAtTx(int node, int slot, int channel)
{
	int i;
	for(i = 0; i < nodeID[node].numInterferringNeighbors; i++)
	{
		if(IsNodeReceiver(slot, channel, nodeID[node].interferenceList[i]))
		{
			//if(slotChannelNodes[slot][channel][nodeID[node].interferenceList[i]] == 1)
			{
				return FALSE;
			}
		}	
	}
	
	return TRUE;	
}

int IsChannelAvailableAtRx(int node, int slot, int channel)
{
	int i;
	for(i = 0; i < nodeID[node].numInterferringNeighbors; i++)
	{
		if(IsNodeTransmitter(slot, channel, nodeID[node].interferenceList[i]))
		{
			//if(slotChannelNodes[slot][channel][nodeID[node].interferenceList[i]] == 1)
			{
				return FALSE;
			}
		}	
	}
	
	return TRUE;
}

int calcDelay(int prevSlot, int nextSlot)
{
	if(nextSlot > prevSlot)
	{
		return (nextSlot - prevSlot);
	}
	else
	{
		return (REAL_SLOTS - nextSlot + prevSlot);
	}
}

int AreTwoVerticesConnected(uint32_t retVertexA, uint32_t retVertexB)
{
	int i, j;
			
	uint8_t nodeA, slotA, channelA, hopA;
	uint8_t nodeB, slotB, channelB, hopB;
			
	nodeA = getNode(retVertexA);
	slotA = getSlot(retVertexA);
	channelA = getChannel(retVertexA);
	hopA = getHops(retVertexA);
	
	if(slotA >= REAL_SLOTS)
	{
		//fprintf(stderr,"vertex %u slot %u\n",retVertexA, slotA);
		return FALSE;
	}
					
	nodeB = getNode(retVertexB);
	slotB = getSlot(retVertexB);
	channelB = getChannel(retVertexB);
	hopB = getHops(retVertexB); 
	
	if(slotB >= REAL_SLOTS)
	{
		//fprintf(stderr,"vertex %u slot %u\n",retVertexB, slotB);
		return FALSE;
	}
	
	
	//rule 1: edge should be present							
	if(networkGraph[nodeA][nodeB]!=0 && networkGraph[nodeA][nodeB]!= UNDEFINED)
	{
		//rule 2: slots should be different
		if(slotA != slotB)
		{
			//rule 3: common channel should be available
			//rule 3a: channel should not intefere with last to last channel on scheduled link
			if(IsChannelAvailableAtTx(nodeA, slotB, channelB) && IsChannelAvailableAtRx(nodeB, slotB, channelB))
			{
				//rule 4: hops should increase by 1
				if(hopB == (hopA + 1))
				{
					//fprintf(stderr,"%u -> %u (%u, %u, %u)\n", nodeA, nodeB, slotB, channelB, hopB);
					//exit(0);
					
					return TRUE;
				}
			}
				
		}
	}
	
	return FALSE;
				
}

int flushDijkstraStorage()
{
	uint32_t i;


	for(i=0; i<MAX_VERTICES_SCHED_ALGO; i++)
	{
		dist[i] = UNDEFINED;
		previousNode[i] = UNDEFINED;
		nodeVisited[i] = FALSE;
	}
}

int setSchedule_shortestPath(uint16_t source, uint16_t destination, int flowId)
{
	uint32_t i, j, k;
	int foundVertex = FALSE;
	
	flushDijkstraStorage();
	
	fprintf(stderr,"\nsource %u destination %u\n",source, destination);


	//check 1
	if(source < 0 || source > numOfNodesInNetwork)
	{
		fprintf(stderr,"error in setSchedule_shortestPath, source invalid %d\n",source);
		exit(0);
	}
	
	//check 2
	if(destination < 0 || destination > numOfNodesInNetwork)
	{
		fprintf(stderr,"error in setSchedule_shortestPath, destination invalid %d\n",destination);
		exit(0);
	}	
		
		
	//Dijkstra's algorithm initialization
	
	foundVertex = FALSE;
	
	for(j=0; j<MAX_VERTICES_SCHED_ALGO; j++)
	{
	
		if(getSlot(j) < REAL_SLOTS)
		{
			dist[j]= networkGraph[source][getNode(j)]; //0 OR 1 OR UNDEFINED, indicates delay, initializing delay to 1 slot.		
		}
		else
		{
			dist[j]= UNDEFINED;
		}
		
		//Dijkstra's algorithm fix first vertex
		//let the vertex be any neighbor
		if(networkGraph[source][getNode(j)] == 1)
		{
		
			if(getSlot(j) < REAL_SLOTS && foundVertex == FALSE)
			{
				minVertex = j;
				foundVertex = TRUE;
			}	
			
		}
		
		
		previousNode[j] = SOURCE_SCHED_ALGO; //-2 for source
	}
	
	
	//fprintf(stderr,"\nVertex %u %u (%u, %u, %u)\n", minVertex, getNode(minVertex), getSlot(minVertex), getChannel(minVertex), getHops(minVertex));
	//fprintf(stderr,"\nAlgorithm Initialized\n");
	
	//Dijkstra's algorithm mark other vertices unvisited
	foundVertex = FALSE;
	
	for(j=0; j<MAX_VERTICES_SCHED_ALGO; j++)
	{

		if(j != minVertex)
		{
			nodeVisited[j] = FALSE;
		}
		else
		{
			nodeVisited[j] = TRUE;
		}
		
	}
	
	//iterate over all unvisited vertices
	for(k=1; k<MAX_VERTICES_SCHED_ALGO; k++)
	{
		//update dist of unvisited vertices
		for(j=0; j<MAX_VERTICES_SCHED_ALGO; j++)
		{
		
			if(nodeVisited[j] == FALSE && AreTwoVerticesConnected(minVertex, j))
			{
				tempDelay = dist[minVertex] + calcDelay(getSlot(minVertex), getSlot(j));
			
			
				if(tempDelay < DELAY_BOUND_SCHED_ALGO && tempDelay < dist[j])
				{
				
					dist[j] = dist[minVertex] + calcDelay(getSlot(minVertex), getSlot(j));
					previousNode[j] = minVertex;
				}	
			}
		}
		
	
		//find the unvisited vertex with minimum dist
		minDistance = UNDEFINED;
		
		for(j=0; j<MAX_VERTICES_SCHED_ALGO; j++)
		{
		
			//we are allowing path only through infra node
			if(nodeID[getNode(j)].type == INFRA && nodeVisited[j] == FALSE && dist[j] < minDistance)
			{
				//fprintf(stderr,"\n MinVertex %u %u %u (%u %u %u)\n",j, dist[j], getNode(j), getSlot(j), getChannel(j), getHops(j));
				minDistance = dist[j];
				minVertex = j;
			}
		}
		
		if(minDistance == UNDEFINED)
		{
			//no new vertex found, break from the loop
			//fprintf(stderr,"\nvalue of k %u\n",k);
			
			break;
		}
		
		//mark it visited
		nodeVisited[minVertex] = TRUE;				
	

	}
	
	fprintf(stderr,"Dijkstra's done, find destination node\n");
	
	foundVertex = FALSE;
	
	for(j=0; j<MAX_SLOTS_SCHED_ALGO; j++)
	{
		for(k=0; k<MAX_CHANNELS_SCHED_ALGO; k++)
		{
			for(i=1; i <MAX_HOPS_SCHED_ALGO; i++)
			{
						
				if(dist[constructVertex(destination, j, k, i)] != UNDEFINED)
				{
					foundVertex = TRUE;
					break;
				}	
			}
			if(foundVertex == TRUE)
				break;
		}
		if(foundVertex == TRUE)
			break;
	}
	
	
	//caution: use of j, k, i still active
	if(foundVertex == FALSE)
	{
		fprintf(stderr,">>> Scheduling not possible <<<\n");
		//exit(0);
		return 0;
	}
	
	minVertex = constructVertex(destination, j, k, i);

	fprintf(stderr,"Vertex %u destination %u\n",minVertex, getNode(minVertex));
	
	k = globalNumSchedElem;
	
	for(i=minVertex; i != SOURCE_SCHED_ALGO;) //-2 is source
	{		
		fprintf(stderr,"node %d slot %d channel %d hops %d\n", getNode(i), getSlot(i), getChannel(i), getHops(i));
		
		//set scheduling elements
		
		if(previousNode[i] == SOURCE_SCHED_ALGO)
			globalScheduleElements[k].transmitter = source;
		else	
			globalScheduleElements[k].transmitter = getNode(previousNode[i]);
		globalScheduleElements[k].receiver = getNode(i);
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId,getChannel(i));
		globalScheduleElements[k].slotToStart = getSlot(i);
		globalScheduleElements[k].numSlots = 1;	
		
		//********************************************************
		slotChannelNodes[getSlot(i)][getChannel(i)][getNode(previousNode[i])] = 1;
		slotChannelNodes[getSlot(i)][getChannel(i)][getNode(i)] = 1;
		//********************************************************
		
		k++;	
		
		i=previousNode[i];
	}
	
	globalNumSchedElem = k;
	
	fprintf(stderr,"globalNumSchedElem %d\n",globalNumSchedElem);
		
	//exit(0);
	
	return 1;
}

int revokeElements_schedAlgo(int flowId)
{
	int i, j, k, l,m;
	int elementsToRemove, phase;
	int flowFound;
	FILE *ft;
	//vm = fopen("voiceMsg.txt","a+");	

	//VJ Start
	k = globalNumSchedElem;
	//VJ End
	
	flowFound = 0;	
	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			flowFound = 1;			
			break;
		}		
	}

	if(flowFound == 0)
	{
		fprintf(stderr,"\n Flow %d could not be found, possibly already deallocated",flowId);
		printf("\n Flow %d could not be found, possibly already deallocated",flowId);
		
		return SUCCESS;
	}
	
	flowFound = -1;
	elementsToRemove = 0;
	
	for(i=0; i<k; i++)
	{
		if(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
			if(flowFound == -1)
			{
				flowFound = i;
			}
			elementsToRemove++;
			slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].transmitter] = 0;
			slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].receiver] = 0;

		}
	}	
	
	if(flowFound == -1)
	{
		fprintf(stderr,"\n Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		printf("\n Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		
		return SUCCESS;	
		exit(0);
	}
	
	j = flowFound + elementsToRemove;

	ft = fopen(ChannelInfoFile,"a+");


	fprintf(stderr,"global scheduling elements being removed %d\n for flowId %d\n",elementsToRemove,flowId);	
	if(j == k) //elements to be removed are the last global elements 
	{
		//for(i=k-elementsToRemove;i<k;i++)
		//fprintf(stderr,"%d ",i);

		k -= elementsToRemove;
	}
	else
	{
		for(i = j; i<k; i++)
		{
			//fprintf(stderr,"%d ",i-elementsToRemove);
			globalScheduleElements[i-elementsToRemove] = globalScheduleElements[i];	
			
		}	
		k -= elementsToRemove;
	}
	
	//VJ Start
	//nodeID[ROOT].numSchedElem = k;
	globalNumSchedElem = k;
	//VJ End

	fprintf(stderr,"\n Remaining scheduling elements\n");
	for(i=0; i<k; i++)
	{
		fprintf(stderr, "t %d\t r %d\t f %d\t c %d\t s %d\n",globalScheduleElements[i].transmitter,
		globalScheduleElements[i].receiver,
		EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel),
		EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel),
		globalScheduleElements[i].slotToStart);
	}

	fprintf(ft,"\n");
	fprintf(stderr,"\n");
	
	globalCallsInProgress--;
	
	//printSlotChannelNodes();

	setScheduleInfo();
	
	fclose(ft);

	//exit(0);

	if(globalCallsInProgress == 0)
	{
		printf("\n 0 calls in progress, all of the following elements should be zero");
		for(i=0; i<NO_OF_DATA_SLOTS; i++)
		{
			for(j=0; j<MAX_NO_CHANNELS; j++)
			{
				for(k=0; k<MAX_NO_NODES; k++)
				{
					if(slotChannelNodes[i][j][k] > 0)
						printf("\n slot %d channel %d node %d: %d",i,j,k,slotChannelNodes[i][j][k]);
					slotChannelNodes[i][j][k] = 0;
				}
			}
		}
	}
	
	
	return SUCCESS;
}

