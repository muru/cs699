#include "header.h"

//node disable change
int anyAncestorIsDead(int node)
{
	int i,j, count = 0;

	if(nodeID[node].state == NOT_JOINED) //added 9-4-10
	{
	return TRUE;
	}

	for(i=nodeID[node].parent; i!= ROOT; )
	{
		count++;
		if(count > numOfNodesInNetwork)
		{
			return TRUE;	
		}
		//fprintf(stderr,"The value of i is %d \n", i);
		if(nodeID[i].state == DEAD)
		{
			//fprintf(stderr," Node %d is dead \n", i);
			return TRUE;
		}

		i = nodeID[i].parent;
	}
	return FALSE;
}

//node disable change
void purgeNodeInfo(int node)
{
//fprintf(stderr,"Purge node information called with argument %d \n", node);

		//deleting all the relevant information of the node
		nodeID[node].sponsorDepth = UNDEFINED;
		nodeID[node].sponsor = UNDEFINED;
		nodeID[node].parent = UNDEFINED;
		nodeID[node].broadcastTurn = 0;
		//nodeID[node].bandwidthInPlace = FALSE;
		
		nodeID[node].currentFlowId = 0;
		nodeID[node].flowTearedDown = TRUE;
		
		nodeID[node].synchronized = FALSE;
		nodeID[node].globalTime = 0;
		nodeID[node].localTime = 0;
		nodeID[node].offset = UNDEFINED;
		nodeID[node].scheduleValidFrom = UNDEFINED;
		nodeID[node].numSchedElem = 0;
		nodeID[node].moreFragment = FALSE;
		nodeID[node].treeUpdate = FALSE;	//ON and OFF based on formation of new routing tree
		nodeID[node].treeUpdateBroadcast = 0;
		nodeID[node].schedRepeat = FALSE;
		nodeID[node].numScheduleMissed = 0;
		nodeID[node].expectedSchedFragmentNo = 1;
		nodeID[node].broadcastSchedFragmentNo = 1;
		nodeID[node].lastFragmentNo = 0;
		nodeID[node].scheduleNumOfFragments = 1;
		nodeID[node].contentionProb = (double)1;
		nodeID[node].numOfAttempts = 0;
		nodeID[node].packetBeingReceived = nodeID[node].packetBeingTransmitted = FALSE;
		nodeID[node].collision = 0;
		
		nodeID[node].controlReceptionCount = 0;
		nodeID[node].controlReceptionCollision = FALSE;
		
		nodeID[node].callDuration = 0;
		nodeID[node].callInProgress = FALSE;
		
		nodeID[node].numOfPacketsInQueue = nodeID[node].numOfPacketsInDataQueue = 0;
		nodeID[node].controlFront = nodeID[node].controlRear = nodeID[node].dataFront = nodeID[node].dataRear = 0;
		//nodeID[node].outStandingControlPacket = FALSE;	
		nodeID[node].backoff = 1;
		nodeID[node].expectingLinkAck = FALSE;
		
		nodeID[node].expectingJoinRequestAck = FALSE;
		nodeID[node].expectingFlowRequestAck = FALSE;
		nodeID[node].flowRequestGranted = 0;
		nodeID[node].flowRetryCount = 0;
		nodeID[node].callRequestSent = 0;	
		nodeID[node].maxQueueSize = 0;

		nodeStat[node].upTime = (double) 0;
		nodeStat[node].endCallTime = (double)0;	
		nodeStat[node].timeJoinRequestSent = 0;
		nodeStat[node].timeJoinGrantReceived = 0;
		nodeStat[node].timeFlowRequestSent = 0;
		nodeStat[node].timeFlowGrantReceived = 0;

		//portability changes start *****************
		nodeID[node].numOfTopologyUpdatesReceived = 0;
		nodeID[node].numOfTopologyUpdatesSent = 0;
		nodeID[node].maxQueueSize = 0;
		nodeID[node].numOfAttempts = 0;

		nodeID[node].expectingParentChange = FALSE;
		nodeID[node].expectingPathChange = FALSE;
		nodeID[node].next_hop = -1;
		nodeID[node].timeLastHandoffSent = 0;
		
		int i, j;
		for(i=0; i<MAX_NO_INFRA_NODES; i++)
		{
			nodeID[node].infra_list[i].infra_node = MAX_NO_INFRA_NODES + 1;
			nodeID[node].infra_list[i].times_observed = 0;
			nodeID[node].infra_list[i].missed = 0;
			for(j=0; j<INFRA_HEARD_THRESHOLD; j++) // portability hack
			{
			nodeID[node].infra_list[i].rssi[j] = 0;
			}
			nodeID[node].infra_list[i].is_neighbor = FALSE;
		}
		//portability changes end ***********************
		
		//mobility change 25-4-10
		for(i=0; i < MAX_DATA_HISTORY; i++)
		{
			nodeID[node].data_history[i] = 0;
		}
		
}

//mobility change 16-4-10
void purgeNeighborInfo(int node)
{
int i,j;
//fprintf(stderr,"Purge neighbor information called with argument %d \n", node);
//changing the network graph
	for(j=0; j < numOfNodesInNetwork; j++)
	{
		if(networkGraph[node][j] == 1) 
		{
		networkGraph[node][j] = UNDEFINED;
		networkGraph[j][node] = UNDEFINED;
		//fprintf(stderr,"Network graph[node][j] changed for node %d \n",j);
		}
		if(networkGraph[j][node] == 1)
		{
		networkGraph[j][node] = UNDEFINED;
		//fprintf(stderr,"Network graph[j][node] changed for node %d \n",j);
		}

		if(j != node) // delete this node's info from other node's interference and neighbor list, ie connectivity matrix
		{
			for(i=0; i < nodeID[j].numNeighbors; i++)
			{
				if(nodeID[j].neighborList[i] == node)
				{
				int k;
				//fprintf(stderr," %d is a neighbor of %d at position %d \n", node, j, i);
					for(k=i; k < nodeID[j].numNeighbors -1 ; k++)
					{
					nodeID[j].neighborList[k] = nodeID[j].neighborList[k+1];
					}
				nodeID[j].numNeighbors--;
				//fprintf(stderr," %d is a neighbor of %d at position %d \n", nodeID[j].neighborList[i], j, i);
				if((nodeID[j].type == INFRA) || (nodeID[node].type == INFRA))
				incomingEdge[j]--; //also need to be deleted from the incoming edge list

				//fprintf(stderr,"Node %d ejected from neighbor list of %d \n",node, j);
				}
			}
						
			for(i=0; i < nodeID[j].numInterferringNeighbors; i++)
			{
				if(nodeID[j].interferenceList[i] == node)
				{
				int k;
				//fprintf(stderr," %d is an interferer of %d at position %d \n", node, j, i);
					for(k=i; k < nodeID[j].numInterferringNeighbors -1 ; k++)
					{
					nodeID[j].interferenceList[k] = nodeID[j].interferenceList[k+1];
					}
				nodeID[j].numInterferringNeighbors--;
				//fprintf(stderr," %d is an interferer of %d at position %d \n", nodeID[j].interferenceList[i], j, i);
				//fprintf(stderr,"Node %d ejected from interference list of %d \n",node, j);
				
				}
			}
						
		}

		if(j == node) // purge this node's information coz it may come alive at some other location with a completely different interference and neighbor list. This includes incomingEdge, neighborList and interferenceList
		{

		//fprintf(stderr,"The incoming edge of node %d is %d, its number of neighbors are %d and number of interferring neighbors are %d \n", node, incomingEdge[node], nodeID[node].numNeighbors, nodeID[node].numInterferringNeighbors);

		incomingEdge[j] = 0;
		nodeID[j].numNeighbors = 0;
		nodeID[j].numInterferringNeighbors = 0;

		//fprintf(stderr,"The interference, neighbor and incomingEdge list for node %d are purged \n",j);
		}
					
	}
		//printNeighborInfo(node);
		//exit(0);
		
}

//node disable change
void dropInfraFlows(int node)
{
	if(nodeID[node].type != INFRA)
	{
	fprintf(stderr,"Function called without infra as argument \n");
	exit(0);
	}
	//fprintf(stderr,"reached here to drop flows \n");

	int i, j, flowId, node_found; 
	//node_found = 0 means that now no scheduling elements have the dead infra as their part, similarly 1 means that the node is still part of some scheduling element and that flow needs to be revoked
	do
	{
	node_found = 0;

		for(i=0; i < nodeID[ROOT].numSchedElem; i++)
		{
			if(globalScheduleElements[i].transmitter == node)
			{
			flowId = EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel);

				for(j=0; j<numOfFlowsInProgress; j++)
				{
					if(flowStat[j].flowId == flowId)
					{
					//fprintf(stderr,"The information about the flow to be revoked is: Source %d Destination %d flowId %d phase %d \n", flowStat[j].source, flowStat[j].destination, flowStat[j].flowId, flowStat[j].phase);	
					break; // this break should just come out of the displaying loop
					}	
				}

		
			revokeElements(flowId);
			node_found = 1;
			break;
			}
		}
	}while(node_found == 1);
	
}

//portability change
double bufferVoiceMsgTime()
{
double buffer;
buffer = (double) (FRAME_DURATION * ((double)numOfInfraNodesInNetwork/2) * (getNumOfFragments(nodeID[ROOT].numSchedElem + convertTreeToSchedElem(nodeID[ROOT].treeUpdate)) + 5));

//buffer = (double) (FRAME_DURATION * ((double)numOfInfraNodesInNetwork/2) * (max_no_fragments + 2));

return buffer;
//numOfInfraNodesInNetwork/2 is done to take into acocunt two control slots in a frame
// +5 is done to provide a slack
}


//portability changes
int isNodeInTree(int node)
{
int i;
	for(i=0; i < numOfNodesInTree; i++)
	{
		if(routingTree[2*i] == node)
		return TRUE;
	}
return FALSE;
}

//************* VJ change start *************
int applyJoinPolicy(int node)
{
	int shortestdist = (transmissionRangeI + 1); //considering that infra transmission range is 250 meters, portability change
	int parent, i;
	double transmissionRange;
	double distance;
	int node_x, node_y;

	getPosition(node, &node_x, &node_y, simTime);
	//mobility hack 19-4-10
	if(node == 25)
	{
	//fprintf(stderr,"Node 25 X coord %d   Y coord %d \n", node_x, node_y);
	}
	
	for(i=0;i<numOfNodesInNetwork;i++)
	{
		if((i == ROOT) || (node != i && nodeID[i].type == INFRA && nodeID[i].state > NOT_JOINED))
		{	

			distance = (double)sqrt(pow(node_x - nodeLocation[i].x, 2) + pow(node_y - nodeLocation[i].y, 2));
			
			if(node < numOfInfraNodesInNetwork)
			{
				transmissionRange = transmissionRangeI;
			}
			else
			{
				transmissionRange = transmissionRangeM;
			}
			
			if(distance < transmissionRangeI) // changed from transmissionRangeI to transmissionRange, transmissionRangeI was correct since this reflected the case wherein an infra can reach mobile, but mobile cannot reach infra, so to say
			{
				if(distance < shortestdist)
				{
					shortestdist = distance;
					parent = i;
				}	
			}
		}	
	}

	if(shortestdist == (transmissionRangeI + 1))
	{
		return -1;
	}
	else
	{	
		return parent;
	}
}
//************* VJ change end *************


//schedule link level ack
//add requesting node into routing tree
int handleJoinReceptionRoot(struct packetFormat * aPacket)
{
	struct event * anEvent;
	int sizeofPacket;
	int node;
	int i;
	//int m;
	int infra_counter, k, rtree_counter, node_in_sched, node_found_flag, node_in_list, joining_node;
	
	node = ROOT;
	
	if(!IsPacketToBeDropped(errorRate))
	{
		anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
		insertEvent(anEvent);
		nodeID[node].packetBeingTransmitted = TRUE;
		sizeofPacket = extractSizeof(anEvent->aPacket);
		anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
		insertEvent(anEvent);
	}
	else
	{
		//do nothing
	}

	//fprintf(stderr,"\n %d node join request",((struct nodeJoin *)aPacket->payloadPacket)->node);

	//Duplicate join request
	if(lastJoinRequest != NULL && lastJoinRequest->node == ((struct nodeJoin *)aPacket->payloadPacket)->node && (isNodeInTree(((struct nodeJoin *)aPacket->payloadPacket)->node) == TRUE)) // so if the node is in the tree, it shall return true which is when you should return but if it returns false then node was dead and so do not return
	{	//fprintf(stderr,"Duplicate join request for node %d \n", ((struct nodeJoin *)aPacket->payloadPacket)->node);	
		return SUCCESS;
	}
	
	if(lastJoinRequest == NULL)
	{
		lastJoinRequest = (struct nodeJoin *)malloc(sizeof(struct nodeJoin));	
	}

	lastJoinRequest->node = ((struct nodeJoin *)aPacket->payloadPacket)->node;
	
	joining_node = ((struct nodeJoin *)aPacket->payloadPacket)->node;

	//node disable hack
	if(simTime > 1200000)
	{
	//fprintf(stderr,"Join request reached for node %d \n", joining_node);
	//fprintf(stderr,"Printing the routing tree \n");
		for(i=0; i < (numOfNodesInTree); i++)
		{
		//fprintf(stderr,"%d ",routingTree[2*i]);
		}
	//exit(0);
	}
	
	for(i=0; i < numOfNodesInTree; i++) // error corrected on 24-3-2010
	{
		if(routingTree[2*i] == ((struct nodeJoin *)aPacket->payloadPacket)->node)
		{
			if(nodeID[((struct nodeJoin *)aPacket->payloadPacket)->node].state == NOT_JOINED)
			{
				//routingTree[2*i + 1] = nodeID[((struct nodeJoin *)aPacket->payloadPacket)->node].parent; // VJ change below
				routingTree[2*i + 1] = applyJoinPolicy(((struct nodeJoin *)aPacket->payloadPacket)->node);
	
				if(routingTree[2*i + 1] == -1)
				{
					fprintf(stderr,"\n join failed when re-joining for %d", joining_node);
					//exit(0);
				}

				nodeID[ROOT].treeUpdate = TRUE;
				schedulePacketChange = TRUE;

				//fprintf(stderr,"\n Parent chosen for %d is %d \n", routingTree[2*i], routingTree[2*i + 1]);

				//portability change
				nodeSoftStateAtRoot[((struct nodeJoin *)aPacket->payloadPacket)->node] = (double)((double)ROOT_TOPOLOGY_UPDATE_REFRESH * (double)SECOND);
	
				//fprintf(stderr,"\n NN %d P %d N %d", numOfNodesInTree, routingTree[2*i + 1], joining_node);

				if(simTime > 1200000) // node rejoined after going dead, node disable hack
				{
				//fprintf(stderr,"Node %d rejoined at simTime %lf \n",joining_node, simTime);
				//exit(0);
				}

				//node disable change, maybe this check is not necessary here. if node still in routing tree, it has not timed out at root, so will be able to receive calls

				/*if(numPathsInVoiceQueue > 0) // check if any pending voice messages for the joined node
				{
				int j;
					for(j=0; j<numPathsInVoiceQueue; j++)
					{
						if(voiceQueue[j].destination == joining_node && voiceQueue[j].source == ROOT)
						{
							anEvent = getVoiceMsgRelayEvent(joining_node, simTime + 30000); //node disable hack
							insertEvent(anEvent);
							fprintf(stderr,"Set voice message relay event for %d \n", joining_node);
						}
					}
				}*/

			}
			else
			{
				//fprintf(stderr,"duplicate join request for node %d \n", joining_node);
				//printf("duplicate join request\n");	
			}	
			return SUCCESS;
		}
	}

	routingTree[2 * numOfNodesInTree] = ((struct nodeJoin *)aPacket->payloadPacket)->node; 
	//routingTree[2 * numOfNodesInTree + 1] = nodeID[((struct nodeJoin *)aPacket->payloadPacket)->node].parent; //node VJ change below
	
	routingTree[2 * numOfNodesInTree + 1] = applyJoinPolicy(((struct nodeJoin *)aPacket->payloadPacket)->node);	
	
	if(routingTree[2*numOfNodesInTree + 1] == -1)
	{
		fprintf(stderr,"\n join failed when joining for %d", joining_node);
		//exit(0);
	}
		
	if(nodeID[((struct nodeJoin *)aPacket->payloadPacket)->node].type == INFRA)
		numOfInfraNodesInTree++;
	numOfNodesInTree++;
	//printf("\t numOfnodesInTree at root %d",numOfNodesInTree);

	nodeID[ROOT].treeUpdate = TRUE;
	schedulePacketChange = TRUE;

	//portability changes start
	nodeSoftStateAtRoot[((struct nodeJoin *)aPacket->payloadPacket)->node] = (double)((double)ROOT_TOPOLOGY_UPDATE_REFRESH * (double)SECOND);
	
	//fprintf(stderr,"\n NN %d P %d N %d", numOfNodesInTree, routingTree[2*(numOfNodesInTree-1)+1], joining_node);

	//you may bring up the node information back here considering that the node now has surfaced at a different location but in the actual implementation, I think this information will need to be built gradually using the regular and reactive updates from other nodes. So this will eventually go in handleTopologyUpdateRoot

	//for distinguishing between nodes rejoined and first joined, check whether their neighbor and interferenceList is empty or not, etc. Can just check whether its incoming edge is 0 or not. Get its current X and Y co-ordinates
	
	if(simTime > 1200000) // node rejoined after going dead, node disable hack
	{
	//fprintf(stderr,"Node %d rejoined at simTime %lf \n",joining_node, simTime);
	//exit(0);
	}

	//node disable change
	if(numPathsInVoiceQueue > 0) // check if any pending voice messages for the joined node
	{
	int j;
		for(j=0; j<numPathsInVoiceQueue; j++)
		{
			if(voiceQueue[j].destination == joining_node && voiceQueue[j].source == ROOT)
			{
				anEvent = getVoiceMsgRelayEvent(joining_node, simTime + 30000); //node disable hack
				insertEvent(anEvent);
				//fprintf(stderr,"Set voice message relay event for %d \n", joining_node);
			}
		}
	}
	//checkVoiceQueue(); // see if any voice messages pending for the rejoined node, for now let it be

	/*for(rtree_counter=0; rtree_counter < numOfNodesInTree; rtree_counter++)
	{
		if(routingTree[2*rtree_counter] == joining_node)
		{
			if(routingTree[2*rtree_counter + 1] == ROOT)
			{
			//add the node to the topology update list for the ROOT
				for(k=0; k<50; k++)
				{
					if(infra_topology_update[node].mobile_list[k].mobile_number != 0)
					{
					//there is an entry here, skip it
					}
					else
					{
					//add the new entry here
					infra_topology_update[node].mobile_list[k].mobile_number = joining_node;
								
					infra_topology_update[node].mobile_list[k].numTopologyUpdateReceived = 0;

					infra_topology_update[node].mobile_list[k].nodeSoftState = (double)(INFRA_TOPOLOGY_UPDATE_REFRESH * SECOND);

					fprintf(stderr,"\n Added node %d at %d \n",infra_topology_update[node].mobile_list[k].mobile_number, node);

					break; 
					}
								
				}
			}
		}
	}
	*/
}

//portability changes start
int handleTopologyUpdateRoot(struct packetFormat * aPacket)
{
	struct event * anEvent;
	struct routingPath * temp_path;
	struct routingPath * handoff_path;
	struct routingPath ** path_array;
	struct routingPath * swap; 
	int sizeofPacket;
	int i,j,node, update_type, flowId, flow_src, flow_dst;
	int update_recvd_from;
	double distance;
	int is_src, is_dst; // boolean variables to check whether in handoff the node is the src or the destination, accordingly the path remains the same either at the end or at the beginning. 
	is_src = is_dst = FALSE;
	int retVal = 0;

	node = ROOT;
	
	if(!IsPacketToBeDropped(errorRate))
	{
		anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
		insertEvent(anEvent);
		nodeID[node].packetBeingTransmitted = TRUE;
		sizeofPacket = extractSizeof(anEvent->aPacket);
		anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
		insertEvent(anEvent);
	}
	else
	{
	//do nothing	
	}

	nodeID[node].numOfTopologyUpdatesReceived++;
	
	nodeSoftStateAtRoot[((struct topologyUpdate *)aPacket->payloadPacket)->node] = (double)((double)ROOT_TOPOLOGY_UPDATE_REFRESH * (double)SECOND);

	//14-4-10 mobility hack
	update_recvd_from = ((struct topologyUpdate *)aPacket->payloadPacket)->node ;

	//if(update_recvd_from == 25)
	//{
		//fprintf(stderr,"Topology update received for %d at %lf, now printing its neighborhood information \n", update_recvd_from, simTime);
	
	for(i=0; i < MAX_INFRA_NEIGHBORS; i++)
	{
		if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node != MAX_NO_INFRA_NODES + 1)
		{
		//fprintf(stderr,"%d is the neighbor with %d rssi \n", ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node, ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].rssi);

		//check both for parent and for handoff here
			if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node == nodeID[update_recvd_from].parent)
			{
				if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].rssi < PARENT_RSSI)
				{
				//fprintf(stderr,"Root has received update from %d at time %lf in which its parent %d 's RSSI has gone down to %d , will now change its parent \n", update_recvd_from, simTime, ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node, ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].rssi);
					
				//exit(0);
				for(j=0; j < numOfNodesInTree; j++) // error corrected on 24-3-2010
				{
					if(routingTree[2*j] == update_recvd_from)
					{
						if(nodeID[update_recvd_from].state == NOT_JOINED)
						{
							//fprintf(stderr,"Reactive update for parent doesnt mean node is not joined \n");
							//exit(0);
						}

						routingTree[2*j + 1] = applyJoinPolicy(update_recvd_from);
	
						if(routingTree[2*j + 1] == -1)
						{
						fprintf(stderr,"\n join failed when re-joining for %d", update_recvd_from);
						exit(0);
						}

						nodeID[ROOT].treeUpdate = TRUE;
						schedulePacketChange = TRUE;

					//fprintf(stderr,"\n Parent chosen for %d is %d \n", routingTree[2*j], routingTree[2*j + 1]);
					//exit(0);
					}
				}

				}
			}
		}
	}

	//***This actually involves two steps, first purging current information and then adding new information
	purgeNeighborInfo(update_recvd_from);	
	rebuildNeighborInfo(update_recvd_from);

	//now comes the part of cross checking. So every infra node in 'this' mobiles node's topology update should be (wrt mobile) in its transmission range or interference range. If not, exit.  
	int infra_found_in_update;
	
	for(i=0; i < nodeID[update_recvd_from].numInterferringNeighbors; i++)
	{
		if(nodeID[nodeID[update_recvd_from].interferenceList[i]].type == INFRA)
		{
			//fprintf(stderr,"Comparing for infra %d \n", nodeID[update_recvd_from].interferenceList[i]);
			infra_found_in_update = FALSE;
			
			for(j=0; j < MAX_INFRA_NEIGHBORS; j++)
			{
				if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[j].infra_node != MAX_NO_INFRA_NODES + 1)
				{
					if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[j].infra_node == nodeID[update_recvd_from].interferenceList[i])
					{
					infra_found_in_update = TRUE;
					}
				}
			}

			if(infra_found_in_update == FALSE)
			{
			//fprintf(stderr,"Infra %d found in interference list but not in topology update for node %d \n", nodeID[update_recvd_from].interferenceList[i], update_recvd_from);
			//exit(0);
			}

		}
	}
	//printNeighborInfo(update_recvd_from);
	//exit(0);

	update_type = ((struct topologyUpdate *)aPacket->payloadPacket)->type;

	if(update_type == HANDOFF)
	{
		//int max_rssi, max_rssi_infra;
		int infras_above_threshold;
		infras_above_threshold = 0; //initialising number of infras above the connectivity threshold
		int connectivity_infras[MAX_INFRA_NEIGHBORS]; // to store the ids of the infras above the connectivity threshold
		for(i=0; i < MAX_INFRA_NEIGHBORS; i++)
		{
			connectivity_infras[i] = MAX_NO_INFRA_NODES + 1;
		}

		//fprintf(stderr,"Root received handoff update from %d \n", update_recvd_from);
		//search for relevant flow id
		for(i=0; i<numOfFlowsInProgress; i++)
		{
			if(flowStat[i].source == update_recvd_from || flowStat[i].destination == update_recvd_from)
			{
				//fprintf(stderr,"FlowId with which node is associated is %d \n", flowStat[i].flowId);
				flowId = flowStat[i].flowId;
				flow_src = flowStat[i].source;
				flow_dst = flowStat[i].destination;

				if(flow_src == update_recvd_from)
				{
					is_src = TRUE;
				}
				else
				{
					is_dst = TRUE;
				}
				//exit(0);
				break;
			}
		}

		//find the infra node with the strongest connectivity
		if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[0].infra_node != MAX_NO_INFRA_NODES + 1)
		{
			//do nothing, this is just to check that there is at least one infra in the neighborhood. 
		}
		else
		{
			fprintf(stderr,"No infra node in the neighborhood \n");
			exit(0);
		}

		for(i=0; i < MAX_INFRA_NEIGHBORS; i++)
		{
			if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node != MAX_NO_INFRA_NODES + 1)
			{
				//fprintf(stderr,"Reached here for infra %d with rssi %d \n", ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node, ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].rssi);

				if(((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].rssi >= CONNECTIVITY_THRESHOLD)
				{
				connectivity_infras[infras_above_threshold] = ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i].infra_node;
				infras_above_threshold++;
				}
			}
		}

		//fprintf(stderr,"Infras above the connectivity threshold are \n");	

		for(i=0; i < infras_above_threshold ; i++)
		{
			//fprintf(stderr,"Infra %d \n", connectivity_infras[i]);
		}

		path_array = (struct routingPath ** )malloc(infras_above_threshold * sizeof(struct routingPath *));
		//first make the new path
		if(is_src == TRUE)
		{
			for(i=0; i < infras_above_threshold; i++)
			{
				path_array[i] = getRoutingPath(connectivity_infras[i], flow_dst);
				//fprintf(stderr,"\n Routing path changes at the source, so the path between the new first_hop_infra and the destination is \n");
				for(j=0; j < path_array[i]->numOfNodesInPath; j++)
				{
					//fprintf(stderr,"%d ", path_array[i]->path[j]);
				}
			}

			//now select the path with the smallest length. For this, we can bubble sort the array. But don't forget to use a temporary pointer while swapping. 
			for(i=0; i < infras_above_threshold; i++)
			{
				for(j=i+1; j < infras_above_threshold; j++)
				{
					if(path_array[j]->numOfNodesInPath < path_array[i]->numOfNodesInPath)
					{
						//swap them
						swap = path_array[i];
						path_array[i] = path_array[j];
						path_array[j] = swap;
					}
				}
			}
			
			for(i=0; i < infras_above_threshold; i++)
			{
				//fprintf(stderr,"\n Repeating, Routing path changes at the source, so the path between the new first_hop_infra and the destination is \n");
				for(j=0; j < path_array[i]->numOfNodesInPath; j++)
				{
					//fprintf(stderr,"%d ", path_array[i]->path[j]);
				}
			}

			revokeElements(flowId);
			//now keep scheduling till you are successful
			//information is deleted from flowStat, now putting it back
			flowStat[numOfFlowsInProgress].source = flow_src;
			flowStat[numOfFlowsInProgress].destination = flow_dst;
			flowStat[numOfFlowsInProgress].flowId = flowId;
			flowStat[numOfFlowsInProgress].voiceFlow = 0;
			flowStat[numOfFlowsInProgress].aPath = handoff_path;

			numOfFlowsInProgress++;
			//fprintf(stderr,"\t Flow no %d added, total flows %d", flowId, numOfFlowsInProgress);

			for(i=0; i < infras_above_threshold ; i++)
			{
				retVal = 0;
				//now constructing the new path
				handoff_path = (struct routingPath *)malloc(sizeof(struct routingPath));
				handoff_path->numOfNodesInPath = path_array[i]->numOfNodesInPath + 1;
				handoff_path->path = (int *)malloc(handoff_path->numOfNodesInPath * sizeof(int ));
			
				//in this case, transfer such that, later we can place the source as the first node of the path
				for(j=0; j < path_array[i]->numOfNodesInPath; j++)
				{
					handoff_path->path[j+1] = path_array[i]->path[j];
				}
				handoff_path->path[0] = flow_src;
	
				//fprintf(stderr,"New constructed path is \n");
				for(j=0; j < handoff_path->numOfNodesInPath; j++)
				{
					//fprintf(stderr,"%d ", handoff_path->path[j]);
				}

				retVal = setSchedulingElements(flow_src, flow_dst, flowId, retVal, 0, 1, handoff_path);
				while(retVal != (-1) && retVal < (MAX_PHASES-1))
				{
				retVal = setSchedulingElements(flow_src, flow_dst, flowId, retVal+1, 0, 1, handoff_path);
				}
				if(retVal == -1)
				{
					break;
				}
			}

			if(retVal != -1)
			{
			//fprintf(stderr,"The call could not be handed off \n");
			callsNotHandedOff++;
			//delete the information from the flowStat
			numOfFlowsInProgress--;
			logMobilityStats(HANDOFF_FAIL, update_recvd_from, flowId, simTime);
			}

			//exit(0);
		}
		else if(is_dst == TRUE)
		{
			for(i=0; i < infras_above_threshold; i++)
			{
				path_array[i] = getRoutingPath(flow_src, connectivity_infras[i]);
				//fprintf(stderr,"\n Routing path changes at the destination, so the path between the new last_hop_infra and the source is \n");
				for(j=0; j < path_array[i]->numOfNodesInPath; j++)
				{
					//fprintf(stderr,"%d ", path_array[i]->path[j]);
				}
			}

			//now select the path with the smallest length. For this, we can bubble sort the array. But don't forget to use a temporary pointer while swapping. 
			for(i=0; i < infras_above_threshold; i++)
			{
				for(j=i+1; j < infras_above_threshold; j++)
				{
					if(path_array[j]->numOfNodesInPath < path_array[i]->numOfNodesInPath)
					{
						//swap them
						swap = path_array[i];
						path_array[i] = path_array[j];
						path_array[j] = swap;
					}
				}
			}
			
			for(i=0; i < infras_above_threshold; i++)
			{
				//fprintf(stderr,"\n Repeating, Routing path changes at the destination, so the path between the new last_hop_infra and the source is \n");
				for(j=0; j < path_array[i]->numOfNodesInPath; j++)
				{
					//fprintf(stderr,"%d ", path_array[i]->path[j]);
				}
			}

			revokeElements(flowId);
			//now keep scheduling till you are successful
			//information is deleted from flowStat, now putting it back
			flowStat[numOfFlowsInProgress].source = flow_src;
			flowStat[numOfFlowsInProgress].destination = flow_dst;
			flowStat[numOfFlowsInProgress].flowId = flowId;
			flowStat[numOfFlowsInProgress].voiceFlow = 0;
			flowStat[numOfFlowsInProgress].aPath = handoff_path;

			numOfFlowsInProgress++;
			//fprintf(stderr,"\t Flow no %d added, total flows %d", flowId, numOfFlowsInProgress);

			for(i=0; i < infras_above_threshold ; i++)
			{
				retVal = 0;
				//now constructing the new path
				handoff_path = (struct routingPath *)malloc(sizeof(struct routingPath));
				handoff_path->numOfNodesInPath = path_array[i]->numOfNodesInPath + 1;
				handoff_path->path = (int *)malloc(handoff_path->numOfNodesInPath * sizeof(int ));
			
				//in this case, transfer such that, later we can place the destination as the last node of the path
				for(j=0; j < path_array[i]->numOfNodesInPath; j++)
				{
					handoff_path->path[j] = path_array[i]->path[j];
				}
				handoff_path->path[j] = flow_dst;

				//fprintf(stderr,"New constructed path is \n");
				for(j=0; j < handoff_path->numOfNodesInPath; j++)
				{
					//fprintf(stderr,"%d ", handoff_path->path[j]);
				}

				retVal = setSchedulingElements(flow_src, flow_dst, flowId, retVal, 0, 1, handoff_path);
				while(retVal != (-1) && retVal < (MAX_PHASES-1))
				{
				retVal = setSchedulingElements(flow_src, flow_dst, flowId, retVal+1, 0, 1, handoff_path);
				}
				if(retVal == -1)
				{
					break;
				}
			}

			if(retVal != -1)
			{
			//fprintf(stderr,"The call could not be handed off \n");
			callsNotHandedOff++;
			//delete the information from the flowStat
			numOfFlowsInProgress--;
			logMobilityStats(HANDOFF_FAIL, update_recvd_from, flowId, simTime);
			}

		}
		
	}	
//} // the if for the hack ends here
	//fprintf(stderr,"Topology update received for %d at %lf \n", ((struct topologyUpdate *)aPacket->payloadPacket)->node, simTime);			
	
}
//portability changes end

//portability changes start
int handleTopologyUpdateCheck()
{

	int count, i, j, node;
	for(count = 1; count < numOfNodesInTree; count++)
	{
		if(nodeSoftStateAtRoot[routingTree[2 * count]] > 0)
		{
			nodeSoftStateAtRoot[routingTree[2 * count]] -= FRAME_DURATION;
			if(nodeSoftStateAtRoot[routingTree[2 * count]] <= 0)
			{
				node = routingTree[2 * count];

				//fprintf(stderr,"Node to be deleted is %d simTime %lf \n", node, simTime);
				//sleep(5);	
					
				for(i=count; i<(numOfNodesInTree - 1); i++)
				{
					routingTree[2 * i + 1] = routingTree[2 * (i+1) + 1];
					routingTree[2 * i] = routingTree[2 * (i+1)];	
				}
				
				numOfNodesInTree--; // decrement numOfInfraNodesInTree as well if the node dead is an infra
				if(nodeID[node].type == INFRA)
				{
					numOfInfraNodesInTree--;
					dropInfraFlows(node);
				}
				
				//fprintf(stderr,"\n node %d removed from tree due to topology time out, type %d \n",node, nodeID[node].type);
				
				//node disable hack
				//if((nodeID[nodeID[node].parent].state == DEAD) || (nodeID[nodeID[nodeID[node].parent].parent].state == DEAD))
				if(anyAncestorIsDead(node) == TRUE)
				{
					//fprintf(stderr,"For node %d, either parent %d is dead or parent's parent %d is dead \n", node, nodeID[node].parent, nodeID[nodeID[node].parent].parent);
					//fprintf(stderr,"Some ancestor is dead for this node %d \n", node);
					continue; // this is a hack. So basically if an infra node goes down, its fine if its child nodes time out and get eliminated from the routing tree. But I don't want their neighborhood information to be lost since they should be able to hear from other infras and rejoin the network. Same goes for the child infra of the infra node if the infra going dead is an upper level infra
				}

				//changing the network graph
				purgeNeighborInfo(node);
				purgeNodeInfo(node);
					
				nodeID[ROOT].treeUpdate = TRUE;
				schedulePacketChange = TRUE;

			}
		}
	}
		
	return SUCCESS;
}
//portability changes end


//this function checks whether flow has timed out
int handleBwRequestTimeout(int flowId)
{
	int i;
	int flowPresent;
	flowPresent = FALSE;
	
	if(numOfFlowsInProgress == 0)
	{		
		return SUCCESS;
	}
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId) //flowId is set when packet is generated by source
		{
			flowPresent = TRUE;
			if(flowStat[i].timeoutTime <= simTime) //timeoutTime is set when bw request is received by the root and this event follows bw request reception
			{
				if(isFlowRevoked(flowId) == FALSE)
				{
					if(nodeID[flowStat[i].source].callDuration > 0)
					{
						noOfCallsErrDropped++;
					}
					
					fprintf(stderr,"\n BW Request Time out : %d flow revoked now, duration left %lf minutes",flowId, nodeID[flowStat[i].source].callDuration/(60*1000));
					sleep(2);	
				
					//printf("\n BW Request Time out : %d flow revoked now, duration left %lf",flowId, nodeID[flowStat[i].source].callDuration/(60*1000));
					if(MAX_NO_CHANNELS < 3)
					{
						revokeElementsSingleChannel(flowId);
						//revokeElements(flowId);
					}
					else
					{
						#ifdef PHASE_SCHEDULER
						revokeElements(flowId);
						#else
						revokeElements_schedAlgo(flowId);
						#endif
					}	
					//revokeElements2(flowId);					
					
				}
				else
				{
					//fprintf(stderr,"\n %d flow was already revoked",flowId);
					//printf("\n %d flow was already revoked",flowId);
					
				}
				
				return SUCCESS;
			}		
		}
	}
	if(flowPresent == TRUE)
	{
		//printf("\n Flow YET TO time out %d",flowId);	
	}	
	else
	{
		//fprintf(stderr,"\n %d flow was already revoked",flowId);	
	}	
}


int handleFlowReceptionRoot(struct packetFormat * aPacket)
{
	struct event * anEvent;
	struct flowRequest * aFlowRequest;
	int sizeofPacket;
	int node;
	int i, countCalls;
	
	int retVal = 0;
	
	int tempNumGlobalSchedElem;
	int k;
	
	node = ROOT;
	
	if(!IsPacketToBeDropped(errorRate))
	{
	anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
	insertEvent(anEvent);
	nodeID[node].packetBeingTransmitted = TRUE;
	sizeofPacket = extractSizeof(anEvent->aPacket);
	anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
	insertEvent(anEvent);
	}
	else
	{
	//do nothing
	}

	aFlowRequest = (struct flowRequest *)aPacket->payloadPacket;
	
	if(lastFlowRequest != NULL && lastFlowRequest->e2eSource == aFlowRequest->e2eSource && lastFlowRequest->e2eDestination == aFlowRequest->e2eDestination && lastFlowRequest->flowId_bwRequest == aFlowRequest->flowId_bwRequest)
	{
		//fprintf(stderr,"\n duplicate flow request");
		//printf("\n duplicate flow request");
		
		return SUCCESS;
	}
	
	if(lastFlowRequest == NULL)
	{
		lastFlowRequest = (struct flowRequest *)malloc(sizeof(struct flowRequest));
	}
	lastFlowRequest->e2eSource = aFlowRequest->e2eSource; 
	lastFlowRequest->e2eDestination = aFlowRequest->e2eDestination;
	lastFlowRequest->flowId_bwRequest = aFlowRequest->flowId_bwRequest;
	
	//portability change
	if(nodeID[aFlowRequest->e2eDestination].callInProgress == TRUE)
	{
	//fprintf(stderr,"Successfully detected the condition wherein a receiver was going to have two simultaneous calls \n");
	return 0;
	//exit(0);
	//return SUCCESS;
	}
	
	//fprintf(stderr,"flowRequest s %d d %d f %d",aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
	
	if(aFlowRequest->e2eSource == 49 && aFlowRequest->e2eDestination == 53 && EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest) == 89)
	{
		return 0;
	}
	
	
	flowRequestReceptionCounter++;
	
	if(destinationIsUp(aFlowRequest->e2eDestination) == SUCCESS) // node disable change
	{
		//setScheduleInfo(simTime);
		
		//fprintf(stderr,"\n Flow request %d reached\n",EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
		
		//sleep(2);
		
		//*
		#ifdef PHASE_SCHEDULER
		
		if(MAX_NO_CHANNELS < 3)
		{
			//fprintf(stderr,"beforeSchedLoop s %d d %d f %d",aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
			
			retVal = setSchedulingElementsSingleChannel(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal);
			
		
			//retVal = setSchedulingElements(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal, 0, -1, NULL);
			//this if condition is cuurently not being used
		}
		else
		{
			retVal = setSchedulingElements(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal, 0, -1, NULL);
		}
		//exit(0);	
		while(retVal != (-1) && retVal < (MAX_PHASES-1)) //earlier MAX_PHASES - 1
		{
			//fprintf(stderr,"\n phase being looked at %d flow id %d \n",retVal+1, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
			//exit(0);
			if(MAX_NO_CHANNELS < 3)
			{
				//fprintf(stderr,"schedloop s %d d %d f %d",aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
				
				retVal = setSchedulingElementsSingleChannel(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal+1);
				
				
				//retVal = setSchedulingElements(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal+1, 0, -1, NULL);
			}
			else
			{
				//if(retVal+1 < MAX_PHASES)
				{
					retVal = setSchedulingElements(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal+1, 0, -1, NULL);
				}
				
				
				//else
				//{
					//retVal++;
				//}	
			}	
		}
		
		if(retVal == (MAX_PHASES))
		{
			//fprintf(stderr,"\n Call rejected %d\n", EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
			//sleep(5);
			//exit(0);
		}
		else
		{
			if(MAX_NO_CHANNELS < 3)
			{
				//fprintf(stderr,"\n flow accepted %d", EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
			}	
		}	
		//sleep(2);
		
		//sleep(5);
		//setSchedulingElements2(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));				
		
		#else
		
		//*/
		
		///*
		tempNumGlobalSchedElem = globalNumSchedElem;
		
		if(setSchedule_shortestPath(aFlowRequest->e2eSource, aFlowRequest->e2eDestination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest)))
		{ 
		 	if(setSchedule_shortestPath(aFlowRequest->e2eDestination, aFlowRequest->e2eSource, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest)))
			{
			
				//fprintf(stderr,"\n Flow admitted %d simTime %lf \n",EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), simTime);
				setScheduleInfo();//just sets schedulePacketChange to true // important step
		
				flowRequestsAdmitted[globalCallsInProgress]++;	
				globalCallsInProgress++;
				noOfCallsAdmitted++;
				noOfCallsAdmitted_Unique[EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest)] = 1;

			
			}
			else
			{
				//fprintf(stderr,"\n Reverse path allocation failed\n");
			
				for(k=tempNumGlobalSchedElem; k<globalNumSchedElem; k++)
				{
					
					//********************************************************
					slotChannelNodes[globalScheduleElements[k].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[k].flowId_channel)][globalScheduleElements[k].transmitter] = 0;
					slotChannelNodes[globalScheduleElements[k].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[k].flowId_channel)][globalScheduleElements[k].receiver] = 0;
					//********************************************************
		
				}
				
				globalNumSchedElem = tempNumGlobalSchedElem;					
			}
		}
		
		#endif
		//*/
	}
	else
	{
		//fprintf(stderr,"\n destination is not up %d \n",aFlowRequest->e2eDestination);
		//exit(0); //for now
	}
	
}

//manan change
int checkVoicePhases(int source, int destination) // why not make this function return a node rather than just simply 1
{
struct routingPath *aPath, *new_aPath;
aPath = getRoutingPath(source, destination);
int i,j,flag,new_destination, node_counter,retVal,new_phase;

for(i=0; i < MAX_PHASES; i++)
{
flag = 0;

	for(j=0; j<aPath->numOfNodesInPath; j++)
	{
		if(phasesPerNode[aPath->path[j]][i] == 1 || phasesPerNode[aPath->path[j]][i] == 2)
		flag = 1;	
	}

	if(flag == 0)
	{
	//fprintf(stderr,"The value returned from checkVoicePhases is %d \n",aPath->path[aPath->numOfNodesInPath-1]);
	return aPath->path[aPath->numOfNodesInPath-1];
	}
}

if(i == MAX_PHASES && aPath->numOfNodesInPath == 2)
{
return -1;
}

if(i == MAX_PHASES && aPath->numOfNodesInPath > 2)
{
//fprintf(stderr,"No common phase found for the voice message in getVoiceDestination\n");
//fprintf(stderr,"Now reducing path and trying...\n");

	for(node_counter = 2; node_counter <= (aPath->numOfNodesInPath-1) ; node_counter++)
	{
	new_aPath = getRoutingPath(source, aPath->path[aPath->numOfNodesInPath - node_counter]);
	//fprintf(stderr,"Path now being found between %d and %d \n",source, aPath->path[aPath->numOfNodesInPath - node_counter]);
		
		for(new_phase = 0; new_phase < MAX_PHASES; new_phase++)
		{
		flag = 0;

			for(j=0; j < new_aPath->numOfNodesInPath; j++)
			{
			if(phasesPerNode[new_aPath->path[j]][new_phase] == 1 || phasesPerNode[new_aPath->path[j]][new_phase] == 2)
			flag = 1;	
			}

			if(flag == 0)
			{
			//fprintf(stderr,"The value returned from checkVoicePhases is %d \n",new_aPath->path[new_aPath->numOfNodesInPath-1]);
			return new_aPath->path[new_aPath->numOfNodesInPath-1];
			}
		}
	}

	if(new_phase == MAX_PHASES) // its actually a redundant check
	{
	return -1;
	}

}
//phase checking code ends here. you are supposed to check for phases only after you know till which bottleneck node are you going to schedule the voice message.
}

int getVoiceDestination(int source, int destination)
{
//the destination here is the end to end destination which is the mobile node so just need to check it till the secondlast node
int i,j,k,phase_check;
struct routingPath * aPath;
aPath = getRoutingPath(source,destination);

//fprintf(stderr," source %d destination %d the routing path in getVoiceDestination is \n",source,destination);
for(i=0;i<aPath->numOfNodesInPath;i++)
{
//fprintf(stderr,"%d  ",aPath->path[i]);
if(aPath->path[i] < 0 || aPath->path[i] > numOfNodesInNetwork)
	{
	fprintf(stderr,"exiting for error checking in getVoiceDestination\n");
	exit(0);
	}
}

if(nodeID[aPath->path[0]].type == INFRA) // added 28-9-09
	{
		
		if(flowsPerNode[aPath->path[0]]+1 > MAX_PHASES)
		{
		//fprintf(stderr,"starting node itself busy, dont schedule right now\n");
		//exit(0);
		return -1;
		}
	}

if(nodeID[aPath->path[1]].type == INFRA)
	{	
		if(flowsPerNode[aPath->path[1]]+1 > MAX_PHASES)
		{
		//fprintf(stderr,"immediate infra bottleneck, dont schedule right now\n");
		return -1;
		}
	}

if(aPath->numOfNodesInPath == 2) // added 28-9-09
{
phase_check = checkVoicePhases(source, destination);
if(phase_check != -1)
return phase_check;
else
return -1;
}

for(i=2;i<=(aPath->numOfNodesInPath-1);i++) // modified from < to <= and also changed return i to i-1
	{
	//fprintf(stderr,"\n checked till %d",aPath->path[i]);
	if(flowsPerNode[aPath->path[i]]+1 > MAX_PHASES)
		{
		phase_check = checkVoicePhases(aPath->path[0],aPath->path[i-1]);
		//its just that the flows per node at path[0] and path[1] are not = 4. But you may still not have a common phase between the starting node and path[i-1]. Thats why checking for the entire path
		if(phase_check != -1)
			{
			return phase_check; // i is the bottleneck node, so return i-1	
			}
		else
			{
			return -1;
			}
		}
	}


phase_check = checkVoicePhases(source, destination);
	if(phase_check != -1)
	{
	return phase_check;
	}
	else
	{
	return -1;
	}

}


int handleVoiceFlowReceptionRoot(struct packetFormat * aPacket)
{
	//fprintf(stderr,"reahced voice flow reception for root\n");
	//FILE *vm;
	struct event * anEvent;
	struct voiceFlowRequest * aFlowRequest;
	struct routingPath * aPath;
	int sizeofPacket;
	int node,i,k,flow_found; // to see whether the setSchedulingElements is successful or not
	flow_found = 0;
	//k will be the numSchedElem at the root
	int phase_info, phase_counter; // used for putting phase information in voiceCurrQueue and voiceQueue	
	int destination;
	int retVal;
	retVal = 0;
	
	node = ROOT;
	
	if(!IsPacketToBeDropped(errorRate))
	{
	anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
	insertEvent(anEvent);
	nodeID[node].packetBeingTransmitted = TRUE;
	sizeofPacket = extractSizeof(anEvent->aPacket);
	anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
	insertEvent(anEvent);
	}
	else
	{
	//do nothing
	}

	aFlowRequest = (struct voiceFlowRequest *)aPacket->payloadPacket;
	
	if(lastVoiceFlowRequest != NULL && lastVoiceFlowRequest->e2eSource == aFlowRequest->e2eSource && lastVoiceFlowRequest->flowId_bwRequest == aFlowRequest->flowId_bwRequest && lastVoiceFlowRequest->overalle2eDestination == aFlowRequest->overalle2eDestination)
	{
		//fprintf(stderr,"\n duplicate voice flow request");
		//printf("\n duplicate voice flow request");
		
		return SUCCESS;
	}
	
	if(lastVoiceFlowRequest == NULL)
	{
		lastVoiceFlowRequest = (struct voiceFlowRequest *)malloc(sizeof(struct voiceFlowRequest));
	}

	lastVoiceFlowRequest->e2eSource = aFlowRequest->e2eSource; 
	lastVoiceFlowRequest->flowId_bwRequest = aFlowRequest->flowId_bwRequest;
	lastVoiceFlowRequest->overalle2eDestination = aFlowRequest->overalle2eDestination;

	if(destinationIsUp(aFlowRequest->overalle2eDestination) == SUCCESS) //node disable change
	{
	destination = getVoiceDestination(aFlowRequest->e2eSource, aFlowRequest->overalle2eDestination);
	}
	else
	{
	//fprintf(stderr,"\n destination is not up in handleVoiceFlowReceptionRoot %d, diverting the voice message to the root \n", aFlowRequest->overalle2eDestination);
	destination = ROOT;
	//exit(0); // for now
	}

	if(nodeID[aFlowRequest->overalle2eDestination].callInProgress == TRUE)
	{
	//fprintf(stderr," Cannot schedule voice message towards receiver %d since it is on another call, storing it in voice Queue \n", aFlowRequest->overalle2eDestination);
	//exit(0);
	destination = -1;
	}	
		//now need to update the flowStat information here

		for(i=0; i<numOfFlowsInProgress; i++)
		{
			if(flowStat[i].flowId == EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest) && flowStat[i].voiceFlow == 1)
			{
			//fprintf(stderr,"updated the flow destination for flow %d source %d destination %d\n",flowStat[i].flowId, flowStat[i].source,destination);
			flowStat[i].destination = destination;
				//portability change
				if(destination == -1)
				{
				// do not include its info in flowStat, should be updated later in setCarryForwardFlow
				}
				else
				{
				aPath = getRoutingPath(flowStat[i].source, flowStat[i].destination);
				flowStat[i].aPath = aPath;
				}
			}
		} // there's no problem with destination being -1

		numFlowRequestsReached[EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest)] = 1;
		//isetScheduleInfo(simTime);
		
		if(MAX_NO_CHANNELS < 3)
		{
		
			fprintf(stderr,"voiceReception s %d d %d f %d",aFlowRequest->e2eSource, destination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
		
			retVal = setSchedulingElementsSingleChannel(aFlowRequest->e2eSource, destination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal);
			//retVal = setSchedulingElements(aFlowRequest->e2eSource, destination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal, 0, -1, NULL);
			//this if condition is currently not being used
		}
		else
		{
	
			//fprintf(stderr,"value of destination received is %d\n",destination);			
			//exit(0);
			if(destination != -1) // -1 is returned when immediate infra node is busy or 1st node is busy
			{
			retVal = setSchedulingElements(aFlowRequest->e2eSource, destination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal, 0, -1, NULL);

			while(retVal != (-1) && retVal < (MAX_PHASES-1))
				{
				//vm = fopen("voiceMsg.txt","a+");
				fprintf(stderr,"This printf should ideally never be called\n");	
				exit(0);
				retVal = setSchedulingElements(aFlowRequest->e2eSource, destination, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest), retVal+1, 0, -1, NULL);
				//fclose(vm);
				}
			
				k = nodeID[ROOT].numSchedElem;

				for(i=0; i<k; i++)
				{
					if(EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel) == EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest))
					{
					//fprintf(stderr,"\n flow allocated in the check of handleVoiceFlowReceptionRoot\n");
					flow_found = 1;
					break;
					}
				}

				if(flow_found == 1)
				{
				//adding the information to voiceCurrQueue
					if(numPathsInVoiceCurrQueue < 50 )
					{
	
					voiceCurrQueue[numPathsInVoiceCurrQueue].source = aFlowRequest->e2eSource;
					voiceCurrQueue[numPathsInVoiceCurrQueue].destination = destination;
					voiceCurrQueue[numPathsInVoiceCurrQueue].overalle2eDestination = aFlowRequest->overalle2eDestination;
					
					voiceCurrQueue[numPathsInVoiceCurrQueue].voiceFlowId = EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest);
					voiceCurrQueue[numPathsInVoiceCurrQueue].voiceMsgDuration = aFlowRequest->voiceMsgDuration + bufferVoiceMsgTime(); // portability change
					voiceCurrQueue[numPathsInVoiceCurrQueue].voiceCopyCallDuration = aFlowRequest->voiceMsgDuration;
					voiceCurrQueue[numPathsInVoiceCurrQueue].rPath = getRoutingPath(aFlowRequest->e2eSource, destination);

					//extracting phase information
					for(i=0; i<numOfFlowsInProgress; i++)
					{
						if(flowStat[i].flowId == EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest))
						{
						voiceCurrQueue[numPathsInVoiceCurrQueue].voice_phase = flowStat[i].phase;
						break;
						}
					}

					//fprintf(stderr,"\n voiceCurrQueue 1 source %d destination %d overalle2eDestination %d flow id %d duration  %lf copyCallDuration %lf phase %d \n", voiceCurrQueue[numPathsInVoiceCurrQueue].source, voiceCurrQueue[numPathsInVoiceCurrQueue].destination, voiceCurrQueue[numPathsInVoiceCurrQueue].overalle2eDestination, voiceCurrQueue[numPathsInVoiceCurrQueue].voiceFlowId, voiceCurrQueue[numPathsInVoiceCurrQueue].voiceMsgDuration, voiceCurrQueue[numPathsInVoiceCurrQueue].voiceCopyCallDuration, voiceCurrQueue[numPathsInVoiceCurrQueue].voice_phase);
					
					//fprintf(stderr,"Routing path is \n");
					for(i=0;i<voiceCurrQueue[numPathsInVoiceCurrQueue].rPath->numOfNodesInPath;i++)
					{
					//fprintf(stderr,"%d ",voiceCurrQueue[numPathsInVoiceCurrQueue].rPath->path[i]);
					}
	
					numPathsInVoiceCurrQueue++;
					//fprintf(stderr,"\n");
					//also remember to add the code in revoke elements
					}
					else
					{
					fprintf(stderr,"System overloaded with current voice messages %d\n",numPathsInVoiceCurrQueue);
					exit(0);
					}
			
				}
				
				else
				{
				fprintf(stderr,"This printf from within handleVoiceFlowReceptionRoot should not be called\n");
				exit(0);
				} 
			}
			else
			{
				if(numPathsInVoiceQueue < 50)
				{

				voiceQueue[numPathsInVoiceQueue].source = aFlowRequest->e2eSource;
				voiceQueue[numPathsInVoiceQueue].destination = aFlowRequest->overalle2eDestination;
				voiceQueue[numPathsInVoiceQueue].overalle2eDestination = aFlowRequest->overalle2eDestination;
				voiceQueue[numPathsInVoiceQueue].callDuration = aFlowRequest->voiceMsgDuration;
				voiceQueue[numPathsInVoiceQueue].voiceQueueFlowId = EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest);
				voiceQueue[numPathsInVoiceQueue].voice_phase = -1; //for now

				//fprintf(stderr,"correctly executing here in rootfunc.c, added the path info to voice queue\n");
				//fprintf(stderr,"source %d destination %d overalle2eDestination %d callDuration %lf voiceQueueFlowId %d\n", voiceQueue[numPathsInVoiceQueue].source, voiceQueue[numPathsInVoiceQueue].destination, voiceQueue[numPathsInVoiceQueue].overalle2eDestination, voiceQueue[numPathsInVoiceQueue].callDuration,			voiceQueue[numPathsInVoiceQueue].voiceQueueFlowId);

				//for calculating the storage capacity
				nodeID[voiceQueue[numPathsInVoiceQueue].source].store_cap += ((aFlowRequest->voiceMsgDuration/30)*24);
				//logging this update
				logStoreCap(voiceQueue[numPathsInVoiceQueue].source,nodeID[voiceQueue[numPathsInVoiceQueue].source].store_cap,simTime);

				numPathsInVoiceQueue++;
				//nodeID[node].controlRear = (nodeID[node].controlRear + 1) % QUEUE_LIMIT;
				//nodeID[node].numOfPacketsInQueue++;
				}
				else
				{
				fprintf(stderr,"System overloaded with numPathsInVoiceQueue >=50 \n");
				exit(0);
				}
			//retVal = -1; 
			}
		}		
}
//manan end

//this function is triggered only when bw request reaches root
int handleBwRequestRoot(struct packetFormat * aPacket)
{
	int i, j, k;
	int flowId;
	struct event * anEvent;
	int sizeofPacket;
	int node;
	int flowExists;
	int flowAllocated;
	int tempNumGlobalSchedElem;
	
	node = ROOT;
	flowExists = 0;
	flowAllocated = 0;
	
	k = nodeID[ROOT].numSchedElem;
	
	if(!IsPacketToBeDropped(errorRate))
	{
		anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
		insertEvent(anEvent);
		nodeID[node].packetBeingTransmitted = TRUE;
		sizeofPacket = extractSizeof(anEvent->aPacket);
		anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
		insertEvent(anEvent);
	}
	else
	{
		//do nothing
	}
	
	flowId = EXTRACT_FLOWID(((struct bwRequest *)aPacket->payloadPacket)->flowId_bwRequest);
	
	//fprintf(stderr,"\nBWRequest Received %d %lf schedElem %d\n",flowId, simTime, k);
	
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId) //set when packet is generated by source
		{
		
			for(j=0; j<k; j++)
			{
				//fprintf(stderr, "\n flowid %d\n", EXTRACT_FLOWID(globalScheduleElements[j].flowId_channel));	
					
				if(EXTRACT_FLOWID(globalScheduleElements[j].flowId_channel) == flowId)
				{
					flowAllocated = 1;
					break;					
				}
			}
			
			if(flowAllocated == 0)
			{
				numFlowRequestsReached[flowId] = 1;
				int retVal;
				retVal = 0;
				
				// node disable change. checking whether the destination is alive or not
				int destination;
				destination = flowStat[i].destination;
				//fprintf(stderr,"The destination in handleBwRequestRoot is %d \n", destination);

				if(destinationIsUp(destination) == SUCCESS) // if the destination is alive, try scheduling the flow
				{
				
				//*
				#ifdef PHASE_SCHEDULER
				
				/*
				retVal = setSchedulingElements(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId,retVal,0, -1, NULL);
				while(retVal != (-1) && retVal < (MAX_PHASES-1))
				{
				//fprintf(stderr,"\n phase being looked at %d flow id %d \n",retVal+1, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest));
				retVal = setSchedulingElements(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId, retVal+1, 0, -1, NULL);	
				}
				*/

				if(MAX_NO_CHANNELS < 3)
				{
					//fprintf(stderr,"bwReception s %d d %d f %d",flowStat[i].source, flowStat[i].destination, flowStat[i].flowId);
				
					retVal = setSchedulingElementsSingleChannel(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId, retVal);				
				}
				else
				{
					retVal = setSchedulingElements(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId, retVal, 0, -1, NULL);
				}
	
				while(retVal != (-1) && retVal < (MAX_PHASES-1)) //earlier MAX_PHASES - 1
				{
					//fprintf(stderr,"\n phase being looked at %d flow id %d \n",retVal+1,flowId);
					
					if(MAX_NO_CHANNELS < 3)
					{
					
						//fprintf(stderr,"bwReception2 s %d d %d f %d: val %d",flowStat[i].source, flowStat[i].destination, flowStat[i].flowId, retVal);
							
						retVal = setSchedulingElementsSingleChannel(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId, retVal+1);
				
					}
					else
					{
				
						retVal = setSchedulingElements(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId, retVal+1, 0, -1, NULL);
					}
				
				}
		
				if(retVal == (MAX_PHASES))
				{
					//fprintf(stderr,"\n Call rejected %d\n", flowId);
					//sleep(5);
					//exit(0);
				}
				else
				{
					//fprintf(stderr,"\n retVal %d flow accepted %d\n",retVal, flowId);
				}	

				#else
				
				//*/
				
				tempNumGlobalSchedElem = globalNumSchedElem;
		
				if(setSchedule_shortestPath(flowStat[i].source, flowStat[i].destination, flowId))
				{ 
		 			if(setSchedule_shortestPath(flowStat[i].destination, flowStat[i].source, flowId))
					{
						
						setScheduleInfo();//just sets schedulePacketChange to true // important step
		
						flowRequestsAdmitted[globalCallsInProgress]++;	
						globalCallsInProgress++;
						noOfCallsAdmitted++;
						noOfCallsAdmitted_Unique[flowId] = 1;

						
					}
					else
					{
						for(k=tempNumGlobalSchedElem; k<globalNumSchedElem; k++)
						{
					
							//********************************************************
							slotChannelNodes[globalScheduleElements[k].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[k].flowId_channel)][globalScheduleElements[k].transmitter] = 0;
							slotChannelNodes[globalScheduleElements[k].slotToStart][EXTRACT_CHANNEL(globalScheduleElements[k].flowId_channel)][globalScheduleElements[k].receiver] = 0;
							//********************************************************
		
						}
				
						globalNumSchedElem = tempNumGlobalSchedElem;	
					}
					
				}
				#endif	
				
				}	
				else
				{
				//fprintf(stderr,"destination %d is not up in handleBwRequestRoot\n",destination);
				}
		
				//setSchedulingElements2(flowStat[i].source, flowStat[i].destination, flowStat[i].flowId);
			}

			flowStat[i].timeoutTime = simTime + (double)BW_TIMEOUT; //update soft state, used by time out event
			flowExists = 1;
			break;
		}
	}
	
	if(flowExists == 0)
	{
		//fprintf(stderr,"\n Unmapped bandwidth request");
		//printf("\n Unmapped bandwidth request");
		
		return SUCCESS;
		//exit(0);
	}
	
	anEvent = getBandwidthTimeoutEvent(simTime + BW_TIMEOUT, flowId);
	insertEvent(anEvent);
}


//schedule link level ack, revoke resource allocated earlier for this flow id
int handleTearDownRoot(struct packetFormat * aPacket)
{
	struct event * anEvent;
	int sizeofPacket;
	int node;
	
	node = ROOT;
	
	//fprintf(stderr,"\n Tear down received by the root");
	
	if(!IsPacketToBeDropped(errorRate))
	{
		anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
		insertEvent(anEvent);
		nodeID[node].packetBeingTransmitted = TRUE;
		sizeofPacket = extractSizeof(anEvent->aPacket);
		anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
		insertEvent(anEvent);
	}
	else
	{
		//to nothing
	}
	
	if(lastTearDown != NULL && lastTearDown->flowId == ((struct tearDown *)aPacket->payloadPacket)->flowId)
	{
		//fprintf(stderr,"\n *** duplicate tear down in handle tear down root for flow id %d ***\n",lastTearDown->flowId);
		//printf("\n *** duplicate tear down in handle tear down root for flow id %d ***\n",lastTearDown->flowId);
		
		/*int i;
		for(i=0;i<numOfFlowsInProgress;i++)
		fprintf(stderr,"flowid %d source %d destination %d\n",flowStat[i].flowId,flowStat[i].source,flowStat[i].destination);*/
		return SUCCESS;
	}
	
	if(lastTearDown == NULL)
	{
		lastTearDown = (struct tearDown *)malloc(sizeof(struct tearDown));	
	}
	lastTearDown->flowId = ((struct tearDown *)aPacket->payloadPacket)->flowId;

	if(MAX_NO_CHANNELS < 3)
	{
		revokeElementsSingleChannel(((struct tearDown *)aPacket->payloadPacket)->flowId);
		//revokeElements(((struct tearDown *)aPacket->payloadPacket)->flowId);
	}
	else
	{
		//fprintf(stderr,"revoking for flow %d voice flow %d\n",((struct tearDown *)aPacket->payloadPacket)->flowId,flowStat[((struct tearDown *)aPacket->payloadPacket)->flowId].voiceFlow);

		
		#ifdef PHASE_SCHEDULER
		revokeElements(((struct tearDown *)aPacket->payloadPacket)->flowId);
		#else
		revokeElements_schedAlgo(((struct tearDown *)aPacket->payloadPacket)->flowId);
		#endif
		
	}	
	
	
	//revokeElements2(((struct tearDown *)aPacket->payloadPacket)->flowId);
}


