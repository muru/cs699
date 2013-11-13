#include "header.h"

//check whether destination is up, set scheduling elements

//function prototyping
void revokeVoiceFlow(int node, int phase);
void reshuffleVoiceCurrQueue(int voiceFlowId);
int minirevokeElements(int flowId);

int destinationIsUp(int destination)
{
	int i;

	if(destination == ROOT) // 10-4-10
	{
	return SUCCESS;
	}

	for(i=0;i<numOfNodesInTree;i++)
	{
		if(routingTree[2*i] == destination)
		{
			//fprintf(stderr,"\n **** %d Destination is up ****", destination);
			return SUCCESS;
		}
	}
	return FAILURE;
}

int isFlowRevoked(int flowId)
{
	int i;
	i = 0;
	//while(i<nodeID[ROOT].numSchedElem)
	while(i<globalNumSchedElem)
	{		
		if(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
			return FALSE;		
		}
		i++;		
	}	
	return TRUE;
}

int printSlotChannelNodes()
{
	FILE *fk;
	
	int i,j,k;
	
	fk = fopen("outputFiles/channelInfo.txt","a+");
	

	fprintf(fk,"\n globalCallsInProgress %d",globalCallsInProgress);
	
	for(i=0; i<NO_OF_DATA_SLOTS; i++)
	{
		for(j=0; j<MAX_NO_CHANNELS; j++)
		{
			for(k=0; k<MAX_NO_NODES; k++)
			{
				if(slotChannelNodes[i][j][k] > 0)
					fprintf(fk,"\n slot %d channel %d node %d: %d",i,j,k,slotChannelNodes[i][j][k]);
				
			}
		}
	}
	fclose(fk);
}

int printSchedulingElements()
{
	int i;
	//printf("\n Root: scheduling elements %d",nodeID[ROOT].numSchedElem);
	//printf("\n Root: scheduling elements %d",globalNumSchedElem);
	
	//printf("\n printing scheduling elements");
	//for(i=0; i<nodeID[ROOT].numSchedElem; i++)
	for(i=0; i<globalNumSchedElem; i++)
	{
		//printf("\n flowId %d slot %d channel %d tx %d rx %d",EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].slotToStart, EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].transmitter, globalScheduleElements[i].receiver);
	}
	return SUCCESS;
}

int printSchedulingElements2()
{
	int i;
	//fprintf(stderr,"\n Root: scheduling elements %d",nodeID[ROOT].numSchedElem);
	
	//fprintf(stderr,"\n printing scheduling elements");
	//for(i=0; i<nodeID[ROOT].numSchedElem; i++)
	for(i=0; i<globalNumSchedElem; i++)
	{
		//fprintf(stderr,"\n flowId %d slot %d channel %d tx %d rx %d",EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].slotToStart, EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].transmitter, globalScheduleElements[i].receiver);
	}
	return SUCCESS;
}


int deAllocateFlowsForNodes(struct routingPath *aPath, int voiceMsg, char *str)
{
	int i, numOfNodesInPath;
	numOfNodesInPath = aPath->numOfNodesInPath;
	
	for(i=1;i<(numOfNodesInPath - 1);i++)
	{
		if(flowsPerNode[aPath->path[i]] > 0)
		{
			flowsPerNode[aPath->path[i]]--;
		}	
	
		if(flowsPerNode[aPath->path[i]] < 0)
		{
			fprintf(stderr,"Error condition, flows per node < 0\n");
			exit(0);
		}
	}
	//manan change
	if(voiceMsg == 1)
	{
		//FILE *vm;
		//vm = fopen("voiceMsg.txt","a+");
	
		if(nodeID[aPath->path[0]].type == INFRA)
		{
			flowsPerNode[aPath->path[0]]--;
		//fprintf(stderr,"in deallocateflows, deallocating for node %d\n",aPath->path[0]);
		
			if(flowsPerNode[aPath->path[0]] < 0)
			{
			fprintf(stderr,"Error condition, flows per node < 0\n");
			exit(0);
			}
		}

		if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
		{
			flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]]--;

			if(flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]] < 0)
			{
			fprintf(stderr,"Error condition, flows per node < 0\n");
			exit(0);
			}

			//fprintf(stderr,"in deallocateflows, deallocating for node %d\n",aPath->path[aPath->numOfNodesInPath-1]);
			//fprintf(stderr,"\nreason is %s\n",str);		
		}
	//fclose(vm);
	}
	
	//manan end
}

int getNonInterferringChannel(int tx, int rx, int slot)
{
	int i, j, channel;
	int flag;
	
	for(channel = 0; channel <MAX_NO_CHANNELS; channel++)
	{
		flag = 0;
		for(i = 0; i < nodeID[rx].numInterferringNeighbors; i++)
		{
			if(slotChannelNodes[slot][channel][nodeID[rx].interferenceList[i]] == 1)
			{
				//fprintf(stderr,"\n channel %d busy in slot %d at %d rx %d",channel, slot, nodeID[rx].neighborList[i], rx);
				flag = 1;
				break;
			}
			else
			{
				for(j = 0; j < nodeID[tx].numInterferringNeighbors; j++)
				{				
					if(slotChannelNodes[slot][channel][nodeID[tx].interferenceList[j]] == 1)
					{
						//fprintf(stderr,"\n channel %d busy in slot %d at %d tx %d",channel, slot, nodeID[tx].neighborList[j], tx);
						flag = 1;
						break;
					}
				}	
			}
				
		}	
		if(flag == 0)
			return channel;
	}
	
	//fprintf(stderr,"\n Channels choked : callsInProgress %d",globalCallsInProgress);
			
	return channel;
}


int deAllocateState(int flowId, int elementsToFree, int numOfSchedElem)
{
	int flowFound;
	int i,j,phase, voiceMsg;
	struct routingPath * aPath;
	flowFound = 0;	
	//FILE *vm;
	//vm=fopen("voiceMsg.txt","a+");
	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			if(flowStat[i].voiceFlow == 1)
			{
				fprintf(vm,"Voice Flow found in call to deallocate state, flow id %d\n",flowStat[i].flowId);
				voiceMsg = 1;
			}
			flowFound = 1;			
			//aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination);
			aPath = flowStat[i].aPath;
			phase = flowStat[i].phase;
			break;
		}		
	}
	
	if(flowFound == 0)
	{
		//sanity check applicable only when no flow time out at root occurs
		fprintf(stderr,"\n Sanity check failed inside deAllocateState: flowfound = 0");
		exit(0);
		
	}
	
	//manan change
	if(voiceMsg == 1)
	{
		for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
		{
			phasesPerNode[aPath->path[j]][phase] = 0;	
		}
		
		if(nodeID[aPath->path[0]].type == INFRA)
			phasesPerNode[aPath->path[0]][phase] = 0;	
		
		if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
			phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][phase] = 0;	
		
	}
	else
	{		
		for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
		{
			phasesPerNode[aPath->path[j]][phase] = 0;	
		}
	}	
	char *str = "deallocate flows called from deallocate state";
	deAllocateFlowsForNodes(aPath, voiceMsg, str);

	//reshuffleFlowQueue(flowId);
	//the numSchedElem thing will be useful for the second for loop of normal calls
	
	for(i=0; i<numOfSchedElem; i++)
	{
		if(globalScheduleElements[i].transmitter == aPath->path[0] && globalScheduleElements[i].receiver == aPath->path[1] && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
			break;
	}	
	
	//if(i == numOfSchedElem && numOfSchedElem != nodeID[ROOT].numSchedElem)
	if(i == numOfSchedElem && numOfSchedElem != globalNumSchedElem)
	{
		fprintf(stderr,"\n flow found %d but no sched elem", flowId);
		exit(0);
		//return SUCCESS;
	}
	else
	{
		// do nothing
	}
	
	j = i + elementsToFree;
	
	for(;i<j;i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].transmitter] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].receiver] = 0;
		
	}
					
}

int flowExists(int flowId)
{
	int i, k;
	int retVal = ABSENT;
	
	//k=nodeID[ROOT].numSchedElem;
	k = globalNumSchedElem;
	
	//fprintf(stderr,"\nNumSchedElem %d",k);
	
	for(i=0; i<k; i++)
	{
	
		//fprintf(stderr,"\ni = %d",i);
		
		if(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
			//fprintf(stderr,"\n flow already allocated");
			retVal = PRESENT;
		}
	}

	return retVal;	
}

//******************************************************************************************

int deAllocateSlotsForNodes(struct routingPath *aPath)
{
	int i, numOfNodesInPath;
	numOfNodesInPath = aPath->numOfNodesInPath;
	
	for(i=1;i<(numOfNodesInPath - 1);i++)
	{
		slotsPerNodeSC[aPath->path[i]] = slotsPerNodeSC[aPath->path[i]] - 2; 
	} // why are we removing two slots????
}

int getMaxSlotsForNodes(struct routingPath *aPath)
{
	int i, maxFlows, numOfNodesInPath;
	
	numOfNodesInPath = aPath->numOfNodesInPath;	
	maxFlows = 0;

	for(i=1;i<(numOfNodesInPath - 1);i++)
	{
		slotsPerNodeSC[aPath->path[i]] = slotsPerNodeSC[aPath->path[i]] + 2 ;
	}
	
	for(i=0; i<MAX_NO_NODES; i++)
	{		
		if(slotsPerNodeSC[i] > maxFlows)
		{
			maxFlows = slotsPerNodeSC[i];
		}	
	}
	return maxFlows;	
}

//*
int setSchedulingElementsSingleChannel(int source, int destination, int flowId, int incomingPhase)
{
	int i,j,k;
	int l, m;
	int flag;

	int maxFlows;
	int channel;
	int slotArray[MAX_NO_HOPS * 2];
	int channelArray[MAX_NO_HOPS * 2];
	int slotArrayIndex1 = 0, slotArrayIndex2 = 0;
	
	
	struct routingPath * aPath;
	
	FILE *fp, *fq, *fr, *fa;
	int countCalls;
	
	//fprintf(stderr,"\nRequest from %d %d %d", source, destination, flowId);
	
	if(flowExists(flowId))
	{
		//printf("\n flow exists flowId %d",flowId);
		//fprintf(stderr,"\n Flow exists flowId %d",flowId);
		//exit(0);
		return (-1);
	}	

	numFlowRequestsReached[flowId] = 1;		
	countCalls = 0;
	for(i=0; i<MAX_FLOW_ID; i++)
	{
		countCalls += numFlowRequestsReached[i];
	}		
	
	
	if(noOfCallsAdmitted_Unique[flowId] == 1)
	{
		//fprintf(stderr,"\n Call was already admitted %d", flowId);
		//fprintf(stderr,"\nTotal received calls %d flowId %d",countCalls, flowId);
		return -1;
	}
	else
	{
		//fprintf(stderr,"\nTotal received calls %d flowId %d",countCalls, flowId);
	}
	
	
	
	noOfTotalFlowRequestsReceived++;
	
	flowRequestsReceived[globalCallsInProgress]++;
	
	//fprintf(stderr, "\n flow in scheduler single channel %d",flowId);
	//exit(0);
	
	fp = fopen("outputFiles/simCalls.txt","a+");
	fq = fopen("outputFiles/admitCalls.txt","a+");
		
	//printf("\n root numSchedElem %d",nodeID[ROOT].numSchedElem);
	
	k = globalNumSchedElem; //nodeID[ROOT].numSchedElem;	

	aPath = getRoutingPath(source, destination);	
	
	//*************************************
	/*
	if(aPath == NULL)
	{
		return (next phase); //routing is not possible in this phase
		if next phase exceeds max phase
		return (-1)
	}
	*/
	//**************************************
	
	maxFlows = getMaxSlotsForNodes(aPath); //this call is not idempotent
			
	if(maxFlows > NO_OF_DATA_SLOTS)
	{
		fprintf(fp,"\n %d %lf", globalCallsInProgress, simTime);
		
		noOfTotalFlowRequestsRejected++;
		flowRequestsRejected[globalCallsInProgress]++;
		
		//fprintf(stderr,"\n ***** No_SLOT_Found maxflows %d flowId %d globalCallsInProgress %d *****",maxFlows,flowId,globalCallsInProgress);
		deAllocateSlotsForNodes(aPath);
		
		fclose(fp);
		fclose(fq);
		
		//add node in the pool of bottleneck nodes
		
		return (-1);
		//return (the same phase); //try routing path with bottleneck node removed
	}
	
	m = slotArrayIndex1;
	for(i=0; i<(aPath->numOfNodesInPath - 1);i++)
	{
		for(j=0; j<NO_OF_DATA_SLOTS; j++)
		{	
			channel = getNonInterferringChannel(aPath->path[i], aPath->path[i + 1], j);
		
			if(channel < MAX_NO_CHANNELS)
			{
				break;
			}
		
		}
		
		if(j == NO_OF_DATA_SLOTS)
		{
			//fprintf(stderr,"\n No common slot found for nodes %d %d for flowId %d",aPath->path[i], aPath->path[i+1],flowId);
			
			//add nodes in the pool of bottleneck nodes
			
			for(l=0; l<i; l++)
			{
				slotChannelNodes[slotArray[m]][channelArray[m]][aPath->path[l]] = 0;
				slotChannelNodes[slotArray[m]][channelArray[m]][aPath->path[l+1]] = 0;	
				m++;
			}
			
			
			return (-1);
			
			
		}
		
		slotArray[slotArrayIndex1] = j;
		channelArray[slotArrayIndex1] = channel;
			
		//mark the resources as busy
		slotChannelNodes[j][channel][aPath->path[i]] = 1;
		slotChannelNodes[j][channel][aPath->path[i+1]] = 1;
					
		slotArrayIndex1++;		
		
		
		
	}
	
	
	for(i=1; i<=(aPath->numOfNodesInPath - 1);i++)
	{
		for(j=0; j<NO_OF_DATA_SLOTS; j++)
		{	
			channel = getNonInterferringChannel(aPath->path[i], aPath->path[i - 1], j);
		
			if(channel < MAX_NO_CHANNELS)
			{
				break;
			}
		
		}
		
		if(j == NO_OF_DATA_SLOTS)
		{
			//fprintf(stderr,"\n No common slot found for nodes %d %d for flowId %d",aPath->path[i], aPath->path[i-1],flowId);
			//add nodes in the pool of bottleneck nodes
			
			for(l=0; l<(aPath->numOfNodesInPath - 1); l++)
			{
				slotChannelNodes[slotArray[m]][channelArray[m]][aPath->path[l]] = 0;
				slotChannelNodes[slotArray[m]][channelArray[m]][aPath->path[l+1]] = 0;	
				m++;
			}
			
			for(l=1; l<i;l++)
			{
			
			
				slotChannelNodes[slotArray[m]][channelArray[m]][aPath->path[l]] = 0;
				slotChannelNodes[slotArray[m]][channelArray[m]][aPath->path[l-1]] = 0;	
				m++;
			}
			
			return (-1);
			//return (the same phase); //try routing path with bottleneck node removed
			
		}
		
		slotArray[slotArrayIndex1] = j;
		channelArray[slotArrayIndex1] = channel;
	
		//mark the resources as busy
		slotChannelNodes[j][channel][aPath->path[i]] = 1;
		slotChannelNodes[j][channel][aPath->path[i-1]] = 1;
		
		slotArrayIndex1++;
	}
	


	for(i=0; i<(aPath->numOfNodesInPath - 1);i++)
	{		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i + 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);		
		globalScheduleElements[k].slotToStart = slotArray[slotArrayIndex2];
		globalScheduleElements[k].numSlots = 1;
		
	
		
		//********************************************************
		slotChannelNodes[slotArray[slotArrayIndex2]][channel][aPath->path[i]] = 1;
		slotChannelNodes[slotArray[slotArrayIndex2]][channel][aPath->path[i+1]] = 1;	
		//********************************************************
		
		slotArrayIndex2++;		
		k++;			
	}	

	for(i=1; i<=(aPath->numOfNodesInPath - 1);i++)
	{
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i - 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);
		globalScheduleElements[k].slotToStart = slotArray[slotArrayIndex2];
		globalScheduleElements[k].numSlots = 1;	

		
		//********************************************************
		slotChannelNodes[slotArray[slotArrayIndex2]][channel][aPath->path[i]] = 1;
		slotChannelNodes[slotArray[slotArrayIndex2]][channel][aPath->path[i-1]] = 1;
		//********************************************************
		
		slotArrayIndex2++;		
		k++;			
	}	

	for(j=0;j<slotArrayIndex2;j++)
	{
		flowStat[i].slotArray[j] = slotArray[j];	
	}
	


	globalNumSchedElem = k;
	
	//fprintf(stderr,"In scheduler, root numElem %d\n", globalNumSchedElem);
	
	setScheduleInfo(simTime);
		
	flowRequestsAdmitted[globalCallsInProgress]++;	
	globalCallsInProgress++;
	
	noOfCallsAdmitted++;	
	
	if(noOfCallsAdmitted_Unique[flowId] == 1)
	{
		//fprintf(stderr,"\n Call was already admitted %d", flowId);
		//exit(0);
	}
	
	noOfCallsAdmitted_Unique[flowId] = 1;
	
	fprintf(stderr,"\n%d flow admitted (s: %d d: %d f: %d) total calls %d", flowId, source, destination, flowId, noOfCallsAdmitted);
	
	for(i=0; i<k; i++)
	{
		//fprintf(stderr,"\n tx %d rx %d f %d s %d", globalScheduleElements[i].transmitter, globalScheduleElements[i].receiver, EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].slotToStart);
	}	
	
	//sleep(3);
	
	//fprintf(stderr,"\n Flow admitted %d simTime %lf\n",flowId, simTime);
	//printf("\n Flow admitted %d simTime %lf\n",flowId, simTime);
	//printSlotChannelNodes();	
	
	fprintf(fq,"\n %d \t %d \t %d \t %lf", noOfCallsAdmitted, flowId, globalCallsInProgress, simTime);
	fprintf(fp,"\n %d %d %d %lf", noOfCallsAdmitted, globalCallsInProgress, 1, simTime);
		
	fclose(fp);
	fclose(fq);

	return (-1);
}
//*/

//*
int revokeElementsSingleChannel(int flowId)
{
	int i, j, k, l;
	int elementsToRemove;
	int flowFound;
	struct routingPath * aPath;
	FILE *ft;
	
	int slotArrayIndex = 0;

	//fprintf(stderr,"\n Inside revoke elements");
	//sleep(3);

	k = globalNumSchedElem; //nodeID[ROOT].numSchedElem;
	
	flowFound = 0;

	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			flowFound = 1;			
			aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination);
			break;
		}		
	}
	
	if(flowFound == 0)
	{
		//fprintf(stderr,"\n Flow %d could not be found, possibly already deallocated",flowId);
		return SUCCESS;
	}
	
	
	for(i=0; i<k; i++)
	{
		if(globalScheduleElements[i].transmitter == aPath->path[0] && globalScheduleElements[i].receiver == aPath->path[1] && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
			break;
		//why aPath->path[1]
	}	
	
	if(i == k)
	{
		//fprintf(stderr,"\n Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		return SUCCESS;	
		//exit(0);
	}
	
	elementsToRemove = (aPath->numOfNodesInPath * 2 - 2);	
        //is this also because of parent child array relationships???
	
	j = i + elementsToRemove; //what do we gain from this statement???

	ft = fopen("outputFiles/channelInfo.txt","a+");

	for(l=0; l<(aPath->numOfNodesInPath - 1);l++,i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;
	}
	
	for(l=1; l<=(aPath->numOfNodesInPath - 1);l++,i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l-1]] = 0;
	}
	
	if(i != j)
	{
		fprintf(stderr,"\n Removing elements: i %d j %d",i,j);
		exit(0);
	}
	
	/*		
	for(;i<j;i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].transmitter] = 0;
		fprintf(ft,"\n slot %d channel %d tx %d",globalScheduleElements[i].slotToStart, EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].transmitter);		
	}
	*/
		
	if(j == k) //elements to be removed are the last global elements 
	{
		k -= elementsToRemove;
	}
	else
	{
		for(i = j; i<k; i++)
		{
			globalScheduleElements[i-elementsToRemove] = globalScheduleElements[i];			
		}	
		k -= elementsToRemove;
	}
	
	//nodeID[ROOT].numSchedElem = k;
	globalNumSchedElem = k;
	
	fprintf(stderr,"\n%d flow revoked scheelem %d", flowId, globalNumSchedElem);
	//sleep(3);

	fprintf(ft,"\n");
	
	fclose(ft);
	
	deAllocateSlotsForNodes(aPath);	
	
	reshuffleFlowQueue(flowId);
	
	globalCallsInProgress--;
	
	if(globalCallsInProgress == 0)
	{
		//printf("\n 0 calls in progress, all of the following elements should be zero");
		for(i=0; i<NO_OF_DATA_SLOTS; i++)
		{
			for(j=0; j<MAX_NO_CHANNELS; j++)
			{
				for(k=0; k<MAX_NO_NODES; k++)
				{
					if(slotChannelNodes[i][j][k] > 0)
					{
						//printf("\n slot %d channel %d node %d: %d",i,j,k,slotChannelNodes[i][j][k]);
					}	
					slotChannelNodes[i][j][k] = 0;
				}
			}
		}
	}	

	
	return SUCCESS;
}
//*/

//******************************************************************************************

//manan change ******************************************************************************

int minirevokeElements(int flowId) // only for revoking the elements of the voice flow
{
	int i, j, k, l,m;
	int elementsToRemove, phase;
	int flowFound,voiceMsg;
	int bottleneck;
	voiceMsg = 0;
	struct routingPath * aPath;
	FILE *ft;
	//vm = fopen("voiceMsg.txt","a+");	

	//VJ Start
	k = globalNumSchedElem;
	//k = nodeID[ROOT].numSchedElem; //****imp stmt******
	//VJ End
	
	flowFound = 0;
	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			//manan change
			if(flowStat[i].voiceFlow == 1)
			{
			//fprintf(stderr,"mini revoke elements called for voice flow id %d\n",flowStat[i].flowId);
			fprintf(vm,"mini revoke elements called for voice flow id %d\n",flowStat[i].flowId);
			voiceMsg = 1;
			}
			//manan end
			flowFound = 1;			
			//aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination);
			aPath = flowStat[i].aPath;
			phase = flowStat[i].phase;
			break;
		}		
	}

	if(flowFound == 0)
	{
		//fprintf(stderr,"\n Flow %d could not be found, possibly already deallocated",flowId);
		//printf("\n Flow %d could not be found, possibly already deallocated",flowId);
		
		return SUCCESS;
	}
	
	for(i=0; i<k; i++)
	{
		if(globalScheduleElements[i].transmitter == aPath->path[0] && globalScheduleElements[i].receiver == aPath->path[1] && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
			break;
	}	
	
	if(i == k)
	{
		//fprintf(stderr,"\n mini Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		//printf("\n mini Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		
		return SUCCESS;	
		//exit(0);
	}
	
	elementsToRemove = (aPath->numOfNodesInPath * 2 - 2);	
	
	j = i + elementsToRemove;

	ft = fopen("outputFiles/channelInfo.txt","a+");
	//manan change
	if(voiceMsg == 1)
	{
	//decrementing the global variable numPathsInVoiceCurrQueue // removed from here, shifted to revokeVoiceFlow
	//numPathsInVoiceCurrQueue--;	

	int slot, channel, node;

		for(l=0; l<(aPath->numOfNodesInPath - 1);l++,i++)
		{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;

		slot = globalScheduleElements[i].slotToStart;
		channel = EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel);
		
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l],slotChannelNodes[slot][channel][aPath->path[l]]);
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l+1],slotChannelNodes[slot][channel][aPath->path[l+1]]);
		
		i++;

		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;

		slot = globalScheduleElements[i].slotToStart;
		channel = EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel);

		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l],slotChannelNodes[slot][channel][aPath->path[l]]);
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l+1],slotChannelNodes[slot][channel][aPath->path[l+1]]);
		
		}

	}
	else
	{
	fprintf(stderr,"In minirevokeElements, should not reach here in the first place for revoking the elements of a real time voice call\n");
	exit(0);
	}
	
	if(i != j)
	{
		fprintf(stderr,"\n Removing elements: i %d j %d",i,j);
		exit(0);
	}
	
	/*		
	for(;i<j;i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].transmitter] = 0;
		fprintf(ft,"\n slot %d channel %d tx %d",globalScheduleElements[i].slotToStart, EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].transmitter);		
	}
	*/
	//fprintf(stderr,"no of global scheduling elements being removed for flowId %d are %d\n",flowId,elementsToRemove);	
	if(j == k) //elements to be removed are the last global elements 
	{

		k -= elementsToRemove;
		
	}
	else
	{
		for(i = j; i<k; i++)
		{	

			globalScheduleElements[i-elementsToRemove] = globalScheduleElements[i];		
		}	
		k -= elementsToRemove;
	}
	
	//VJ Change
	globalNumSchedElem = k;
	//nodeID[ROOT].numSchedElem = k;
	//VJ End

	fprintf(ft,"\n");
	//fprintf(stderr,"\n");
	
	//manan change
	if(voiceMsg == 1)
	{
		if(nodeID[aPath->path[0]].type == INFRA)
		phasesPerNode[aPath->path[0]][phase] = 0;
	
		for(j=1;j<aPath->numOfNodesInPath-1;j++)
		{
			phasesPerNode[aPath->path[j]][phase] = 0;
		}
		
		if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
			phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][phase] = 0;
		
		for(j=0;j<=aPath->numOfNodesInPath-1;j++)
		{
		//fprintf(stderr,"\nNode %d Phase %d phasesPerNode %d\n",aPath->path[j],phase,phasesPerNode[aPath->path[j]][phase]);
		}
	
	}
	else
	{
	fprintf(stderr,"In minirevokeElements, should not reach here in the first place for revoking the elements of a real time voice call\n");
	exit(0);
	}

	char *str = "deallocate flows called from mini revoke elements";
	deAllocateFlowsForNodes(aPath, voiceMsg, str);
	
	//printing flow per node
	for(i=0;i<aPath->numOfNodesInPath;i++)
	//fprintf(stderr,"Node %d Flows per node %d\n",aPath->path[i], flowsPerNode[aPath->path[i]]);
	//printing ends

	reshuffleFlowQueue(flowId);
	
	globalCallsInProgress--;
	
	printSlotChannelNodes();
	fclose(ft);
	//fclose(vm);
	setScheduleInfo();

	if(globalCallsInProgress == 0)
	{
		//printf("\n 0 calls in progress, all of the following elements should be zero");
		for(i=0; i<NO_OF_DATA_SLOTS; i++)
		{
			for(j=0; j<MAX_NO_CHANNELS; j++)
			{
				for(k=0; k<MAX_NO_NODES; k++)
				{
					if(slotChannelNodes[i][j][k] > 0)
					{
						//printf("\n slot %d channel %d node %d: %d",i,j,k,slotChannelNodes[i][j][k]);
					}	
					slotChannelNodes[i][j][k] = 0;
				}
			}
		}
	}
	
	//resetting the values of the voice path that is now terminated
	//nodeID[aPath->path[0]].voiceCopyCallDuration = 0;
	//nodeID[aPath->path[0]].voicee2eDest = -1;
	//nodeID[aPath->path[0]].voiceTempDestination = -1;
	//nodeID[aPath->path[0]].voiceFlowRequestSent = 0;
	nodeID[aPath->path[0]].callDuration = 10; // this needs some clarification still
	//nodeID[aPath->path[0]].voiceFlow = 0; // this is done in handleGenerateDataPacket, so no need to do here

	return SUCCESS;
}

void reshuffleVoiceCurrQueue(int voiceFlowId)
{
int i,j,k,counter;
struct voiceCurrentQueue temp;

for(i=0;i<numPathsInVoiceCurrQueue;i++)
	{
	if(voiceCurrQueue[i].voiceFlowId == voiceFlowId)
		{
		temp = voiceCurrQueue[i];
		voiceCurrQueue[i] = voiceCurrQueue[numPathsInVoiceCurrQueue-1];
		voiceCurrQueue[numPathsInVoiceCurrQueue-1] = temp;
		}
	}
numPathsInVoiceCurrQueue--;

//fprintf(stderr,"printing contents in reshuffleVoiceCurrQueue\n");
struct routingPath * aPath;
for(i=0;i<numPathsInVoiceCurrQueue;i++)
{
aPath = getRoutingPath(voiceCurrQueue[i].source,voiceCurrQueue[i].destination);
//fprintf(stderr,"voiceFlowId %d routing path no. %d in voiceCurrQueue is: ",voiceCurrQueue[i].voiceFlowId,i);
for(j=0;j<aPath->numOfNodesInPath;j++)
{
//fprintf(stderr,"%d ",aPath->path[j]);
}
//fprintf(stderr,"\n");
}

}

void revokeVoiceFlow(int node, int phase) 
{
int i,j,k,l,counter,counter1; // i is very very very important
struct routingPath * aPath, *check;
int flowid_flag; // this will be used to ensure that the correct flow is being revoked, 0 means that now we are on the correct path to be revoked

int flowid_counter; // used for the above purpose
int exception_counter; // this for the situations wherein there are two paths of the exact same sizes and the same nodes and they have voice flows running on the given phase as well as some other phase. So in the voiceCurrentQueue, if the flow to be revoked is towards the rear, the other exact path which is in the front will qualify for all the conditions and will be revoked also along with the intended path. So the solution is to include the phase information in each of the paths in voiceCurrentQueue.
exception_counter = 0;
int flow_stat_ctr;

//printing the contents of voiceCurrentQueue
for(counter=0;counter<numPathsInVoiceCurrQueue;counter++)
{
check = getRoutingPath(voiceCurrQueue[counter].source,voiceCurrQueue[counter].destination);
//fprintf(stderr,"voiceFlowId %d routing path no. %d in voiceCurrQueue in revokeVoiceFlow is: ",voiceCurrQueue[counter].voiceFlowId,counter);
for(counter1=0;counter1<check->numOfNodesInPath;counter1++)
{
//fprintf(stderr,"%d ",check->path[counter1]);
}
//fprintf(stderr,"\n");
}
	for(i=0;i<numPathsInVoiceCurrQueue;i++)
	{
	flowid_flag = 0;

	aPath = getRoutingPath(voiceCurrQueue[i].source,voiceCurrQueue[i].destination);
		
		for(j=0;j<aPath->numOfNodesInPath;j++)
		{
		//fprintf(stderr,"Node %d phasesPerNode %d ",aPath->path[j],phasesPerNode[aPath->path[j]][phase]);
			if(aPath->path[j] == node && phasesPerNode[aPath->path[j]][phase] == 2 && voiceCurrQueue[i].voice_phase == phase)
			{

			//fprintf(stderr,"Node in the path %d, node in arg %d \n",aPath->path[j],node);

				for(flowid_counter = 1; flowid_counter < (aPath->numOfNodesInPath-1) ; flowid_counter++)
				{
					if(phasesPerNode[aPath->path[flowid_counter]][phase] == 2)
					{
					flowid_flag = 0;
					}
					else
					{
					flowid_flag = 1;
					break;
					}	
				}
				
				if(nodeID[aPath->path[0]].type == INFRA)
				{
					if(phasesPerNode[aPath->path[0]][phase] != 2)
					flowid_flag = 1;
				}
				
				if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
				{
					if(phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][phase] != 2)
					flowid_flag = 1;
				}

			if(flowid_flag == 0)
			{
			exception_counter++;
			//store the information about the flow in the voiceQueue before revoking it
			if(numPathsInVoiceQueue < 50)
			{
			voiceQueue[numPathsInVoiceQueue].source = voiceCurrQueue[i].source;
			voiceQueue[numPathsInVoiceQueue].destination = voiceCurrQueue[i].destination;
			voiceQueue[numPathsInVoiceQueue].overalle2eDestination = voiceCurrQueue[i].overalle2eDestination;
			voiceQueue[numPathsInVoiceQueue].callDuration = voiceCurrQueue[i].voiceMsgDuration;
			voiceQueue[numPathsInVoiceQueue].voiceQueueFlowId = voiceCurrQueue[i].voiceFlowId;
			voiceQueue[numPathsInVoiceQueue].voice_phase = voiceCurrQueue[i].voice_phase;

			//for calculating the storage capacity
			nodeID[voiceQueue[numPathsInVoiceQueue].source].store_cap += ((voiceCurrQueue[i].voiceMsgDuration/30)*24);
			//logging this update
			logStoreCap(voiceQueue[numPathsInVoiceQueue].source,nodeID[voiceQueue[numPathsInVoiceQueue].source].store_cap,simTime);

			numPathsInVoiceQueue++;

			for(flow_stat_ctr = 0; flow_stat_ctr < numVoiceMsg; flow_stat_ctr++)
			{
				if(voiceFlowStat[flow_stat_ctr].voiceFlowId == voiceCurrQueue[i].voiceFlowId)
				{
				voiceFlowStat[flow_stat_ctr].revoked++;
				}
			}

//printing the contents of voiceQueue
for(counter=0;counter<numPathsInVoiceQueue;counter++)
{
check = getRoutingPath(voiceQueue[counter].source,voiceQueue[counter].destination);
//fprintf(stderr,"routing path no. %d flow id %d in voiceQueue in revokeVoiceFlow is: ",counter, voiceQueue[counter].voiceQueueFlowId);
for(counter1=0;counter1<check->numOfNodesInPath;counter1++)
{
//fprintf(stderr,"%d ",check->path[counter1]);
}
}
//fprintf(stderr,"\n");			
//printing ends
			//numPathsInVoiceCurrQueue--;
			//fprintf(stderr,"correctly executing here in revokeVoiceFlow\n value of numPathsInVoiceQueue %d and  numPathsInVoiceCurrQueue %d\n",numPathsInVoiceQueue,numPathsInVoiceCurrQueue);
			fprintf(vm,"revoke voice flow called\n");
			}
			else
			{
			fprintf(stderr,"System overloaded in revokeVoiceFlow %d\n", numPathsInVoiceQueue);
			exit(0);
			}
			minirevokeElements(voiceCurrQueue[i].voiceFlowId);
			reshuffleVoiceCurrQueue(voiceCurrQueue[i].voiceFlowId);

			i = -1; //resetting the counter to -1 so that when the closing brace is encountered, it increments to 0 and then we can compare with all the remaining paths in the voiceCurrQueue, very very important statement

			if(exception_counter > 1)
			{
			fprintf(stderr,"two same paths with different phases error\n");
			exit(0);
			}

			}

			}
		}
	
	}

	if(exception_counter == 0)
	{
	fprintf(stderr,"If revokeVoiceFlow is called, then at least one flow must be revoked\n");
	exit(0);
	}
}

//manan end **********************************************************************************

int setSchedulingElements(int source, int destination, int flowId, int incomingPhase, int carryForward, int handoff, struct routingPath * handoff_path)
{
	//fprintf(stderr,"called with incoming phase %d\n",incomingPhase);
	int i,j,k,l; // i & k are very very important, not to be touched
	int flag;

	int maxFlows;
	int phase, slot, currentSlot, channel;
	//manan change
	int currentSlot1,channel1;
	int counter,phase_counter,bottleneck_node;
	//manan end
	
	int countCalls;
	
	int elementsToFree;
	int dropped, voiceMsg,prevSchedElem;
	voiceMsg = 0;
	//int bottleneck_node; //manan change

	struct routingPath * aPath;
	
	FILE *fp, *fq, *fr, *fa;
	//vm = fopen("voiceMsg.txt","a+");	

	if(flowExists(flowId))
	{
		//printf("\n flow exists flowId %d",flowId);
		//fprintf(stderr,"\n Flow exists flowId %d",flowId);
		//exit(0);
		return (-1);
		//no need to schedule again
	}
	
	//manan change
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].voiceFlow == 1 && flowStat[i].flowId == flowId)
		{
			//fprintf(stderr,"reached inside setscheduling elements voice flow matched Source %d Destination %d Flow id %d\n",source,destination,flowId);
			voiceMsg = 1;
			//exit(0);
		}
	}
	//manan end	
	
	numFlowRequestsReached[flowId] = 1;
		
	countCalls = 0;
	for(i=0; i<MAX_FLOW_ID; i++)
	{
		countCalls += numFlowRequestsReached[i];
	}
		
	//fprintf(stderr,"\nTotal received calls %d",countCalls);
	
	if(noOfCallsAdmitted_Unique[flowId] == 1)
	{
		fprintf(stderr,"\n Call was already admitted %d", flowId);
		return -1;
	}

		
	noOfTotalFlowRequestsReceived++;
	
	flowRequestsReceived[globalCallsInProgress]++;
	
	//printf("\n flow in scheduler %d",flowId);
	
	fp = fopen("outputFiles/simCalls.txt","a+");
	fq = fopen("outputFiles/admitCalls.txt","a+");
	fr = fopen("outputFiles/scheduleInfo.txt","a+");
	
	//printf("\n root numSchedElem %d",nodeID[ROOT].numSchedElem);

	elementsToFree = 0;

	if(handoff == 1)
	{
		aPath = handoff_path; 
	}
	else
	{
		aPath = getRoutingPath(source, destination);	
	}

	//***********************
	/*
	if(aPath == NULL)
	{
		return (next phase);
		if next phase exceeds max phases
		return (-1)
	}
	*/
	//***********************

	//fprintf(stderr,"\n The routing path in setSchedulingElements is : ");
	for(j=0; j<aPath->numOfNodesInPath; j++)
	{
		//fprintf(stderr,"%d ",aPath->path[j]);
	}
	
	if(voiceMsg == 1)
	{
	//printing phases per node for node 2, remove it later, very later
	//int count;
	//for(count=0;count<MAX_PHASES;count++)
	//fprintf(stderr,"phases per node for 2: phase %d value %d\n",count,phasesPerNode[2][count]);

		for(i=incomingPhase; i<MAX_PHASES; i++)
		{	
			flag = 0;
		
			if(nodeID[aPath->path[0]].type == INFRA)
				if(phasesPerNode[aPath->path[0]][i] == 1 || phasesPerNode[aPath->path[0]][i] == 2)
					flag = 1;

			if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
				if(phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][i] == 1 || phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][i] == 2)
					flag = 1;

		for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
		{
			if(phasesPerNode[aPath->path[j]][i] == 1 || phasesPerNode[aPath->path[j]][i] == 2)
				flag = 1;
		}
		if(flag == 0)
			break;
		}
		
	}
	else
	{
	//here I also need to check whether any voice flow is getting revoked or not
	int dummy_phase_counter;
	int free_phase_found;
	free_phase_found = 0;
	int inner_flag;
	
	for(i=incomingPhase; i<MAX_PHASES; i++)
	{
		flag = 0;
		for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
		{
			if(phasesPerNode[aPath->path[j]][i] == 1)
			{
				//fprintf(stderr,"%d node phase %d busy\n",aPath->path[j], i);
				flag = 1;
			}	
		}
		
		//put bottleneck nodes for this phase into bottleneck node list
		//return (the same phase)
		
		//fprintf(stderr,"\n %d phase is busy \n", i);
		
		if(flag == 0)
		{
			//checking if there was a voice msg scheduled in this phase or not, if this phase is less than the MAX_PHASE then first check for other free phases before revoking the voice flow
			for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
			{
			if(phasesPerNode[aPath->path[j]][i] == 2)
				{
				//putting hacks in here, for checking the validity of the code, delete the hacks later
				int faltu,faltu_nodes;
				/*for(faltu=i+1;faltu<MAX_PHASES;faltu++)
				{
					for(faltu_nodes=1;faltu_nodes<(aPath->numOfNodesInPath - 1);faltu_nodes++)
					{
					phasesPerNode[aPath->path[faltu_nodes]][faltu] = 1;
					}
				}
				for(faltu=i+1;faltu<MAX_PHASES;faltu++)
				{
					for(faltu_nodes=1;faltu_nodes<(aPath->numOfNodesInPath - 1);faltu_nodes++)
					{
					fprintf(stderr,"Node %d Phase %d Phases per node %d \n",aPath->path[faltu_nodes],faltu,phasesPerNode[aPath->path[faltu_nodes]][faltu]);
					}
				}*/
				//hacks end here

				//fprintf(stderr,"voice flow getting revoked abruptly, phase %d, node %d\n",i,aPath->path[j]);
				//now checking to see if there are other free phases available or not
					if(i<(MAX_PHASES-1))
					{
						for(dummy_phase_counter = i+1; dummy_phase_counter < MAX_PHASES; dummy_phase_counter++)
						{
						inner_flag = 0;
							for(faltu_nodes=1; faltu_nodes<(aPath->numOfNodesInPath - 1); faltu_nodes++)
							{
							if(phasesPerNode[aPath->path[faltu_nodes]][dummy_phase_counter] == 1 || phasesPerNode[aPath->path[faltu_nodes]][dummy_phase_counter] == 2)
							inner_flag = 1;
							}
							if(inner_flag == 0)
							{
							//fprintf(stderr,"value of phase before is: %d\n",i);
							i = dummy_phase_counter;
							free_phase_found = 1;
							//fprintf(stderr,"value of phase after is: %d\n",i);
							break;
							}
						}
						if(free_phase_found == 0)
						{
						//fprintf(stderr,"call to revokeVoiceFlow 1 (%d,%d);\n",aPath->path[j],i);	revokeVoiceFlow(aPath->path[j],i);	
						}
					}
					else
					{
					//fprintf(stderr,"call to revokeVoiceFlow 2 (%d,%d);\n",aPath->path[j],i);	
					revokeVoiceFlow(aPath->path[j],i);	
					}
				//fprintf(stderr,"The value of the phase, after checking remaining phases is %d\n",i);
				}
			}
		
		break; // control does not go back to incrementer part of for loop, hence i < MAX_PHASES
		}
	}
	}//phase checking else ends here
	
	//so i guess in this case that a phase per node value of 0 means a free phase and so we need the flag to remain 0 for an entire inner iteration to ensure that at least one phase is free among all the nodes

	if(i == MAX_PHASES)
	{		
		fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
		
		noOfTotalFlowRequestsRejected++;
		flowRequestsRejected[globalCallsInProgress]++;

		fprintf(fr,"\n No_Common_Phase_Found flowId %d globalCallsInProgress %d",flowId,globalCallsInProgress);
		//fprintf(stderr,"\n ***** No_Common_Phase_Found flowId %d globalCallsInProgress %d *****",flowId,globalCallsInProgress);
		//char *str = "deallocate called for counter == MAX_PHASES";		
		//deAllocateFlowsForNodes(aPath, voiceMsg, str);
		
		fclose(fp);
		fclose(fq);
		fclose(fr);
		
		//checking reason, later convert it to a function
		//nodeID[aPath->path[0]].voiceFlow = 1; // very important statement	

		maxFlows = 0;
		for(counter=1; counter<(aPath->numOfNodesInPath -1); counter++)
		{		
			if((flowsPerNode[aPath->path[counter]]+1) > maxFlows)
			{
			maxFlows = flowsPerNode[aPath->path[counter]]+1;
			}
			if((flowsPerNode[aPath->path[counter]]+1) > MAX_PHASES) 
			{
				

				//fprintf(stderr,"\n");
				for(phase_counter=0; phase_counter < MAX_PHASES; phase_counter++)
				{
				//fprintf(stderr,"*** Special: Node %d Phase %d Phases per node %d\n",aPath->path[counter], phase_counter, phasesPerNode[aPath->path[counter]][phase_counter]);
				}

			//fprintf(stderr,"\n debug string: Node %d Bottleneck node is  %d flows at node %d simtime %lf\n", aPath->path[0], aPath->path[counter], flowsPerNode[aPath->path[counter]]+1,simTime);
			//manan end
			//fprintf(stderr,"\n phases full at %d: phases %d MAX_PHASES %d callsInProgress %d",aPath->path[counter], flowsPerNode[aPath->path[counter]]+1, MAX_PHASES, globalCallsInProgress);
			
			//return maxFlows;	
			}
		}

		//exit(0);
		return (MAX_PHASES);
		//return (-1); //no common phase found, so no point in trying more
	}
	else
	{

	for(counter = 1; counter < (aPath->numOfNodesInPath-1);counter++)
		{
		flowsPerNode[aPath->path[counter]]++;
			if(flowsPerNode[aPath->path[counter]] >= MAX_PHASES)
			{
			int x,y;
				for(y=0;y<aPath->numOfNodesInPath;y++)
				{
				for(x=0; x < MAX_PHASES; x++)
				{
				//fprintf(stderr,"Node %d Phase %d Phases per Node %d\n",aPath->path[y],x,phasesPerNode[aPath->path[y]][x]);
				}
				//fprintf(stderr,"flows per node error: Node %d Flows Per Node %d\n",aPath->path[y],flowsPerNode[aPath->path[y]]);
				}
				
				
			//exit(0);
			}
		}	

	if(voiceMsg == 1)
	{	//fprintf(stderr,"\n");
		for(counter = 0; counter <= (aPath->numOfNodesInPath-1);counter++)
		{
		//fprintf(stderr," Node %d Flows Per Node %d\n",aPath->path[counter],flowsPerNode[aPath->path[counter]]);
		}
	}
	else
	{	//fprintf(stderr,"\n");
		for(counter = 0; counter <= (aPath->numOfNodesInPath-1);counter++)
		{
		//fprintf(stderr," Node %d Flows Per Node %d\n",aPath->path[counter],flowsPerNode[aPath->path[counter]]);
		}
	}	
	
	if(voiceMsg == 1)
	{
		//vm =fopen("voiceMsg.txt","a+");
		
		fprintf(vm,"value of carry forward passed to getmaxflowsfornodes is %d for node %d\n",carryForward,aPath->path[0]);
		if(carryForward == 0)
		{
			if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
			{
			flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]]++;

			//fprintf(stderr," Node %d Flows Per Node %d\n",aPath->path[aPath->numOfNodesInPath-1],flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]]);

			if(flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]] > MAX_PHASES)
				{
				fprintf(stderr,"severe error encountered, carry forward 0, exiting\n");
				exit(0); // exit hack
				}
			//fprintf(vm,"shouldnt be reaching here right now\n");
			//printf("shouldnt be reaching here right now\n");
			}
			if(nodeID[aPath->path[0]].type == INFRA)
			{
			fprintf(stderr,"logical error in getMaxFlowsForNodes, in carry forward 0, 1st node can not be an infra node\n");
			exit(0);
			}
		}
		else
		{
		if(nodeID[aPath->path[0]].type == INFRA)
		{
		flowsPerNode[aPath->path[0]]++;
		//fprintf(stderr," Node %d Flows Per Node %d\n",aPath->path[0],flowsPerNode[aPath->path[0]]);
		if(flowsPerNode[aPath->path[0]] > MAX_PHASES)
			{
			fprintf(stderr,"severe error encountered, carry forward 1, 1st node, exiting\n");
			exit(0); // exit hack
			}
		}

		if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
		{
		flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]]++;

		//fprintf(stderr," Node %d Flows Per Node %d\n",aPath->path[aPath->numOfNodesInPath-1],flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]]);

		if(flowsPerNode[aPath->path[aPath->numOfNodesInPath-1]] > MAX_PHASES)
			{
			fprintf(stderr,"severe error encountered, carry forward 1, last node, exiting\n");
			exit(0); // exit hack
			}
		}
			
		}
	//fclose(vm);
	}	
		maxFlows = 0;
		for(counter = 0; counter<=(aPath->numOfNodesInPath-1); counter++)
		{
		if(flowsPerNode[aPath->path[counter]] > maxFlows)
		maxFlows = flowsPerNode[aPath->path[counter]];
		}

		if(maxFlows > (globalCallsInProgress+1))
		{
		fprintf(stderr,"\n maxFlows %d globalCallsInProgress %d",maxFlows, globalCallsInProgress);
		//printf("\n maxFlows %d globalCallsInProgress %d; error exiting the simulation",maxFlows, globalCallsInProgress);
		
		for(counter=0; counter<numOfNodesInNetwork; counter++)
		{
			//fprintf(stderr,"\n node %d flows %d",counter,flowsPerNode[counter]);
		}
		
		printSchedulingElements2();
		exit(0);
		}
		
	}

	phase = i;	
	slot = i * 4;
	//eg. i=3, then the slots to be used should be starting from i guess 12 thru 15
	
	//******************************************
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			flowStat[i].phase = phase;
			flowStat[i].aPath = aPath; // changed 12-4-10
			break;
		}		
	}//The phase information of the flowStat structure gets updated here only
	
	//manan change
	if(voiceMsg == 1)
	{
		fprintf(vm,"reached here, matched the if without the 'for' for flow id %d\n", flowStat[i].flowId);
		//exit(0);

		if(nodeID[aPath->path[0]].type == INFRA)
		phasesPerNode[aPath->path[0]][phase] = 2;
		else
		phasesPerNode[aPath->path[0]][phase] = 0;
	
		for(j=1;j<aPath->numOfNodesInPath-1;j++)
		{
			phasesPerNode[aPath->path[j]][phase] = 2;
		}
		if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
		phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][phase] = 2;
		else
		phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][phase] = 0;	
		
		for(j=0;j<=aPath->numOfNodesInPath-1;j++)
		{
		//fprintf(stderr,"\nNode %d Phase %d phasesPerNode %d\n",aPath->path[j],phase,phasesPerNode[aPath->path[j]][phase]);
		}
	}
	else
	{
		for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
		{
		phasesPerNode[aPath->path[j]][phase] = 1;	
		//printf("\nNode %d Phase %d phasesPerNode %d\n",aPath->path[j],phase,phasesPerNode[aPath->path[j]][phase]);
		//fprintf(stderr,"\nNode %d Phase %d phasesPerNode %d",aPath->path[j],phase,phasesPerNode[aPath->path[j]][phase]);
		}//booking the phase i guess for all the intermediate nodes	
		//fprintf(stderr,"\n");
	}
	//manan end
	
	//******************************************
	//manan change 

	//VJ Start
	//k = nodeID[ROOT].numSchedElem;
	//prevSchedElem = nodeID[ROOT].numSchedElem; // changed 3-10-09
	k = globalNumSchedElem;
	prevSchedElem = globalNumSchedElem;
	//VJ End

	if(voiceMsg == 1)
	{
	//manan voice message scheduling start
	for(i=0; i<(aPath->numOfNodesInPath - 1);i++)
		{ //this loop runs till the second last node, nodes are numbered from 0 hence < noofnodesinpath -1
		if(i % 2 == 0)
			{
			currentSlot = slot + 0;
			currentSlot1 = slot + 2;
			}
		else
			{
			currentSlot = slot + 1;
			currentSlot1 = slot + 3;
			}
			
		channel = getNonInterferringChannel(aPath->path[i], aPath->path[i + 1], currentSlot);
		channel1 = getNonInterferringChannel(aPath->path[i], aPath->path[i + 1], currentSlot1);
		
		//there should be no interference on the hop on the channel on which the source of the hop is transmitting
		
		if(channel >= MAX_NO_CHANNELS || channel1 >= MAX_NO_CHANNELS)
		{
			fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
			
			noOfTotalFlowRequestsRejected++;
			flowRequestsRejected[globalCallsInProgress]++;
			
			fprintf(fr,"\n No_Channel_Found flowId %d globalCallsInProgress %d",flowId,globalCallsInProgress);
			//fprintf(stderr,"\n ***** voice message No_Channel_Found slot %d flowId %d globalCallsInProgress %d *****",currentSlot, flowId,globalCallsInProgress);			
			deAllocateState(flowId, elementsToFree, k);
			
			
			
			fclose(fp);
			fclose(fq);
			fclose(fr);
			
			return phase; //there is a scope here to check another phase for a non interfering channel
			
			
			
		}
		
		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i + 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);		
		globalScheduleElements[k].slotToStart = currentSlot;
		globalScheduleElements[k].numSlots = 1;
		
		elementsToFree++;
		
		//********************************************************
		slotChannelNodes[currentSlot][channel][aPath->path[i]] = 1;
		slotChannelNodes[currentSlot][channel][aPath->path[i+1]] = 1;	
		//********************************************************
		
		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot,channel,aPath->path[i],slotChannelNodes[currentSlot][channel][aPath->path[i]]);
		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot,channel,aPath->path[i+1],slotChannelNodes[currentSlot][channel][aPath->path[i+1]]);
		
		k++;	
		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i + 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel1);		
		globalScheduleElements[k].slotToStart = currentSlot1;
		globalScheduleElements[k].numSlots = 1;
		
		elementsToFree++;
		
		//********************************************************
		slotChannelNodes[currentSlot1][channel1][aPath->path[i]] = 1;
		slotChannelNodes[currentSlot1][channel1][aPath->path[i+1]] = 1;	

		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot1,channel1,aPath->path[i],slotChannelNodes[currentSlot1][channel1][aPath->path[i]]);
		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot1,channel1,aPath->path[i+1],slotChannelNodes[currentSlot1][channel1][aPath->path[i+1]]);

		k++;		
		}	
	//manan voice message scheduling end
	}
	else
	{
	for(i=0; i<(aPath->numOfNodesInPath - 1);i++)
		{ //this loop runs till the second last node, nodes are numbered from 0 hence < noofnodesinpath -1
		if(i % 2 == 0)
			currentSlot = slot + 0;
		else
			currentSlot = slot + 1;
			
		channel = getNonInterferringChannel(aPath->path[i], aPath->path[i + 1], currentSlot);
		//there should be no interference on the hop on the channel on which the source of the hop is transmitting
		
		if(channel >= MAX_NO_CHANNELS)
		{
			fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
			
			noOfTotalFlowRequestsRejected++;
			flowRequestsRejected[globalCallsInProgress]++;
			
			fprintf(fr,"\n No_Channel_Found flowId %d globalCallsInProgress %d",flowId,globalCallsInProgress);
			//fprintf(stderr,"\n ***** loop 1 No_Channel_Found slot %d flowId %d globalCallsInProgress %d *****",currentSlot, flowId,globalCallsInProgress);			
			deAllocateState(flowId, elementsToFree, k);
			//does above call clear slotchannelnodes array?
			
			fclose(fp);
			fclose(fq);
			fclose(fr);
			return phase; //there is a scope here to check another phase for a non interfering channel
			
			//add nodes to bottleneck node list
			//return (current phase)
			
		}
		
		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i + 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);		
		globalScheduleElements[k].slotToStart = currentSlot;
		globalScheduleElements[k].numSlots = 1;
		
		elementsToFree++;
		
		//********************************************************
		slotChannelNodes[currentSlot][channel][aPath->path[i]] = 1;
		slotChannelNodes[currentSlot][channel][aPath->path[i+1]] = 1;	
		//********************************************************
		
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot,channel,aPath->path[i],slotChannelNodes[currentSlot][channel][aPath->path[i]]);
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot,channel,aPath->path[i+1],slotChannelNodes[currentSlot][channel][aPath->path[i+1]]);
		
		//fprintf(stderr,"\nGlobal Scheduling element %d\n",k);
		//fprintf(stderr," Transmitter %d Receiver %d Flowid_channel %d slot_to_start %d num_slots %d\n",globalScheduleElements[k].transmitter,globalScheduleElements[k].receiver,globalScheduleElements[k].flowId_channel,globalScheduleElements[k].slotToStart,globalScheduleElements[k].numSlots);	
		//manan end		
		k++;		
		}	

	for(i=1; i<=(aPath->numOfNodesInPath - 1);i++)
		{
		//this loop runs till the last node, nodes are numbered from 0, hence <= noofnodesinpath-1 
		if(i % 2 == 0)
			currentSlot = slot + 2;
		else
			currentSlot = slot + 3;
			
		channel = getNonInterferringChannel(aPath->path[i],aPath->path[i - 1], currentSlot);

		if(channel >= MAX_NO_CHANNELS)
		{
					
			noOfTotalFlowRequestsRejected++;
			flowRequestsRejected[globalCallsInProgress]++;

			fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
			fprintf(fr,"\n No_Channels_Found flowId %d globalCallsInProgress %d",flowId, globalCallsInProgress);
			//fprintf(stderr,"\n ***** loop 2 No_Channel_Found slot %d flowId %d globalCallsInProgress %d *****",currentSlot, flowId,globalCallsInProgress);			
			deAllocateState(flowId, elementsToFree, k);
			//does above call clear slotchannelnodes array?
			
			fclose(fp);
			fclose(fq);
			fclose(fr);
			return phase; //there is a scope here to check another phase for a non interfering channel
			
			//add nodes to bottleneck node list
			//return (current phase)
		}
		
		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i - 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);
		globalScheduleElements[k].slotToStart = currentSlot;
		globalScheduleElements[k].numSlots = 1;	

		elementsToFree++;
		
		//********************************************************
		slotChannelNodes[currentSlot][channel][aPath->path[i]] = 1;
		slotChannelNodes[currentSlot][channel][aPath->path[i-1]] = 1;
		//********************************************************
		
		
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot,channel,aPath->path[i],slotChannelNodes[currentSlot][channel][aPath->path[i]]);
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",currentSlot,channel,aPath->path[i-1],slotChannelNodes[currentSlot][channel][aPath->path[i-1]]);
		
		//fprintf(stderr,"\nGlobal Scheduling element %d\n",k);
		//fprintf(stderr," Transmitter %d Receiver %d Flowid_channel %d slot_to_start %d num_slots %d\n",globalScheduleElements[k].transmitter,globalScheduleElements[k].receiver,globalScheduleElements[k].flowId_channel,globalScheduleElements[k].slotToStart,globalScheduleElements[k].numSlots);
				
		k++;
		
		}	
	}//the else of voice messaging ends here
	
	//VJ Start
	//nodeID[ROOT].numSchedElem = k; // update the number of scheduling elements
	globalNumSchedElem = k;
	//VJ End
	
	if(voiceMsg == 1)
	{
	for(l=prevSchedElem;l<k;l++)
	{
	fprintf(vm,"\nGlobal Scheduling element %d\n",l);
	fprintf(vm," Transmitter %d Receiver %d Flowid_channel %d slot_to_start %d num_slots %d\n",globalScheduleElements[l].transmitter,globalScheduleElements[l].receiver,globalScheduleElements[l].flowId_channel,globalScheduleElements[l].slotToStart,globalScheduleElements[l].numSlots);
	}
	}

	setScheduleInfo();//just sets schedulePacketChange to true // important step
		
	flowRequestsAdmitted[globalCallsInProgress]++;	
	globalCallsInProgress++;
	noOfCallsAdmitted++;
	noOfCallsAdmitted_Unique[flowId] = 1;


	//fprintf(stderr,"\n Flow admitted %d totalCallsAdmitted %d simTime %lf milliseconds %lf minutes\n\n",flowId, noOfCallsAdmitted, simTime, simTime/(1000*60));
	//printf("\n Flow admitted %d simTime %lf \n",flowId, simTime);
	
	fprintf(stderr,"\n Flow admitted %d totalCallsAdmitted %d",flowId, noOfCallsAdmitted);
	
	fa = fopen("outputFiles/channelInfo.txt","a+");
	fprintf(fa,"\n Call admitted");
	fclose(fa);
	
	//printSlotChannelNodes();	
	
	fprintf(fq,"\n %d \t %d \t %d \t %lf", noOfCallsAdmitted, flowId, globalCallsInProgress, simTime);

	//fprintf(stderr,"\n noOfCallsAdmitted %d \t flowId %d \t globalCallsInProgress %d \t simTime %lf\n", noOfCallsAdmitted, flowId, globalCallsInProgress, simTime);

	fprintf(fp,"\n %d %d %d %lf", noOfCallsAdmitted, globalCallsInProgress, 1, simTime);
		
	fclose(fp);
	fclose(fq);
	fclose(fr);

	if(voiceMsg == 1)
	{
	fprintf(vm,"Just done scheduling for voice messages for flowId %d\n",flowId);
	}
	//fclose(vm);
	return (-1);
	
}

//manan change*****************************************************************************8
void setCarryForwardFlow(int new_source, int temp_destination, int overalle2eDestination, int carryForward, int flowId)
{
int i,flow_found,tempFlowId;
int retVal;
flow_found = 0;
retVal = 0;
struct routingPath * aPath;
aPath = getRoutingPath(new_source,temp_destination);
//hopDistribution[aPath->numOfNodesInPath-1]++;

for(i=0;i<numOfFlowsInProgress;i++)
{
	if(flowStat[i].flowId == flowId)
	{
	flow_found = 1;
	flowStat[i].source = new_source;
	flowStat[i].destination = temp_destination; // updated later at the root
	flowStat[i].flowId = flowId;
	flowStat[i].voiceFlow = 1;
	flowStat[i].aPath = aPath;
	}
}

if(flow_found == 1) // this is for handling the condition that 
{}
else
{
flowStat[numOfFlowsInProgress].source = new_source;
flowStat[numOfFlowsInProgress].destination = temp_destination; // updated later at the root
flowStat[numOfFlowsInProgress].flowId = flowId;
flowStat[numOfFlowsInProgress].voiceFlow = 1;
flowStat[numOfFlowsInProgress].aPath = aPath;
numOfFlowsInProgress++;
}

nodeID[new_source].callRequestSent = 1;  
nodeID[new_source].currentFlowId = flowId;
nodeID[new_source].callInProgress = FALSE;
nodeID[new_source].flowTearedDown = FALSE;
numFlowRequestsReached[flowId] = 1;		

//fprintf(stderr,"The details of carry forward go this way\n");
//fprintf(stderr,"Source %d Destination %d Flowid %d Call Duration %lf minutes\n",new_source,temp_destination,flowId, nodeID[new_source].callDuration/(1000*60));
//exit(0);
		
	retVal = setSchedulingElements(new_source, temp_destination,flowId, retVal, carryForward, -1, NULL);
	while(retVal != (-1) && retVal < (MAX_PHASES-1))
	{
	retVal = setSchedulingElements(new_source, temp_destination,flowId, retVal + 1, carryForward, -1, NULL);
	fprintf(stderr,"This printf from within the setCarryForwardFlow should ideally never be called\n");
	exit(0);
	}	

	int k;
	flow_found = 0;
	
	//VJ Start
	//k = nodeID[ROOT].numSchedElem;
	k = globalNumSchedElem;
	//VJ End

	for(i=0; i<k; i++)
	{
		if(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
		//fprintf(stderr,"\n flow allocated in the check of SetCarryForwardFlow\n");
		flow_found = 1;
		break;
		}
	}
	//adding the information to voiceCurrQueue
	if(flow_found == 1)
	{
	
	if(numPathsInVoiceCurrQueue < 50)
	{

	voiceCurrQueue[numPathsInVoiceCurrQueue].source = new_source;
	voiceCurrQueue[numPathsInVoiceCurrQueue].destination = temp_destination;
	voiceCurrQueue[numPathsInVoiceCurrQueue].overalle2eDestination = overalle2eDestination;
	voiceCurrQueue[numPathsInVoiceCurrQueue].voiceFlowId = flowId;
	voiceCurrQueue[numPathsInVoiceCurrQueue].voiceMsgDuration = nodeID[new_source].callDuration + bufferVoiceMsgTime(); // portability change
	voiceCurrQueue[numPathsInVoiceCurrQueue].voiceCopyCallDuration = nodeID[new_source].callDuration;
	//extracting phase information to add to voiceCurrQueue
		for(i=0; i<numOfFlowsInProgress; i++)
		{
			if(flowStat[i].flowId == flowId)
			{
			voiceCurrQueue[numPathsInVoiceCurrQueue].voice_phase = flowStat[i].phase;
			break;
			}
		}
	voiceCurrQueue[numPathsInVoiceCurrQueue].rPath = getRoutingPath(new_source,temp_destination);
	
	//fprintf(stderr,"\n voiceCurrQueue 2 source %d destination %d overalle2eDestination %d flow id %d duration  %lf copyCallDuration %lf phase %d \n", voiceCurrQueue[numPathsInVoiceCurrQueue].source, voiceCurrQueue[numPathsInVoiceCurrQueue].destination, voiceCurrQueue[numPathsInVoiceCurrQueue].overalle2eDestination,   voiceCurrQueue[numPathsInVoiceCurrQueue].voiceFlowId, voiceCurrQueue[numPathsInVoiceCurrQueue].voiceMsgDuration, voiceCurrQueue[numPathsInVoiceCurrQueue].voiceCopyCallDuration, voiceCurrQueue[numPathsInVoiceCurrQueue].voice_phase);
				
	//fprintf(stderr,"Routing path is \n");
	for(i=0;i<voiceCurrQueue[numPathsInVoiceCurrQueue].rPath->numOfNodesInPath;i++)
	{
		//fprintf(stderr,"%d ",voiceCurrQueue[numPathsInVoiceCurrQueue].rPath->path[i]);
	}	

	numPathsInVoiceCurrQueue++;
	
	}
	else
	{
	fprintf(stderr,"System overloaded with current voice messages %d\n",numPathsInVoiceCurrQueue);
	exit(0);
	} 
	}
	else
	{
		fprintf(stderr,"shouldnt reach here ideally in setCarryForwardFlow\n");
		exit(0);
	}

	//adding the logging code here, may be important in future
	
	for(i=0; i<aPath->numOfNodesInPath; i++)
	{
		if(nodeStat[aPath->path[i]].endCallTime > simTime)
		{
			if(nodeStat[aPath->path[i]].endCallTime > (nodeStat[aPath->path[i]].upTime + (double)nodeID[new_source].callDuration))
			{
				nodeStat[aPath->path[i]].upTime += (nodeStat[aPath->path[i]].endCallTime - (nodeStat[aPath->path[i]].upTime + (double)nodeID[new_source].callDuration));
				nodeStat[aPath->path[i]].endCallTime = simTime + nodeStat[aPath->path[i]].endCallTime - (nodeStat[aPath->path[i]].upTime + (double)nodeID[new_source].callDuration);
			}
			else
			{
				nodeStat[aPath->path[i]].upTime += ((nodeStat[aPath->path[i]].upTime + (double)nodeID[new_source].callDuration) - nodeStat[aPath->path[i]].endCallTime);
				nodeStat[aPath->path[i]].endCallTime = simTime + (nodeStat[aPath->path[i]].upTime + (double)nodeID[new_source].callDuration) - nodeStat[aPath->path[i]].endCallTime;
			}
		}
		else
		{
			nodeStat[aPath->path[i]].upTime += nodeID[new_source].callDuration;
			nodeStat[aPath->path[i]].endCallTime = simTime + nodeID[new_source].callDuration;
			//so by these means initially 
		}				
	}
	
	logFlowStats(flowId,"\n Source",(double) new_source);
	logFlowStats(flowId,"\n Destination",(double) temp_destination);
	logFlowStats(flowId,"\n No_Of_Hops",(double) aPath->numOfNodesInPath-1);
	
	logFlowStats(flowId,"\n Parent_Node",(double)nodeID[new_source].parent);
	logFlowStats(flowId,"\n Parent_Depth",(double)nodeID[nodeID[new_source].parent].depth);
	logFlowStats(flowId,"\n Time_Flow_Request_Sent",simTime);
	logFlowStats(flowId,"\n Call_Duration", (double) nodeID[new_source].callDuration);
	logFlowStats(flowId,"\n Voice Message Call : Yes");
	
	
}
//manan end
//***********************************************

void reshuffleVoiceQueue(int voiceQueueFlowId)
{
int i,j,k;
struct voiceMsgQueue temp;

for(i=0;i<numPathsInVoiceQueue;i++)
	{
	if(voiceQueue[i].voiceQueueFlowId == voiceQueueFlowId)
		{
		temp = voiceQueue[i];
		voiceQueue[i] = voiceQueue[numPathsInVoiceQueue-1];
		voiceQueue[numPathsInVoiceQueue-1] = temp;
		}
	}
numPathsInVoiceQueue--;

//fprintf(stderr,"printing contents in reshuffleVoiceQueue\n");
struct routingPath * aPath;
for(i=0;i<numPathsInVoiceQueue;i++)
{
aPath = getRoutingPath(voiceQueue[i].source,voiceQueue[i].destination);
//fprintf(stderr,"voiceFlowId %d routing path no. %d in voiceQueue is: ",voiceQueue[i].voiceQueueFlowId,i);
for(j=0;j<aPath->numOfNodesInPath;j++)
{
//fprintf(stderr,"%d ",aPath->path[j]);
}
//fprintf(stderr,"\n");
}

}

//*****************************manan change start**************************
void checkVoiceQueue()
{

int i,bottleneck;

if(numPathsInVoiceQueue > 0)
	{
		//check which route can be scheduled, //below, code exists for scheduling flow properly//take necessary parts
		for(i=0;i<numPathsInVoiceQueue;i++)
		{
		bottleneck = getVoiceDestination(voiceQueue[i].source, voiceQueue[i].destination);
		
		//portability change
		if((destinationIsUp(voiceQueue[i].destination) == FAILURE) || (nodeID[voiceQueue[i].destination].callInProgress == TRUE))
		{
		bottleneck = -1;
		//fprintf(stderr,"Either the destination %d is down, or it is on another call %d \n", voiceQueue[i].destination, nodeID[voiceQueue[i].destination].callInProgress);
		}
		else
		{
		//do nothing
		}
		
		if(bottleneck == -1)
		{
		//fprintf(stderr,"do not schedule\n");
		}
		else
		{
			//fprintf(stderr,"bottleneck is getting relieved, can schedule\n");
			//fprintf(stderr,"parameters are source %d destination %d bottleneck %d callDuration %lf\n",voiceQueue[i].source, voiceQueue[i].destination, bottleneck,voiceQueue[i].callDuration);
			
			//copying code from blow segment to carry forward a voice message
			nodeID[voiceQueue[i].source].callDuration = voiceQueue[i].callDuration;
			nodeID[voiceQueue[i].source].voiceFlow = 1;
			//nodeID[voiceQueue[i].source].voiceCopyCallDuration = nodeID[voiceQueue[i].source].callDuration; 

			//updating storage capacity
			nodeID[voiceQueue[i].source].store_cap -= ((voiceQueue[i].callDuration/30)*24); 
			//logging this update
			logStoreCap(voiceQueue[i].source,nodeID[voiceQueue[i].source].store_cap,simTime);

			//hopDistribution[voiceQueue[i].rPath->numOfNodesInPath-1]++;//what if a new bottleneck has come up
			if(voiceQueue[i].source >= numOfInfraNodesInNetwork) // changed 2-10-09
			setCarryForwardFlow(voiceQueue[i].source,bottleneck, voiceQueue[i].overalle2eDestination, 0,voiceQueue[i].voiceQueueFlowId);
			else
			setCarryForwardFlow(voiceQueue[i].source,bottleneck, voiceQueue[i].overalle2eDestination, 1,voiceQueue[i].voiceQueueFlowId);

			//numPathsInVoiceQueue--;
			reshuffleVoiceQueue(voiceQueue[i].voiceQueueFlowId);	
			
		}
			
		}
	}
}
//manan change end**************************************************

int revokeElements(int flowId)
{
	int i, j, k, l,m;
	int elementsToRemove, phase;
	int flowFound,voiceMsg;
	int bottleneck;
	voiceMsg = 0;
	struct routingPath * aPath;
	FILE *ft;
	//vm = fopen("voiceMsg.txt","a+");	

	//VJ Start
	//k = nodeID[ROOT].numSchedElem; //****imp stmt******
	k = globalNumSchedElem;
	//VJ End
	
	flowFound = 0;
	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			//manan change
			if(flowStat[i].voiceFlow == 1)
			{
			//fprintf(stderr,"revoke elements called for voice flow id %d\n",flowStat[i].flowId);
			fprintf(vm,"revoke elements called for voice flow id %d\n",flowStat[i].flowId);
			voiceMsg = 1;
			}
			//manan end
			flowFound = 1;			
			//aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination); //need to disable this 
			aPath = flowStat[i].aPath; // portability change
			phase = flowStat[i].phase;
			//portability change, setting callInProgess of the destination to false
			//nodeID[flowStat[i].destination].callInProgress = FALSE;
			//fprintf(stderr,"call in progress set to false for receiver %d \n", flowStat[i].destination);
			break;
		}		
	}

	if(flowFound == 0)
	{
		//fprintf(stderr,"\n Flow %d could not be found, possibly already deallocated",flowId);
		//printf("\n Flow %d could not be found, possibly already deallocated",flowId);
		
		return SUCCESS;
	}
	
	for(i=0; i<k; i++)
	{
		if(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		//(globalScheduleElements[i].transmitter == aPath->path[0] && globalScheduleElements[i].receiver == aPath->path[1] && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
			break;
	}	
	
	if(i == k)
	{
		//fprintf(stderr,"\n Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		//printf("\n Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		
		return SUCCESS;	
		exit(0);
	}
	
	elementsToRemove = (aPath->numOfNodesInPath * 2 - 2);	
	
	j = i + elementsToRemove;

	ft = fopen("outputFiles/channelInfo.txt","a+");
	//manan change
	if(voiceMsg == 1)
	{
	//decrementing the global variable numPathsInVoiceCurrQueue, moved to reshuffleVoiceFlowQueue
	//numPathsInVoiceCurrQueue--;	

	int slot, channel, node;

		for(l=0; l<(aPath->numOfNodesInPath - 1);l++,i++)
		{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;

		slot = globalScheduleElements[i].slotToStart;
		channel = EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel);
		
		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l],slotChannelNodes[slot][channel][aPath->path[l]]);
		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l+1],slotChannelNodes[slot][channel][aPath->path[l+1]]);
		
		i++;

		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;

		slot = globalScheduleElements[i].slotToStart;
		channel = EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel);

		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l],slotChannelNodes[slot][channel][aPath->path[l]]);
		fprintf(vm,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l+1],slotChannelNodes[slot][channel][aPath->path[l+1]]);
		
		}

	}
	else
	{
	int slot, channel, node;

		for(l=0; l<(aPath->numOfNodesInPath - 1);l++,i++)
		{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;

		slot = globalScheduleElements[i].slotToStart;
		channel = EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel);

		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l],slotChannelNodes[slot][channel][aPath->path[l]]);
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l+1],slotChannelNodes[slot][channel][aPath->path[l+1]]);
		}
	
	for(l=1; l<=(aPath->numOfNodesInPath - 1);l++,i++)
		{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l-1]] = 0;

		slot = globalScheduleElements[i].slotToStart;
		channel = EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel);

		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l],slotChannelNodes[slot][channel][aPath->path[l]]);
		//fprintf(stderr,"Slot %d Channel %d Node %d slotChannelNodes %d\n",slot,channel,aPath->path[l-1],slotChannelNodes[slot][channel][aPath->path[l-1]]);
		}
	}
	
	if(i != j)
	{
		fprintf(stderr,"\n Removing elements: i %d j %d",i,j);
		exit(0);
	}
	
	/*		
	for(;i<j;i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].transmitter] = 0;
		fprintf(ft,"\n slot %d channel %d tx %d",globalScheduleElements[i].slotToStart, EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel), globalScheduleElements[i].transmitter);		
	}
	*/

	//fprintf(stderr,"global scheduling elements %d being removed for flowId %d\n",elementsToRemove,flowId);	
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

	fprintf(ft,"\n");
	//fprintf(stderr,"\n");
	
	//manan change
	if(voiceMsg == 1)
	{
		if(nodeID[aPath->path[0]].type == INFRA)
		phasesPerNode[aPath->path[0]][phase] = 0;
	
		for(j=1;j<aPath->numOfNodesInPath-1;j++)
		{
			phasesPerNode[aPath->path[j]][phase] = 0;
		}
		
		if(nodeID[aPath->path[aPath->numOfNodesInPath-1]].type == INFRA)
			phasesPerNode[aPath->path[aPath->numOfNodesInPath-1]][phase] = 0;
		
		for(j=0;j<=aPath->numOfNodesInPath-1;j++)
		{
		//fprintf(stderr,"\nNode %d Phase %d phasesPerNode %d\n",aPath->path[j],phase,phasesPerNode[aPath->path[j]][phase]);
		}
	
	}
	else
	{
		for(j=1; j<(aPath->numOfNodesInPath - 1); j++)
		{
		fprintf(ft,"\t %d",aPath->path[j]);
		phasesPerNode[aPath->path[j]][phase] = 0;	
		//fprintf(stderr,"Node %d Phase %d Phases per node %d\n",aPath->path[j],phase,phasesPerNode[aPath->path[j]][phase]);
		}//deallocating phases for all the intermediate nodes i guess
	}
	char *str = "deallocate flows called from revoke elements";
	deAllocateFlowsForNodes(aPath, voiceMsg, str);

	reshuffleFlowQueue(flowId);
	
	//printing flow per node
	for(i=0;i<aPath->numOfNodesInPath;i++)
	{
	//fprintf(stderr,"Node %d Flows per node %d\n",aPath->path[i], flowsPerNode[aPath->path[i]]);
	}
	//printing ends

	globalCallsInProgress--;
	
	printSlotChannelNodes();

	setScheduleInfo();
	
	//manan change

	if(voiceMsg == 1)
	{
	struct routingPath * voicepath;
	int maxFlows, numOfNodesInPath, new_source, new_destination, tempFlowId,bottleneck_found,temp_destination, voice_curr_queue_counter;
	bottleneck_found = 0;

	//fprintf(stderr,"routing path passed in the revoke elements is \n");
	for(i=0;i<aPath->numOfNodesInPath;i++)
	{
	//fprintf(stderr,"%d  ",aPath->path[i]);
	}

	for(voice_curr_queue_counter = 0; voice_curr_queue_counter < numPathsInVoiceCurrQueue; voice_curr_queue_counter++)
	{
		if(voiceCurrQueue[voice_curr_queue_counter].voiceFlowId == flowId)
		break;
	}
	//fprintf(stderr,"\nthe value of e2eDest for comparison is %d\n",voiceCurrQueue[voice_curr_queue_counter].overalle2eDestination);		
	//fprintf(stderr,"after modifications, reachin to revoke elements, stopping here\n");

	//fprintf(stderr,"for carry forward, path being found between %d %d and %d\n", aPath->path[aPath->numOfNodesInPath-1], voiceCurrQueue[voice_curr_queue_counter].destination, voiceCurrQueue[voice_curr_queue_counter].overalle2eDestination);

	//exit(0);
	new_source = aPath->path[aPath->numOfNodesInPath-1];
	new_destination = voiceCurrQueue[voice_curr_queue_counter].overalle2eDestination;
	
	voicepath = getRoutingPath(new_source,new_destination);
	numOfNodesInPath = voicepath->numOfNodesInPath;	
	if(numOfNodesInPath == 1)
	{
	//fprintf(stderr,"voice flow termination reached\n");
	fprintf(voice_end,"FlowId %d End_Time %lf \n",flowId,simTime);
	
	for(i=0; i<numVoiceMsg; i++)
	{
		if(voiceFlowStat[i].voiceFlowId == flowId)
		{
			voiceFlowStat[i].end_time = simTime;
		}
	}
	reshuffleVoiceCurrQueue(voiceCurrQueue[voice_curr_queue_counter].voiceFlowId);
	
	/*int i;
	for(i=0;i<numOfFlowsInProgress;i++)
	fprintf(stderr,"flowid %d source %d destination %d\n",flowStat[i].flowId,flowStat[i].source,flowStat[i].destination);*/
	checkVoiceQueue(); // check if there are any messages left in the voice queue and if yes, then schedule
	
	return SUCCESS; // this will probably return back to voice_msg_check event
	}
	
	//putting hacks, delete them later*******************
	/*int hack;
	if(flowsPerNode[3] == 4)
	{flowsPerNode[3] = 0;
	
	for(hack=0;hack<MAX_PHASES;hack++)
	phasesPerNode[3][hack] = 0;
	
	for(hack=0;hack<MAX_PHASES;hack++)
	phasesPerNode[1][hack] = 1;
	flowsPerNode[1] = 4;
	}
	else{
	//flowsPerNode[2] = 0;
	flowsPerNode[1] = 0; 
	for(hack=0;hack<MAX_PHASES;hack++)
	phasesPerNode[1][hack] = 0;
	}*/
	// hack over***********************************************
	
	// changed 1-10-09///////////////////////////////
	if(new_destination == -1)
	{
	//fprintf(stderr,"voice copy call duration %f\n",nodeID[aPath->path[0]].voiceCopyCallDuration);
	fprintf(stderr,"voice flow %d\n",nodeID[aPath->path[0]].voiceFlow);
	fprintf(stderr,"source %d destination %d\n",new_source,new_destination);
	exit(0);
	}///////////////////////////////////////////////
	
	temp_destination = getVoiceDestination(new_source,new_destination);
	
	//portability change
	if((destinationIsUp(new_destination) == FAILURE) || (nodeID[new_destination].callInProgress == TRUE))
	{
	temp_destination = -1;
	//fprintf(stderr,"Either the destination %d is down, or it is on another call %d \n", new_destination, nodeID[new_destination].callInProgress);
	//exit(0); // successfully reached here in one test case
	}
	else
	{
	//do nothing
	}
		
	//fprintf(stderr,"value returned by getVoiceDestination is %d\n",temp_destination);
	//exit(0);

	if(nodeID[voicepath->path[0]].type == INFRA)
	{
		if(flowsPerNode[voicepath->path[0]]+1 > MAX_PHASES)
		{
		fprintf(vm,"revoke severe error, exiting\n");
		fprintf(stderr,"revoke severe error, exiting\n");
		exit(0);
		}
	}

	if(temp_destination != -1)
	{		
		int retVal;
		retVal = 0;

		voiceCurrQueue[voice_curr_queue_counter].source = new_source;
		voiceCurrQueue[voice_curr_queue_counter].destination = temp_destination;
		//voiceCurrQueue[voice_curr_queue_counter].voiceMsgDuration =
		//voiceCurrQueue[voice_curr_queue_counter].voiceCopyCallDuration;
		voiceCurrQueue[voice_curr_queue_counter].voiceMsgDuration = 60000 + bufferVoiceMsgTime(); // portability change
		voiceCurrQueue[voice_curr_queue_counter].rPath = getRoutingPath(new_source,temp_destination);

		hopDistribution[voiceCurrQueue[voice_curr_queue_counter].rPath->numOfNodesInPath-1]++;

		flowStat[numOfFlowsInProgress].source = new_source;
		flowStat[numOfFlowsInProgress].destination = temp_destination; // updated later at the root
		flowStat[numOfFlowsInProgress].flowId = flowId;
		flowStat[numOfFlowsInProgress].voiceFlow = 1;
		flowStat[numOfFlowsInProgress].aPath = getRoutingPath(new_source,temp_destination); // portability change
		numOfFlowsInProgress++;
		
		//nodeID[new_source].callDuration = voiceCurrQueue[voice_curr_queue_counter].voiceCopyCallDuration;
		nodeID[new_source].callDuration = 60000;
		nodeID[new_source].voiceFlow = 1;
		nodeID[new_source].callRequestSent = 1;  
		nodeID[new_source].currentFlowId = flowId;
		nodeID[new_source].callInProgress = FALSE;
		nodeID[new_source].flowTearedDown = FALSE;
		numFlowRequestsReached[flowId] = 1;		

		//fprintf(stderr,"The details of carry forward go this way\n");
		//fprintf(stderr,"Source %d Destination %d Flowid %d Call Duration %lf minutes\n",new_source,temp_destination,flowId, nodeID[new_source].callDuration/(1000*60));
	
		for(i=0; i<numVoiceMsg; i++)
		{
			if(voiceFlowStat[i].voiceFlowId == flowId)
			{
				voiceFlowStat[i].fragmented++;
			}
		}
		retVal = setSchedulingElements(new_source, temp_destination,flowId, retVal, 1, -1, NULL);
		while(retVal != (-1) && retVal < (MAX_PHASES-1))
		{
		retVal = setSchedulingElements(new_source, temp_destination,flowId, retVal + 1, 1, -1, NULL);
		fprintf(stderr,"This printf from within the setCarryForwardFlow in revoke elements should ideally never be called\n");
		exit(0);
		}	
		
		if(retVal != -1)
		{
		fprintf(stderr,"set scheduling elements in revoke elements should have gone true\n");
		exit(0);
		}

		//now extracting phase information and adding it to voiceCurrQueue
		for(i=0;i<numOfFlowsInProgress;i++)
		{
			if(flowStat[i].flowId == flowId)
			{
			voiceCurrQueue[voice_curr_queue_counter].voice_phase = flowStat[i].phase;
			break;
			}
		}
	}
	else
	{

		if(numPathsInVoiceQueue < 50)
		{

		voiceQueue[numPathsInVoiceQueue].source = new_source;
		voiceQueue[numPathsInVoiceQueue].destination = new_destination;
		voiceQueue[numPathsInVoiceQueue].overalle2eDestination = new_destination;
		//voiceQueue[numPathsInVoiceQueue].callDuration = voiceCurrQueue[voice_curr_queue_counter].voiceCopyCallDuration;
		voiceQueue[numPathsInVoiceQueue].callDuration = 60000;
		voiceQueue[numPathsInVoiceQueue].voiceQueueFlowId = flowId;
		voiceQueue[numPathsInVoiceQueue].voice_phase = -1; //for now
		
		//for calculating the storage capacity
		nodeID[new_source].store_cap += ((60000/30)*24);
		//logging this update
		logStoreCap(voiceQueue[numPathsInVoiceQueue].source,nodeID[voiceQueue[numPathsInVoiceQueue].source].store_cap,simTime);

		numPathsInVoiceQueue++;

		for(i=0; i<numVoiceMsg; i++)
		{
			if(voiceFlowStat[i].voiceFlowId == flowId)
			{
				voiceFlowStat[i].fragmented++;
			}
		}

		reshuffleVoiceCurrQueue(voiceCurrQueue[voice_curr_queue_counter].voiceFlowId);

		//fprintf(stderr,"correctly executing here \n");
		}
		else
		{
		fprintf(stderr,"System overloaded in revoke elements in voiceQueue %d\n",numPathsInVoiceQueue);
		exit(0);
		}
	}
			
	}//end for if(voiceMsg == 1)	
	
	//code for checking whether a bottleneck node freed, and if voice mesg left, schedule accordingly
	checkVoiceQueue();
	//code for checking whether a bottleneck node freed, and if voice mesg left, schedule accordingly end

	//manan end

	fclose(ft);

	if(globalCallsInProgress == 0)
	{
		//printf("\n 0 calls in progress, all of the following elements should be zero");
		for(i=0; i<NO_OF_DATA_SLOTS; i++)
		{
			for(j=0; j<MAX_NO_CHANNELS; j++)
			{
				for(k=0; k<MAX_NO_NODES; k++)
				{
					if(slotChannelNodes[i][j][k] > 0)
					{
						//printf("\n slot %d channel %d node %d: %d",i,j,k,slotChannelNodes[i][j][k]);
					}	
					slotChannelNodes[i][j][k] = 0;
				}
			}
		}
	}
	
	
	return SUCCESS;
}

//*****************************
int getFreeSlot(int tx, int rx)
{
	int slot;
	
	for(slot = 0; slot < NO_OF_DATA_SLOTS; slot++)
	{
		if(slotsPerNode[tx][slot] == 0 && slotsPerNode[rx][slot] == 0)
			return slot;
	}
	
	return slot;
}

int checkFlowAdmission(struct routingPath *aPath)
{
	int l;
	int currentSlot;
	
	for(l=0; l<(aPath->numOfNodesInPath - 1);l++)
	{
		currentSlot = getFreeSlot(aPath->path[l], aPath->path[l+1]);
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			return FALSE;
		}
	}
	
	for(l=1; l<=(aPath->numOfNodesInPath - 1);l++)
	{	
		currentSlot = getFreeSlot(aPath->path[l], aPath->path[l-1]);
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*int revokeElements2(int flowId)
{
	int i, j, k, l;
	int elementsToRemove, phase;
	int flowFound;
	struct routingPath * aPath;
	FILE *ft;
	int flowIdFound;
	int slotIndex;
	int currentSlot;
	
	k = nodeID[ROOT].numSchedElem;
	
	flowFound = 0;
	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			flowFound = 1;			
			aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination);
			flowIdFound = i;
			break;
		}		
	}
	
	if(flowFound == 0)
	{
		fprintf(stderr,"\n Flow %d could not be found, possibly already deallocated",flowId);
		return SUCCESS;
	}
	
	
	for(i=0; i<k; i++)
	{
		if(globalScheduleElements[i].transmitter == aPath->path[0] && globalScheduleElements[i].receiver == aPath->path[1] && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
			break;
	}	
	
	if(i == k)
	{
		fprintf(stderr,"\n Revoking elements: flow %d found, no sched elements found, must be timedout\n",flowId);
		return SUCCESS;	
		//exit(0);
	}
	
	elementsToRemove = (aPath->numOfNodesInPath * 2 - 2);	
	
	j = i + elementsToRemove;

	ft = fopen("channelInfo.txt","a+");

	slotIndex = 0;

	for(l=0; l<(aPath->numOfNodesInPath - 1);l++,i++)
	{
		currentSlot = flowStat[flowIdFound].slotArray[slotIndex];
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			fprintf(stderr,"\n flowIdfound %d currentSlot %d slotIndex %d numOfNodes %d",flowStat[flowIdFound].flowId, currentSlot, slotIndex, aPath->numOfNodesInPath);
			exit(0);
		}
		
		slotIndex++;
		slotsPerNode[aPath->path[l]][currentSlot] = 0;
		slotsPerNode[aPath->path[l+1]][currentSlot] = 0;
	
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l+1]] = 0;
	}
	
	for(l=1; l<=(aPath->numOfNodesInPath - 1);l++,i++)
	{	
		currentSlot = flowStat[flowIdFound].slotArray[slotIndex];
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			fprintf(stderr,"\n flowIdfound %d currentSlot %d slotIndex %d numOfNodes %d",flowStat[flowIdFound].flowId, currentSlot, slotIndex, aPath->numOfNodesInPath);
			exit(0);
		}
				
		slotIndex++;
		slotsPerNode[aPath->path[l]][currentSlot] = 0;
		slotsPerNode[aPath->path[l-1]][currentSlot] = 0;
		
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l]] = 0;
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][aPath->path[l-1]] = 0;
	}
	
	if(i != j)
	{
		fprintf(stderr,"\n Sanity check failed while Removing elements: i %d j %d",i,j);
		exit(0);
	}
	
		
	if(j == k) //elements to be removed are the last global elements 
	{
		k -= elementsToRemove;
	}
	else
	{
		for(i = j; i<k; i++)
		{
			globalScheduleElements[i-elementsToRemove] = globalScheduleElements[i];			
		}	
		k -= elementsToRemove;
	}
	nodeID[ROOT].numSchedElem = k;
	
	fprintf(ft,"\n");	
	fclose(ft);
		
	reshuffleFlowQueue(flowId);
	
	globalCallsInProgress--;
	
	//printSlotChannelNodes();
	
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
*/

//to add: slotIndex has to be input to this function
/*int deAllocateState2(int flowId, int elementsToFree, int numOfSchedElem)
{
	int flowFound;
	int i,j;
	struct routingPath * aPath;
	int flowIdFound;
	int slotIndex, currentSlot;
	
	flowFound = 0;	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			flowFound = 1;			
			aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination);
			flowIdFound = i;
			break;
		}		
	}
	
	if(flowFound == 0)
	{
		//sanity check applicable only when no flow time out at root occurs
		fprintf(stderr,"\n Sanity check failed inside deAllocateState: flowfound = 0");
		exit(0);
		
	}
			
	//free node-slot array, extract slots from flowStats	
	
	slotIndex = 0;
	for(i=0; i<(aPath->numOfNodesInPath - 1);i++)
	{
		currentSlot = flowStat[flowIdFound].slotArray[slotIndex];
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			fprintf(stderr,"\n flowIdfound %d currentSlot %d slotIndex %d numOfNodes %d",flowStat[flowIdFound].flowId, currentSlot, slotIndex, aPath->numOfNodesInPath);
			exit(0);
		}
		
		slotsPerNode[aPath->path[i]][currentSlot] = 0;
		slotsPerNode[aPath->path[i+1]][currentSlot] = 0;
		slotIndex++;
	}
	
	for(i=1; i<=(aPath->numOfNodesInPath - 1);i++)
	{
		currentSlot = flowStat[flowIdFound].slotArray[slotIndex];
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			fprintf(stderr,"\n flowIdfound %d currentSlot %d slotIndex %d numOfNodes %d",flowStat[flowIdFound].flowId, currentSlot, slotIndex, aPath->numOfNodesInPath);
			exit(0);
		}
		
		slotsPerNode[aPath->path[i]][currentSlot] = 0;
		slotsPerNode[aPath->path[i-1]][currentSlot] = 0;
		slotIndex++;
	}
	
	
	for(i=0; i<numOfSchedElem; i++)
	{
		if(globalScheduleElements[i].transmitter == aPath->path[0] && globalScheduleElements[i].receiver == aPath->path[1] && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
			break;
	}	
	
	if(i == numOfSchedElem && numOfSchedElem != nodeID[ROOT].numSchedElem)
	{
		fprintf(stderr,"\n flow found %d but no sched elem", flowId);
		exit(0);
		//return SUCCESS;
	}
	else
	{
		// do nothing
	}
	
	j = i + elementsToFree;
	
	for(;i<j;i++)
	{
		slotChannelNodes[globalScheduleElements[i].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[i].flowId_channel)][globalScheduleElements[i].transmitter] = 0;
	}
					
}
*/
//old function, not used currently

/*int setSchedulingElements2(int source, int destination, int flowId)
{
	int i,j,k;
	int flag;

	int flowFound, flowIdFound;
	int retVal;
	int currentSlot, channel;
	int slotIndex;
	
	int elementsToFree;
	int dropped;

	struct routingPath * aPath;
	
	FILE *fp, *fq, *fr, *fa;
	
	if(flowExists(flowId))
	{
		printf("\n flow exists flowId %d",flowId);
		fprintf(stderr,"\n Flow exists flowId %d",flowId);
		//exit(0);
		return SUCCESS;
	}

	flowFound = 0;	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId)
		{
			flowFound = 1;			
			flowIdFound = i;
			break;
		}		
	}
	
	if(flowFound == 0)
	{
		//sanity check applicable only when no flow time out at root occurs
		fprintf(stderr,"\n Sanity check failed inside setElements2: flowfound = 0");
		exit(0);
		
	}
	
	noOfTotalFlowRequestsReceived++;
	
	flowRequestsReceived[globalCallsInProgress]++;
	
	printf("\n flow in scheduler %d",flowId);
	
	fp = fopen("simCalls.txt","a+");
	fq = fopen("admitCalls.txt","a+");
	fr = fopen("scheduleInfo.txt","a+");
		
	//printf("\n root numSchedElem %d",nodeID[ROOT].numSchedElem);
	
	k = nodeID[ROOT].numSchedElem;
	
	elementsToFree = 0;

	aPath = getRoutingPath(source, destination);	
	
	retVal = checkFlowAdmission(aPath); //this call is idempotent
			
	if(retVal == FALSE)
	{
		fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
		
		noOfTotalFlowRequestsRejected++;
		flowRequestsRejected[globalCallsInProgress]++;
		
		fprintf(fr,"\n flow can not be admitted flowId %d globalCallsInProgress %d", flowId,globalCallsInProgress);
		fprintf(stderr,"\n flow can not be admitted flowId %d globalCallsInProgress %d", flowId,globalCallsInProgress);
		
		fclose(fp);
		fclose(fq);
		fclose(fr);
		
		return SUCCESS;
	}
	
	slotIndex = 0;
	//look where flowStat[i].phase is used
	
	for(i=0; i<(aPath->numOfNodesInPath - 1);i++)
	{
		//*******************************
		currentSlot = getFreeSlot(aPath->path[i], aPath->path[i+1]);
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			fprintf(stderr,"\n while scheduling flowId %d currentSlot %d",flowId, currentSlot);
			exit(0);
		}
				
		slotsPerNode[aPath->path[i]][currentSlot] = 1;
		slotsPerNode[aPath->path[i+1]][currentSlot] = 1;
		
		flowStat[flowIdFound].slotArray[slotIndex] = currentSlot;
		slotIndex++;
		//*******************************	
			
		channel = getNonInterferringChannel(aPath->path[i], aPath->path[i + 1], currentSlot);
		
		if(channel >= MAX_NO_CHANNELS)
		{
			fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
			
			noOfTotalFlowRequestsRejected++;
			flowRequestsRejected[globalCallsInProgress]++;
			
			fprintf(fr,"\n No_Channel_Found flowId %d globalCallsInProgress %d",flowId,globalCallsInProgress);
			fprintf(stderr,"\n ***** No_Channel_Found flowId %d globalCallsInProgress %d *****",flowId,globalCallsInProgress);
						
			deAllocateState2(flowId, elementsToFree, k);
			
			fclose(fp);
			fclose(fq);
			fclose(fr);
			return SUCCESS;
		}
		
		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i + 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);		
		globalScheduleElements[k].slotToStart = currentSlot;
		globalScheduleElements[k].numSlots = 1;
		
		elementsToFree++;
		
		//********************************************************
		slotChannelNodes[currentSlot][channel][aPath->path[i]] = 1;
		slotChannelNodes[currentSlot][channel][aPath->path[i+1]] = 1;	
		//********************************************************
				
		k++;			
	}	

	for(i=1; i<=(aPath->numOfNodesInPath - 1);i++)
	{
		//*******************************
		currentSlot = getFreeSlot(aPath->path[i], aPath->path[i-1]);
		if(currentSlot >= NO_OF_DATA_SLOTS)
		{
			fprintf(stderr,"\n while scheduling flowId %d currentSlot %d",flowId, currentSlot);
			exit(0);
		}
		
		slotsPerNode[aPath->path[i]][currentSlot] = 1;
		slotsPerNode[aPath->path[i-1]][currentSlot] = 1;
		
		flowStat[flowIdFound].slotArray[slotIndex] = currentSlot;
		slotIndex++;
		
		//*******************************	
			
		channel = getNonInterferringChannel(aPath->path[i],aPath->path[i - 1], currentSlot);

		if(channel >= MAX_NO_CHANNELS)
		{
					
			noOfTotalFlowRequestsRejected++;
			flowRequestsRejected[globalCallsInProgress]++;

			fprintf(fp,"\n %d %d %lf", globalCallsInProgress, 0, simTime);
			fprintf(fr,"\n No_Channels_Found flowId %d globalCallsInProgress %d",flowId, globalCallsInProgress);
			fprintf(stderr,"\n ***** No_Channels_Found flowId %d globalCallsInProgress %d *****",flowId, globalCallsInProgress);			
			
			deAllocateState2(flowId, elementsToFree, k);
			
			fclose(fp);
			fclose(fq);
			fclose(fr);
			return SUCCESS;
		}
		
		
		globalScheduleElements[k].transmitter = aPath->path[i];
		globalScheduleElements[k].receiver = aPath->path[i - 1];
		globalScheduleElements[k].flowId_channel = FLOWID_CHANNEL(flowId, channel);
		globalScheduleElements[k].slotToStart = currentSlot;
		globalScheduleElements[k].numSlots = 1;	

		elementsToFree++;
		
		//********************************************************
		slotChannelNodes[currentSlot][channel][aPath->path[i]] = 1;
		slotChannelNodes[currentSlot][channel][aPath->path[i-1]] = 1;
		//********************************************************
				
		k++;			
	}	

	nodeID[ROOT].numSchedElem = k;
	setScheduleInfo(simTime);
		
	flowRequestsAdmitted[globalCallsInProgress]++;	
	globalCallsInProgress++;
	noOfCallsAdmitted++;

	fprintf(stderr,"\n Flow admitted %d simTime %lf\n",flowId, simTime);
	//printf("\n Flow admitted %d simTime %lf\n",flowId, simTime);
	
	fa = fopen("channelInfo.txt","a+");
	fprintf(fa,"\n Call admitted");
	fclose(fa);
	
	//printSlotChannelNodes();	
	
	fprintf(fq,"\n %d \t %d \t %d \t %lf", noOfCallsAdmitted, flowId, globalCallsInProgress, simTime);
	fprintf(fp,"\n %d %d %d %lf", noOfCallsAdmitted, globalCallsInProgress, 1, simTime);
		
	fclose(fp);
	fclose(fq);
	fclose(fr);

}
*/

//*************************************

int reshuffleFlowQueue(int flowId)
{
	int i,j;

	for(i=0; i<numOfFlowsInProgress; i++)
	{
		if(flowStat[i].flowId == flowId)
			break;
	}
	if(i == numOfFlowsInProgress)
	{
		fprintf(stderr,"\n Sanity check failed inside reshuffleFlowQueue: no flow found");
		exit(0);
	}
	if(i == (numOfFlowsInProgress - 1)) //last element
	{
		numOfFlowsInProgress--;
		if(numOfFlowsInProgress == 0)
		{
			//globalSlotNo = UNDEFINED;
			//globalNextEventTime = UNDEFINED;
		}
		//printf("\n %d flow revoked, total flows %d",flowId, numOfFlowsInProgress);
		//fprintf(stderr,"\n %d flow revoked, total flows %d",flowId, numOfFlowsInProgress);
		return SUCCESS;
	}	
	for(j=i+1; j < numOfFlowsInProgress; j++) //shift elements
	{
		flowStat[i] = flowStat[j];
		i++;
	}
	numOfFlowsInProgress--;
	
	//printf("\n %d flow revoked, total flows %d",flowId, numOfFlowsInProgress);
	//fprintf(stderr,"\n %d flow revoked, total flows %d",flowId, numOfFlowsInProgress);
	
	if(numOfFlowsInProgress == 0)
	{
		//globalSlotNo = UNDEFINED;
		//globalNextEventTime = UNDEFINED;		
	}
}
