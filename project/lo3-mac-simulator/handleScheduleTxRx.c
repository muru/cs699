#include "header.h"

#define START_FLOW_AT 960000

// not used
/*int updateRSSI(int node, int source, int RSSI)
{
	nodeID[node].nodeRSSI[source] = RSSI;
}*/

//mobility change 14-4-10
void performAveraging(int node, int infra, int rssi)
{
//performing weighted moving average
//First shift all values up and put current rssi in the last place
int i,j;
float average;
struct event * anEvent;

	for(i=0; i < INFRA_HEARD_THRESHOLD -1 ; i++)
	{
		nodeID[node].infra_list[infra].rssi[i] = nodeID[node].infra_list[infra].rssi[i+1];
	}
	nodeID[node].infra_list[infra].rssi[INFRA_HEARD_THRESHOLD-1] = rssi;

//now perform averaging, the 5th observation will be given weight 5, 4th one 4 and so on
//store values in an array rather than print the long lines over and over again
int temp[INFRA_HEARD_THRESHOLD];
	for(i=0; i < INFRA_HEARD_THRESHOLD; i++)
	{
	temp[i] = nodeID[node].infra_list[infra].rssi[i];
	}
	
	average = (float)((temp[0]*1)+(temp[1]*2)+(temp[2]*3)+(temp[3]*4)+(temp[4]*5))/(float)15 ;
	if(node == 25)
	{
	//fprintf(stderr,"The average RSSI value for node %d is %f \n", infra, average);
	}

	//store this information
	nodeID[node].infra_list[infra].average = average;
	// 18-4-10, if parent RSSI goes below X threshold, set expecting parent change to true
	if(nodeID[node].infra_list[infra].infra_node == nodeID[node].parent)
	{
		if((int)(nodeID[node].infra_list[infra].average) < PARENT_RSSI)
		{
		//get its current position and time to verify
		int node_x, node_y;
		getPosition(node, &node_x, &node_y, simTime);
		//exit(0);
		//in order to send the reactive update only once
			if(nodeID[node].state >= JOINED)
			{

			if(nodeID[node].expectingParentChange == TRUE)
			{
			//update already sent
			}
			else
			{
			anEvent = getTopologyUpdateEvent(simTime, node, node, nodeID[node].parent,TX, REACTIVE);	
			insertEvent(anEvent);	
			nodeID[node].expectingParentChange = TRUE;
	
			//fprintf(stderr,"Average for node %d with respect to its parent %d is %f at time %lf, its X and Y coordinates are %d and %d respectively , sending reactive update\n", node, nodeID[node].infra_list[infra].infra_node, average, simTime, node_x, node_y);

			logMobilityStats(EXPECTING_PARENT_CHANGE, node, infra, simTime); // this should always be the format 
			}

			}
		}
	}
	
}

//mobility change 14-4-10
void updateNeighborInfo(int node, int infra, int rssi)
{
	//if(node == 25)  // mobility hack
	//{
	int i,j,k;
	//int fragment_hearing;
	struct event * anEvent;
	
	if(nodeID[node].infra_list[infra].is_neighbor == TRUE)
	{
	nodeID[node].infra_list[infra].timeLastHeard = simTime;
	performAveraging(node, infra, rssi);
	//fprintf(stderr,"Node %d is now a neighbor of %d \n", infra, node);
	}
	else
	{
		if(nodeID[node].infra_list[infra].times_observed == INFRA_HEARD_THRESHOLD)
		{
		//perform averaging, send reactive update, set is_neighbor to true, and start timer
		nodeID[node].infra_list[infra].infra_node = infra;
		nodeID[node].infra_list[infra].is_neighbor = TRUE;
		nodeID[node].infra_list[infra].timeLastHeard = simTime;

		performAveraging(node, infra, rssi);
		//fragment_hearing = FRAGMENT_HEARING(node,infra); 
		anEvent = getNextExpectedFragmentEvent(simTime + FRAGMENT_TIMEOUT, node, infra);
		insertEvent(anEvent);
		//mobility hack 16-4-10, this is reactive, should not result in another topology update event being set. 
		if(simTime > 1200000)
		{
			anEvent = getTopologyUpdateEvent(simTime, node, node, nodeID[node].parent,TX, REACTIVE);	
			insertEvent(anEvent);
		}

			//fprintf(stderr,"RSSI values with which neighbor %d is heard are \n", infra);
			for(i=0; i < INFRA_HEARD_THRESHOLD; i++)
			{
			//fprintf(stderr," RSSI at position %d is %d \n", i, nodeID[node].infra_list[infra].rssi[i]);
			}
			
		}
		else
		{
			for(i=0; i < INFRA_HEARD_THRESHOLD; i++)
			{
				if(nodeID[node].infra_list[infra].rssi[i] == 0) // mobility hack
				{
				nodeID[node].infra_list[infra].rssi[i] = rssi;
				break;
				}
			}
		nodeID[node].infra_list[infra].times_observed++;
		//fprintf(stderr,"Infra %d heard for %d times \n", infra, nodeID[node].infra_list[infra].times_observed);
		}
	}
	//}

}

//mobility change 17-4-10
void getCurrentTopology(int infra)
{
	//fprintf(stderr,"Get Current Topology called with %d \n", infra);

	int i,j,k;
	double distance;
	int infra_x, infra_y, dst_x, dst_y;
	getPosition(infra, &infra_x, &infra_y, simTime);
	
	//purging its entire neighborhhod information
	incomingEdge[j] = 0;
	nodeID[j].numNeighbors = 0;
	nodeID[j].numInterferringNeighbors = 0;

	
	//rebuild the list, the code from node being alive can be used here 
	for(i=0; i < numOfNodesInNetwork; i++)
	{
		if(i == infra)
		{
		//do nothing
		}
		else
		{
		getPosition(i, &dst_x, &dst_y, simTime);
		
		distance = (double)sqrt(pow(infra_x - dst_x, 2) + pow(infra_y - dst_y, 2));
		
			if(distance <= transmissionRangeI)
			{
			
			}
		}
	}
}

// used to register schedule change
int setScheduleInfo()
{
	schedulePacketChange = TRUE;
}

// used both for setting next schedule broadcast and checking schedule reception misses
// setTest is set to 0 for setting
// setTest is set to 1 for checking
double getNextExpectedScheduleTime(double thisTime, int numOfFragments, int setTest)
{
	int i;
	int slack = 1; 
	double returnTime;
	
	if(setTest == 1)
	{
		returnTime = thisTime - fmod(fmod(simTime, FRAME_DURATION), CONTROL_SLOT_DURATION);
	}
	else
	{
		returnTime = thisTime - fmod(fmod(thisTime, FRAME_DURATION), CONTROL_SLOT_DURATION);
	}
	#ifdef DEBUG_FRAC
		//fprintf(stderr,"\n Inside getNextExpectedScheduleTime %lf i %d simTime %lf", returnTime, i, simTime);
		//sleep(2);
	
	#endif	

	if(setTest == 1)
	{
		for(i=1;i<=slack*numOfFragments;i++)
		{
			returnTime = returnTime + CONTROL_SLOT_DURATION;		

			if(fmod(returnTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
			{
				returnTime = returnTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);	
				
				#ifdef DEBUG_FRAC
	
				//fprintf(stderr,"\n Inside getNextExpectedScheduleTime %lf i %d simTime %lf", returnTime, i, simTime);
				//sleep(2);
	
				#endif	
				
				
			}
		}
	}	
	
	
	for(i=1;i<=((numOfPresentInfraNodesInTree+1) * numOfFragments);i++)
	{
		returnTime = returnTime + CONTROL_SLOT_DURATION;		

		if(fmod(returnTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
		{
			returnTime = returnTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);	
			
				#ifdef DEBUG_FRAC
	
				//fprintf(stderr,"\n Inside getNextExpectedScheduleTime %lf i %d simTime %lf", returnTime, i, simTime);
				//sleep(2);
	
				#endif	
		}				
	}
	return returnTime;
}

// called when infra node joins for the first time
// infra node looks at the broadcast turn with respect to parent and sets first broadcast event
double getFirstScheduleBroadcastTime(int node, int numOfFragments, int tempParent)
{
	double nextScheduleTime;
	int i,j;
	
	nextScheduleTime = simTime;
	nextScheduleTime -= fmod(fmod(nextScheduleTime, FRAME_DURATION), CONTROL_SLOT_DURATION);
	nextScheduleTime += CONTROL_SLOT_DURATION;
			
	if(fmod(nextScheduleTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
	{
		nextScheduleTime = nextScheduleTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);
		
		#ifdef DEBUG_FRAC
	
		//fprintf(stderr,"\n Inside getFirstScheduleBroadcast %lf i %d simTime %lf node %d", nextScheduleTime, i, simTime, node);
		//sleep(2);
	
		#endif	
	}

	for(i=1; i<=(nodeID[node].broadcastTurn - nodeID[tempParent].broadcastTurn - 1) ;i++)
	{
		nextScheduleTime = nextScheduleTime + CONTROL_SLOT_DURATION;

		if(fmod(nextScheduleTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
		{
			nextScheduleTime = nextScheduleTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);
			
			#ifdef DEBUG_FRAC
	
			//fprintf(stderr,"\n Inside getFirstScheduleBroadcast %lf i %d simTime %lf node %d", nextScheduleTime, i, simTime, node);
			//sleep(2);
	
			#endif	
		}// this is for skipping over the data slots
	}
	return nextScheduleTime;
}

// once infra node becomes part of the schedule broadcast, it repeats the its turn based on number of infra nodes in the tree
double getNextScheduleBroadcastTime(int node, int numOfFragments)
{
	int i;
	double returnTime;
	
	returnTime = simTime; 
	
	nodeID[node].numOfInfraNodesInTree = numOfPresentInfraNodesInTree;
	
	for(i=1;i<=((nodeID[node].numOfInfraNodesInTree + 1) * numOfFragments);i++)
	{
		returnTime = returnTime + CONTROL_SLOT_DURATION;		

		#ifdef DEBUG_FRAC
	
		//fprintf(stderr,"\n Inside getNextScheduleBroadcast returnTime %lf i %d simTime %lf node %d", returnTime, i, simTime, node);
		//sleep(2);
	
		#endif	
	
		if(fmod(returnTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
		{
			returnTime = returnTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);
			
			#ifdef DEBUG_FRAC
	
			//fprintf(stderr,"\n Inside getNextScheduleBroadcast returnTime %lf i %d simTime %lf node %d", returnTime, i, simTime, node);
			//sleep(2);
	
			#endif	
				
		}				
	}
	
	return returnTime;

}

// set next schedule broadcast time for root
double rootNextScheduleFragmentBroadcast(double time)
{
	double nextScheduleTime;	
	int i;	

	nextScheduleTime = time;
	
	#ifdef DEBUG_FRAC
	
	//fprintf(stderr,"\n Inside rootNextSchedule nextScheduletime Time %lf i %d simTime %lf", nextScheduleTime, i, simTime);
	//sleep(2);
	
	#endif	
	
	for(i=1;i<=(numOfPresentInfraNodesInTree +1);i++)
	{
		nextScheduleTime = nextScheduleTime + CONTROL_SLOT_DURATION;
		if(fmod(nextScheduleTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
		{
			nextScheduleTime = nextScheduleTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);	
			
			#ifdef DEBUG_FRAC
	
			//fprintf(stderr,"\n Inside fmod_rootNextSchedule nextScheduletime Time %lf i %d simTime %lf", nextScheduleTime, i, simTime);
			//sleep(2);
	
			#endif	
			
			
		}				
	}						
	
	#ifdef DEBUG_FRAC
	
	//fprintf(stderr,"\n Inside rootNextSchedule nextScheduletime Time %lf i %d simTime %lf", nextScheduleTime, i, simTime);
	//sleep(2);
	
	#endif	
	
	return nextScheduleTime;
}

//if tree update is on, get the number of tree size in terms of number of schedule fragments. The other function "getNumOfFragments" takes scheduling elements as input and returns number of fragments. This conversion is just to maintain an abstraction of this function.

int convertTreeToSchedElem(int rTree)
{
	int treeToSchedElem = 0;
		
	if(rTree == 1)
	{
		treeToSchedElem = (numOfPresentNodesInTree * 2)/(sizeof(struct scheduleElement));
		if(numOfPresentNodesInTree % (sizeof(struct scheduleElement)) != 0)
			treeToSchedElem++;
	}						
	return treeToSchedElem;
}

//handle schedule broadcast of root and infrastructure nodes, sets RX schedule events of neighbors
//ROOT handles schedule change and inserts next schedule broadcast event in event queue

//************* VJ change start ************* portability changes 

int getParent(int node)
{
	int i;
	
	if(node == ROOT)
	{
		return ROOT;
	}
	
	//fprintf(stderr,"Node %d, nodes %d", node, numOfNodesInNetwork);
	
	for(i=0;i<numOfNodesInNetwork;i++)
	{
		
		if(routingTree[2*i] == node)
		{
			return routingTree[2*i + 1];
		}			
	}
	return -1;
}

//used for applyJoinPolicy as well
int getNumOfHops(int node)
{
	int countHops=0;
	int retParent;
	
	if(node == ROOT)
	{
		return 1;
	}
	
	//consider only those which are joined
	if(nodeID[node].state > NOT_JOINED)
	{
		retParent = getParent(node);
			
		while(retParent != ROOT)		
		{
			if(retParent == -1)
			{
				return 1;
			}
			node = retParent;
			retParent = getParent(node);
			countHops++;			
		}
		
		return (countHops+1);
	}
	return 1;
}

int maxTreeHops()
{
	int maxHops = 1;
	int retHops = 1;
	int i;
	//for(i=numOfInfraNodesInNetwork; i < (numOfInfraNodesInNetwork + numOfMobileNodesInNetwork); i++)
	for(i=1; i < (numOfInfraNodesInNetwork + numOfMobileNodesInNetwork); i++)
	{
		retHops = getNumOfHops(i);
		
		//printf("\n %d - %d",i,retHops);
		
		if(retHops > maxHops)
		{
			maxHops = retHops;
		}
	}
	return maxHops;
}

//************* VJ change end *************



int setFlowActiveAtOfNewFlows()
{
	int i;
	
	for(i=0; i < nodeID[ROOT].numSchedElem; i++)
	{
		if(activeFlowIDTime[EXTRACT_FLOWID(nodeID[ROOT].localScheduleElements[i].flowId_channel)] != 0)
		{
		}
		else
		{
			activeFlowIDTime[EXTRACT_FLOWID(nodeID[ROOT].localScheduleElements[i].flowId_channel)] = nodeID[ROOT].lastScheduleTime;

		}
	}
}

int handleScheduleBroadcast(int node, struct packetFormat * aPacket)
{
	
	int index, i; //iterators
	//used for intermediate timing calculations
	double tempTime;
	double nextScheduleTime, nextScheduleTimeNonRoot;
	double currentScheduleTime,nextExpectedScheduleTime;
	
	struct event * anEvent;

	int sizeofPacket;
	int numOfFragments;

	//extracting schedule info
	int rTree;	
	int treeToSchedElem;	
	int maxElemInPacket;
	int j;
	int TreeDepth = 1;
	
	maxElemInPacket = getSplitSchedulingElements(); // this just returns the number of scheduling elements that can fit into one packet
		
	currentScheduleTime = simTime;
	currentScheduleTime -= fmod(fmod(currentScheduleTime, FRAME_DURATION), CONTROL_SLOT_DURATION);
	//as far as i think, it gives the beginning of the control slot that would have just started before the current schedule time

	//node disable change
	if(nodeID[node].state == DEAD) //not joined condition checked downstairs
	{
	//fprintf(stderr,"Node returning from here is %d \n", node);  // node does not receive schedule once it is dead. So this line is printed only once. On each schedule broadcast, the next broadcast time is calculated. So once you return, the next chance does not come
		return SUCCESS;
	}
	
	if(node == ROOT)
	{
		
		//VJ Start
		nodeID[ROOT].numSchedElem = globalNumSchedElem;
		//VJ End
		
		for(i=0; i < nodeID[ROOT].numSchedElem; i++)
		{
			nodeID[ROOT].localScheduleElements[i] = globalScheduleElements[i];
		}
		
		
		//fprintf(stderr,"\n SchedElemRoot %d %lf\n", nodeID[ROOT].numSchedElem, simTime);
		if(schedulePacketChange == TRUE)
		{
		schedulePacketChange = FALSE;
		//fprintf(stderr,"\nSchedElemRoot %d\n", nodeID[ROOT].numSchedElem);
		}
		
		
		numOfPresentNodesInTree = numOfNodesInTree;
		numOfPresentInfraNodesInTree = numOfInfraNodesInTree;
		numOfPresentSchedElem = nodeID[ROOT].numSchedElem;
						
		rTree = nodeID[ROOT].treeUpdate;		
		treeToSchedElem = convertTreeToSchedElem(rTree);		
		numOfFragments = getNumOfFragments(nodeID[ROOT].numSchedElem + treeToSchedElem); 
		
		//node disable hack
		//max_no_fragments = numOfFragments;
		if(numOfFragments > max_no_fragments)
		{
			max_no_fragments = numOfFragments;
		}
		//fprintf(stderr,"\nnumOfPresentNodesInTree %d numOfPresentInfraNodesInTree %d numOfPresentSchedElem %d rTree %d treeToSchedElem %d\n",numOfPresentNodesInTree,numOfPresentInfraNodesInTree,numOfPresentSchedElem,rTree,treeToSchedElem);		
		#ifdef DEBUG_FRAC
	
			//fprintf(stderr,"\n Inside fmod_of_broadcast currentScheduleTime %lf", currentScheduleTime);
			//sleep(2);
	
		#endif	
		
		TreeDepth = maxTreeHops();
		
		//fprintf(stderr,"\n depth %d", TreeDepth);
				
		for(i=1;i<=(numOfPresentInfraNodesInTree + 1)*(numOfFragments)*TreeDepth;i++) //changed on 28-4-10
		{	
			currentScheduleTime = currentScheduleTime + CONTROL_SLOT_DURATION;			
			if(fmod(currentScheduleTime, FRAME_DURATION) == NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)
			{
				currentScheduleTime = currentScheduleTime + (NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) + (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION);	
				
				#ifdef DEBUG_FRAC
	
				//fprintf(stderr,"\n Inside fmod_of_broadcast currentScheduleTime %lf i %d simTime %lf node %d", currentScheduleTime, i, simTime, node);
				//sleep(2);
	
				#endif	
				
			}					
		}
		
		#ifdef DEBUG_FRAC
	
		//fprintf(stderr,"\n Outside broadcast scheduleValidFrom %lf i %d simTime %lf node %d", currentScheduleTime, i, simTime, node);
		//sleep(2);
	
		#endif
			
		nodeID[node].scheduleValidFrom = currentScheduleTime;
		
		//fprintf(stderr," root svf %lf\n", nodeID[node].scheduleValidFrom);
		
		nodeID[node].lastScheduleTime = currentScheduleTime; // added on 24-4-10

		setFlowActiveAtOfNewFlows();


		setUpNewSchedule(ROOT); //extract scheduling elements for root
		
		//MANAN_CHANGE
		//VJ start: commenting following
		/*
		for(i=1; i < numOfNodesInNetwork; i++)
		{
		//anEvent = getSetUpNewScheduleEvent(currentScheduleTime,i);
		//insertEvent(anEvent);
			for(j=0; j < nodeID[ROOT].numSchedElem; j++)
			{
				nodeID[i].newScheduleElements[j] = globalScheduleElements[j];
			}

			nodeID[i].tempNumSchedElem = nodeID[ROOT].numSchedElem;
		}
		*/
		//VJ end: commenting end
		
		//setUpNewSchedule(ROOT); //extract scheduling elements for root
		
		//fprintf(stderr,"sim_time %lf current_schedule_time %lf \n", simTime, currentScheduleTime);
		//*****************************************************************		
		logNodeStats(ROOT,SIZE_SCHEDULE,(double)(sizeof(struct schedule) + sizeof(struct packetFormat) + sizeof(struct scheduleElement) * nodeID[ROOT].numSchedElem));	
		logNodeStats(ROOT,NUM_SCHEDULE_FRAGMENTS,(double)numOfFragments);
		
	}

	/*if(node == ROOT)
	{
	setUpNewSchedule(ROOT); //extract scheduling elements for root
	}*/
	
	rTree = nodeID[node].treeUpdate; 
	treeToSchedElem = convertTreeToSchedElem(rTree);	
	numOfFragments = getNumOfFragments(nodeID[node].numSchedElem + treeToSchedElem);
	
	if(numOfFragments > 1)
	{
		nodeID[node].moreFragment = TRUE;
	}
	else
	{
		nodeID[node].moreFragment = FALSE;
	}

	nextScheduleTime = simTime;	
	nextScheduleTime -= fmod(fmod(nextScheduleTime, FRAME_DURATION), CONTROL_SLOT_DURATION);
	
	//**********************************************
	nodeID[node].remSchedElem = nodeID[node].numSchedElem;
	nodeID[node].remTreeNodes = numOfPresentNodesInTree;	

	//fprintf(stderr,"\n Node %d remSchedElem %d remTreeNodes %d currSchedElem %d currTreeNodes %d \n",node,nodeID[node].remSchedElem,nodeID[node].remTreeNodes,nodeID[node].currSchedElem,nodeID[node].currTreeNodes);
	for(index = 0; index < nodeID[node].numNeighbors; index++)
	{
		nodeID[nodeID[node].neighborList[index]].scheduleMissed = FALSE;
	}

	for(i=0; i<numOfFragments; i++)
	{
		//********************************************
		nodeID[node].broadcastSchedFragmentNo = i+1;
		if(nodeID[node].remSchedElem > 0 || nodeID[node].remTreeNodes > 0)
		{
			if(nodeID[node].remSchedElem - maxElemInPacket >= 0)
			{
				nodeID[node].currSchedElem = maxElemInPacket;			
				nodeID[node].remSchedElem -= maxElemInPacket;
				nodeID[node].currTreeNodes = 0;
			}
			else
			{
				nodeID[node].currSchedElem = nodeID[node].remSchedElem;	
				
				if(nodeID[node].treeUpdate == TRUE)
				{
					if((maxElemInPacket - nodeID[node].currSchedElem) * sizeof(struct scheduleElement) > (nodeID[node].remTreeNodes * 2))
						nodeID[node].currTreeNodes = nodeID[node].remTreeNodes;
					else
						nodeID[node].currTreeNodes = ((maxElemInPacket - nodeID[node].currSchedElem) * sizeof(struct scheduleElement))/2;
				}
				else
				{
					nodeID[node].currTreeNodes = 0;
				}		
				nodeID[node].remTreeNodes -= nodeID[node].currTreeNodes;		
				nodeID[node].remSchedElem = 0;
			}
		}
		//********************************************
		
		if(i == (numOfFragments-1))
		{
			nodeID[node].moreFragment = FALSE; // since this is the last fragment
			sizeofPacket = sizeof(struct packetFormat) + sizeof(struct schedule) + nodeID[node].currSchedElem * sizeof(struct scheduleElement) + nodeID[node].currTreeNodes * 2;
			
			if((nodeID[node].currSchedElem * sizeof(struct scheduleElement) + nodeID[node].currTreeNodes * 2) > (maxElemInPacket * sizeof(struct scheduleElement)))
			{
				fprintf(stderr,"\n Size of packet %d exceeds max size %d curr schedelem %d curr tree nodes %d total schedelem %d total tree nodes %d treeToSchedElem %d numOfFragments %d fragment no %d",(nodeID[node].currSchedElem * sizeof(struct scheduleElement) + nodeID[node].currTreeNodes * 2), (maxElemInPacket * sizeof(struct scheduleElement)), nodeID[node].currSchedElem, nodeID[node].currTreeNodes, nodeID[node].numSchedElem, numOfPresentNodesInTree, treeToSchedElem, numOfFragments, i+1);
				exit(0);
			}
			
			if(node != ROOT)
			{							
				
				if(nodeID[node].lastScheduleMissed == FALSE)
				{
					//*************************
					nextScheduleTimeNonRoot = getNextScheduleBroadcastTime(node, numOfFragments);
					anEvent = getScheduleEvent(nextScheduleTimeNonRoot, node, node, TX);
					insertEvent(anEvent);

					//fprintf(stderr,"Infra %d is going to broadcast next schedule at time %lf numOfFragments is %d \n", node, nextScheduleTimeNonRoot, numOfFragments);
				}	

			}	

		}	
		else
		{			
			sizeofPacket = sizeof(struct packetFormat) + sizeof(struct schedule) + getSplitSchedulingElements() * sizeof(struct scheduleElement);
		}
		
		//set schedule reception events of all neighbors of this node
		//mobility abstraction to be put here 
		//getCurrentTopology(node);
		purgeNeighborInfo(node);
		rebuildNeighborInfo(node);	

		for(index = 0; index < nodeID[node].numNeighbors; index++)
		{
			//schedule reception events of ALIVE neighbors who are children			
			if(nodeID[nodeID[node].neighborList[index]].state != DEAD)
			{	
						
				if(!IsPacketToBeDropped(errorRate))
				{
					/*
					nodeID[nodeID[node].neighborList[index]].controlReceptionCount++;
					if(nodeID[nodeID[node].neighborList[index]].controlReceptionCount > 1)
					{
						nodeID[nodeID[node].neighborList[index]].controlReceptionCollision = TRUE;
					}										
					*/					
					anEvent = getScheduleEvent(nextScheduleTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, nodeID[node].neighborList[index], RX);
					insertEvent(anEvent);
				}
				else
				{
					//MANAN_CHANGE
					nodeID[nodeID[node].neighborList[index]].scheduleMissed = TRUE;

					logNodeStats(nodeID[node].neighborList[index],TIME_SCHEDULE_DROP,simTime);
				}
			}
		}

		nextScheduleTime = rootNextScheduleFragmentBroadcast(nextScheduleTime);	
		//nextScheduleTime was equalled to simTime earlier
		//printf("\n simTime %lf nextScheduleTime %lf",simTime, nextScheduleTime);
	}	
	
	//*************************************
	
	if(nodeID[node].treeUpdate == TRUE)
	{
		nodeID[node].treeUpdateBroadcast++;
	}
	
	if(nodeID[node].treeUpdateBroadcast == TREE_UPDATE_REPEAT)
	{
		nodeID[node].treeUpdateBroadcast = 0;
		nodeID[node].treeUpdate = FALSE;
	}
	
	//*************************************
		
	//nodeID[node].treeUpdate = FALSE;					
	
		
	if(node == ROOT)
	{		
		anEvent = getScheduleEvent(nextScheduleTime, ROOT, ROOT, TX);
		insertEvent(anEvent);	
		
		//fprintf(stderr,"Root is going to broadcast next schedule at time %lf simTime is %lf \n", nextScheduleTime, simTime);
	}
		
	return SUCCESS;
}



//get synchronized, update routing tree, for infrastructure nodes: broadcast schedule packet further
int handleScheduleReception(int node, struct packetFormat * aPacket)
{
	struct schedule * aSchedule;
	struct event * anEvent;

	double nextScheduleTime, nextExpectedScheduleTime;
	double joinScheduleTime;
	
	int rTree, moreFrag, schedElements, treeToSchedElem, numOfFragments;
	int schedElem_frag_rTree_repeat;
	int ver_type_piggyback;
	int RSSI;
	int index;
	
	//order in breadth wise transmission
	int numPrevInfraNodes;

	int i;
	int scheduleChangeReceived;
	int stillPartOfTree;
	
	//VJ start
	int tempParent;
	//VJ end	

	numPrevInfraNodes = 0;
	numOfFragments = 0;
	scheduleChangeReceived = FALSE;
	
	//mobility hack 13-4-10
	/*if(node == 25)
	{
	fprintf(stderr,"Schedule received from %d with RSSI %d \n", aPacket->source, aPacket->rssi);
	}*/

	/*if(aPacket->rssi < INFRA_RSSI)
	{
	fprintf(stderr,"Received packet with rssi %d less than INFRA_RSSI from %d \n", aPacket->rssi, aPacket->source);
	exit(0);
	//return SUCCESS;
	}*/

	updateNeighborInfo(node, aPacket->source, aPacket->rssi);	// mobility change 14-4-10

	//schedule received from non parent node
	if(nodeID[node].state != NOT_JOINED && aPacket->source != nodeID[node].parent)
	{	
		return SUCCESS;		
	} // this is for just receiving a schedule from one of its neighbors and sort of ignoring it if i am right
		
	if(nodeID[node].expectingJoinRequestAck == TRUE && nodeID[node].state == NOT_JOINED && aPacket->source != nodeID[node].parent)
	{
		return SUCCESS;
	}//this also i think represents a case when received a schedule from one of its neighbors
	
	if(nodeID[node].expectingJoinRequestAck == TRUE && nodeID[node].state == NOT_JOINED)
	{
		tempParent = aPacket->source;
	}	
				
	aSchedule = (struct schedule *)aPacket->payloadPacket;		
		
	nodeID[node].globalTime = aSchedule->timestamp;
	nodeID[node].offset = nodeID[node].globalTime - nodeID[node].localTime;
	nodeID[node].synchronized = TRUE;
	
	logNodeStats(node,TIME_SYNC,simTime);
		
	if(nodeID[node].state != NOT_JOINED && aSchedule->fragmentNo != nodeID[node].expectedSchedFragmentNo)
	{	
		nodeID[node].expectedSchedFragmentNo = 1;
		if(aSchedule->fragmentNo != 1)
			return SUCCESS;		
	}
	
	//VJ: commented, moved to the part where full schedule is received
	//nodeID[node].scheduleValidFrom = aSchedule->validFrom;
	
	
	//extract schedule info
	schedElem_frag_rTree_repeat = aSchedule->schedElem_frag_rTree_repeat;		
	schedElements = EXTRACT_SCHEDELEM(schedElem_frag_rTree_repeat);
	moreFrag = EXTRACT_FRAG_BIT(schedElem_frag_rTree_repeat);

	nodeID[node].treeUpdate = rTree = EXTRACT_RTREE_BIT(schedElem_frag_rTree_repeat);
	
	if(nodeID[node].lastScheduleTime == UNDEFINED) //executed only once, when node receives the schedule for the first time
	{
		nodeID[node].lastScheduleTime = nodeID[nodeID[node].parent].lastScheduleTime; //aSchedule->validFrom;		
	}
	else
	{
		//set up new schedule by extracting scheduling elements	
									
		if(nodeID[node].state > NOT_JOINED && nodeID[node].lastScheduleTime < nodeID[nodeID[node].parent].lastScheduleTime) //aSchedule->validFrom)
		{
		//scheduleValidFrom gets updated only when schedulePacketChange is set to true, at other times it is set to the old value itself. i guess there is one more place
						
			if(moreFrag == 1)
			{
				nodeID[node].expectedSchedFragmentNo++;
				return SUCCESS;
			}
			
			nodeID[node].scheduleValidFrom = nodeID[nodeID[node].parent].lastScheduleTime; //aSchedule->validFrom;
			
			//if(node < numOfInfraNodesInNetwork)
			//fprintf(stderr,"\n update received by %d",node);
			
			if(node == 76)
			{
				//fprintf(stderr,"\n schedule changed %lf %lf", aSchedule->validFrom, simTime);
			}
			
			if(node == 2 || node == 1)
			{
				//fprintf(stderr,"\n LastScheduleTime at node %d : %lf svf %lf simTime %lf p %d schedParent %lf",node,nodeID[node].lastScheduleTime, nodeID[node].scheduleValidFrom, simTime, nodeID[node].parent, nodeID[nodeID[node].parent].scheduleValidFrom);
				//sleep(5);
			}
			
			//if(aSchedule->validFrom > simTime)
			if(nodeID[nodeID[node].parent].lastScheduleTime > simTime)
				nodeID[node].lastScheduleTime = nodeID[nodeID[node].parent].lastScheduleTime; //aSchedule->validFrom;
			else
				nodeID[node].lastScheduleTime = simTime + (CONTROL_SLOT_DURATION - fmod(fmod(simTime, FRAME_DURATION), CONTROL_SLOT_DURATION));	
				//taking to the start of the next control slot
						
			nodeID[node].numOfInfraNodesInTree = numOfPresentInfraNodesInTree;
			nodeID[node].numSchedElem = numOfPresentSchedElem;
			
			//this event is necessary when schedule interval dynamically changes
			//anEvent = getNewScheduleParamEvent(nodeID[node].lastScheduleTime, node);
			//insertEvent(anEvent);
			
			//nodeID[node].lastScheduleMissed = FALSE;
			
			scheduleChangeReceived = TRUE;
			
			setUpNewSchedule(node); // very important call
		}
	}

	if(moreFrag == 1)  
	{
		nodeID[node].expectedSchedFragmentNo++;
		return SUCCESS;
	}	

	nodeID[node].numOfInfraNodesInTree = numOfPresentInfraNodesInTree;
	nodeID[node].numSchedElem = numOfPresentSchedElem;
		
	nodeID[node].timeLastScheduleReception = simTime;
	
	//nodeID[node].scheduleNumOfFragments = nodeID[node].expectedSchedFragmentNo;
		
	nodeID[node].lastFragmentNo = nodeID[node].expectedSchedFragmentNo;
			
	treeToSchedElem = convertTreeToSchedElem(rTree);	
	nodeID[node].scheduleNumOfFragments = numOfFragments = getNumOfFragments(nodeID[node].numSchedElem + treeToSchedElem);

	//*********************************************
	//VJ: 1/08/10 not sure whether this is required or not
	if(nodeID[node].lastScheduleMissed == TRUE && nodeID[node].state != NOT_JOINED)
	{
		if(nodeID[node].type == INFRA)
		{
			//nextScheduleTime = getFirstScheduleBroadcastTime(node, numOfFragments);	
			nextScheduleTime = getFirstScheduleBroadcastTime(node, numOfFragments, nodeID[node].parent);	
			anEvent = getScheduleEvent(nextScheduleTime, node, node, TX);
			insertEvent(anEvent);
		}	
	}
		
	nodeID[node].lastScheduleMissed = FALSE;
	//********************************************	

	if(nodeID[node].state == NOT_JOINED)
	{
		if(nodeID[node].expectingJoinRequestAck == TRUE)
		{				
			if(rTree == PRESENT) //routing tree update present
			{
				for(index = 0; index < numOfPresentNodesInTree; index++)
				{
					if(nodeID[routingTree[2 * index]].type == INFRA)
						numPrevInfraNodes++;
					if(routingTree[2 * index] == node)
					{
						nodeID[node].broadcastTurn = numPrevInfraNodes; 
						nodeID[node].expectingJoinRequestAck = FALSE;
						nodeID[node].state = JOINED;
						nodeID[node].expectingJoinRequestAck = FALSE; // not needed
						//fprintf(stderr,"\n **** Debug %d node joined at %lf ****",node,simTime);
						
						if(nodeID[node].wasOrphan == TRUE)
						{
							//fprintf(stderr,"\n Orphan node re joined %d",node);
						}
						nodeID[node].wasOrphan = FALSE;
						
						nodeStat[node].timeJoinGrantReceived = simTime;
						logNodeStats(node,TIME_JOIN_GRANT,(double)simTime);
						
						//fprintf(stderr,"\n node %d n(infra nodes) %d simTime %lf START_FLOW_AT %lf", node, numOfInfraNodesInNetwork, simTime, (double) START_FLOW_AT);
						
						#if GENERATE_REQUESTS_FROM_FILE
						#else
						
						if(node >= numOfInfraNodesInNetwork && simTime < (double) START_FLOW_AT)
						{	
							//node disable hack for not allowing rejoined nodes to have new canStartFlow request events
							if(simTime > 1200000)
							{
							//do nothing
							}
							else
							{
							
							countNodesFlow++;
							
							anEvent = getCanStartFlowRequestEvent((double) START_FLOW_AT,node);
							insertEvent(anEvent);
							}
						}// this is where it all starts
						#endif					
						
						nodeID[node].parent = routingTree[2 * index + 1];
						
						nodeID[node].depth = nodeID[nodeID[node].parent].depth + 1;
						
						anEvent = getScheduleReceptionTimeout(simTime + (double)SCHEDULE_TIMEOUT, node);	
						insertEvent(anEvent);
						
						if(nodeID[node].type == INFRA)
						{
							nextScheduleTime = getFirstScheduleBroadcastTime(node, numOfFragments, tempParent);	
							anEvent = getScheduleEvent(nextScheduleTime, node, node, TX);
							insertEvent(anEvent);
						}
						
						//*************************				
						nextExpectedScheduleTime = getNextExpectedScheduleTime(simTime, nodeID[node].scheduleNumOfFragments, 1);
						anEvent = getNextExpectedScheduleEvent(nextExpectedScheduleTime, node);
						insertEvent(anEvent);

						//portability changes start
						double temp;
						temp = generateTopologyUpdateTime(simTime);
						//temp = (double)((rand() % (int) ((TOPOLOGY_UPDATE_PERIOD * SECOND)/FRAME_DURATION)) * FRAME_DURATION);
						//fprintf(stderr,"number of infra nodes in network is %d \n", numOfInfraNodesInNetwork);
						anEvent = getTopologyUpdateEvent(temp, node,node,nodeID[node].parent,TX, REGULAR);	
						insertEvent(anEvent);
						
						//fprintf(stderr,"\n Node: %d  Schedule heard: %lf Topology Update time: %lf \n",node, simTime, temp);
						
						//portability changes end
						
					}					
				}				
			}
			
		}

		//if join request not sent already and thus not joined as yet, send join request
		if(nodeID[node].expectingJoinRequestAck == FALSE && nodeID[node].state == NOT_JOINED)
		{
			//node disable hack
			//if((node == 6 || node == 33 || node == 34 || node == 35 || node == 36 || node == 40 || node == 44) && simTime > 1440000)
			//if(simTime > 1200000)
			//{
			//fprintf(stderr,"Node %d received schedule, source/sponsor is %d \n", node, aPacket->source);
			//exit(0);
			//}
		
			//temporary parent			
			nodeID[node].parent = aPacket->source;
			nodeID[node].sponsor = aPacket->source;
			nodeID[node].sponsorDepth = nodeID[nodeID[node].parent].depth;
			
			//to do:incorporate depth to calculate join_wait_time which is static for now	
			
			joinScheduleTime = JOIN_WAIT_TIME + simTime;
			
			nodeStat[node].timeJoinRequestSent = joinScheduleTime;
			
			logNodeStats(node,TIME_JOIN_REQUEST,(double)simTime);			
			
			anEvent = getJoinRequestEvent(joinScheduleTime, node, node, nodeID[node].parent, TX);
			insertEvent(anEvent);
			anEvent = getJoinRequestTimeoutEvent(joinScheduleTime + JOIN_TIMEOUT, node);
			insertEvent(anEvent);									
			nodeID[node].expectingJoinRequestAck = TRUE;
			
			//fprintf(stderr,"\n Join request sent %d",node);
		}
	}

	//mobility change 19-4-10
	if(nodeID[node].expectingParentChange == TRUE && rTree == PRESENT)
	{
		
		//exit(0);
		for(index = 0; index < numOfPresentNodesInTree; index++)
		{
			if(routingTree[2 * index] == node)
			{
			nodeID[node].parent = routingTree[2 * index + 1];	
			//fprintf(stderr,"reached here to accept parent change due to mobility; for %d, now the parent is %d at time %lf \n", routingTree[2 * index], routingTree[2 * index + 1], simTime);	

			nodeID[node].expectingParentChange = FALSE;
			logMobilityStats(NEW_PARENT_RECEIVED, node, nodeID[node].parent, simTime);
			//exit(0);
			}
		}
	}

	nodeID[node].expectedSchedFragmentNo = 1;
	
	//portability changes start
	stillPartOfTree = FALSE;
	for(index = 0; index < numOfPresentNodesInTree; index++)
	{
		if(routingTree[2 * index] == node)
		{
			stillPartOfTree = TRUE;
		}
	}
	
	if(nodeID[node].state > NOT_JOINED && stillPartOfTree == FALSE)
	{
		//fprintf(stderr,"\n Accident removal by root for node %d \n", node);
		//exit(0);
	}
	//portability changes end

	return SUCCESS;
}


int handleScheduleReceptionTimeout(int node)
{
	int m;
	struct event * anEvent;

	if(nodeID[node].state == DEAD)
	{
		//fprintf(stderr,"\n %d node is already dead",node);
		return SUCCESS;
	}
	
	if(node == ROOT)
	{
		fprintf(stderr,"\n Sanity check failed inside handleschedulereception: node ROOT \n");
		exit(0);
	}
	
	if(nodeID[node].state == NOT_JOINED)
		return SUCCESS;
	
	if(nodeID[node].timeLastScheduleReception == UNDEFINED)
	{
		fprintf(stderr,"\n Sanity check failed inside handleScheduleReceptionTimeout: timeLastScheduleReception UNDEFINED");
		exit(0);
	}
	
	
	if(nodeID[node].timeLastScheduleReception + (SCHEDULE_TIMEOUT) < (simTime + FRAME_DURATION) ) // node disable hack
	{
		nodeID[node].state = NOT_JOINED;
		
		nodeID[node].wasOrphan = TRUE;
		
		nodeID[node].expectingJoinRequestAck = FALSE;
		logNodeStats(node,TIME_NODE_ORPHAN,simTime);
				
		/*for(m=node;m!=ROOT;)
		{
			fprintf(stderr,"\n %d <- %d", m, nodeID[m].parent);
			m = nodeID[m].parent;
		}*/
		
		//printf("\n %d node (parent %d) became orphan: node depth %d lastSchedReception %lf timeout %lf simTime %lf\n",node, nodeID[node].parent, nodeID[node].depth, nodeID[node].timeLastScheduleReception, (double)SCHEDULE_TIMEOUT, simTime);
		//fprintf(stderr,"\n\n **** %d node (parent %d) became orphan: node depth %d lastSchedReception %lf timeout %lf simTime %lf\n",node, nodeID[node].parent, nodeID[node].depth, nodeID[node].timeLastScheduleReception, (double)SCHEDULE_TIMEOUT, simTime);
		//exit(0);
		//sleep(5);

		//node disable change , I think I need to purge the contention and the data queues here
		purgeNodeInfo(node);
		//nodeID[node].numOfPacketsInQueue = nodeID[node].numOfPacketsInDataQueue = 0;
		//nodeID[node].controlFront = nodeID[node].controlRear = nodeID[node].dataFront = nodeID[node].dataRear = 0;

	}
	else
	{
		//anEvent = getScheduleReceptionTimeout(simTime + (double)SCHEDULE_TIMEOUT, node);
		anEvent = getScheduleReceptionTimeout(nodeID[node].timeLastScheduleReception + (double)SCHEDULE_TIMEOUT, node);
		insertEvent(anEvent);	
		//node disable hack
		if(node == 6 || (node >= 33 && node <= 44 ) )
		{
		//fprintf(stderr,"Schedule reception timeout for %d set at %lf minutes \n", node, ((nodeID[node].timeLastScheduleReception + (double)SCHEDULE_TIMEOUT)/(double)(60000)));
		}
	}	
}

int handleExpectedScheduleReception(int node)
{
	int numOfFragments;
	double nextExpectedScheduleTime;
	struct event * anEvent;
	
	if(nodeID[node].timeLastScheduleReception == UNDEFINED)
	{
		fprintf(stderr,"\n Sanity check failed inside handleExpectedScheduleReceptionTimeout: timeLastScheduleReception UNDEFINED");
		exit(0);
	}
	
	numOfFragments = getNumOfFragments(nodeID[node].numSchedElem);
		
	if(getNextExpectedScheduleTime(nodeID[node].timeLastScheduleReception, nodeID[node].scheduleNumOfFragments, 0) < simTime)
	{	
		nodeID[node].numScheduleMissed++;

		if(nodeID[node].numScheduleMissed == SCHEDULE_MISS_THRESHOLD)
		{	
			
			//fprintf(stderr,"\n\n **** %d Schedules Missed at %d lastSchedReceptionTime %lf simTime %lf\n",SCHEDULE_MISS_THRESHOLD,node,nodeID[node].timeLastScheduleReception,simTime);
			//exit(0);
			nodeID[node].lastScheduleMissed = TRUE;		
			nodeID[node].numScheduleMissed = 0;
		}	
	}
	else
	{
		nodeID[node].lastScheduleMissed = FALSE;
		nodeID[node].numScheduleMissed = 0;
	}
	
	//*************************				
	nextExpectedScheduleTime = getNextExpectedScheduleTime(simTime, nodeID[node].scheduleNumOfFragments, 0);
	anEvent = getNextExpectedScheduleEvent(nextExpectedScheduleTime, node);
	insertEvent(anEvent);	
	
}

//mobility change 15-4-10
void handleExpectedFragmentReception(int node, struct packetFormat * aPacket)
{
	int mobile, infra; 
	struct event * anEvent;
	int i,j;

	mobile = node;
	infra = ((struct expected_fragment *)aPacket->payloadPacket)->infra_node;

	//fprintf(stderr,"The mobile is %d and the event is for infra %d \n", mobile, infra);
	
	if(nodeID[mobile].infra_list[infra].timeLastHeard < (simTime - FRAGMENT_TIMEOUT))
	{
		//fprintf(stderr,"Node %d has not heard from infra %d in the last 5 seconds, sending reactive update \n", mobile, infra);

		//reset the information about this node 
		nodeID[mobile].infra_list[infra].infra_node = MAX_NO_INFRA_NODES + 1;
		nodeID[mobile].infra_list[infra].times_observed = 0;
		nodeID[mobile].infra_list[infra].missed = 0;
		for(j=0; j<INFRA_HEARD_THRESHOLD; j++) // portability hack
			{
			nodeID[mobile].infra_list[infra].rssi[j] = 0;
			}
		nodeID[mobile].infra_list[infra].is_neighbor = FALSE;
	
	//now send a reactive update
	//mobility hack 17-4-10
		if(simTime > 1200000)
		{
		anEvent = getTopologyUpdateEvent(simTime, mobile, mobile, nodeID[mobile].parent,TX, REACTIVE);	
		insertEvent(anEvent);
		}	
		
	//exit(0);
	}
	else
	{
	anEvent = getNextExpectedFragmentEvent(simTime + FRAGMENT_TIMEOUT, node, infra);
	insertEvent(anEvent);
	}

}

int handleNewScheduleParam(int node)
{
	nodeID[node].scheduleInterval = scheduleInterval;
	nodeID[node].scheduleSlots = scheduleSlots;
}
