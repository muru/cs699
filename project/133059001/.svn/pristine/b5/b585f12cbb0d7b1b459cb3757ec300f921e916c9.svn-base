#include "header.h"

int initializeNodes()
{
	//Assuming root element is already up
	int index;
	for(index = 0; index < MAX_NO_NODES; index++)
	{
		if(index == ROOT)
		{
			//nodeID is a variable of the type struct nodeInfo defined in header.h 
			nodeID[index].localTime = 0;
			nodeID[index].offset = 0;
			nodeID[index].state = ALIVE;
		}

		nodeID[index].numNeighbors = 0;
		nodeID[index].numInterferringNeighbors = 0;

		if(index != ROOT)
			nodeID[index].state = DEAD;

		nodeID[index].sponsorDepth = UNDEFINED;
		nodeID[index].sponsor = UNDEFINED; // mobility change 2-5-10 
		nodeID[index].parent = UNDEFINED;
		nodeID[index].broadcastTurn = 0;
		//nodeID[index].bandwidthInPlace = FALSE;
		
		nodeID[index].currentFlowId = 0;
		//manan change
		//nodeID[index].currentVoiceFlowId = 0;
		//nodeID[index].voiceTempDestination = -1;
		//nodeID[index].voicee2eDest = -1;
		nodeID[index].voiceFlow = 0;
		nodeID[index].store_cap = 0;
		//nodeID[index].residueCallDuration = -1;
		//nodeID[index].voiceCarryForward = 0;
		//nodeID[index].voiceCopyCallDuration = 0;
		//manan end
		nodeID[index].flowTearedDown = TRUE;
		
		nodeID[index].synchronized = FALSE;
		nodeID[index].globalTime = 0;
		nodeID[index].localTime = 0;
		nodeID[index].offset = UNDEFINED;
		nodeID[index].lastScheduleTime = UNDEFINED;
		nodeID[index].lastScheduleMissed = FALSE; //UNDEFINED;
		nodeID[index].timeLastScheduleReception = UNDEFINED;
		//nodeID[index].lastScheduleChangeBroadcastTime = UNDEFINED;
		nodeID[index].scheduleInterval = UNDEFINED;
		nodeID[index].scheduleSlots = UNDEFINED;
		//nodeID[index].scheduleChangeBroadcastTime = UNDEFINED;
		nodeID[index].scheduleValidFrom = UNDEFINED;
		nodeID[index].numSchedElem = 0;
		//nodeID[index].remNumSchedElem = 0;
		//nodeID[index].intermediateSchedElem = 0;
		nodeID[index].moreFragment = FALSE;
		nodeID[index].treeUpdate = FALSE;	//ON and OFF based on formation of new routing tree
		
		nodeID[index].treeUpdateBroadcast = 0;
		nodeID[index].wasOrphan = FALSE; //maybe this is for some node which was previously a part of the tree
		
		nodeID[index].schedRepeat = FALSE;
		nodeID[index].numScheduleMissed = 0;
		nodeID[index].newScheduleReceived = FALSE;
		nodeID[index].expectedSchedFragmentNo = 1;
		nodeID[index].broadcastSchedFragmentNo = 1;
		nodeID[index].lastFragmentNo = 0;
		nodeID[index].scheduleNumOfFragments = 1;
		
		nodeID[index].contentionProb = (double)1;

		//nodeID[index].contentionPacketType = UNDEFINED; //to do:check its usage
		nodeID[index].packetBeingReceived = nodeID[index].packetBeingTransmitted = FALSE;
		nodeID[index].collision = 0;
		
		nodeID[index].controlReceptionCount = 0;
		nodeID[index].controlReceptionCollision = FALSE;
		
		nodeID[index].callDuration = 0;
		nodeID[index].callInProgress = FALSE;
		//manan change
		//nodeID[index].voiceCallInProgress = FALSE;
		//manan end		
		nodeID[index].numOfPacketsInQueue = nodeID[index].numOfPacketsInDataQueue = 0;
		nodeID[index].controlFront = nodeID[index].controlRear = nodeID[index].dataFront = nodeID[index].dataRear = 0;
		//nodeID[index].outStandingControlPacket = FALSE;	
		nodeID[index].backoff = 1;
		nodeID[index].expectingLinkAck = FALSE;
		
		nodeID[index].expectingJoinRequestAck = FALSE;
		nodeID[index].expectingFlowRequestAck = FALSE;
		nodeID[index].flowRequestGranted = 0;
		nodeID[index].flowRetryCount = 0;
		nodeID[index].callRequestSent = 0;	

		nodeStat[index].upTime = (double) 0;
		nodeStat[index].endCallTime = (double)0;	
		nodeStat[index].timeJoinRequestSent = 0;
		nodeStat[index].timeJoinGrantReceived = 0;
		nodeStat[index].timeFlowRequestSent = 0;
		nodeStat[index].timeFlowGrantReceived = 0;
		nodeStat[index].numChannelControlPacketLosses = 0;
		nodeStat[index].numQueueControlPacketDrops = 0; // this is actually for contention packets
		nodeStat[index].numFlowRequestDrops = 0;
		
		//portability changes start *****************
		nodeID[index].numOfTopologyUpdatesReceived = 0;
		nodeID[index].numOfTopologyUpdatesSent = 0;
		nodeID[index].maxQueueSize = 0;
		nodeID[index].numOfAttempts = 0;

		nodeID[index].expectingParentChange = FALSE;
		nodeID[index].expectingPathChange = FALSE;
		nodeID[index].next_hop = -1;
		nodeID[index].timeLastHandoffSent = 0;
		
		int i, j;
		for(i=0; i<MAX_NO_INFRA_NODES; i++)
		{
			nodeID[index].infra_list[i].infra_node = MAX_NO_INFRA_NODES + 1;
			nodeID[index].infra_list[i].times_observed = 0;
			nodeID[index].infra_list[i].missed = 0;
			for(j=0; j<INFRA_HEARD_THRESHOLD; j++) // portability hack
			{
			nodeID[index].infra_list[i].rssi[j] = 0;
			}
			nodeID[index].infra_list[i].is_neighbor = FALSE;
		}
		//portability changes end ***********************
		
		//mobility change 25-4-10
		for(i=0; i < MAX_DATA_HISTORY; i++)
		{
			nodeID[index].data_history[i] = 0;
		}

		for(i=0; i < MAX_SCHED_ELEMENTS; i++)
		{
			nodeID[index].localScheduleElements[i].transmitter = -1;
			nodeID[index].localScheduleElements[i].receiver = -1;
			nodeID[index].localScheduleElements[i].flowId_channel = -1;
			nodeID[index].localScheduleElements[i].slotToStart = -1;
			nodeID[index].localScheduleElements[i].numSlots = -1;

		}
	
		for(i=0; i<2000; i++)
		{
			nodeID[index].flowScheduleSet[i] = 0;
		}
	
		//nodeID[index].tempNumSchedElem = 0;
		nodeID[index].scheduleMissed = FALSE;
		
	}

}

int extractInput(char *filename)
{
	FILE *fp;
	char letter;
	int index;

	int nodeIndex;
	int nodeNeighborIndex;
	int nodeInterferenceIndex;
	int nodeIterator;

	nodeIndex = 0;
	nodeNeighborIndex = 0;
	nodeInterferenceIndex = 0;

	numOfInfraNodesInNetwork = 0;
	numOfMobileNodesInNetwork = 0;
	nodeIterator = 0;

	if( (fp = fopen(filename,"r")) == NULL )
	{
		fprintf(stderr, "\n Error in opening input file");
		exit(1);
	}
	else
	{
		//fprintf(stderr, "\n File opened successfully");
	}
	
	while( (letter = fgetc(fp)) != EOF )
	{		
		if(letter == '\n')
		{
			#ifdef DEBUG_ENABLE
				//printf("\n %d: ",nodeIndex);
				for(index = 0; index < nodeNeighborIndex; index++)
				{
					//printf(" %d \t", nodeID[nodeIndex].neighborList[index]);
				}
				//printf("\n");
			#endif	
			nodeIndex++;
			numOfNodesInNetwork = nodeIndex;
			nodeNeighborIndex = 0;
			nodeInterferenceIndex = 0;
			nodeIterator = 0; // i think the purpose of node iterator is to denote which node is the neighbor of nodeIndex
			continue;
		}

		if( letter == 'I' )
		{
			nodeID[nodeIndex].type = INFRA;
			numOfInfraNodesInNetwork++;
		}	
		else if( letter == 'M' )
		{
			nodeID[nodeIndex].type = MOBILE;
			numOfMobileNodesInNetwork++;
		}	
		else
		{
		}

		if( letter == '1' || letter == '2' )
		{
			nodeID[nodeIndex].interferenceList[nodeInterferenceIndex] = nodeIterator;
			nodeInterferenceIndex++;
			nodeID[nodeIndex].numInterferringNeighbors++;
					
		}

		if( letter == '1' )
		{
			nodeID[nodeIndex].neighborList[nodeNeighborIndex] = nodeIterator;
			nodeNeighborIndex++;
			nodeID[nodeIndex].numNeighbors++;
		}
		
		if( letter == '1' || letter == '0' || letter == '2')
			nodeIterator++;
	}
	
	fclose(fp);
	return 0;
}

