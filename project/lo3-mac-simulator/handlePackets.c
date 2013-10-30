#include "header.h"

double flowTimeOutLimit = 30*1000;
double marginFlowTimeOutLimit = 5*1000;

//************************************************************************************************************
//MANAN_CHANGE
int scheduleIsMissed(int node)
{
	int i;

	if(nodeID[node].scheduleMissed == TRUE)
	{
		return TRUE;
	}

	for(i = nodeID[node].parent; i!= ROOT; )
	{
	//fprintf(stderr,"The value of i is %d \n", i);
		if(nodeID[i].scheduleMissed == TRUE)
		{
		//fprintf(stderr," Node %d is dead \n", i);
		return TRUE;
		}

	i = nodeID[i].parent;
	}
return FALSE;
}

//mobility change 5-5-10
void refreshDataQueue(int infra)
{
	int i,j, flowId, flowId_found;
	struct packetFormat * aPacket;
	struct data * aData;

	for(i=0; i < nodeID[infra].numOfPacketsInDataQueue; i++)
	{
		flowId_found = FALSE;
		aPacket = nodeID[infra].dataPacketQueue[i];
		aData = (struct data *)aPacket->payloadPacket;
		flowId = aData->flowId;
		
		for(j=0; j < nodeID[infra].numSchedElem; j++)
		{
			if(globalScheduleElements[j].transmitter == infra)
			{
				if(EXTRACT_FLOWID(globalScheduleElements[j].flowId_channel) == flowId)
				{
					flowId_found = TRUE;
					break;
				}
			}
		}

		if(flowId_found == FALSE)
		{
			reshuffleQueue(infra, i);
		}
	}
}

//mobility change 6-5-10
void printDataQueue(int infra)
{
	int i,j;
	struct packetFormat * aPacket;
	struct data * aData;

	if(infra == 0)
	{
		//fprintf(stderr,"\n Schedule received \n");

		for(i=0; i < nodeID[infra].numOfPacketsInDataQueue; i++)
		{
			aPacket = nodeID[infra].dataPacketQueue[i];
			aData = (struct data *)aPacket->payloadPacket;
	
			//fprintf(stderr,"Packet %d : flowId %d e2eSource %d e2eDestination %d \n", i, aData->flowId, aData->e2eSource, aData->e2eDestination);
		}
	}
}

//invoked by nodes when they receive modified schedule
//scheduling elements are extracted
int checkIfSource(int node)
{
	int i;
	
	for(i = 0; i < numOfFlowsInProgress; i++)
	{
		if(flowStat[i].source == node)
			return TRUE;
	}
	
	return FALSE;
}

int getDestination(int node)
{
	int i;
	
	for(i = 0; i < numOfFlowsInProgress; i++)
	{
		if(flowStat[i].source == node)
			return flowStat[i].destination;
	}
	
	fprintf(stderr,"\n Destination could not be found");
	printf("\n Destination could not be found");
	
	exit(0);

}

// follow fresh schedule on reception of schedule change
int setUpNewSchedule(int node)
{
	struct event * anEvent;
	struct event * prevEvent;
	struct packetFormat * aPacket;
	struct scheduleElement * anElement;
	int tempLength;
	int tempe2eDestination;
	double nextDataBoundary;
	double nextDataTransmit;
	int i, j, k, flowSource, flowDestination;
	int thisNodeSlots;

	int newFlowId[100];
	int newFlowIdIndex = 0;

	int inScheduleElem=0;     //check if this node part of current schedule
	
	FILE *fz;
	
	tempLength = 40;
	thisNodeSlots = 0;
	
	flowSource = 255;
	flowDestination = 255;
	
	//VJ: Start
	nextDataBoundary = nodeID[node].lastScheduleTime;
	//nextDataBoundary = simTime; //changed on 24-4-10
	//VJ: End

	//MANAN_CHANGE
	//VJ start: commenting
	/*
	if(node == ROOT)
	{
	//no updation required here
	}
	else
	{
		nodeID[node].numSchedElem = nodeID[node].tempNumSchedElem;
	}
	*/

	if(nodeID[node].numSchedElem > 0 && (nextDataBoundary == 0 || nextDataBoundary == UNDEFINED))
	{
		fprintf(stderr,"\n Sanity check failed inside set up schedule: nextDataBoundary %lf lastScheduleTime %lf",nextDataBoundary, nodeID[node].lastScheduleTime);
		exit(0);	
	} // this if condition is never going to get executed since nextdataboundary has just been set to simtime except for the beginning of simulation
	
	nextDataBoundary -= fmod(nextDataBoundary, FRAME_DURATION); 
	nextDataBoundary += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION); 
	//the above two statements bring us at the beginning of the data slots in the frame

	//likely to be removed
	nodeID[node].scheduleInterval = scheduleInterval;
	nodeID[node].scheduleSlots = scheduleSlots;

	//MANAN_CHANGE
	//schedule changed
	//copy elements from parent	
	if(node == ROOT)
	{
		//VJ Start
		nodeID[ROOT].numSchedElem = globalNumSchedElem;
		//VJ End
		
		for(i=0; i < nodeID[ROOT].numSchedElem; i++)
		{
			nodeID[ROOT].localScheduleElements[i] = globalScheduleElements[i];
		}
	}
	else
	{
		//VJ Start
		//No need to explicitly handle scheduleMissed
		//schedulePacketReception takes care of invoking setUpSchedule at right time
		//VJ End
	
		//if(scheduleIsMissed(node) == TRUE)
		//{
		//do not update scheduling elements
		//}
		//else
		//{
			nodeID[node].numSchedElem = nodeID[nodeID[node].parent].numSchedElem;
		
			if(node == 2)
			{
				//fprintf(stderr,"\n node 2: numSchedElem %d lst %lf p %d pnum %d simTime %lf", nodeID[node].numSchedElem, nodeID[node].lastScheduleTime, nodeID[node].parent, nodeID[nodeID[node].parent].numSchedElem, simTime);
			}
		
			if(node == 6 && nodeID[ROOT].numSchedElem == 20)
			{
				//fprintf(stderr,"\n schedElem node 6 %d", nodeID[node].numSchedElem);
				for(i=0; i < nodeID[node].numSchedElem; i++)
				{
					nodeID[node].localScheduleElements[i] = nodeID[nodeID[node].parent].localScheduleElements[i];
				
					//fprintf(stderr,"\n tx %d rx %d f %d s %d", nodeID[node].localScheduleElements[i].transmitter, nodeID[node].localScheduleElements[i].receiver, EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel), nodeID[node].localScheduleElements[i].slotToStart);
				
				}
			}	
				
			//if(node == 6 && nodeID[ROOT].numSchedElem == 20)
			//fprintf(stderr,"\n 2... \n");
			
			nodeID[node].numSchedElem = nodeID[ROOT].numSchedElem;
			
			for(i=0; i < nodeID[node].numSchedElem; i++)
			{
				nodeID[node].localScheduleElements[i] = nodeID[ROOT].localScheduleElements[i];
				
				if(node == 6 && nodeID[ROOT].numSchedElem == 20)
				{
					//fprintf(stderr,"\n tx %d rx %d f %d s %d", nodeID[node].localScheduleElements[i].transmitter, nodeID[node].localScheduleElements[i].receiver, EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel), nodeID[node].localScheduleElements[i].slotToStart);
				}
				
				if(node == 6 && nodeID[node].localScheduleElements[i].transmitter == 6 && EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) == 19)
				{
					//fprintf(stderr,"\n node 6 is tx of flow 19 slot %d", nodeID[node].localScheduleElements[i].slotToStart);
					//exit(0);
				}
				
			}
		//}
	}
	
	//copied elements from parents
	//now process them
	
	//fprintf(stderr,"\n...schedule received by %d numOfFlowsInProgress %d numSchedElem %d\n",node, numOfFlowsInProgress, nodeID[node].numSchedElem);
	
	//fprintf(stderr,"\n Schedule changed");
	
	if(node == 76)
	{
		//fprintf(stderr,"\n Schedule change received");
	}
	
	if(nodeID[node].numOfPacketsInDataQueue == QUEUE_LIMIT)
		nodeID[node].numOfPacketsInDataQueue = 0;
	
	for(i=0;i<nodeID[node].numSchedElem;i++)
	{
	
		if(nodeID[node].localScheduleElements[i].transmitter == node)
		{
			thisNodeSlots++;
			
			for(j=0; j<numOfFlowsInProgress; j++)
			{
					
				if(flowStat[j].flowId == EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel))
				{
				
					//fprintf(stderr,"\n...%d %d\n",flowStat[j].source, flowStat[j].destination);
				
					flowSource = flowStat[j].source;
					flowDestination = flowStat[j].destination; // portability change
					
					if(flowStat[j].flowId == 193)
					{
						//fprintf(stderr,"\n setup-init: node %d s %d d %d", node, flowSource, flowDestination);
					}
					
					
					break;
				}	
			}	
			
			if(j == numOfFlowsInProgress)
			{
				continue;
			}
			
			if(node == 2 && flowStat[j].flowId == 3)
			{
				//fprintf(stderr,"\n 2,3 %d", nodeID[node].flowScheduleSet[flowStat[j].flowId]);
				//sleep(2);
			}	
			
			if(flowStat[j].flowSentAt + flowTimeOutLimit < activeFlowIDTime[flowStat[j].flowId])
			{
				//fprintf(stderr,"\n Flow grant for %d flow exceeded limit sentat %lf grantat %lf: node %d", flowStat[j].flowId, flowStat[j].flowSentAt, activeFlowIDTime[flowStat[j].flowId], node);
				//sleep(2);
				continue;
			}
			
			
			if(nodeID[node].flowScheduleSet[flowStat[j].flowId] == 1)
			{
			
				if(node == 6 && nodeID[node].localScheduleElements[i].transmitter == 6 && EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) == 19)
				{
					//fprintf(stderr,"\n node 6 has flow 19 set slot %d", nodeID[node].localScheduleElements[i].slotToStart);
					//exit(0);
				}
			
				continue;
			}
			else
			{
			
				if(node == 6 && flowStat[j].flowId == 19)
				{
					//fprintf(stderr,"\n event should set for node 6 for flow 19 %d\n", nodeID[node].localScheduleElements[i].slotToStart);
				}
			
				if(newFlowIdIndex == 0)
				{
					newFlowId[newFlowIdIndex] = flowStat[j].flowId;
					newFlowIdIndex++;
				}
				else
				{
					if(newFlowId[--newFlowIdIndex] != flowStat[j].flowId)
					{
						newFlowId[++newFlowIdIndex] = flowStat[j].flowId;
						newFlowIdIndex++;
					}
					else
					{
						++newFlowIdIndex;
					}
					
				}	
			}
			
			if(node == flowSource)
			{
				//fprintf(stderr,"\n Schedule reached source %d %d \n",flowStat[j].source, flowStat[j].flowId);
			}
			if(node == flowDestination)
			{
				//fprintf(stderr,"\n Schedule reached destination \n",flowStat[j].destination, flowStat[j].flowId);
			}
			
			
			if(flowSource == node && nodeID[node].callInProgress == FALSE)
			{	
				nodeID[node].flowRequestGranted = 1;
				inScheduleElem = 1;   // node found in schedule
				
				//fprintf(stderr,"\n\n %d ->-> node's request accepted at %lf\n\n",node, simTime);

				//mobility change 26-4-10
				if(nodeID[node].expectingPathChange == TRUE)
				{
					if(nodeID[node].localScheduleElements[i].receiver != nodeID[node].next_hop)
					{
						nodeID[node].expectingPathChange = FALSE;
					//fprintf(stderr," I ExpectingPathChange for %d is set to false, the new next hop is %d \n", node, nodeID[node].localScheduleElements[i].receiver);
					//logMobilityStats(NEW_PATH_RECEIVED, node, nodeID[node].localScheduleElements[i].receiver, simTime);
					}
				}
				nodeID[node].next_hop = nodeID[node].localScheduleElements[i].receiver;
				//fprintf(stderr," I Next hop for %d is %d \n", node, nodeID[node].next_hop);

				if (nodeID[node].callRequestSent == 0)    // request already timed out, cannot generate data for this call.
				{
					return SUCCESS;
				}	
				else
				{
					fz = fopen("outputFiles/callAccepted.txt","a+");
					fprintf(fz,"\n flowId %d %d source %d %d",EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel), flowStat[j].flowId, node, flowSource);
					fclose(fz);
					numSuccessCalls[EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel)] = 1;
				}
				

				tempe2eDestination = flowStat[j].destination; //getDestination(node);
				
				if(EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) == 193)	
				{
					//fprintf(stderr,"\n setup: e2eDestination %d %d node %d", tempe2eDestination, flowStat[j].destination, node);
				}
				
				nodeStat[node].timeFlowGrantReceived = simTime;
				logNodeStats(node,TIME_FLOW_GRANT,(double)simTime);	
				logFlowStats(EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel),"\n Time_Flow_Request_Granted",(double)simTime);
				
				//nextDataTransmit = getNextDataTransmitTime(nextDataBoundary, nodeID[node].localScheduleElements[i].slotToStart - 1);
				
				
				//*************************************************
				
				nextDataBoundary = activeFlowIDTime[EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel)];
				if(nextDataBoundary > simTime)
				{
					nextDataBoundary -= fmod(nextDataBoundary, FRAME_DURATION); 
					nextDataBoundary += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION); 
				}
				else
				{
					nextDataBoundary = simTime;
					nextDataBoundary -= fmod(simTime, FRAME_DURATION); 
					nextDataBoundary += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION); 
				}	
				
				nextDataTransmit = nextDataBoundary;
				//fprintf(stderr,"\n NextDataTransmit %lf slotToStart %d",nextDataTransmit,nodeID[node].localScheduleElements[i].slotToStart);
				
				
				#ifdef DEBUG_FRAC
	
				//fprintf(stderr,"\n 1 Inside setupNewSchedule nextDataTransmit %lf", nextDataTransmit);
				//sleep(2);
	
				#endif	
				
				for(k=0;k<(nodeID[node].localScheduleElements[i].slotToStart - 1);k++)
				{
					nextDataTransmit += (double)DATA_SLOT_DURATION;
		
					#ifdef DEBUG_FRAC
	
					//fprintf(stderr,"\n Inside setupNewSchedule nextDataTransmit %lf k %d", nextDataTransmit, k);
					//sleep(2);
	
					#endif	
					
					if(fmod(nextDataTransmit, FRAME_DURATION) == 0)
					{
						nextDataTransmit += (double)(NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION);
						
						#ifdef DEBUG_FRAC
	
						fprintf(stderr,"\n Inside fmod nextDataTransmit %lf k %d", nextDataTransmit, k);
						sleep(2);
	
						#endif	
						
					}
				}		
				
				//**************************************************
				
				if(nextDataTransmit < simTime)
				{
					fprintf(stderr,"\n --> sanity check failed: node %d nextDataTransmit %lf nextDataBoundary %lf simTime %lf validfrom %lf root's validfrom %lf timeLastScheduleReception %lf parent's validfrom %lf \n", node, nextDataTransmit, nextDataBoundary, simTime, nodeID[node].scheduleValidFrom, nodeID[ROOT].scheduleValidFrom, nodeID[node].timeLastScheduleReception, nodeID[nodeID[node].parent].scheduleValidFrom);
					printf("\n sanity check failed: nextDataTransmit %lf nextDataBoundary %lf simTime %lf\n",nextDataTransmit, nextDataBoundary, simTime);
					exit(0);						
				}
				
				//VJ change: 15th Feb, 2011: added seqNumber
				
				nodeID[node].seqNumber = 0;

				anEvent = getDataSourceSendEvent(nextDataTransmit , node, tempLength, nodeID[node].seqNumber, node, tempe2eDestination, EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel), node, nodeID[node].localScheduleElements[i].receiver, nodeID[node].localScheduleElements[i].slotToStart, TX);
				insertEvent(anEvent);
				
				//fprintf(stderr,"\n Data generate event node %d slot %d flow %d time %lf simTime %lf", node, nodeID[node].localScheduleElements[i].slotToStart, EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) ,nextDataTransmit, simTime);
		
				if(EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) == 19)
				{
					//fprintf(stderr,"\n Flow source set at %lf for flow %d at %lf node %d <<< scheduleTime %lf", nextDataTransmit, EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel), simTime, node, nodeID[node].lastScheduleTime);
				}
					
				if(node == flowSource)
				{
					//fprintf(stderr,"\n Source set %d flow %d", node, flowStat[j].flowId);
					//sleep(2);
				}
				
				nodeID[node].callInProgress = TRUE;	//very important step
				
			}
			else
			{	
				inScheduleElem = 1;   // node found in schedule

				//mobility change 26-4-10
				/*
				if(nodeID[node].type == MOBILE)
				{
					if(nodeID[node].expectingPathChange == TRUE)
					{
						if(nodeID[node].localScheduleElements[i].receiver != nodeID[node].next_hop)
						{
						nodeID[node].expectingPathChange = FALSE;
						//fprintf(stderr," II ExpectingPathChange for %d is set to false, the new next hop is %d \n", node, nodeID[node].localScheduleElements[i].receiver);
						logMobilityStats(NEW_PATH_RECEIVED, node, nodeID[node].localScheduleElements[i].receiver, simTime);
						}
					}

					nodeID[node].next_hop = nodeID[node].localScheduleElements[i].receiver;
					//fprintf(stderr," II Next hop for %d is %d \n", node, nodeID[node].next_hop);
				}
				*/
				/*if((flowDestination == node) && (nodeID[node].callInProgress == FALSE)) // portability change
				{
				nodeID[node].callInProgress = TRUE;	
				fprintf(stderr,"Call in progress set to true for receiver %d \n", node);
				}*/

				//nextDataTransmit = (double) getNextDataTransmitTime(nextDataBoundary, nodeID[node].localScheduleElements[i].slotToStart - 1);	
				
				//*************************************************
				
				
				nextDataBoundary = activeFlowIDTime[EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel)];
				if(nextDataBoundary > simTime)
				{
					nextDataBoundary -= fmod(nextDataBoundary, FRAME_DURATION); 
					nextDataBoundary += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION); 
				}
				else
				{
					nextDataBoundary = simTime;
					nextDataBoundary -= fmod(simTime, FRAME_DURATION); 
					nextDataBoundary += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION); 
				}
	
				nextDataTransmit = nextDataBoundary;
				
				#ifdef DEBUG_FRAC
	
				//fprintf(stderr,"\n 2 Inside setupNewSchedule nextDataTransmit %lf", nextDataTransmit);
				//sleep(2);
	
				#endif	
				
				
				//fprintf(stderr,"\n NextDataTransmit %lf slotToStart %d",nextDataTransmit,nodeID[node].localScheduleElements[i].slotToStart);
				
				for(k=0;k<(nodeID[node].localScheduleElements[i].slotToStart - 1);k++)
				{
					nextDataTransmit += (double)DATA_SLOT_DURATION;
					
					#ifdef DEBUG_FRAC
	
					//fprintf(stderr,"\n Inside setupNewSchedule nextDataTransmit %lf k %d", nextDataTransmit, k);
					//sleep(2);
	
					#endif	
		
					if(fmod(nextDataTransmit, FRAME_DURATION) == 0)
					{
						nextDataTransmit += (double)(NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION);
						
						#ifdef DEBUG_FRAC
	
						//fprintf(stderr,"\n Inside fmod nextDataTransmit %lf k %d", nextDataTransmit, k);
						//sleep(2);
	
						#endif	
						
					}
				}		
				
				//**************************************************
				
				if(nextDataTransmit < simTime)
				{
					fprintf(stderr,"\n sanity check failed: node %d nextDataTransmit %lf nextDataBoundary %lf simTime %lf valid from %lf root's validfrom %lf timeLastScheduleReception %lf parent's validfrom %lf \n", node, nextDataTransmit, nextDataBoundary,simTime, nodeID[node].scheduleValidFrom, nodeID[node].scheduleValidFrom, nodeID[node].timeLastScheduleReception, nodeID[nodeID[node].parent].scheduleValidFrom);
					printf("\n sanity check failed: nextDataTransmit %lf nextDataBoundary %lf simTime %lf\n",nextDataTransmit, nextDataBoundary,simTime);
					exit(0);	
				}
				
				anEvent = getDataSendEvent(nextDataTransmit, node, nodeID[node].localScheduleElements[i]); 								
				insertEvent(anEvent);
				
				
				if(EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel) == 19)
				{
					//fprintf(stderr,"\n Flow %d set at >>> %lf for %d at %lf p %d parentTime %lf <<< schedTime %lf", EXTRACT_FLOWID(nodeID[node].localScheduleElements[i].flowId_channel), nextDataTransmit, node, simTime, nodeID[node].parent, nodeID[nodeID[node].parent].lastScheduleTime, nodeID[node].lastScheduleTime);
				}
				
				//fprintf(stderr,"\n Data send event node %d slot %d time %lf simTime %lf", node, nodeID[node].localScheduleElements[i].slotToStart, nextDataTransmit, simTime);
				
				if(node == flowDestination)
				{
					//fprintf(stderr,"\n Destination set %d flow %d", node, flowStat[j].flowId);
					//sleep(2);
				}
				
			}
		}
		
	}
	
	for(j=0; j<newFlowIdIndex; j++)
	{	
		nodeID[node].flowScheduleSet[newFlowId[j]] = 1;
		
		if(node == 2 && newFlowId[j] == 3)
		{
			//fprintf(stderr,"\n flow %d set at %d", newFlowId[j], node);
			//sleep(5);
		}	
	}
	
	/*
	
	//node disable change, if the node has no part in schedule and if some call duration remaining, then reset. Happens only due to BW-request timeout after call is established before max_retry. If call established after max_retry , ie schedule didnt reach on time, in handlePriorityQueue, relevant parameters reset
	if(inScheduleElem == 0 && nodeID[node].callDuration > 0 && nodeID[node].callInProgress == TRUE)
	{
		nodeID[node].callDuration = 0;
		fprintf(stderr,"\n node %d cd set to 0 %lf",node, nodeID[node].callDuration);
		exit(0);
		//fprintf(stderr,"Reached the not in schedule and call duration remaining condition \n");
		//make a watchpoint here
	}
	//mobility change 26-4-10
	if(inScheduleElem == 0 && nodeID[node].type == MOBILE)
	{
		clearDataHistory(node);
		nodeID[node].next_hop = -1;

		if(nodeID[node].callInProgress == TRUE)
		{
			//fprintf(stderr,"Reached here to set call in progress as false, should be wrt voice msgs only \n");
			nodeID[node].callInProgress = FALSE;

			if(node == 25)
			{
			canStartFlowRequest(node); // mobility hack 8-5-10
			}
		}
	}
	//mobility change 3-5-10
	if(inScheduleElem == 0 && nodeID[node].type == INFRA)
	{
		nodeID[node].dataFront = nodeID[node].dataRear = 0;
		nodeID[node].numOfPacketsInDataQueue = 0;
	}
	//mobility change 5-5-10
	if(inScheduleElem == 1 && nodeID[node].type == INFRA)
	{
		refreshDataQueue(node);
		//printDataQueue(node);
	}
	*/
	
	//printf("\n Setting up new schedule <--> Allocated slots for (%d) %d",node,thisNodeSlots);	 
}
//**********************************************************************************************************
// this function does not seem to be used, likely to be removed

int getUnusedFlowId()
{
	int i, flowId;
	
	flowId = globalUniqueFlowId;
	globalUniqueFlowId++;
	return flowId;		
}
/*	
	for(i=0; i<MAX_FLOW_ID; i++)
	{
		if(flowIdPool[i] == 0)
		{
			flowIdPool[i] = 1;
			return (i+1);
		}	
	}
	
	printf("Dearth of flow ids");
	exit(0);
}
*/
int chooseDestination(int node)
{
	int i;
	int destination;

	i = 0;
	while(i<100)
	{
		destination = numOfInfraNodesInNetwork + (rand() % numOfMobileNodesInNetwork);

		if(destination != node && (isPartOfAnyFlow(destination) == FALSE) && destination != 25) // portability change, mobility hack 8-5-10 
			return destination;
		i++;
	}
	
	fprintf(stderr,"\n Destination num can not be found in 100 random attempts");	
        return (numOfInfraNodesInNetwork + (node + 10) % numOfMobileNodesInNetwork);
	
}



int scheduleFlowRequest(int node)
{
	FILE *fp, *fq, *fr, *fs;

	struct event * anEvent;
	struct routingPath * aPath;
	int flowDestination;
	double flowScheduleTime;
	double callDuration;
	int tempFlowID;
	
	double fsTimeToStart, fsCallDuration;
	int fsFlowId, fsSource, fsDestination; 
	
	int i;
	
	char *filename, *convStr, *flowStr;	
	
	char ch;

	if( simTime + ((double) (10*60*1000)) > endSimTime )
	{
		return SUCCESS;
	}

	flowStr = (char *)malloc(20 * sizeof(char));
	convStr = (char *)malloc(15 * sizeof(char));
	
	//putting in the hacks for flow destination, change them back later
	//if(node == 25 )
	//flowDestination = 101;
	/*if(node == 42)
	flowDestination = 38;
	if(node == 43)
	flowDestination = 35;
	if(node == 36)
	flowDestination = 40;
	if(node == 32)
	flowDestination = 44;
	if(node == 37)
	flowDestination = 27;*/

	
	//hacks end for the flow destination
 	
 	#if GENERATE_REQUESTS_FROM_FILE
 	fs = fopen("recordFlow.txt","r");
 	
 	while((ch = fgetc(fs)) != EOF)
	{
		if(ch == '(')
		{
			fscanf(fs,"%lf",&fsTimeToStart);
			fscanf(fs,"%d",&fsFlowId);
			
			fscanf(fs,"%d",&fsSource);
			fscanf(fs,"%d",&fsDestination);
			fscanf(fs,"%lf",&fsCallDuration);
			
			//fprintf(stderr,"\n st %lf f %d s %d d %d cd %lf", fsTimeToStart, fsFlowId, fsSource, fsDestination, fsCallDuration);
			//sleep(2);

			if(node == fsFlowId)
				break;
						
		}	
	}
	node = fsSource;
	fclose(fs);
 	
 	#else
 	
 	#endif

	
	
	if(node < numOfInfraNodesInNetwork)
	{
		fprintf(stderr,"\n\n Infra Nodes can not initiate flow request: node %d",node);
		printf("\n\n Infra Nodes can not initiate flow request: node %d",node);
		//return SUCCESS;
		exit(0);
	}
	
	
 	
 	#if GENERATE_REQUESTS_FROM_FILE
 	
 	flowDestination = fsDestination;
	tempFlowID = fsFlowId;
 	
 	#else
 	
	flowDestination = chooseDestination(node);
	tempFlowID = getUnusedFlowId();
	
	#endif
	
	//node disable hack
	/*if(simTime > 1440000 && real_time_call_counter < 1) // simTime greater than 24 minutes coz topology update will timeout in 3 min 20 seconds
	{
	flowDestination = 44; // the mobile node which was chosen to be disabled
	real_time_call_counter++;

	fprintf(stderr,"\n This condition is executed \n");
	}
	if(node == 12)
	{
	flowDestination = chooseDestination(node);
	}
	if(simTime > 1980000 && real_time_call_counter == 1) // the mobile should now be awake
	{
	flowDestination = 44; // the mobile node which was chosen to be disabled
	real_time_call_counter++;
	}*/

	aPath = getRoutingPath(node, flowDestination);	
	//insert new flow element in queue
	
	flowStat[numOfFlowsInProgress].source = node;
	flowStat[numOfFlowsInProgress].destination = flowDestination;
	flowStat[numOfFlowsInProgress].flowId = tempFlowID;
	flowStat[numOfFlowsInProgress].flowScheduleSet = 0;
	flowStat[numOfFlowsInProgress].voiceFlow = 0;
	flowStat[numOfFlowsInProgress].aPath = aPath; // portability change
	

	numOfFlowsInProgress++;
	//fprintf(stderr,"\t Flow no %d added, total flows %d", tempFlowID, numOfFlowsInProgress);
	
	//********Rajesh change******

	nodeID[flowDestination].prevSeqNumber = NEGATIVE_UNDEFINED;
	nodeID[flowDestination].seqNumber = 0;
	for(i = 0; i < DATA_PACKET_SLIDING_WINDOW; i++)
	{
		nodeID[flowDestination].slidingWindow[i] = 1;
	}
	nodeID[flowDestination].windowCount = 0;
	
	nodeID[node].prevSeqNumber = NEGATIVE_UNDEFINED;
	nodeID[node].seqNumber = 0;
	for(i = 0; i < DATA_PACKET_SLIDING_WINDOW; i++)
	{
		nodeID[node].slidingWindow[i] = 1;
	}
	nodeID[node].windowCount = 0;
	
	//********Rajesh change ends******	
	

	strcpy(flowStr, "flows/flow");
	sprintf(convStr,"%d",tempFlowID);
	filename = strcat(flowStr,convStr);

	fp = fopen(filename,"w+");
	
	fq = fopen("outputFiles/flowStartTimes.txt","a+");
	
	noOfCallsOriginated++;

	nodeID[node].callRequestSent = 1;   // call request in progress, very important step

	nodeStat[node].timeFlowRequestSent = simTime;
	logNodeStats(node,TIME_FLOW_REQUEST,(double)simTime);



	//***********************************************************************
	double x;
	double z;                     // Uniform random number (0 < z < 1)
	double exp_value;             // Computed exponential value to be returned

	x = (double) MEAN_CALL_DURATION;

	// Pull a uniform random number (0 < z < 1)	
	do
	{
		z = (double) (rand()/(double)(RAND_MAX));
  	}while ((z == 0) || (z == 1));

  	// Compute exponential random variable using inversion method
  	exp_value = -x * log(z);
  	//fprintf(stderr,"\n z %lf log(z) %lf",z,log(z));
	//************************************************************************

	//hack for callDuration
	/*if(node == 41)
	callDuration = 1200000; //should get revoked through dropInfraFlows
	if(node == 42)
	callDuration = 330000; // normal revoking
	if(node == 36)
	callDuration = 600000; // should timeout due to bw_req_timeout
	if(node == 43)
	callDuration = 1200000; // nothing should happen, should get over after 20 minutes
	if(node == 32)
	callDuration = 60000;*/
	
	#if GENERATE_REQUESTS_FROM_FILE

	callDuration = fsCallDuration;
		
	#else	
	
	callDuration = exp_value;
	
	#endif
	
	//callDuration = 720000;
	//hack ends for call duration
	//callDuration = (double)exponDistriValue((double)MEAN_CALL_DURATION);

	//mobility hack 8-5-10
	if(node == 25)
	{
		callDuration = 300000;
	}
			
	if(callDuration == 0)
	{
		fprintf(stderr,"\n Sanity check failed while retriving callDuration");
		exit(0);
	}		
			
	nodeID[node].callDuration = callDuration;
	
	//fprintf(stderr,"\n node %d cd %lf cd %lf", node, nodeID[node].callDuration, nodeID[31].callDuration);
	//sleep(2);
	
	logFlowStats(tempFlowID,"\n Flow_Id",(double)tempFlowID);	
	
	
	//fprintf(stderr,"\n%d %d %d\n",node, flowDestination, aPath->numOfNodesInPath);
	hopDistribution[aPath->numOfNodesInPath-1]++;
	
	//fprintf(stderr,"\n flow id %d s %d d %d hops %d call duration %lf",tempFlowID, node, flowDestination,aPath->numOfNodesInPath-1,(callDuration)/(1000*60));
	
	#if GENERATE_REQUESTS_FROM_FILE
	#else
	fr = fopen("recordFlow.txt", "a+");
	fprintf(fr,"(%lf %d %d %d %lf)\n",simTime, tempFlowID, node, flowDestination, (callDuration));
	fclose(fr);
	#endif
	
	uniqueCallsOriginated[tempFlowID] = 1;
	
	fprintf(dump,"Inside scheduleFlowRequest for node %d destination %d flowId %d hops %d callDuration %lf minutes\n",node,flowDestination,tempFlowID,aPath->numOfNodesInPath-1,(callDuration)/(1000*60));		
	for(i=0; i<aPath->numOfNodesInPath; i++)
	{
		if(nodeStat[aPath->path[i]].endCallTime > simTime)
		{
			if(nodeStat[aPath->path[i]].endCallTime > (nodeStat[aPath->path[i]].upTime + (double)callDuration))
			{
				nodeStat[aPath->path[i]].upTime += (nodeStat[aPath->path[i]].endCallTime - (nodeStat[aPath->path[i]].upTime + (double)callDuration));
				nodeStat[aPath->path[i]].endCallTime = simTime + nodeStat[aPath->path[i]].endCallTime - (nodeStat[aPath->path[i]].upTime + (double)callDuration);
			}
			else
			{
				nodeStat[aPath->path[i]].upTime += ((nodeStat[aPath->path[i]].upTime + (double)callDuration) - nodeStat[aPath->path[i]].endCallTime);
				nodeStat[aPath->path[i]].endCallTime = simTime + (nodeStat[aPath->path[i]].upTime + (double)callDuration) - nodeStat[aPath->path[i]].endCallTime;
			}
		}
		else
		{
			nodeStat[aPath->path[i]].upTime += callDuration;
			nodeStat[aPath->path[i]].endCallTime = simTime + callDuration;
			//so by these means initially 
		}				
	}
	
	logFlowStats(tempFlowID,"\n Source",(double) node);
	logFlowStats(tempFlowID,"\n Destination",(double) flowDestination);
	logFlowStats(tempFlowID,"\n No_Of_Hops",(double) aPath->numOfNodesInPath-1);
	
	logFlowStats(tempFlowID,"\n Parent_Node",(double)nodeID[node].parent);
	logFlowStats(tempFlowID,"\n Parent_Depth",(double)nodeID[nodeID[node].parent].depth);
	logFlowStats(tempFlowID,"\n Time_Flow_Request_Sent",simTime);
	logFlowStats(tempFlowID,"\n Call_Duration", (double) nodeID[node].callDuration);
	
	
	flowScheduleTime = simTime; 
	
	fprintf(fq,"\nflowId %d source %d destination %d time %lf",tempFlowID, node, flowDestination, flowScheduleTime);
	fclose(fq);
	
	anEvent = getFlowRequestEvent(flowScheduleTime, node, flowDestination, FLOWID_BW(tempFlowID, 0),  node, nodeID[node].parent, TX);	
	insertEvent(anEvent);
	
	anEvent = getFlowRequestTimeoutEvent(flowScheduleTime + FLOW_TIMEOUT, node);
	insertEvent(anEvent);
	
	//***********
	nodeID[node].currentFlowId = tempFlowID;
	//***********
	
	//*******************
	nodeID[node].callInProgress = FALSE;
	//nodeID[node].flowRequestGranted = 0; //not needed to set
	//nodeID[node].flowRetryCount = 0; //not needed to set
	//*******************
	nodeID[node].flowTearedDown = FALSE;
	anEvent = getBandwidthRequestEvent(simTime + (double) BW_INTERVAL,FLOWID_BW(tempFlowID,0),node,nodeID[node].parent, TX);
	insertEvent(anEvent);
	
	nodeID[node].expectingFlowRequestAck = FALSE;	//may not have been used, verification pending
	
	//insert next call request
	#if GENERATE_REQUESTS_FROM_FILE
	#else
	canStartFlowRequest(node);
	#endif
	
	fclose(fp);	
	free(flowStr);
	free(convStr);
}

int canStartFlowRequest(int node)
{
	fprintf(dump,"Inside canStartFlowRequest for node %d\n",node);	
	struct event * anEvent;
	double callInterval;	
	
	//***********************************************************************
	double x;
	double z;                     // Uniform random number (0 < z < 1)
	double exp_value;             // Computed exponential value to be returned

	x = (double) MEAN_CALL_FREQUENCY;

	// Pull a uniform random number (0 < z < 1)	
	do
	{
		z = (double) (rand()/(double)(RAND_MAX));
  	}while ((z == 0) || (z == 1));

  	// Compute exponential random variable using inversion method
  	exp_value = -x * log(z);
  	//fprintf(stderr,"\n z %lf log(z) %lf",z,log(z));
	//************************************************************************
	
	//callInterval = (double)exponDistriValue((double) MEAN_CALL_FREQUENCY);
	
	callInterval = exp_value;
	
	
	if(callInterval <= 0)
	{
		fprintf(stderr,"\n Sanity check failed while retriving call interval: callInterval %lf",callInterval);
		exit(0);
	}
	
	//hacks here for making the start time same
	//if(node == 25)
	//{
	//anEvent = getNextFlowRequestEvent(simTime + 1800000, node); 
	//}
	//else
	//{
	//anEvent = getNextFlowRequestEvent(simTime, node);
	//hack ends
	//anEvent = getNextFlowRequestEvent(simTime + (double)callInterval, node);
	anEvent = getNextFlowRequestEvent(simTime + (double)nodeID[node].callDuration + (double)callInterval, node);
	//}
	insertEvent(anEvent);
		
	//fprintf(stderr,"\n Ready to start the flow for node %d at %lf minutes \n",node, (simTime + (double) nodeID[node].callDuration + (double)callInterval)/60000);
	//printf("\n OBSERVE Ready to start the flow for node %d at %lf minutes \n",node, (simTime + (double) nodeID[node].callDuration + (double)callInterval)/60000);
}

//manan change start

int canStartVoiceFlowRequest(int node)
{
	struct event * anEvent;
	
	//anEvent = getNextFlowRequestEvent(simTime + (double)callInterval, node);
	//insertEvent(anEvent);
			
	//printf("\n Ready to start the flow");
}


int scheduleVoiceFlowRequest(int node, int overalle2eDestination)
{
	struct event * anEvent;
	double x;
	double z;                     // Uniform random number (0 < z < 1)
	double exp_value;             // Computed exponential value to be returned
	double voiceMsgDuration;
	FILE *fp, *fq;
	//vm = fopen("voiceMsg.txt","a+");
	struct routingPath * aPath;
	int flowDestination;
	double voiceFlowScheduleTime;
	int tempFlowID,i;
	char *filename, *convStr, *flowStr;

	if( simTime + ((double) (5*60*1000)) > endSimTime )
	{
		return SUCCESS;
	}

	if(node < numOfInfraNodesInNetwork)
	{
		fprintf(stderr,"\n\n Infra Nodes can not initiate flow request: node %d",node);
		//printf("\n\n Infra Nodes can not initiate flow request: node %d",node);
		//return SUCCESS;
		exit(0);
	}

	aPath = getRoutingPath(node,overalle2eDestination);
	voiceHopDistribution[aPath->numOfNodesInPath-1]++; //used for calculating the voice hop distribution
	
	x = (double) MEAN_VOICE_CALL_DURATION;

	// Pull a uniform random number (0 < z < 1)	
	/*do
	{
		z = (double) (rand()/(double)(RAND_MAX));
  	}while ((z == 0) || (z == 1));

  	// Compute exponential random variable using inversion method
  	exp_value = -x * log(z);*/
  	
	voiceMsgDuration = 60000;//hack for voice msg duration value
	//voiceMsgDuration = exp_value;
	
	if(voiceMsgDuration <= 0)
	{
		fprintf(stderr,"\n Sanity check failed while retriving VoiceMsgDuration");
		exit(0);
	}
		
	nodeID[node].callDuration = voiceMsgDuration;
	
	fprintf(stderr,"\n voice msg");
	exit(0);
	

	//nodeID[node].voiceCopyCallDuration = nodeID[node].callDuration; // for keeping a record of the voice call duration
	//fprintf(stderr,"The voice message shall go on for %lf milliseconds ie %lf minutes\n",voiceMsgDuration,voiceMsgDuration/(1000*60));
	//printf("The voice message shall go on for %lf milliseconds ie %lf minutes",voiceMsgDuration,voiceMsgDuration/(1000*60));
	
	flowStr = (char *)malloc(20 * sizeof(char));
	convStr = (char *)malloc(15 * sizeof(char));
		
	//flowDestination = nodeID[node].voiceTempDestination;
	flowDestination = -1;
	tempFlowID = getUnusedFlowId();
	
	//fprintf(stderr,"\nInside scheduleVoiceFlowRequest, flow destination is Node: %d and e2eDestination is %d\n",flowDestination,overalle2eDestination);
	//printf("\nInside scheduleVoiceFlowRequest, flow destination is Node: %d and e2eDestination is %d\n",flowDestination,overalle2eDestination);
	
	//insert new flow element in queue
	flowStat[numOfFlowsInProgress].source = node;
	flowStat[numOfFlowsInProgress].destination = -1; // updated later at the root
	flowStat[numOfFlowsInProgress].flowId = tempFlowID;
	flowStat[numOfFlowsInProgress].voiceFlow = 1;
	flowStat[numOfFlowsInProgress].aPath = aPath; // this should also be later updated at root, portability change

	//at present using the same array for logging voice messages for the sake of consistency	

	numOfFlowsInProgress++;
	//fprintf(stderr,"\t Voice Flow no %d added, total flows %d", tempFlowID, numOfFlowsInProgress);

	strcpy(flowStr, "flows/flow");
	sprintf(convStr,"%d",tempFlowID);
	filename = strcat(flowStr,convStr);

	fp = fopen(filename,"w+");
	
	fq = fopen("outputFiles/flowStartTimes.txt","a+");
	
	noOfCallsOriginated++; // should it be this or not, saw its references, think its not important

	nodeID[node].callRequestSent = 1;   // call request in progress
	
	nodeStat[node].timeFlowRequestSent = simTime; // this should be voiceFlowRequestsent for sure
	logNodeStats(node,TIME_FLOW_REQUEST,(double)simTime); // accordingly here

	logFlowStats(tempFlowID,"\n Flow_Id",(double)tempFlowID);	
	
	voiceFlowScheduleTime = simTime + nodeID[node].callDuration; //thus includes the recording time as well
	
	fprintf(fq,"\nflowId %d source %d destination %d time %lf",tempFlowID, node, overalle2eDestination, voiceFlowScheduleTime);
	fprintf(voice_start,"\nflowId %d source %d destination %d hops %d start_time %lf",tempFlowID, node, overalle2eDestination, (aPath->numOfNodesInPath-1), voiceFlowScheduleTime);
	//fprintf(stderr,"\nflowId %d source %d destination %d time %lf\n",tempFlowID, node, overalle2eDestination, voiceFlowScheduleTime);

	voiceFlowStat[numVoiceMsg].source = node;
	voiceFlowStat[numVoiceMsg].destination = overalle2eDestination;
	voiceFlowStat[numVoiceMsg].hops = aPath->numOfNodesInPath-1;
	voiceFlowStat[numVoiceMsg].start_time = voiceFlowScheduleTime;
	voiceFlowStat[numVoiceMsg].voiceFlowId = tempFlowID;
	voiceFlowStat[numVoiceMsg].fragmented = 0;
	voiceFlowStat[numVoiceMsg].revoked = 0;
	numVoiceMsg++;
	
	fclose(fq);

	//fprintf(stderr,"successful carry forward 0 event executed\n");
	//printf("successful carry forward 0 event executed\n");
	//fclose(vm);
	anEvent = getVoiceFlowRequestEvent(voiceFlowScheduleTime, node, FLOWID_BW(tempFlowID, 0),  node, nodeID[node].parent, overalle2eDestination, voiceMsgDuration, TX);	
	insertEvent(anEvent);
	//}
	
	anEvent = getFlowRequestTimeoutEvent(voiceFlowScheduleTime + FLOW_TIMEOUT, node);
	insertEvent(anEvent);
	
	//***********
	nodeID[node].currentFlowId = tempFlowID;
	//***********
	
	//*******************
	nodeID[node].callInProgress = FALSE;
	//nodeID[node].flowRequestGranted = 0; //not needed to set
	//nodeID[node].flowRetryCount = 0; //not needed to set
	//*******************
	nodeID[node].flowTearedDown = FALSE;
	//anEvent = getBandwidthRequestEvent(voiceFlowScheduleTime + (double) BW_INTERVAL,FLOWID_BW(tempFlowID,0),node,nodeID[node].parent, TX);
	//insertEvent(anEvent);
	
	nodeID[node].expectingFlowRequestAck = FALSE;	//may not have been used, verification pending
	
	fclose(fp);	
	//fclose(vm);
	free(flowStr);
	free(convStr);
}

//manan change end

int scheduleTearDown(int node, int flowId)
{
	struct event * anEvent;
	anEvent = getTearDownEvent(simTime, flowId, node, nodeID[node].parent, TX);
	
	insertEvent(anEvent);
}

//***********************************************************************************************************************

int handleJoinAckTimeout(int node)
{
	struct event * anEvent;
	double joinScheduleTime;
	if(nodeID[node].state == JOINED || nodeID[node].expectingJoinRequestAck == FALSE)
	{
		// to nothing
	}
	else
	{
		//fprintf(stderr,"\n  JOIN ACK TIMEOUT at node %d join request %lf simTime %lf",node,nodeStat[node].timeJoinRequestSent,simTime);
		//exit(0);
		removeFromQueue(node);
		/*
		joinScheduleTime = JOIN_WAIT_TIME + simTime; 
		nodeStat[node].timeJoinRequestSent = joinScheduleTime;
		logNodeStats(node,TIME_JOIN_REQUEST,(double)joinScheduleTime);
		
		anEvent = getJoinRequestEvent(joinScheduleTime, node, node, nodeID[node].parent, TX);
		insertEvent(anEvent);
		anEvent = getJoinRequestTimeoutEvent(joinScheduleTime + JOIN_TIMEOUT, node);
		insertEvent(anEvent);
		*/
		nodeID[node].expectingJoinRequestAck = FALSE;	
		
		//fprintf(stderr,"\n  NEXT JOIN timings at node %d join request %lf simTime %lf",node, joinScheduleTime, joinScheduleTime + JOIN_TIMEOUT);
		
	}
}

int handleFlowAckTimeout(int node)
{
	struct event * anEvent;
	double flowScheduleTime;
	int tempFlowID;
	
	tempFlowID = 1;
	
	if(nodeID[node].state == JOINED || nodeID[node].expectingFlowRequestAck == FALSE)
	{
		//do nothing
	}
	else
	{
		//fprintf(stderr,"\n  FLOW ACK TIMEOUT at node %d",node);
		//noOfFlowRequestsTimedOut++;
		//we dont remove it from the queue coz its just a flow
		
	}
}

/*
int setUpNewSchedule2(int node)
{
	struct event * anEvent;
	struct event * prevEvent;
	struct packetFormat * aPacket;
	struct scheduleElement * anElement;
	int tempLength;
	int tempe2eDestination;
	double nextDataBoundary;
	double nextDataTransmit;
	int i, j, k, flowSource, flowDestination;
	int thisNodeSlots;

	int inScheduleElem=0;     //check if this node part of current schedule
	
	FILE *fz;
	
	tempLength = 40;
	thisNodeSlots = 0;
	
	//VJ: Start
	nextDataBoundary = nodeID[node].lastScheduleTime;
	//nextDataBoundary = simTime; //changed on 24-4-10
	//VJ: End

	if(nodeID[node].numSchedElem > 0 && (nextDataBoundary == 0 || nextDataBoundary == UNDEFINED))
	{
		fprintf(stderr,"\n Sanity check failed inside set up schedule: nextDataBoundary %lf lastScheduleTime %lf",nextDataBoundary, nodeID[node].lastScheduleTime);
		exit(0);	
	} // this if condition is never going to get executed since nextdataboundary has just been set to simtime except for the beginning of simulation
	
	nextDataBoundary -= fmod(nextDataBoundary, FRAME_DURATION); 
	nextDataBoundary += (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION); 
	//the above two statements bring us at the beginning of the data slots in the frame

	//likely to be removed
	nodeID[node].scheduleInterval = scheduleInterval;
	nodeID[node].scheduleSlots = scheduleSlots;
	
	for(i=0;i<nodeID[node].numSchedElem;i++)
	{
	
		if(globalScheduleElements[i].transmitter == node)
		{
			thisNodeSlots++;
			
			for(j=0; j<numOfFlowsInProgress; j++)
			{
					
				if(flowStat[j].flowId == EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel))
				{
					flowSource = flowStat[j].source;
					flowDestination = flowStat[j].destination; // portability change
					break;
				}	
			}	
			
			if(flowSource == node && nodeID[node].callInProgress == FALSE)
			{	
				nodeID[node].flowRequestGranted = 1;
				inScheduleElem = 1;   // node found in schedule

				//mobility change 26-4-10
				if(nodeID[node].expectingPathChange == TRUE)
				{
					if(globalScheduleElements[i].receiver != nodeID[node].next_hop)
					{
					nodeID[node].expectingPathChange = FALSE;
					fprintf(stderr," I ExpectingPathChange for %d is set to false, the new next hop is %d \n", node, globalScheduleElements[i].receiver);
					//logMobilityStats(NEW_PATH_RECEIVED, node, globalScheduleElements[i].receiver, simTime);
					}
				}
				nodeID[node].next_hop = globalScheduleElements[i].receiver;
				//fprintf(stderr," I Next hop for %d is %d \n", node, nodeID[node].next_hop);

				if (nodeID[node].callRequestSent == 0)    // request already timed out, cannot generate data for this call.
					return SUCCESS;
				else
				{
					fz = fopen("callAccepted.txt","a+");
					fprintf(fz,"\n flowId %d %d source %d %d",EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel), flowStat[j].flowId, node, flowSource);
					fclose(fz);
					numSuccessCalls[EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel)] = 1;
				}
				

				tempe2eDestination = getDestination(node);
				
				nodeStat[node].timeFlowGrantReceived = simTime;
				logNodeStats(node,TIME_FLOW_GRANT,(double)simTime);	
				logFlowStats(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel),"\n Time_Flow_Request_Granted",(double)simTime);
				
				//nextDataTransmit = getNextDataTransmitTime(nextDataBoundary, globalScheduleElements[i].slotToStart - 1);
				
				
				//*************************************************
				
				nextDataTransmit = nextDataBoundary;
				//fprintf(stderr,"\n NextDataTransmit %lf slotToStart %d",nextDataTransmit,globalScheduleElements[i].slotToStart);
				
				for(k=0;k<(globalScheduleElements[i].slotToStart - 1);k++)
				{
					nextDataTransmit += (double)DATA_SLOT_DURATION;
		
					if(fmod(nextDataTransmit, FRAME_DURATION) == 0)
					{
						nextDataTransmit += (double)(NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION);
					}
				}		
				
				//**************************************************
				
				if(nextDataTransmit < simTime)
				{
					fprintf(stderr,"\n sanity check failed: node %d nextDataTransmit %lf nextDataBoundary %lf simTime %lf validfrom %lf root's validfrom %lf timeLastScheduleReception %lf parent's validfrom %lf \n", node, nextDataTransmit, nextDataBoundary, simTime, nodeID[node].scheduleValidFrom, nodeID[ROOT].scheduleValidFrom, nodeID[node].timeLastScheduleReception, nodeID[nodeID[node].parent].scheduleValidFrom);
					printf("\n sanity check failed: nextDataTransmit %lf nextDataBoundary %lf simTime %lf\n",nextDataTransmit, nextDataBoundary, simTime);
					exit(0);						
				}
				
				anEvent = getDataSourceSendEvent(nextDataTransmit , node, tempLength, node, tempe2eDestination, EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel), node, globalScheduleElements[i].receiver, globalScheduleElements[i].slotToStart, TX);
				insertEvent(anEvent);
				
				nodeID[node].callInProgress = TRUE;	//very important step
				
			}
			else
			{	
				inScheduleElem = 1;   // node found in schedule

				//mobility change 26-4-10
				if(nodeID[node].type == MOBILE)
				{
					if(nodeID[node].expectingPathChange == TRUE)
					{
						if(globalScheduleElements[i].receiver != nodeID[node].next_hop)
						{
						nodeID[node].expectingPathChange = FALSE;
						fprintf(stderr," II ExpectingPathChange for %d is set to false, the new next hop is %d \n", node, globalScheduleElements[i].receiver);
						logMobilityStats(NEW_PATH_RECEIVED, node, globalScheduleElements[i].receiver, simTime);
						}
					}

					nodeID[node].next_hop = globalScheduleElements[i].receiver;
					//fprintf(stderr," II Next hop for %d is %d \n", node, nodeID[node].next_hop);
				}
				/*if((flowDestination == node) && (nodeID[node].callInProgress == FALSE)) // portability change
				{
				nodeID[node].callInProgress = TRUE;	
				fprintf(stderr,"Call in progress set to true for receiver %d \n", node);
				}///

				//nextDataTransmit = (double) getNextDataTransmitTime(nextDataBoundary, globalScheduleElements[i].slotToStart - 1);	
				
				//*************************************************
				
				nextDataTransmit = nextDataBoundary;
				//fprintf(stderr,"\n NextDataTransmit %lf slotToStart %d",nextDataTransmit,globalScheduleElements[i].slotToStart);
				
				for(k=0;k<(globalScheduleElements[i].slotToStart - 1);k++)
				{
					nextDataTransmit += (double)DATA_SLOT_DURATION;
		
					if(fmod(nextDataTransmit, FRAME_DURATION) == 0)
					{
						nextDataTransmit += (double)(NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION);
					}
				}		
				
				//**************************************************
				
				if(nextDataTransmit < simTime)
				{
					fprintf(stderr,"\n sanity check failed: node %d nextDataTransmit %lf nextDataBoundary %lf simTime %lf valid from %lf root's validfrom %lf timeLastScheduleReception %lf parent's validfrom %lf \n", node, nextDataTransmit, nextDataBoundary,simTime, nodeID[node].scheduleValidFrom, nodeID[node].scheduleValidFrom, nodeID[node].timeLastScheduleReception, nodeID[nodeID[node].parent].scheduleValidFrom);
					printf("\n sanity check failed: nextDataTransmit %lf nextDataBoundary %lf simTime %lf\n",nextDataTransmit, nextDataBoundary,simTime);
					exit(0);	
				}
				
				anEvent = getDataSendEvent(nextDataTransmit, node, globalScheduleElements[i]); 
				insertEvent(anEvent);
			}
		}
		
	}
	//node disable change, if the node has no part in schedule and if some call duration remaining, then reset. Happens only due to BW-request timeout after call is established before max_retry. If call established after max_retry , ie schedule didnt reach on time, in handlePriorityQueue, relevant parameters reset
	if(inScheduleElem == 0 && nodeID[node].callDuration > 0 && nodeID[node].callInProgress == TRUE)
	{
	nodeID[node].callDuration = 0;
	fprintf(stderr,"Reached the not in schedule and call duration remaining condition \n");
	//make a watchpoint here
	}
	//mobility change 26-4-10
	if(inScheduleElem == 0 && nodeID[node].type == MOBILE)
	{
		clearDataHistory(node);
		nodeID[node].next_hop = -1;

		if(nodeID[node].callInProgress == TRUE)
		{
			fprintf(stderr,"Reached here to set call in progress as false, should be wrt voice msgs only \n");
			nodeID[node].callInProgress = FALSE;

			if(node == 25)
			{
			canStartFlowRequest(node); // mobility hack 8-5-10
			}
		}
	}
	//mobility change 3-5-10
	if(inScheduleElem == 0 && nodeID[node].type == INFRA)
	{
		nodeID[node].dataFront = nodeID[node].dataRear = 0;
		nodeID[node].numOfPacketsInDataQueue = 0;
	}
	//mobility change 5-5-10
	if(inScheduleElem == 1 && nodeID[node].type == INFRA)
	{
		refreshDataQueue(node);
		//printDataQueue(node);
	}
	//printf("\n Setting up new schedule <--> Allocated slots for (%d) %d",node,thisNodeSlots);	 
}
*/
