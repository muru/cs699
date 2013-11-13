#include "header.h"

//*****************************************************************************************************
//handle data packet queue

// mobility change on 24-4-10
int flowInProgress(int flowId)
{
	int i;
	for(i=0; i<numOfFlowsInProgress; i++)
	{
		if(flowStat[i].flowId == flowId)
		{
		//fprintf(stderr,"Reached here, matched %d for %d \n", flowId, flowStat[i].flowId);
		return TRUE;
		}
	}
return FALSE;
}

int canInsertNextEvent(int node, struct scheduleElement anElement, double nextEventTiming)
{
	//fprintf(stderr,"node %d  nextEventtiming %lf  :  transmitter %d  receiver %d  flowId %d  slot_to_start %d \n", node, nextEventTiming, anElement.transmitter, anElement.receiver, EXTRACT_FLOWID(anElement.flowId_channel), anElement.slotToStart);
	//exit(0);	
	//now checking whether there exists a scheduling element in the node's scheduling elements which has same parameters
	struct scheduleElement * queueElement;
	int i;
	int element_found, no_event_found;
	element_found = FALSE;	
	no_event_found = TRUE;
	struct event * curr_queue_event;
	char * event_name;
	event_name = (char *)malloc(30*sizeof(char));
	double event_time;
	

	for(i=0; i < nodeID[node].numSchedElem; i++)
	{
		if(nodeID[node].localScheduleElements[i].transmitter == anElement.transmitter && nodeID[node].localScheduleElements[i].receiver == anElement.receiver && nodeID[node].localScheduleElements[i].flowId_channel == anElement.flowId_channel && nodeID[node].localScheduleElements[i].slotToStart == anElement.slotToStart)
		{
			//fprintf(stderr,"Matching scheduling element found in node %d 's scheduling elements at position %d \n", node, i);
			//exit(0);
			element_found = TRUE;
		}
	}

	//now checking in the event queue
	curr_queue_event = queueHead;
	while(curr_queue_event != NULL)
	{
		resolve_name(curr_queue_event->type, event_name);

		if( strcmp(event_name,"DATA_SLOT_TRANSMIT") == 0 )
		{
		queueElement = (struct scheduleElement *)curr_queue_event->aPacket->payloadPacket;		
		//fprintf(stderr," transmitter %d  receiver %d  flowId %d  slot_to_start %d event_time %lf \n", queueElement->transmitter, queueElement->receiver, EXTRACT_FLOWID(queueElement->flowId_channel), queueElement->slotToStart, curr_queue_event->time);
		//exit(0);
			//now comparing the contents of this scheduling element and the one passed in the argument
			if(anElement.transmitter == queueElement->transmitter && anElement.receiver == queueElement->receiver && anElement.flowId_channel == queueElement->flowId_channel && anElement.slotToStart == queueElement->slotToStart)
			{
				//fprintf(stderr,"Matching scheduling element in event queue found for node %d \n", node);
				//fprintf(stderr,"element_found %d  no_event_found %d \n", element_found, no_event_found);
				//exit(0);
				//now measuring time gap
				if(nextEventTiming == curr_queue_event->time)
				{
					//fprintf(stderr,"Event for next frame already present in the event queue, so do not insert \n");
					no_event_found = FALSE;
					//exit(0);
				}
				
			}
		}
	curr_queue_event = curr_queue_event->next;
	}

	free(event_name);

	if(element_found == TRUE && no_event_found == TRUE)
	{
	return TRUE;
	}
	else
	{
	return FALSE;
	}
	
}

//mobility change 29-4-10
int canSendMoreData(int node, int flowId, int pkt_src, int pkt_dst)
{
	int i;
	for(i=0; i < nodeID[node].numSchedElem; i++)
	{
		if(nodeID[node].localScheduleElements[i].transmitter == pkt_src && nodeID[node].localScheduleElements[i].receiver == pkt_dst && EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) == flowId)
		{
			//fprintf(stderr," transmitter %d -->  receiver %d  flowId  %d \n", pkt_src, pkt_dst, flowId);
			return TRUE;
		}
	}
return FALSE;
}

//mobility change 30-4-10
int nodeIsMobile(int node, int flowId, int pkt_src)
{
	int i;
	for(i=0; i < nodeID[ROOT].numSchedElem; i++)
	{
		if(globalScheduleElements[i].transmitter == pkt_src && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
			//fprintf(stderr," transmitter %d flowId  %d \n", pkt_src, flowId);
			return TRUE;
		}
	}
return FALSE;
}

//mobility change 30-4-10
int getNewNextHop(int node, int flowId, int pkt_src)
{
	int i;
	for(i=0; i < nodeID[ROOT].numSchedElem; i++)
	{
		if(globalScheduleElements[i].transmitter == pkt_src && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
			//fprintf(stderr," transmitter %d flowId  %d \n", pkt_src, flowId);
			return globalScheduleElements[i].receiver;
		}
	}
return -1;
}

//mobility change 30-4-10
int getNewSlot(int node, int flowId, int pkt_src)
{
	int i;
	for(i=0; i < nodeID[ROOT].numSchedElem; i++)
	{
		if(globalScheduleElements[i].transmitter == pkt_src && EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == flowId)
		{
			//fprintf(stderr," transmitter %d flowId  %d \n", pkt_src, flowId);
			return globalScheduleElements[i].slotToStart;
		}
	}
return -1;
}

void clearDataHistory(int node)
{
	int i;
	for(i=0; i < MAX_DATA_HISTORY; i++)
	{
		nodeID[node].data_history[i] = 0;
	}
}

//mobility change 25-4-10
void performDataAveraging(int node, int infra, int rssi)
{
//First shift all values up and put current rssi in the last place
int i,j;
double average;
struct event * anEvent;

    for(i=0; i < MAX_DATA_HISTORY -1 ; i++)
    {
        nodeID[node].data_history[i] = nodeID[node].data_history[i+1];
    }
    nodeID[node].data_history[MAX_DATA_HISTORY - 1] = rssi;

//now putting the check loop to avoid the false positives
	for(i=0; i < MAX_DATA_HISTORY; i++)
	{
		if((nodeID[node].data_history[i] > -50) || (nodeID[node].data_history[i] < -100))
		{
		//fprintf(stderr,"This should not have happened, now assigning default value \n");
		//nodeID[node].data_history[i] = -60;
		//return ;
		}
	}
//now perform averaging, the 30th observation will be given weight 30, 29th one 29 and so on
//store values in an array rather than print the long lines over and over again
int temp[MAX_DATA_HISTORY];

    for(i=0; i < MAX_DATA_HISTORY; i++)
    {
    temp[i] = nodeID[node].data_history[i];
    }
   
    //seems I will have to use some formula here, first add all the values with weights and then divide
    float numerator, denominator;
    numerator = denominator = 0;

    for(i=0; i < MAX_DATA_HISTORY; i++)
    {
        numerator += (temp[i] * (i+1));
    }   
   
    for(i=0; i < MAX_DATA_HISTORY; i++)
    {
        denominator += (i+1);
    }

    average =  ((double)numerator/(double)denominator) ;
    //fprintf(stderr,"The data packet rssi average is %f \n", average);
    //exit(0);   
   
    if((int)(ceil(average)) < INFRA_RSSI)
    {
	if(nodeID[node].expectingPathChange == TRUE)
	{
	//do nothing, handoff request already sent
	}
	else
	{
        //send a handoff update
		if((int)(simTime - nodeID[node].timeLastHandoffSent) > HANDOFF_WAIT) // mobility change 30-4-10
		{
		int node_x, node_y;
		getPosition(node, &node_x, &node_y, simTime);
		//exit(0);
		anEvent = getTopologyUpdateEvent(simTime, node, node, nodeID[node].parent,TX, HANDOFF);
		insertEvent(anEvent);
		nodeID[node].expectingPathChange = TRUE;
		nodeID[node].timeLastHandoffSent = simTime;	

		//fprintf(stderr,"Average for node %d with respect to its last hop %d is %lf at time %lf, its X and Y coordinates are %d and %d respectively , sending handoff update\n", node, infra, average, simTime, node_x, node_y);

		logMobilityStats(EXPECTING_PATH_CHANGE, node, infra, simTime);
		}
		else
		{
			//fprintf(stderr,"Handoff just sent, please wait \n");
		}
	}
	//exit(0);
    }
}

//mobility change 25-4-10
int updateConnectivityInfo(int node, int infra, int rssi)
{
    int i,j,k;
    int enough_history; //boolean variable to check whether enough packets are there or not, 1 means enough and 0 means not enough
    enough_history = 1;

    //first insert the new value in the history
    	if(rssi > -50 || rssi < -100)
	{
	//fprintf(stderr," rssi %d for node %d wrt infra %d is not set properly \n", rssi, node, infra);
    	//return SUCCESS; // mobility change 3-5-10
	}

    for(i=0; i < MAX_DATA_HISTORY; i++)
    {
        if(nodeID[node].data_history[i] == 0)
        {
            nodeID[node].data_history[i] = rssi;
            enough_history = 0;
            break;
        }
    }

    if(enough_history == 0)
    {
        return SUCCESS;
    }
    else
    {
        //fprintf(stderr,"Got 30 data packets for node %d with respect to infra %d \n", node, infra);
	//exit(0);
        performDataAveraging(node, infra, rssi);
    }

   
}

int searchQueueIndex(int node, int flowId, int linkDestination)
{
	int i;
	struct packetFormat * aPacket;
	struct data * aData;
	
	if(nodeID[node].numOfPacketsInDataQueue == 0)
		return UNDEFINED;
	for(i=0;i<nodeID[node].numOfPacketsInDataQueue;i++)
	{		
		aPacket = nodeID[node].dataPacketQueue[i];
		aData = (struct data *)aPacket->payloadPacket;

		if(aData->flowId == flowId) 
		{
			if(aData->e2eDestination == node) //reply to the same node from where it is received at the link layer
				return i;
			if(aPacket->source != linkDestination) //to forward the packet over the path
				return i;
		}
	}
	return UNDEFINED;
}
//data packet removed could be any intermediate packet, so shift the elements in array to fill the gap
int reshuffleQueue(int node, int index)
{
	int i,j;
	i=index;
	j=i+1;
	if(j == nodeID[node].numOfPacketsInDataQueue)
	{
		nodeID[node].numOfPacketsInDataQueue--;
		if(nodeID[node].numOfPacketsInDataQueue == 0)
		{
			nodeID[node].dataRear = 0; //this implementation does not require dataFront
		}
		else
		{
			nodeID[node].dataRear = (nodeID[node].dataRear - 1) % QUEUE_LIMIT;
		}
		return SUCCESS;
	}
	
	//VJ change starts 29th May, 2011
	
	if(nodeID[node].numOfPacketsInDataQueue > 0)
	{
		nodeID[node].dataRear = (nodeID[node].dataRear - 1) % QUEUE_LIMIT;
	}
	
	//VJ change ends 29th May, 2011
	
	for(j=i+1; j < nodeID[node].numOfPacketsInDataQueue; j++)
	{
		nodeID[node].dataPacketQueue[i] = nodeID[node].dataPacketQueue[j];
		i++;
	}
	nodeID[node].numOfPacketsInDataQueue--;
}

int flowDoesNotExist(int flowId)
{
	int j;
	for(j=0; j<numOfFlowsInProgress; j++)
	{
					
		if(flowStat[j].flowId == flowId)
		{
			return 0;	
		}	
	}
	return 1;
}

int insertInDataQueue(int node, struct packetFormat * aPacket)
{
	struct packetFormat * otherPacket, * morePacket;
	struct data * aData;
	
	//this copy is necessary since after every event the respective memory allocated is freed
	otherPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));
	otherPacket->ver_type_piggyback = aPacket->ver_type_piggyback;
	otherPacket->source = aPacket->source; 
	otherPacket->destination = aPacket->destination; 
	
	aData = (struct data *)malloc(sizeof(struct data));
	aData->length = ((struct data *)aPacket->payloadPacket)->length;	
	aData->seqNumber = ((struct data *)aPacket->payloadPacket)->seqNumber;	
	aData->e2eSource = ((struct data *)aPacket->payloadPacket)->e2eSource;
	aData->e2eDestination = ((struct data *)aPacket->payloadPacket)->e2eDestination;
	aData->flowId = ((struct data *)aPacket->payloadPacket)->flowId;
	
	otherPacket->payloadPacket = (uint8_t *)aData;
		
	if(node == 3)
	{
		//fprintf(stderr,"\n dataQueueSize %d source %d time %lf",nodeID[node].numOfPacketsInDataQueue, otherPacket->source, simTime);
	}	
		
	if(flowDoesNotExist(aData->flowId))
	{
		//fprintf(stderr,"\n flow does not exist");
		//sleep(5);
		return 0;
	}
		
	if(nodeID[node].numOfPacketsInDataQueue < QUEUE_LIMIT)
	{
		nodeID[node].dataPacketQueue[nodeID[node].dataRear] = otherPacket;
		nodeID[node].dataRear = (nodeID[node].dataRear + 1) % QUEUE_LIMIT;
		nodeID[node].numOfPacketsInDataQueue++;
	}
	else
	{
		//fprintf(stderr,"\n Data Queue full at node %d flow %d",node, aData->flowId);		
		//sleep(5);
		//exit(0);
		logFlowStats(aData->flowId,"\n QUEUE_PACKET_DROP",simTime);	
	}
	//morePacket = nodeID[node].dataPacketQueue[nodeID[node].dataFront];	
}

double getNextDataTransmitTime(double nextDataBoundary, int numSlots)
{
	double returnTime;
	int i;
	
	returnTime = nextDataBoundary;
		
	#ifdef DEBUG_FRAC
	
	//fprintf(stderr,"\n Inside getNextDataTransmit nextDataBoundary %lf", returnTime);
	//sleep(2);
	
	#endif		
		
	for(i=0;i<numSlots;i++)
	{
		returnTime += DATA_SLOT_DURATION;
		
		#ifdef DEBUG_FRAC
	
		//fprintf(stderr,"\n Inside getNextDataTransmit nextDataBoundary %lf i %d", returnTime, i);
		//sleep(2);
	
		#endif	
		
		//printf("\n **** returnTime %lf ****",returnTime);
		if(fmod(returnTime, FRAME_DURATION) == 0)
		{
			returnTime += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION);
			
			#ifdef DEBUG_FRAC
	
			//fprintf(stderr,"\n Inside fmod %lf i %d", returnTime, i);
			//sleep(2);
	
			#endif
			
		}
	}		
	
	//printf("\n nextDataBoundary %lf returnTime %lf",nextDataBoundary, returnTime);
	
	return returnTime;
}


//**********************************************************************************************************************
//hanlde data packet transmission and reception

int isFlowActive(int flowId)
{
	int j;
	for(j=0; j<numOfFlowsInProgress; j++)
	{
		if(flowStat[j].flowId == flowId)
		{
			return TRUE;
		}	
	}
	
	return FALSE;
}

//search for flowID, if packet present, pop out from queue, insert RX event
int handleTransmitDataPacket(int node, struct packetFormat * aPacket)
{
	struct event * anEvent;
	struct scheduleElement * anElement;
	struct scheduleElement aScheduleElement;
	struct data * aData;
	
	double nextEventTiming;
	
	int sizeofPacket;
	int flowId;
	int queueIndex;
	int index;
	int slotNo;
	
	anElement = (struct scheduleElement *)aPacket->payloadPacket;	
	flowId = EXTRACT_FLOWID(anElement->flowId_channel);

	queueIndex = searchQueueIndex(node,flowId, anElement->receiver);
	
	if(node == 2)
	{
		//fprintf(stderr,"\n About to transmit to %d f %d", anElement->receiver, EXTRACT_FLOWID(anElement->flowId_channel));
	}
	
	if(queueIndex != UNDEFINED)
	{		
		aPacket = nodeID[node].dataPacketQueue[queueIndex];
		aData = (struct data *)aPacket->payloadPacket;
		reshuffleQueue(node,queueIndex);

		sizeofPacket = extractSizeof(aPacket);
		
		if(anElement->receiver == 2)
		{
			//fprintf(stderr,"\n About to receive from %d f %d", node, aData->flowId);
		}
		
		for(index = 0; index < nodeID[node].numNeighbors; index++)
		{
			if(nodeID[node].neighborList[index] == anElement->receiver || nodeID[nodeID[node].neighborList[index]].packetBeingTransmitted == TRUE)
			{
				
			}
			else
			{
				if(!IsPacketToBeDropped(errorRate))
				{
					anEvent = getDataReceptionEvent(simTime, nodeID[node].neighborList[index], RX_BEGIN);
					insertEvent(anEvent);
			
					anEvent = getDataReceptionEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), nodeID[node].neighborList[index], RX_END);
					insertEvent(anEvent);
					
					anEvent = getStopTransmissionEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node);
					insertEvent(anEvent);
				
				}
			}

		}
					
		if(!IsPacketToBeDropped(errorRate))
		{	
			//nodeID[node].seqNumber++;
			
			//fprintf(stderr,"\nIncrement 1 node %d",node);
			
	
			anEvent = getDataSourceSendEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aData->length, aData->seqNumber, aData->e2eSource, aData->e2eDestination, EXTRACT_FLOWID(anElement->flowId_channel), anElement->transmitter, anElement->receiver, anElement->slotToStart, RX);
			//fprintf(stderr,"\t %d --> %d \t ",anElement->transmitter, anElement->receiver);
			insertEvent(anEvent);
		}
		else
		{
			//printf("\t data packet is to be dropped node %d",node);			
			logFlowStats(EXTRACT_FLOWID(anElement->flowId_channel),"\n CHANNEL_PACKET_LOSS",simTime);
		}
	}	

	aScheduleElement.transmitter = anElement->transmitter;
	aScheduleElement.receiver = anElement->receiver;
	aScheduleElement.flowId_channel = anElement->flowId_channel;
	aScheduleElement.slotToStart = anElement->slotToStart;
	aScheduleElement.numSlots = anElement->numSlots;
	 

	if(nodeID[node].scheduleSlots == UNDEFINED)
	{
		fprintf(stderr,"\n Sanity check failed for schedule slots inside data generate: scheduleSlots UNDEFINED");
		exit(0);
	}
	nextEventTiming = getNextDataTransmitTime(simTime, nodeID[node].scheduleSlots);
	//nextEventTiming = getNextDataTransmitTime(nodeID[node].scheduleValidFrom, nodeID[node].scheduleSlots); // changed on 28-4-10
	
	if(nodeID[node].lastScheduleMissed == UNDEFINED)
	{
		fprintf(stderr,"\n Sanity check failed inside datatransmit: lastScheduleMissed UNDEFINED \n");
		exit(0);
	}
	
	if(nodeID[node].lastScheduleMissed == TRUE)
	{
		// do nothing, continue data transmission with old schedule
	}

	//set next TX event; changed on 24-4-10
	//if(nextEventTiming < nodeID[node].lastScheduleTime)  // check hack on 27-4-10
	if(canInsertNextEvent(node, aScheduleElement, nextEventTiming) == TRUE || isFlowActive(EXTRACT_FLOWID(aScheduleElement.flowId_channel)) == TRUE)
	{
		//fprintf(stderr,"This if condition comes out to be true for getDataSendEvent for node %d \n", node);
		
		//if(node == 3)
		//fprintf(stderr,"\n Next event inserted for node %d for slot %d receiver %d", node, aScheduleElement.slotToStart, aScheduleElement.receiver);
		
		anEvent = getDataSendEvent(nextEventTiming, node, aScheduleElement); 
		insertEvent(anEvent);						
	}//meaning thereby that if the time by which the next data packet to be transmitted comes, the schedule hasnt been changed then transmit the data packet
	else
	{
		//if(node == 3)
		//fprintf(stderr,"\n Next event was not inserted for node %d for slot %d receiver %d", node, aScheduleElement.slotToStart, aScheduleElement.receiver);
	}	
}

//insert RX event for data packet, set next TX event
int handleGenerateDataPacket(int node, struct packetFormat * aPacket)
{
	struct event * anEvent;
	struct data * aData;
	
	double dataNextEventTime;
	
	int sizeofPacket;
	int slotNo;
	int index;	
				
	//fprintf(stderr,"\n 1... data generate event at node %d callDuration %lf tearDown %d", node, nodeID[node].callDuration, nodeID[node].flowTearedDown);
	//sleep(2);
	
	if(nodeID[node].callDuration == 0)
	{
		fprintf(stderr,"\n Call duration 0");
		return SUCCESS;
		exit(0);
	} // thats why i made call duration 10 in minirevokeElements

	aData = (struct data *)aPacket->payloadPacket;
	sizeofPacket = extractSizeof(aPacket);
		
	if(nodeID[node].flowTearedDown == TRUE)
	{			
	//return SUCCESS; // changed 2-10-09	
	}			
	
	//fprintf(stderr,"\n data generate event at node %d", node);
	//sleep(2);
	
	for(index = 0; index < nodeID[node].numNeighbors; index++)
	{
		if(nodeID[node].neighborList[index] == aPacket->source || nodeID[nodeID[node].neighborList[index]].packetBeingTransmitted == TRUE)
		{
			
		}
		else
		{
			if(!IsPacketToBeDropped(errorRate))
			{
				anEvent = getDataReceptionEvent(simTime, nodeID[node].neighborList[index], RX_BEGIN);
				insertEvent(anEvent);							
				
				anEvent = getDataReceptionEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), nodeID[node].neighborList[index], RX_END);
				insertEvent(anEvent);
				
				anEvent = getStopTransmissionEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node);
				insertEvent(anEvent);
				
			}
		}

	}
			
	if(!IsPacketToBeDropped(errorRate))
	{
		nodeID[node].seqNumber++;
		
		//fprintf(stderr,"\nIncrement 2 node %d seqNum %d",node,nodeID[node].seqNumber);
		
		anEvent = getDataSourceSendEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aData->length, nodeID[node].seqNumber, aData->e2eSource, aData->e2eDestination, aData->flowId, aPacket->source, aPacket->destination, aData->slotNo, RX);
		insertEvent(anEvent);
		//printf("\t %d --> %d",aPacket->source, aPacket->destination);
	}
	else
	{			
		logFlowStats(aData->flowId,"\n CHANNEL_PACKET_LOSS",simTime);
	}		

	logFlowStats(aData->flowId,"\n PACKET_SENT_BY_SOURCE",simTime);
	
	nodeID[node].callDuration -= FRAME_DURATION;
	
	if(aData->flowId == 1)
	{
		//fprintf(stderr,"\n call duration remaining %lf", nodeID[node].callDuration);
	}
	
	
	if(nodeID[node].scheduleSlots == UNDEFINED)
	{
		fprintf(stderr,"\n Sanity check failed for schedule slots inside data generate: scheduleSlots UNDEFINED");
		exit(0);
	}
	
	dataNextEventTime = getNextDataTransmitTime(simTime, nodeID[node].scheduleSlots);
	
	if(nodeID[node].lastScheduleMissed == UNDEFINED)
	{
		fprintf(stderr,"\n Sanity check failed inside dataGenerate: lastScheduleMissed UNDEFINED \n");
		exit(0);
	}

	if(nodeID[node].lastScheduleMissed == TRUE)
	{
		// to nothing, follow old schedule
	}

	
	if(nodeID[node].callDuration <= 0)
	{

		nodeID[node].flowTearedDown = TRUE;
		nodeID[node].flowRequestGranted = 0;
		nodeID[node].flowRetryCount = 0;
		
		//mobility change 26-4-10
		clearDataHistory(node);
		nodeID[node].next_hop = -1;
		//scheduleTearDown(node, aData->flowId);			
		
		//manan change
		if(nodeID[node].voiceFlow == 1)
		{
		nodeID[node].voiceFlow = 0;	
		//fprintf(stderr,"\nvoice flow tear down condition true, Flow %d Node %d, now voiceFlow %d\n", aData->flowId, node, nodeID[node].voiceFlow);
		}
		else
		{
		//fprintf(stderr,"\n Tear down scheduled for flow %d simTime %lf for node %d voiceFlow %d call duration %lf\n",aData->flowId,simTime, node,nodeID[node].voiceFlow,nodeID[node].callDuration);
	
		//adding code on 27-10-09, this is unbelievable, man!, its a hack 
			if(node == ROOT)
			{}
			else
			{
			scheduleTearDown(node, aData->flowId);	
			}
		//I suppose this should work because the node 0 by itself is never going to initiate a call. So whenever a tear down is called with node 0 as the argument, it is supposed to be a voice message, whose resources must already have been deallocated by revokeElements or minirevokeElements through VOICE_MSG_CHECK event or revokeVoiceFlow event
		}
		
		nodeID[node].callInProgress = FALSE;
		nodeID[node].callRequestSent = 0;        // this request is done, reset request flag
		logFlowStats(aData->flowId,"\n Time_Tear_Down_Sent",simTime);
		
		//if(node >= numOfInfraNodesInNetwork)
		//canStartFlowRequest(node); // hack for suppressing new flows

		return SUCCESS;
	}
	else
	{
		//set next TX event
		if(canSendMoreData(node, aData->flowId, aPacket->source, aPacket->destination) == TRUE)
		{
			//nodeID[node].seqNumber++;
			
			//fprintf(stderr,"\nIncrement 3 node %d seqNum %d",node, nodeID[node].seqNumber);
			
			if(aPacket->source == 35 && aPacket->destination == 2)
			{
				//fprintf(stderr,"\n Sending packets %lf RCD %lf", simTime, nodeID[node].callDuration);
			}
			
			anEvent = getDataSourceSendEvent(dataNextEventTime, node, aData->length, nodeID[node].seqNumber, aData->e2eSource, aData->e2eDestination, aData->flowId, aPacket->source, aPacket->destination, aData->slotNo, TX);
			insertEvent(anEvent);
		}
		else
		{
			//fprintf(stderr,"This should be executed only with respect to mobility :: node %d flowId %d pkt_src %d pkt_dst %d \n", node, aData->flowId, aPacket->source, aPacket->destination);
			//exit(0);
			//now checking whether the node is actually mobile or not
			if(nodeIsMobile(node, aData->flowId, aPacket->source) == TRUE)
			{
				int new_next_hop, new_slot;
				new_next_hop = getNewNextHop(node, aData->flowId, aPacket->source);
				new_slot = getNewSlot(node, aData->flowId, aPacket->source);

				if(new_next_hop == -1)
				{
					fprintf(stderr,"If the node is mobile, then a receiver should have been returned \n");
					exit(0);
				}
				if(new_slot == -1)
				{
					fprintf(stderr,"If the node is mobile, then a receiver should have been returned \n");
					exit(0);
				}
		
				nodeID[node].seqNumber++;
				
				//fprintf(stderr,"\nIncrement 4 node %d",node);
				
				anEvent = getDataSourceSendEvent(dataNextEventTime, node, aData->length, nodeID[node].seqNumber, aData->e2eSource, aData->e2eDestination, aData->flowId, aPacket->source, new_next_hop, new_slot, TX);
				insertEvent(anEvent);

				//logMobilityStats(NEW_PATH_RECEIVED, node, new_next_hop, simTime);
			}
			else
			{
				//fprintf(stderr,"This condition should have been executed only with respect to mobility :: node %d flowId %d pkt_src %d pkt_dst %d \n", node, aData->flowId, aPacket->source, aPacket->destination);
				//exit(0);
			}

		}	
	
	}
	
}

//insert packet in queue
int handleReceivedDataPacket(int node, struct packetFormat * aPacket)
{
	struct data * aData;
	char *str, *convStr, *strInput;
	int i;

	str = (char *)malloc(100 * sizeof(char));
	convStr = (char *)malloc(50 * sizeof(char));
	
	aData = (struct data *)aPacket->payloadPacket;
	
	//fprintf(stderr,"\n data packet received at node %d", node);
	//sleep(2);
	
	if(aData->e2eDestination == node)
	{
		insertInDataQueue(node,aPacket);
		
		strcpy(str,"\n Data_Packet_Received with seqNumber ");
		sprintf(convStr,"%d at destination node %d", aData->seqNumber, node);
		strInput = strcat(str,convStr);
						
		//********Rajesh change******

		monitorFlowQuality(node, *aData);

		//********Rajesh change******
		
		logFlowStats(aData->flowId, strInput, simTime);
		
		//fprintf(stderr,"\n\n ======= Data packet received at destination %d from %d with rssi %d =======",node, aPacket->source, aPacket->rssi);
		
		updateConnectivityInfo(node, aPacket->source, aPacket->rssi);
		//return SUCCESS;
	}
	else if(aData->e2eSource == node)
	{
		strcpy(str,"\n Data_Packet_Received with seqNumber ");
		sprintf(convStr,"%d at source node %d", aData->seqNumber, node);
		strInput = strcat(str,convStr);
		
		logFlowStats(aData->flowId, strInput, simTime);

		//fprintf(stderr,"\n\n ======= Bi directional flow established at source %d, packet received from %d with rssi %d  ======= \n\n", node, aPacket->source, aPacket->rssi);

		updateConnectivityInfo(node, aPacket->source, aPacket->rssi); // mobility change 25-4-10 
	}
	else
	{
		if(node == 2)
		{
			//fprintf(stderr,"\n source %d",aPacket->source);
		}
	
	
		if(node >= numOfInfraNodesInNetwork)
		{
			fprintf(stderr,"\n handset can't be intermediate node %d %d %d", aData->e2eSource, aData->e2eDestination, aData->flowId);
			exit(0);
		}
	
		insertInDataQueue(node,aPacket);
	}
	
	free(str);
	free(convStr);
}

//********Rajesh change******

//! monitor the flow quality based on packet lost in a sliding window of previous 100 data packets
int monitorFlowQuality(int node, struct data dPacket)
{
	int difference, start, end, i, nextHop;
	int count = 0;

	//VJ: Not sure why PrevSeqNumber is needed, can be done away with 15th Feb, 2011.
	
	
	if(nodeID[node].prevSeqNumber == NEGATIVE_UNDEFINED)
	{
		//fprintf(stderr,"\nDefining prevSeqNumber");
		nodeID[node].prevSeqNumber = dPacket.seqNumber;
		//return SUCCESS;
	}

	nodeID[node].slidingWindow[dPacket.seqNumber % DATA_PACKET_SLIDING_WINDOW] = 1;
	if(nodeID[node].windowCount < DATA_PACKET_SLIDING_WINDOW)
	{
		nodeID[node].windowCount++;
	}

	if(nodeID[node].prevSeqNumber < dPacket.seqNumber)
	{
		nodeID[node].prevSeqNumber++;
		while(nodeID[node].prevSeqNumber != dPacket.seqNumber)
		{
		
			//fprintf(stderr,"\nnodeID[node].prevSeqNumber %d dPacket.seqNumber %d",nodeID[node].prevSeqNumber, dPacket.seqNumber);
		
			nodeID[node].slidingWindow[nodeID[node].prevSeqNumber % DATA_PACKET_SLIDING_WINDOW] = 0;
			nodeID[node].prevSeqNumber++;
		}
	}

	// count number of packet losses in last 100 data packets
	
	if(dPacket.seqNumber < DATA_PACKET_SLIDING_WINDOW)
	{	
		for(i = 0; i < nodeID[node].windowCount; i++)
		{
			if(nodeID[node].slidingWindow[i] == 0)
			{
				count++;
			}	
		}
	}
// ***********Instead of above if follwing loop should be executed because we have initilized all entries in slidingWindow to 1
/*	for(i = 0; i < nodeID[node].windowCount; i++)
	{
		if(nodeID[node].slidingWindow[i] == 0)
		{
			count++;
		}	
	}
*/
	if(count > MAX_END_TO_END_DATA_PACKET_LOSS_THRESHOLD)
	{
		// terminate flow and record it is call drop
		nodeID[node].callDuration = 0;

		//********callDuration at source node should be set to negative to send a teardown on call drop as below
		//nodeID[dPacket.e2eSource].callDuration = -1;
		
		uniqueCallsDropped[dPacket.flowId] = 1;
		flowDropDueToE2EPacketLosses++;
		
		//fprintf(stderr,"\nFlow terminated due to packet loss at node %d dPacket.seqNumber %d",node, dPacket.seqNumber);
		//sleep(3);
		//exit(0);

		return FAILURE;
	}

	nodeID[node].prevSeqNumber = dPacket.seqNumber;
	return SUCCESS;
}

//********Rajesh change ends******


//************************************************************************************************************

