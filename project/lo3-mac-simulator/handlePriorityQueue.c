#include "header.h"

//portability change
int isPartOfAnyFlow(int node)
{
	int i,j;
	for(i=0; i<numOfFlowsInProgress; i++)
	{
		if(flowStat[i].source == node || flowStat[i].destination == node)
		{
			/*if(flowStat[i].destination == node)
			{
			canStartFlowRequest(node);
			fprintf(stderr,"\n Node %d was a part of another call, hence we allow it to make a call later, not now \n", node);
			}
			else
			{
			fprintf(stderr,"Choosing another destination since node %d is on call\n", node);
			}*/
		
		return TRUE;
		}
	}
return FALSE;
}

int printEventQueue()
{
	struct event * anEvent;
	anEvent = queueHead;
	int type;
	
	/*
	anEvent = queueHead;
	
	printf("\n\n Printing event queue");	
	while(anEvent != NULL)
	{
			if(anEvent->type == TX || anEvent->type == RX || anEvent->type == RX_BEGIN || anEvent->type == RX_END)
			{
				type = EXTRACT_TYPE(anEvent->aPacket->ver_type_piggyback);				

				if(type == SCHEDULE)				
				{
					printf("\n Extracted schedule event: Event type %d Event Time %lf Event node %d Source node %d timestamp %lf", anEvent->type, anEvent->time, anEvent->node, anEvent->aPacket->source,((struct schedule *)anEvent->aPacket->payloadPacket)->timestamp);
				}
				if(type == JOIN_REQUEST)
				{
					printf("\n Extracted join request event: Event type %d Event Time %lf Event node %d Source node %d", anEvent->type, anEvent->time, anEvent->node, anEvent->aPacket->source);
				}
				if(type == FLOW_REQUEST)
				{
					printf("\n Extracted flow request event: Event type %d Event Time %lf Event node %d Link Source node %d End Source node %d", anEvent->type, anEvent->time, anEvent->node, anEvent->aPacket->source, ((struct flowRequest *)anEvent->aPacket->payloadPacket)->e2eSource);
				}
				if(type == BW_REQUEST)
				{
					printf("\n Extracted bw request event: Event type %d Event Time %lf Event node %d", anEvent->type, anEvent->time, anEvent->node);
				}
				if(type == TEAR_DOWN)
				{
					printf("\n Extracted tear down event: Event type %d Event Time %lf Event node %d Link Source node %d Flow id %d", anEvent->type, anEvent->time, anEvent->node, anEvent->aPacket->source, ((struct tearDown *)anEvent->aPacket->payloadPacket)->flowId);
				}
				if(type == LINK_LEVEL_ACK)
				{			
					printf("\n Extracted link level ack event: Event type %d Event Time %lf Event node %d Source node %d", anEvent->type, anEvent->time, anEvent->node, anEvent->aPacket->source);
				}
				if(type == DATA)
				{			
					printf("\n Extracted data generate event: Event type %d Event Time %lf Event node %d Source node %d", anEvent->type, anEvent->time, anEvent->node, anEvent->aPacket->source, anEvent->aPacket->destination);
				}
				
			}
			if(anEvent->type == CONTENTION_SLOT_BEGINS)
			{
				printf("\n Extracted contention slot event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}
			if(anEvent->type == JOIN_REQUEST_TIMEOUT)
			{
				printf("\n Extracted join request event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}
			if(anEvent->type == FLOW_REQUEST_TIMEOUT)
			{
				printf("\n Extracted flow request event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}
			if(anEvent->type == BW_REQUEST_TIMEOUT)
			{
				printf("\n Extracted bw time out event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}
			if(anEvent->type == LINK_ACK_TIMEOUT)
			{
				printf("\n Extracted ack time out event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}
			if(anEvent->type == MAKE_ALIVE)
			{
				printf("\n Extracted alive event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}
			if(anEvent->type == DATA_SLOT_TRANSMIT)
			{
				printf("\n Extracted data transmit event: Event type %d Event Time %lf Event node %d",anEvent->type, anEvent->time, anEvent->node);
			}		
			anEvent = anEvent->next;
	}
	printf("\n\n Printing done");
	*/
		
}

//mobility change 17-4-10
void rebuildNeighborInfo(int node)
{
int i, j;
int src_x, src_y, dst_x, dst_y;
double distance;

getPosition(node, &src_x, &src_y, simTime);

			for(i=0; i < numOfNodesInNetwork; i++) // and not in tree coz a node may settle under ROOT as well
			{
				if(i != node)
				{
				//calculate distance from node 
				getPosition(i, &dst_x, &dst_y, simTime);
				distance = (double)sqrt(pow(src_x - dst_x, 2) + pow(src_y - dst_y, 2));
				//fprintf(stderr,"Distance of %d from %d is %lf \n", node, i, distance);

				//connectivityMatrix[currentNode][scanNode] = 1;
				//now here make the addition in the neighbor list, similarly downstairs in the interference list
					
				if(nodeID[node].type == MOBILE && nodeID[i].type == MOBILE)
				{	
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi directionally undefined wrt %d and %d \n",node, i);
				
					//now adding to the neighbor's list and interference list
					if(distance <= transmissionRangeM)
					{
					nodeID[node].neighborList[nodeID[node].numNeighbors] = i;
					nodeID[node].numNeighbors++;

					nodeID[i].neighborList[nodeID[i].numNeighbors] = node;
					nodeID[i].numNeighbors++;
					
					//fprintf(stderr,"Neighbor list bi-directionally updated for %d and %d \n",node, i);
					}
					
					if(distance <= interferenceRangeM)
					{
					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	

					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;

					//fprintf(stderr,"Interference list bi-directionally updated for %d and %d \n",node, i);
					}
				}
				else if(nodeID[node].type == MOBILE && nodeID[i].type == INFRA)
				{
					if(distance <= transmissionRangeM)
					{
					networkGraph[node][i] = 1;
					incomingEdge[node]++;

					networkGraph[i][node] = 1;
					incomingEdge[i]++;
					
					//fprintf(stderr,"Network graph bi-directionally oned for %d and %d \n",node, i);

					nodeID[node].neighborList[nodeID[node].numNeighbors] = i;
					nodeID[node].numNeighbors++;
					
					nodeID[i].neighborList[nodeID[i].numNeighbors] = node;
					nodeID[i].numNeighbors++;
					
					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	

					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;

					//fprintf(stderr,"Neighbor and interference list bi-directionally updated for %d and %d \n",node, i);
					}
					else if(distance <= transmissionRangeI && distance > transmissionRangeM) //transmissionRangeI is the same as interferenceRangeM
					{
					networkGraph[i][node] = 1;
					incomingEdge[i]++;
					networkGraph[node][i] = UNDEFINED;

					//fprintf(stderr,"Network graph oned for %d and %d \n",i, node);
					
					nodeID[i].neighborList[nodeID[i].numNeighbors] = node;
					nodeID[i].numNeighbors++;
					//fprintf(stderr,"Neighbor list updated for %d and %d \n",i, node);	

					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	

					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;

					//fprintf(stderr,"Inteference list bi-directionally oned for %d and %d \n", node, i);
					
					}
					else if(distance <= interferenceRangeI && distance > transmissionRangeI) 
					{
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi-directionally undefined wrt %d and %d \n",node, i);

					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;
					
					//fprintf(stderr,"Interference List updated for %d and %d \n", i, node);
					}
					else
					{
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi-directionally undefined wrt %d and %d \n",node, i);
					}
					
				}
				else if(nodeID[node].type == INFRA && nodeID[i].type == INFRA) 
				{
					if(distance <= transmissionRangeI)
					{
					networkGraph[node][i] = 1;
					incomingEdge[node]++;

					networkGraph[i][node] = 1;
					incomingEdge[i]++;
					
					//fprintf(stderr,"Network graph bi-directionally oned for %d and %d \n",node, i);

					//now adding both the nodes to each other's neighbor list
					nodeID[node].neighborList[nodeID[node].numNeighbors] = i;
					nodeID[node].numNeighbors++;

					nodeID[i].neighborList[nodeID[i].numNeighbors] = node;
					nodeID[i].numNeighbors++;

					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	

					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;

					//fprintf(stderr,"Neighbor list and Interference List bi-directionally updated for %d and %d \n",node, i);
					}
					else if(distance > transmissionRangeI && distance <= interferenceRangeI)
					{
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi-directionally undefined wrt %d and %d \n",node, i);

					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	

					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;

					//fprintf(stderr,"Interference list bi-directionally updated for %d and %d \n",node, i);
					}
					else
					{
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi-directionally undefined wrt %d and %d \n",node, i);
					}

				}
				else if(nodeID[node].type == INFRA && nodeID[i].type == MOBILE)	
				{

					if(distance <= transmissionRangeM)
					{
					networkGraph[node][i] = 1;
					incomingEdge[node]++;
					networkGraph[i][node] = 1;
					incomingEdge[i]++;
					//fprintf(stderr,"Network graph bi-directionally oned for %d and %d \n",node, i);

					nodeID[i].neighborList[nodeID[i].numNeighbors] = node;
					nodeID[i].numNeighbors++;
					nodeID[node].neighborList[nodeID[node].numNeighbors] = i;
					nodeID[node].numNeighbors++;

					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	
					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;

					//fprintf(stderr,"Neighbor list and Interference List bi-directionally oned for %d and %d \n",node, i);
					}
					else if(distance > transmissionRangeM && distance <= transmissionRangeI)  //transmissionRangeI is the same as interferenceRangeM
					{
					networkGraph[node][i] = 1;
					incomingEdge[node]++;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph oned for %d and %d \n",node, i);

					nodeID[node].neighborList[nodeID[node].numNeighbors] = i;
					nodeID[node].numNeighbors++;
					//fprintf(stderr,"Neighbor list updated for %d and %d \n",node, i);

					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	
					
					nodeID[i].interferenceList[nodeID[i].numInterferringNeighbors] = node;
					nodeID[i].numInterferringNeighbors++;
					//fprintf(stderr,"Interference List bi-directionally updated for %d and %d \n",node, i);
					}
					else if(distance > transmissionRangeI && distance <= interferenceRangeI)
					{
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi-directionally undefined wrt %d and %d \n", node, i);
			
					nodeID[node].interferenceList[nodeID[node].numInterferringNeighbors] = i;
					nodeID[node].numInterferringNeighbors++;	
					//fprintf(stderr,"Interference list updated for %d and %d \n", node, i);
					}
					else
					{
					networkGraph[node][i] = UNDEFINED;
					networkGraph[i][node] = UNDEFINED;
					//fprintf(stderr,"Network graph bi-directionally undefined wrt %d and %d \n", node, i);
					}
				}

				}
			}
//fprintf(stderr,"Call to rebuild neighbor info over \n");

}

//mobility change 19-4-10
int chooseAppropriateDirection(int new_x, int new_y)
{
	int direction;
	//fprintf(stderr,"choose appropriate direction called with arguments: x_coord %d and y_coord %d \n", new_x, new_y);
	
	//first make a generic condition
	if(new_x >= 4600 && new_x <= 5400 && new_y >= 4600 && new_y <= 5400)
	{
		//choose direction randomly here
		direction = (int)((rand() % 8) + 1);
		//fprintf(stderr,"direction randomly chosen from the generic condition is %d \n", direction);
		return direction;
	}
	else if(new_x < 4600 && new_y >= 4600 && new_y <= 5400)
	{
		//choose between 2-4
		direction = (int)(rand() % 3);
		if(direction == 0)
		return 2;
		else if(direction == 1)
		return 3;
		else if(direction == 2)
		return 4;
	
	}
	else if(new_x > 5400 && new_y >= 4600 && new_y <= 5400)
	{
		//choose between 6-8
		direction = (int)(rand() % 3);
		if(direction == 0)
		return 6;
		else if(direction == 1)
		return 7;
		else if(direction == 2)
		return 8;
	
	}
	else if(new_y < 4600 && new_x >= 4600 && new_x <= 5400)
	{
		//choose between 1,2,8
		direction = (int)(rand() % 3);
		if(direction == 0)
		return 1;
		else if(direction == 1)
		return 2;
		else if(direction == 2)
		return 8;
	
	}
	else if(new_y > 5400 && new_x >= 4600 && new_x <= 5400)
	{
		//choose between 4,5,6
		direction = (int)(rand() % 3);
		if(direction == 0)
		return 4;
		else if(direction == 1)
		return 5;
		else if(direction == 2)
		return 6;
	
	}
	else if(new_x < 4600 && new_y < 4600)
	{
		return 2;
	}
	else if(new_x < 4600 && new_y > 5400)
	{
		return 4;
	}
	else if(new_x > 5400 && new_y > 5400)
	{
		return 6;
	}
	else if(new_x > 5400 && new_y < 4600)
	{
		return 8;
	}

return -1; // error checking  
}

//return TRUE when queue is not empty
int eventQueueNotEmpty()
{
	if(queueHead == NULL)
		return FALSE;
	else
		return TRUE;
}

//pop event at the front of the queue
struct event * nextEvent()
{
	struct event * extractEvent;
	extractEvent = queueHead;
	queueHead = queueHead->next;
	return extractEvent;
}

//free the allocated memory to structures
int destroyEvent(struct event * thisEvent)
{
	if((thisEvent->type <= RX_END) || (thisEvent->type == DATA_SLOT_TRANSMIT) || (thisEvent->type == FRAGMENT_RECEPTION_TIMEOUT) || (thisEvent->type == START_MOBILE))
	{	
		free((void *)thisEvent->aPacket->payloadPacket);
		free((void *)thisEvent->aPacket);
	}
	free((void *)thisEvent);
}

int totalEventCount = 0;

//insert event into priority queue, the priority is based on the event time
int insertEvent(struct event * thisEvent)
{
	struct event *temp, *prev;
	int count = 0;
	
	//hack
	/*int type;
	type = thisEvent->type;

	if(type == FRAGMENT_RECEPTION_TIMEOUT)
	{
	fprintf(stderr,"Reached here at %lf time, event time is %lf \n", simTime, thisEvent->time);
	}*/

	if(queueHead == NULL)
	{

		queueHead = thisEvent;
		return SUCCESS;
	}
	
	prev = temp = queueHead;

	//insert the event at the front
	if(queueHead->time > thisEvent->time)
	{
		thisEvent->next = queueHead;
		queueHead = thisEvent;
	}
	//so this means that the event with the smallest time is at the head of the queue and then in the increasing order of time
	else
	{
		if(totalEventCount > 11500)
		{
			//fprintf(stderr,"\n count %d",totalEventCount);			
			//sleep(1);
		}
		//find requisite location to insert the event
		while(temp != NULL)		
		{
			if(simTime > 48 * 60 * 1000)
			{
				//fprintf(stderr,"%d\n",count);
			}
			count++;
			
			if(count > 11500)
			{
				//fprintf(stderr,"\n count %d %lf %lf",count, temp->time, thisEvent->time);				
				//sleep(1);
			}
			
			if(temp->time < thisEvent->time)
			{
				prev = temp;
				temp = temp->next;
			}
			else
			{
				break;
			}	
		}
		
		if(simTime > 48 * 60 * 1000)
		{
			//fprintf(stderr,"Out of loop %d simTime %lf\n",count, simTime);
			//usleep(1000);
		}
		
		totalEventCount = count;
		
		if(count > 11500)
		{
			//fprintf(stderr,"\nCame out of the loop");
		}
		
		if(temp == NULL)
		{
			prev->next = thisEvent;
		}	
		else
		{
			thisEvent->next = prev->next;
			prev->next = thisEvent;
		}			
	}
	
}	

int flowWasNotAccepted(int flowId)
{
	int i;
	int retVal = TRUE;
	for(i=0;i<nodeID[ROOT].numSchedElem;i++)
	{
		if(flowId == EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel))
		{
			retVal = FALSE;
		}
	}
	return retVal;
}

//call requisite functions pertaining to an event and further pertaining to a packet type
int processEvent(struct event * thisEvent)
{
	int type;
	int i,m;
	double contentionSlotBoundary,tempTime;
	struct event * anEvent;
	FILE *fp;	
	

	if(thisEvent->type == MAKE_ALIVE)
	{	
		//node disable hack just for printing later things
		if(simTime > 1200000) // for awakening of mobile node
		{
		//fprintf(stderr,"\n node alive %d simTime %lf \n", thisEvent->node, simTime);
		//exit(0); //for now
		//calculating the neighborhood information. In actuality, needs to be done by reactive updates to the root
		// placing the node 44, 15 meters south of infra node 2, and when 7 comes up, place 7 200 meters above node 4
		int x_coord, y_coord; 
		int tx_range, ix_range;
		double distance;

		
		if(nodeID[thisEvent->node].type == MOBILE)
		{
		x_coord = nodeLocation[thisEvent->node].x = 4800; 
		y_coord = nodeLocation[thisEvent->node].y = 4985; // below node 2

		tx_range = transmissionRangeM;
		ix_range = interferenceRangeM;
		}
		else if(nodeID[thisEvent->node].type == INFRA)
		{
		x_coord = nodeLocation[thisEvent->node].x = 5400; //node disable hack
		y_coord = nodeLocation[thisEvent->node].y = 5000;

		tx_range = transmissionRangeI;
		ix_range = interferenceRangeI;
		}

		rebuildNeighborInfo(thisEvent->node);
			
		}

		nodeID[thisEvent->node].state = NOT_JOINED;

		tempTime = simTime;
		
		tempTime += (FRAME_DURATION - fmod(tempTime, FRAME_DURATION)); //this is for synchronising with the frame durations. it tells when the next frame is going to start
		
		contentionSlotBoundary = tempTime + (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION);

		anEvent = getContentionSlotEvent(contentionSlotBoundary, thisEvent->node);	//inserts next contention slot event, the contention slot event pops out contention packets from contention queue and transmits through CSMA
		insertEvent(anEvent);

		return SUCCESS;
	}
	if(thisEvent->type == MAKE_DEAD)
	{
		logNodeStats(thisEvent->node,TIME_NODE_DEAD,simTime);
		//fprintf(stderr,"\n make dead event at node %d simTime %lf \n",thisEvent->node, simTime);
		//exit(0);
		nodeID[thisEvent->node].state = DEAD;
	}

	//portability changes start*****************************
	if(thisEvent->type == TOPOLOGY_UPDATE_CHECK)
	{
		anEvent = getTopologyCheckEvent(simTime + FRAME_DURATION);
		insertEvent(anEvent);
		handleTopologyUpdateCheck(); 
		return SUCCESS;			
	}

	//mobility change 17-4-10
	if(thisEvent->type == MOBILITY) 
	{
		//fprintf(stderr,"Mobility event reached \n");
		//exit(0);
		int mobile_node, direction;
		struct event * anEvent;
		//int length;

		mobile_node = 25; // currently the node which is going to move
		direction = 7; //current direction, going westwards
		
		anEvent = getStartMobilityEvent(simTime, mobile_node, direction);
		insertEvent(anEvent);
		
		anEvent = getStopMobilityEvent(simTime + 60000, mobile_node);
		insertEvent(anEvent);
	}

	//mobility change 17-4-10
	if(thisEvent->type == START_MOBILE)
	{
		//fprintf(stderr,"Start mobility event reached for mobile %d \n", thisEvent->node);
		int direction;
		int i;

		direction = ((struct mobile_direction *)thisEvent->aPacket->payloadPacket)->direction;

		for(i=0; i < MAX_MOVING_NODES; i++)
		{
			if(moving_nodes[i].mobile_node == -1)
			{
				moving_nodes[i].mobile_node = thisEvent->node;
				moving_nodes[i].direction = direction;
				moving_nodes[i].started_at = simTime;
				break;
			}
		}
		//fprintf(stderr,"Following information added to moving_nodes for node %d \n", thisEvent->node);
		for(i=0; i < MAX_MOVING_NODES; i++)
		{
			if(moving_nodes[i].mobile_node != -1)
			{
				//fprintf(stderr,"Node %d Direction %d Starting at %lf position %d \n", moving_nodes[i].mobile_node, moving_nodes[i].direction, moving_nodes[i].started_at, i);
				break;
			}
		}
	}

	if(thisEvent->type == STOP_MOBILE)
	{
		//fprintf(stderr,"Stop mobility event reached for mobile %d \n", thisEvent->node);

		int i;
		int new_x, new_y;
		struct event * anEvent;
		int direction;

		for(i=0; i < MAX_MOVING_NODES; i++)
		{
			if(moving_nodes[i].mobile_node == thisEvent->node)
			{

				//fix the new position of the node in the topology
				getPosition(thisEvent->node, &new_x, &new_y, simTime);
				//fprintf(stderr,"The new coordinates of node %d are x %d and y %d \n", thisEvent->node, new_x, new_y);

				nodeLocation[thisEvent->node].x = new_x;
				nodeLocation[thisEvent->node].y = new_y;
				
				//fprintf(stderr,"The new coordinates of node %d are x %d and y %d \n", thisEvent->node, nodeLocation[thisEvent->node].x, nodeLocation[thisEvent->node].y);

				moving_nodes[i].mobile_node = -1;
				moving_nodes[i].direction = 0;
				moving_nodes[i].started_at = 0;
				//fprintf(stderr,"Following information deleted from moving_nodes for node %d \n", thisEvent->node);
				
				//fprintf(stderr,"Node %d Direction %d Starting at %lf position %d \n", moving_nodes[i].mobile_node, moving_nodes[i].direction, moving_nodes[i].started_at, i);
				
				//restart the mobility after 5 minutes
				//direction = (int)((rand() % 8) + 1);
				direction = chooseAppropriateDirection(new_x, new_y);
				//fprintf(stderr,"direction randomly chosen is %d \n", direction);

				if(direction == -1)
				{
					//fprintf(stderr,"Wrong direction returned \n");
					exit(0);
				}

				anEvent = getStartMobilityEvent(simTime, thisEvent->node, direction);
				insertEvent(anEvent);

				anEvent = getStopMobilityEvent(simTime + 60000, thisEvent->node);
				insertEvent(anEvent);
				//exit(0);
				break;
			}
		}	
	}

	if(thisEvent->type == NODE_DISABLE) // node disable change
	{
	
		int mobile_node; // choose a mobile node at random
		int infra_node; // choose an infra node at random
		struct event * anEvent;
	
		//mobile_node = numOfInfraNodesInNetwork + (rand() % numOfMobileNodesInNetwork);
		//infra_node = 1 + (rand() % numOfInfraNodesInNetwork) ;
		mobile_node = 44; // portability hack for now
		 infra_node = 2; // node disable hack for now	

		//since we only want nodes which are not on call
		while(nodeID[mobile_node].callRequestSent == 1)
		{
		mobile_node = numOfInfraNodesInNetwork + (rand() % numOfMobileNodesInNetwork);
		}

		//fprintf(stderr,"\n Mobile chosen for random disabling is %d \n", mobile_node);	
		//fprintf(stderr,"\n Infra node chosen for disabling is %d \n", infra_node);
		//printing this node's interference list anf neighbor list
		int i, j;
		//fprintf(stderr,"Printing the neighbor list for node %d \n", infra_node);
		for(i=0; i < nodeID[infra_node].numNeighbors; i++)
		{
		//fprintf(stderr,"Node %d   Type %d \n", nodeID[infra_node].neighborList[i], nodeID[nodeID[infra_node].neighborList[i]].type);
		}

		//fprintf(stderr,"Printing the Interference list for node %d \n", infra_node);
		for(i=0; i < nodeID[infra_node].numInterferringNeighbors; i++)
		{
		//fprintf(stderr,"Node %d   Type %d \n", nodeID[infra_node].interferenceList[i], nodeID[nodeID[infra_node].interferenceList[i]].type);
		}
		//fprintf(stderr,"Printing the neighbor list for node %d \n", mobile_node);
		for(i=0; i < nodeID[mobile_node].numNeighbors; i++)
		{
		//fprintf(stderr,"Node %d   Type %d \n", nodeID[mobile_node].neighborList[i], nodeID[nodeID[mobile_node].neighborList[i]].type);
		}

		//fprintf(stderr,"Printing the Interference list for node %d \n", mobile_node);
		for(i=0; i < nodeID[mobile_node].numInterferringNeighbors; i++)
		{
		//fprintf(stderr,"Node %d   Type %d \n", nodeID[mobile_node].interferenceList[i], nodeID[nodeID[mobile_node].interferenceList[i]].type);
		}

		anEvent = getDeadEvent(simTime, mobile_node); // for mobile disbaling at 20 minutes
		insertEvent(anEvent);

		anEvent = getDeadEvent(simTime + 60000, infra_node); // for infra disabling after one minute ie 21 minutes
		insertEvent(anEvent);

		anEvent = getAliveEvent(simTime + (10 * 60000), mobile_node); // for mobile awakening after 10 minutes ie 30 min
		insertEvent(anEvent);
	
		anEvent = getAliveEvent(simTime + (11 * 60000), infra_node); // for infra awakening after 10 minutes of its going down ie 31 minutes
		insertEvent(anEvent);
	}
	//portability changes end*****************************
	
	if(thisEvent->type == VOICE_MSG_RELAY) // node disable change
	{
	//By now, the node must have received the schedule for sure, so just checking for callInProgress should suffice. Putting double check here and exiting if wrong.
	int i, j, l, not_on_call, bottleneck;
	not_on_call = TRUE;
	
	if(numPathsInVoiceQueue > 0)
	{
		for(l=0; l < nodeID[ROOT].numSchedElem; l++)
		{
			if(globalScheduleElements[l].transmitter == thisEvent->node)
			{
			not_on_call = FALSE;
			break;
			}
		}

		if(not_on_call == TRUE && nodeID[thisEvent->node].callInProgress == TRUE)
		{
		fprintf(stderr,"from scheduling elements, the node is not on call, but its callInProgress is true, this is an error condition \n");
		exit(0);
		}

		if(not_on_call == FALSE)
		{
		//fprintf(stderr,"The node is on call, will relay the voice message later \n");
		return SUCCESS;
		}

		for(i=0; i < numPathsInVoiceQueue; i++)
		{
			if(voiceQueue[i].source == ROOT && voiceQueue[i].destination == thisEvent->node)
			{
			bottleneck = getVoiceDestination(voiceQueue[i].source, voiceQueue[i].destination);
				if(bottleneck == -1)
				{
				//fprintf(stderr,"do not schedule\n");
				}
				else
				{
				//fprintf(stderr,"bottleneck is getting relieved, can schedule\n");
				//fprintf(stderr,"parameters are source %d destination %d bottleneck %d callDuration %lf\n",voiceQueue[i].source, voiceQueue[i].destination, bottleneck,voiceQueue[i].callDuration);
			
			
				nodeID[voiceQueue[i].source].callDuration = voiceQueue[i].callDuration;
				nodeID[voiceQueue[i].source].voiceFlow = 1;
				//nodeID[voiceQueue[i].source].voiceCopyCallDuration = nodeID[voiceQueue[i].source].callDuration; 

				//updating storage capacity
				nodeID[voiceQueue[i].source].store_cap -= ((voiceQueue[i].callDuration/30)*24); 
				//logging this update
				logStoreCap(voiceQueue[i].source,nodeID[voiceQueue[i].source].store_cap,simTime);

				//hopDistribution[voiceQueue[i].rPath->numOfNodesInPath-1]++;//what if a new bottleneck has come up
				
				setCarryForwardFlow(voiceQueue[i].source,bottleneck, voiceQueue[i].overalle2eDestination, 1,voiceQueue[i].voiceQueueFlowId);

				//numPathsInVoiceQueue--;
				reshuffleVoiceQueue(voiceQueue[i].voiceQueueFlowId);	
			
				}	
			}
		}

	} //numPathsInVoiceQueue if condition 
	
	}

	//manan change
	if(thisEvent->type == VOICE_MSG_CHECK)
	{	
		if(numPathsInVoiceCurrQueue > 0)
		{
		int i;
			for(i=0;i<numPathsInVoiceCurrQueue;i++)
			{

			voiceCurrQueue[i].voiceMsgDuration -= FRAME_DURATION;
			fprintf(at,"Flow id %d Call duration %lf\n",voiceCurrQueue[i].voiceFlowId,voiceCurrQueue[i].voiceMsgDuration);
		
				if(voiceCurrQueue[i].voiceMsgDuration <= 0)
				{
					
					revokeElements(voiceCurrQueue[i].voiceFlowId);
								
				}
			}
		
		
		}
		anEvent = getVoiceMsgCheckEvent(thisEvent->time + FRAME_DURATION);
		insertEvent(anEvent);	
	}

	if(thisEvent->type == STORE_CAP)
	{	//fprintf(stderr,"\n\n\nfine\n\n\n");
		
		int counter;
		for(counter = 0; counter < numOfInfraNodesInNetwork ; counter++)
		{
			logStoreCap(counter,nodeID[counter].store_cap,simTime);
		}	


		anEvent = getStorageCapacityEvent(thisEvent->time + 300000);
		insertEvent(anEvent);	
	}
	//this is going to screw my event queue dump code, need to take care of that

	if(thisEvent->type == SET_UP_NEW_SCHEDULE)
	{
		/*if(simTime > 960000)
		{
		fprintf(stderr,"Reached here\n");
		}*/
	}
	//manan end

	if(thisEvent->type == LINK_ACK_TIMEOUT)
	{
		handleLinkAckTimeout(thisEvent->node);
	}
	if(thisEvent->type == CONTENTION_SLOT_BACKOFF)
	{
		handleContentionSlot(thisEvent->node);	//transmit packets in contention queue	
	}
	if(thisEvent->type == CONTENTION_SLOT_BEGINS) 
	{
		nodeID[thisEvent->node].packetBeingReceived = FALSE;
		nodeID[thisEvent->node].collision = 0;
		nodeID[thisEvent->node].backoff = 1;
		
		handleContentionSlot(thisEvent->node);	//transmit packets in contention queue	
		anEvent = getContentionSlotEvent(simTime+FRAME_DURATION, thisEvent->node);	//next contention slot event
		//simTime has already been updated before the call of processEvent
		insertEvent(anEvent);
	}
	if(thisEvent->type == KEEP_JOINED_TIMEOUT)
	{
		handleScheduleReceptionTimeout(thisEvent->node);
	}
	if(thisEvent->type == JOIN_REQUEST_TIMEOUT)
	{
		handleJoinAckTimeout(thisEvent->node);
	}
	if(thisEvent->type == FLOW_REQUEST_TIMEOUT)
	{
		handleFlowAckTimeout(thisEvent->node);
	}
	if(thisEvent->type == BW_REQUEST_TIMEOUT)
	{
		handleBwRequestTimeout(thisEvent->node);
	}
	if(thisEvent->type == DATA_SLOT_TRANSMIT)
	{
		handleTransmitDataPacket(thisEvent->node, thisEvent->aPacket);
	}
	if(thisEvent->type == START_NEW_SCHEDULE)
	{
		handleNewScheduleParam(thisEvent->node);
	}
	if(thisEvent->type == SCHEDULE_RECEPTION_TIMEOUT)
	{
		handleExpectedScheduleReception(thisEvent->node);
	}
	//mobility change 15-4-10
	if(thisEvent->type == FRAGMENT_RECEPTION_TIMEOUT)
	{
		//fprintf(stderr,"\n Reached here for node %d \n", thisEvent->node);
		handleExpectedFragmentReception(thisEvent->node, thisEvent->aPacket);
	}
	if(thisEvent->type == HANDLE_CONTENTION_SLOT)
	{
		handleContentionSlot(thisEvent->node);
	}
	if(thisEvent->type == CAN_START_FLOW)
	{
	//if(thisEvent->node == 25)
	//{
	canStartFlowRequest(thisEvent->node);
	//}
		//test & hack purposes
		/*if(thisEvent->node == 41 || thisEvent->node == 42 || thisEvent->node == 43 || thisEvent->node == 36 || thisEvent->node == 32)
		{
			if(real_time_call_counter > 5)
			{}
			else
			{
			fprintf(stderr,"Inside processEvent for CAN_START_FLOW for node %d\n",thisEvent->node);
			canStartFlowRequest(thisEvent->node);
			real_time_call_counter++;
			//scheduleVoiceFlowRequest(thisEvent->node, 0);//this is also a good idea
			}
		}
		else
		{}*/
		
		//test end
		
	}
	#if GENERATE_REQUESTS_FROM_FILE
	if(thisEvent->type == GENERATE_FLOW_REQUEST)// && nodeID[thisEvent->node].state == JOINED && (isPartOfAnyFlow(thisEvent->node) == FALSE))
	{
		scheduleFlowRequest(thisEvent->node);
	}
	#endif
	if(thisEvent->type == NEXT_FLOW_REQUEST && nodeID[thisEvent->node].state == JOINED && (isPartOfAnyFlow(thisEvent->node) == FALSE)) // node disable change
	{
		fprintf(dump,"Inside processEvent for NEXT_FLOW_REQUEST for node %d\n",thisEvent->node);
		scheduleFlowRequest(thisEvent->node);
	}
	if(thisEvent->type == TX)
	{	
		type = EXTRACT_TYPE(thisEvent->aPacket->ver_type_piggyback);
		if(type == SCHEDULE)
		{
			handleScheduleBroadcast(thisEvent->node, thisEvent->aPacket);	//sets RX events of neighbors
		}
		if(type == VOICE_FLOW_REQUEST)
		{
			//fprintf(stderr,"Reached here in handlepriorityqueue\n");
		}
		
		if(type == JOIN_REQUEST || type == FLOW_REQUEST || type == TEAR_DOWN || type == BW_REQUEST || type == VOICE_FLOW_REQUEST || type == TOPOLOGY_UPDATE)
		{	
			if(type == BW_REQUEST)
			{
				int flowid, destination;

				for(i=0;i<numOfFlowsInProgress;i++)
				{
					if(flowStat[i].source == thisEvent->node) //source is set when packet is generated by source
					{
						destination = flowStat[i].destination;
						break;
					}
				}
				if(i == numOfFlowsInProgress)
				{					
					//printf("\n no node matched with this node as source %d voiceFlow %d flow id %d\n",thisEvent->node,nodeID[thisEvent->node].voiceFlow,EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest));
					//this is not an error statement, its generated by vijay's code as well
					return SUCCESS;	
				}
				else
				{
					//*******************************
					
					if(nodeID[thisEvent->node].currentFlowId != EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest))
					{
						//fprintf(stderr,"\n\n ++++ BW request of terminated flow current flow %d terminated flow %d\n",nodeID[thisEvent->node].currentFlowId, EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest));
						//printf("\n\n ++++ BW request of terminated flow current flow %d terminated flow %d\n",nodeID[thisEvent->node].currentFlowId, EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest));
						
						return SUCCESS;
					}
					//*******************************			
					if(nodeID[thisEvent->node].flowTearedDown == TRUE)
						return SUCCESS;
						
					if(nodeID[thisEvent->node].flowRequestGranted == 0 && nodeID[thisEvent->node].flowRetryCount > MAX_FLOW_RETRY)
					{
						//fprintf(stderr,"\n\n flowId %d node %d flowRetryCount %d callsInProgress %d lastScheduleTime %lf validfrom %lf simTime %lf\n\n",EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest), thisEvent->node, nodeID[thisEvent->node].flowRetryCount, globalCallsInProgress, nodeID[thisEvent->node].lastScheduleTime, nodeID[thisEvent->node].scheduleValidFrom, simTime);

						numTimedOutCalls[EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest)] = 1;
											
						flowid = EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest);
	
					        nodeID[thisEvent->node].flowRequestGranted = 0;
                				nodeID[thisEvent->node].flowRetryCount = 0;
						nodeID[thisEvent->node].callInProgress = FALSE;

						nodeID[thisEvent->node].callRequestSent = 0;    // no response frpm prev. call request to root
						//nodeID[thisEvent->node].voiceFlow = 1; //VJ start: To enable stored voice
						//{
						
						//scheduleVoiceFlowRequest(thisEvent->node, destination); //VJ end: To enable stored voice
						
						//numVoiceMsg++; moving to scheduleVoiceFlowRequest
						//}

						//reshuffleFlowQueue(EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest)); -> uncommenting this thing gives troubles, not sure whether troubles should have occured or not

						//************ VJ change start ************
												
						if(!flowWasNotAccepted(EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest)))
						{
							//fprintf(stderr,"\n\n **** flow was accepted at the root \n **** flow timed out at client \n **** let it time out at root \n\n");						
						}
						else
						{
							reshuffleFlowQueue(EXTRACT_FLOWID(((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest));	
						}
						//************ VJ change end ************

						#ifndef GENERATE_REQUESTS_FROM_FILE
						canStartFlowRequest(thisEvent->node); //comment for suppressing new flows
						#endif

						//manan change

						fprintf(vm,"in handlepriorityqueue, in TX BW request, after max flow retry, voice flow for node %d initiating\n",thisEvent->node);

						fprintf(vm,"\nThe e2e destination is %d, exiting\n",destination);	
						//manan end
						return SUCCESS;	
					}	
					if(nodeID[thisEvent->node].flowRequestGranted == 0)
					{
						nodeID[thisEvent->node].flowRetryCount++;
					 //fprintf(stderr,"\n Node %d flow retry count %d %lf\n",thisEvent->node,nodeID[thisEvent->node].flowRetryCount, simTime);
					}	
					//flow not teared down	
					//packet source is generator of one of the flows, so set next bw request event for the flow
					//if(nodeID[thisEvent->node].voiceFlow != 1)
					//{
					anEvent = getBandwidthRequestEvent(simTime + (double)BW_INTERVAL,((struct bwRequest *)thisEvent->aPacket->payloadPacket)->flowId_bwRequest, thisEvent->node,nodeID[thisEvent->node].parent, TX);
					insertEvent(anEvent);
					//}	
				}	
			}
			//portability changes start
			if(type == TOPOLOGY_UPDATE && nodeID[thisEvent->node].state > NOT_JOINED && (((struct topologyUpdate *)thisEvent->aPacket->payloadPacket)->type == REGULAR))
			{
			
			anEvent = getTopologyUpdateEvent(simTime + (double)(TOPOLOGY_UPDATE_PERIOD * SECOND), thisEvent->node,thisEvent->node,nodeID[thisEvent->node].parent,TX, REGULAR);	
			insertEvent(anEvent);

			//fprintf(stderr,"Topology update time for node %d is %lf \n",thisEvent->node, simTime + (double)(TOPOLOGY_UPDATE_PERIOD * SECOND));
			}
			//portability changes end
				
			handleRequestTX(thisEvent->node, thisEvent->aPacket);	//inserts request into control queue
		}
		if(type == DATA)
		{
			//fprintf(stderr,"\n Generate data packet event at node %d", thisEvent->node);
			//sleep(2);
			handleGenerateDataPacket(thisEvent->node, thisEvent->aPacket);
		}
		
	}	
	if(thisEvent->type == RX)
	{
		type = EXTRACT_TYPE(thisEvent->aPacket->ver_type_piggyback);
		if(type == SCHEDULE)
		{
				handleScheduleReception(thisEvent->node, thisEvent->aPacket);
		}
		if(type == DATA)
		{
				handleReceivedDataPacket(thisEvent->node, thisEvent->aPacket);
		}
	}
	if(thisEvent->type == STOP_TRANSMISSION)
	{
		nodeID[thisEvent->node].packetBeingTransmitted = FALSE;
	}
	if(thisEvent->type == RX_BEGIN)
	{
		type = EXTRACT_TYPE(thisEvent->aPacket->ver_type_piggyback);
		if(type == JOIN_REQUEST || type == FLOW_REQUEST || type == TEAR_DOWN || type == BW_REQUEST || type == VOICE_FLOW_REQUEST || type == TOPOLOGY_UPDATE) //type may not actually matter here
		{
					
			if(nodeID[thisEvent->node].packetBeingReceived == FALSE)
			{
				nodeID[thisEvent->node].packetBeingReceived = TRUE;				
			}
			else
			{
		
				if(nodeID[thisEvent->node].collision == 0)
					nodeID[thisEvent->node].collision = 2;
				else
					nodeID[thisEvent->node].collision++;	
			}
		}		
	}
	if(thisEvent->type == CONTENTION_RX_BEGIN)
	{
			if(nodeID[thisEvent->node].packetBeingReceived == FALSE)
			{
				nodeID[thisEvent->node].packetBeingReceived = TRUE;				
			}
			else
			{
		
				if(nodeID[thisEvent->node].collision == 0)
					nodeID[thisEvent->node].collision = 2;
				else
					nodeID[thisEvent->node].collision++;	
			}
	}
	if(thisEvent->type == CONTENTION_RX_END)
	{
			
			if(nodeID[thisEvent->node].collision == 0)
			{
				nodeID[thisEvent->node].packetBeingReceived = FALSE;	
			}
			if(nodeID[thisEvent->node].collision > 0)
			{
				nodeID[thisEvent->node].collision--;
				if(nodeID[thisEvent->node].collision == 0)
					nodeID[thisEvent->node].packetBeingReceived = FALSE;					
			}
	
	}
	if(thisEvent->type == DATA_RX_BEGIN)
	{
			if(nodeID[thisEvent->node].packetBeingReceived == FALSE)
			{
				nodeID[thisEvent->node].packetBeingReceived = TRUE;				
			}
			else
			{
			
				if(nodeID[thisEvent->node].collision == 0)
					nodeID[thisEvent->node].collision = 2;
				else
					nodeID[thisEvent->node].collision++;	
			}
	}

	if(thisEvent->type == DATA_RX_END)
	{
			//nodeID[thisEvent->node].packetBeingReceived = FALSE;
			if(nodeID[thisEvent->node].collision == 0)
			{
				nodeID[thisEvent->node].packetBeingReceived = FALSE;	
			}
			if(nodeID[thisEvent->node].collision > 0)
			{
				nodeID[thisEvent->node].collision--;
				if(nodeID[thisEvent->node].collision == 0)
					nodeID[thisEvent->node].packetBeingReceived = FALSE;					
			}
	
	}
	if(thisEvent->type == RX_END)
	{
		type = EXTRACT_TYPE(thisEvent->aPacket->ver_type_piggyback);
		nodeID[thisEvent->aPacket->source].packetBeingTransmitted = FALSE;
		if(type == JOIN_REQUEST || type == FLOW_REQUEST || type == TEAR_DOWN || type == BW_REQUEST || type == VOICE_FLOW_REQUEST || type == TOPOLOGY_UPDATE) //type may not actually matter here
		{
	
			if(nodeID[thisEvent->node].collision == 0)
			{
			
				//manan change test purpose if condition
				if(type == VOICE_FLOW_REQUEST)
				{
				//fprintf(stderr,"reached RX_END for VOICE_FLOW_REQUEST for node %d\n",thisEvent->node);
				if(nodeID[thisEvent->node].collision == 0)
				{
				//fprintf(stderr,"collision count also is 0\n");
				}
				else
				{
				fprintf(stderr,"problem with collision count\n");
				exit(0);
				}
				}
				//manan end

				if(thisEvent->node == ROOT)
				{


					if(type == FLOW_REQUEST)
					{
						handleFlowReceptionRoot(thisEvent->aPacket);
						//printEventQueue();						
					}
					if(type == JOIN_REQUEST)
					{
						handleJoinReceptionRoot(thisEvent->aPacket);	
					}
					if(type == TEAR_DOWN)
					{
						handleTearDownRoot(thisEvent->aPacket);
					}
					if(type == BW_REQUEST)
					{
						handleBwRequestRoot(thisEvent->aPacket);
					}	
					if(type == VOICE_FLOW_REQUEST)
					{
						handleVoiceFlowReceptionRoot(thisEvent->aPacket);
					//fprintf(stderr,"The voice flow request is at root, exiting\n");
					//exit(0);
					}
					if(type == TOPOLOGY_UPDATE)
					{
					handleTopologyUpdateRoot(thisEvent->aPacket); // actually nothing significant happening here. 
					}
				}	
				else
				{					
					handleContentionPacketNode(thisEvent->node, thisEvent->aPacket);					
				}	
				nodeID[thisEvent->node].packetBeingReceived = FALSE;					
			}
			if(nodeID[thisEvent->node].collision > 0)
			{
				nodeID[thisEvent->node].collision--;
				if(nodeID[thisEvent->node].collision == 0)
					nodeID[thisEvent->node].packetBeingReceived = FALSE;					
			}
		}		
		if(type == LINK_LEVEL_ACK)
		{
			handleLinkAck(thisEvent->node);
		}				
	}	
	return SUCCESS;
}
