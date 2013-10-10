#include "header.h"

//transmit packets from control queue using ALOHA
int handleContentionSlot(int node)
{
	struct event * anEvent;
	struct packetFormat * aPacket;
	struct packetFormat * otherPacket;
	int sizeofPacket, index;
	double backoff;
	int type, flowSource;
	int l;
	double tempTime, contentionSlotBoundary;
	//FILE *vm;
	
				
	if(nodeID[node].numOfPacketsInQueue <= 0)
		return SUCCESS;
	
	aPacket = nodeID[node].packetQueue[nodeID[node].controlFront];	
	
	if(contentionFrame(nodeID[node].contentionProb) == FALSE)
	{
		/*char * packet_type;
		packet_type = (char *)malloc(20*sizeof(char));
		resolve_packet(EXTRACT_TYPE(aPacket->ver_type_piggyback), packet_type);*/

		//printf("\n %d contention transmission rejected contentionProb %lf",node,nodeID[node].contentionProb);
		//fprintf(stderr,"\n Node  %d contention transmission rejected for %s  contentionProb %lf",node, packet_type, nodeID[node].contentionProb);
		//free(packet_type);
		return SUCCESS;
	}	
	
	
	if(aPacket == NULL)
	{
		fprintf(stderr, "\n Sanity check failed in handleContentionSlot: null packet");
		exit(0);
	}
	sizeofPacket = extractSizeof(aPacket);
	
	if(nodeID[node].parent == UNDEFINED)
	{
		
		if(nodeID[node].wasOrphan == TRUE)
		{
		return SUCCESS; // node disable change
		}
		else
		{
		//fprintf(stderr,"\n\n Parent Undefined for node %d type %d",node,EXTRACT_TYPE(aPacket->ver_type_piggyback));
		//exit(0);
		
		nodeID[node].state = NOT_JOINED;

		tempTime = simTime;
		
		tempTime += (FRAME_DURATION - fmod(tempTime, FRAME_DURATION)); //this is for synchronising with the frame durations. it tells when the next frame is going to start
		
		contentionSlotBoundary = tempTime + ((double)10*FRAME_DURATION) + (NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION);

		anEvent = getContentionSlotEvent(contentionSlotBoundary, node);	//inserts next contention slot event, the contention slot event pops out contention packets from contention queue and transmits through CSMA
		insertEvent(anEvent);

		
		return SUCCESS;
		}
	}

	//node disable change
	if(nodeID[nodeID[node].parent].state == DEAD)
	{
	return SUCCESS;
	}

	//hack 12-4-10
	/*if(node == 61 && simTime > 2041186)
	{
		int type;
		type = EXTRACT_TYPE(aPacket->ver_type_piggyback);
		if(type == TOPOLOGY_UPDATE)
		{
		fprintf(stderr,"Reached here for setting RX_BEGIN and RX_END events for parent and CONTENTION_RX_BEGIN, END for others \n");
		//exit(0);
		}
	}*/
	
	//printf("\n type %d %d --> %d",EXTRACT_TYPE(aPacket->ver_type_piggyback), node, nodeID[node].parent);
	//to do: case to be handled where destinations of both transmissions are each other // hack
	
	for(index = 0; index < nodeID[node].numNeighbors; index++)
	{
		if(nodeID[nodeID[node].neighborList[index]].packetBeingTransmitted == TRUE)
		{
			if(nodeID[node].neighborList[index] == nodeID[node].parent)
			{
				//hack 12-4-10
				/*if(node == 61 && simTime > 2041186)
				{
				fprintf(stderr,"\n %d node parent %d Parent is transmitting packet \n",node,nodeID[node].parent);
				exit(0);
				}*/
				return SUCCESS;
			}

				//hack 12-4-10
				/*if(node == 61 && simTime > 2041186)
				{
				fprintf(stderr,"\n %d node neighbor %d is transmitting packet \n",node,nodeID[node].neighborList[index]);
				exit(0);
				}*/
			
		}
		else
		{		
				

			if(nodeID[node].neighborList[index] != nodeID[node].parent) //why this if condition, coz sending explicitly for parent below
			{
				
				if(!IsPacketToBeDropped(errorRate))
				{
					anEvent = getContentionReceptionEvent(simTime, nodeID[node].neighborList[index], RX_BEGIN); //but why contention reception with respect to the neighbor?? its for transmitting from this node to its neighbor
					insertEvent(anEvent);
						
					anEvent = getContentionReceptionEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), nodeID[node].neighborList[index], RX_END);
					insertEvent(anEvent);

					nodeID[node].packetBeingTransmitted = TRUE;					
					anEvent = getStopTransmissionEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node);
					insertEvent(anEvent);

					//hack 12-4-10
				/*if(node == 61 && simTime == 2041209)
				{
				fprintf(stderr,"\n Reached in the else part, neighbor is %d, time is %lf \n", nodeID[node].neighborList[index], simTime);
				//exit(0);
				}*/

				}
			}

		}
	}
				
	backoff = 0;
	
	//check whether it is possible to transmit control packet in remaining time of contention slot
	if((double)(CONTROL_SLOT_DURATION * NO_OF_CONTROL_SLOTS + CONTENTION_SLOT_DURATION * NO_OF_CONTENTION_SLOTS) - fmod((simTime + backoff + PACKET_TX_TIME), FRAME_DURATION) >= 0)
	{

		//depending on packet type, insert next event
		type = EXTRACT_TYPE(aPacket->ver_type_piggyback);
		if(type == JOIN_REQUEST)
		{
			//forward the join request			
			if(!IsPacketToBeDropped(errorRate))
			{
				anEvent = getJoinRequestEvent(simTime + backoff, ((struct nodeJoin *)aPacket->payloadPacket)->node, node, nodeID[node].parent, RX_BEGIN);	
				insertEvent(anEvent);			

				nodeID[node].packetBeingTransmitted = TRUE;
				
				sizeofPacket = extractSizeof(aPacket);
				anEvent = getJoinRequestEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE), ((struct nodeJoin *)aPacket->payloadPacket)->node, node, nodeID[node].parent, RX_END);	
				insertEvent(anEvent);
			}
			else
			{
				//printf("\t join request packet is to be dropped node %d",node);
				nodeStat[node].numChannelControlPacketLosses++;
			}
			
			nodeID[node].expectingLinkAck = TRUE;
			
			anEvent = getAckTimeoutEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE) +(double)ACK_TIMEOUT, node);
			insertEvent(anEvent);
		}
		//portability changes start
		if(type == TOPOLOGY_UPDATE)
		{
			
			//if(!IsPacketToBeDropped(errorRate))
			//{
				double temp, temp1, temp2;

				anEvent = getTopologyUpdateEvent(simTime + backoff, ((struct topologyUpdate *)aPacket->payloadPacket)->node, node, nodeID[node].parent, RX_BEGIN, ((struct topologyUpdate *)aPacket->payloadPacket)->type);	
				insertEvent(anEvent);

				nodeID[node].packetBeingTransmitted = TRUE;
				
				sizeofPacket = extractSizeof(aPacket);

				temp = (double)(simTime + backoff);
				temp1 = (double)(sizeofPacket*8)/(double)DATA_RATE;
				//temp2 = (double)(temp + temp1);
				anEvent = getTopologyUpdateEvent((double)(simTime + backoff + temp1), ((struct topologyUpdate *)aPacket->payloadPacket)->node, node, nodeID[node].parent, RX_END, ((struct topologyUpdate *)aPacket->payloadPacket)->type);	
				insertEvent(anEvent);
				
				// + ((double)sizeofPacket*8/(double)DATA_RATE)(double)0.448
				
				nodeID[node].numOfTopologyUpdatesSent++;

				//hack 12-4-10
				/*if(node == 61 && simTime > 2041186)
				{
				fprintf(stderr,"\n Time for RX_END is %lf , %lf  sizeofPacket %lf, data_rate %lf  time %lf,  mult %lf, second time %lf \n ", temp + temp1, temp + temp1, (double)sizeofPacket, (double)DATA_RATE,temp + temp1, (double)((double)sizeofPacket*(double)8),  temp + temp1);
				exit(0);
				}*/

				
				//fprintf(stderr,"In handlecontentionslot, inserted RX_END event for topology update for node %d and parent %d \n",node, nodeID[node].parent);
			
			//}
			//else
			//{
				//printf("\t join request packet is to be dropped node %d",node);
				//nodeStat[node].numChannelControlPacketLosses++;
			//}
			
			nodeID[node].expectingLinkAck = TRUE;
			
			anEvent = getAckTimeoutEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE) +(double)ACK_TIMEOUT, node);
			insertEvent(anEvent);
		}
		
		//portability changes end
		
		if(type == FLOW_REQUEST)
		{
			if(!IsPacketToBeDropped(errorRate))
			{
				fprintf(dump,"Inside handleContentionSlot for node %d\n",node);
				anEvent = getFlowRequestEvent(simTime + backoff, ((struct flowRequest *)aPacket->payloadPacket)->e2eSource, ((struct flowRequest *)aPacket->payloadPacket)->e2eDestination, ((struct flowRequest *)aPacket->payloadPacket)->flowId_bwRequest, node, nodeID[node].parent, RX_BEGIN);	
				insertEvent(anEvent);
				nodeID[node].packetBeingTransmitted = TRUE;
				sizeofPacket = extractSizeof(aPacket);
				anEvent = getFlowRequestEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE), ((struct flowRequest *)aPacket->payloadPacket)->e2eSource, ((struct flowRequest *)aPacket->payloadPacket)->e2eDestination, ((struct flowRequest *)aPacket->payloadPacket)->flowId_bwRequest, node, nodeID[node].parent, RX_END);	
				insertEvent(anEvent);
			}
			else
			{
				//printf("\t flow request packet is to be dropped node %d",node);
				nodeStat[node].numChannelControlPacketLosses++;
			}	
			nodeID[node].expectingLinkAck = TRUE;
			anEvent = getAckTimeoutEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE) + (double)ACK_TIMEOUT, node);
			insertEvent(anEvent);			

		}
		if(type == BW_REQUEST)
		{
			//*********************************************			
			for(l=0; l<numOfFlowsInProgress; l++)
			{
					
				if(flowStat[l].flowId == EXTRACT_FLOWID(((struct bwRequest *)aPacket->payloadPacket)->flowId_bwRequest))
				{
					flowSource = flowStat[l].source;
					break;
				}	
			}	
			
			if(l==numOfFlowsInProgress)
			{
				//fprintf(stderr,"\n %d node Bandwidth request, flowId %d received did not match to any flow", node, EXTRACT_FLOWID(((struct bwRequest *)aPacket->payloadPacket)->flowId_bwRequest));
				
				removeFromQueue(node);
				//exit(0);
				return SUCCESS;
			}
			
			if(flowSource == node && nodeID[node].flowTearedDown == TRUE)
			{				
				removeFromQueue(node);
				return SUCCESS;
			}									
			
			//*******************************************
			
			if(!IsPacketToBeDropped(errorRate))
			{
				anEvent = getBandwidthRequestEvent(simTime + backoff, ((struct bwRequest *)aPacket->payloadPacket)->flowId_bwRequest, node, nodeID[node].parent, RX_BEGIN);	
				insertEvent(anEvent);
				nodeID[node].packetBeingTransmitted = TRUE;
				sizeofPacket = extractSizeof(aPacket);
				anEvent = getBandwidthRequestEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE),((struct bwRequest *)aPacket->payloadPacket)->flowId_bwRequest, node, nodeID[node].parent, RX_END);	
				insertEvent(anEvent);
			}
			else
			{
				//printf("\t bw request packet is to be dropped node %d",node);
				nodeStat[node].numChannelControlPacketLosses++;
			}	
			nodeID[node].expectingLinkAck = TRUE;
			anEvent = getAckTimeoutEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE) + (double)ACK_TIMEOUT, node);
			insertEvent(anEvent);			

		}
		if(type == TEAR_DOWN)
		{
			
			if(!IsPacketToBeDropped(errorRate))
			{
				anEvent = getTearDownEvent(simTime + backoff, ((struct tearDown *)aPacket->payloadPacket)->flowId, node, nodeID[node].parent, RX_BEGIN);	
				insertEvent(anEvent);
				nodeID[node].packetBeingTransmitted = TRUE;
				sizeofPacket = extractSizeof(aPacket);
				anEvent = getTearDownEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE), ((struct tearDown *)aPacket->payloadPacket)->flowId, node, nodeID[node].parent, RX_END);	
				insertEvent(anEvent);
			}
			else
			{
				//printf("\t tear down packet is to be dropped node %d",node);
				nodeStat[node].numChannelControlPacketLosses++;
			}
			nodeID[node].expectingLinkAck = TRUE;
			anEvent = getAckTimeoutEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE) + (double)ACK_TIMEOUT, node);
			insertEvent(anEvent);
		}

		//manan change
		if(type == VOICE_FLOW_REQUEST)
		{	int src, e2edst;
			//vm=fopen("voiceMsg.txt","a+");
			
			src = ((struct voiceFlowRequest *)aPacket->payloadPacket)->e2eSource;
			e2edst = ((struct voiceFlowRequest *)aPacket->payloadPacket)->overalle2eDestination;
			if(!IsPacketToBeDropped(errorRate))
			{
				anEvent = getVoiceFlowRequestEvent(simTime + backoff, ((struct voiceFlowRequest *)aPacket->payloadPacket)->e2eSource, ((struct voiceFlowRequest *)aPacket->payloadPacket)->flowId_bwRequest, node, nodeID[node].parent, ((struct voiceFlowRequest *)aPacket->payloadPacket)->overalle2eDestination, ((struct voiceFlowRequest *)aPacket->payloadPacket)->voiceMsgDuration, RX_BEGIN);	
				insertEvent(anEvent);
				nodeID[node].packetBeingTransmitted = TRUE;
				sizeofPacket = extractSizeof(aPacket);
				anEvent = getVoiceFlowRequestEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE), ((struct voiceFlowRequest *)aPacket->payloadPacket)->e2eSource, ((struct voiceFlowRequest *)aPacket->payloadPacket)->flowId_bwRequest, node, nodeID[node].parent, ((struct voiceFlowRequest *)aPacket->payloadPacket)->overalle2eDestination, ((struct voiceFlowRequest *)aPacket->payloadPacket)->voiceMsgDuration, RX_END);	
				insertEvent(anEvent);
			}
			else
			{
				//printf("\t flow request packet is to be dropped node %d",node);
				nodeStat[node].numChannelControlPacketLosses++;
			}	
			nodeID[node].expectingLinkAck = TRUE;
			anEvent = getAckTimeoutEvent(simTime + backoff + ((double)sizeofPacket*8/(double)DATA_RATE) + (double)ACK_TIMEOUT, node);
			insertEvent(anEvent);			

			fprintf(vm,"Reached here, in handlecontentionslot, inserted RX_END event in event queue\n");
			fprintf(vm,"Source %d e2eDestination %d \n",src, e2edst);
			//fclose(vm);
			//exit(0);
			
		}
		//manan end
	}
}

//**********************************************************************************************************************
//packet received in contention slot, hence schedule link level ack
//insert control packet in control queue of received node
int handleContentionPacketNode(int node, struct packetFormat * aPacket)
{
	fprintf(dump,"Inside handleContentionPacketNode for node %d\n",node);
	struct event * anEvent;
	int sizeofPacket;
	int type;
	//FILE *vm;

		//manan change
		type = EXTRACT_TYPE(aPacket->ver_type_piggyback);
		if(type == VOICE_FLOW_REQUEST)
		{
		fprintf(vm,"in handleContentionPacketNode for VOICE_FLOW_REQUEST\n");
		}
		//manan end
		
	if(!IsPacketToBeDropped(errorRate))
	{
		anEvent = getLinkLevelAckEvent(simTime, node, aPacket->source, RX_BEGIN);
		insertEvent(anEvent);
		nodeID[node].packetBeingTransmitted = TRUE;
		sizeofPacket = extractSizeof(anEvent->aPacket);
		anEvent = getLinkLevelAckEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node, aPacket->source, RX_END);
		insertEvent(anEvent);
	}

	if(type == TOPOLOGY_UPDATE && nodeID[node].type == INFRA)
	{

	nodeID[node].numOfTopologyUpdatesReceived++;
	//fprintf(stderr,"\n Node %d received update of %d at time %lf \n",node,((struct topologyUpdate *)aPacket->payloadPacket)->node,simTime );
			
	/*int i; // counter for mobile list
	int mobile;

	mobile = ((struct topologyUpdate *)aPacket->payloadPacket)->node;
	//fprintf(stderr,"\n Parent %d received update from %d \n",node,mobile);
		for(i=0; i<50; i++)
		{
			if(infra_topology_update[node].mobile_list[i].mobile_number == mobile)
			{
			infra_topology_update[node].mobile_list[i].nodeSoftState = (double)(INFRA_TOPOLOGY_UPDATE_REFRESH * SECOND);

			infra_topology_update[node].mobile_list[i].numTopologyUpdateReceived++;

			}
		}
	*/	
	}
	
	insertInQueue(node, aPacket);
	
	//packet may get transmitted in current contention slot towards root
	//sizeofPacket = LINK_LEVEL_ACK_SIZE; //extractSizeof(anEvent->aPacket);
	//anEvent = getHandleContentionSlotEvent(simTime + ((double)sizeofPacket*8/(double)DATA_RATE), node);
	//insertEvent(anEvent);

}

//**********************************************************************************************************************
//hanlde control packet queue
//remove packet from control queue when ack is received
int removeFromQueue(int node)
{
	if(nodeID[node].numOfPacketsInQueue == 0)
	{		
		nodeID[node].controlFront = 0;
		nodeID[node].controlRear = 0;
	}
	else
	{
		nodeID[node].controlFront = (nodeID[node].controlFront + 1) % QUEUE_LIMIT;
		
		nodeID[node].numOfPacketsInQueue--;
		
		if(nodeID[node].numOfPacketsInQueue <= 0)
		{
			nodeID[node].controlFront = 0;
			nodeID[node].controlRear = 0;
		}
	}

	//printf("\n contention queue size at node %d: %d removed",node,nodeID[node].numOfPacketsInQueue);	
}


int removeFromQueueAccidently(int node)
{
	struct packetFormat * aPacket;
	int type;
	
	
	if(nodeID[node].numOfPacketsInQueue == 0)
	{		
		nodeID[node].controlFront = 0;
		nodeID[node].controlRear = 0;
	}
	else
	{
		aPacket = nodeID[node].packetQueue[nodeID[node].controlFront];	
		type = EXTRACT_TYPE(aPacket->ver_type_piggyback);
		if(type == JOIN_REQUEST)
		{
		}
		else if(type == FLOW_REQUEST)
		{
			fprintf(stderr,"Flow request removed in contention slot of %d", node);
			exit(0);
		}
		else
		{
		}
		
		nodeID[node].controlFront = (nodeID[node].controlFront + 1) % QUEUE_LIMIT;
		
		nodeID[node].numOfPacketsInQueue--;
		
		if(nodeID[node].numOfPacketsInQueue <= 0)
		{
			nodeID[node].controlFront = 0;
			nodeID[node].controlRear = 0;
		}
	}

	//printf("\n contention queue size at node %d: %d removed",node,nodeID[node].numOfPacketsInQueue);	
}

int nodeSourceOfThisFlow(int node, int flowId)
{
	int j;
	for(j=0; j<numOfFlowsInProgress; j++)
	{
					
		if(flowStat[j].flowId == flowId && flowStat[j].source == node)
		{
			return 1;
		}	
	}
	
	return 0;
}


int setFlowSentAtTime(int flowId)
{
	int j;
	for(j=0; j<numOfFlowsInProgress; j++)
	{
					
		if(flowStat[j].flowId == flowId)
		{
			flowStat[j].flowSentAt = simTime;		
			return 0;
		}	
	}
	
	return 1;
}

//insert control packet in control queue
int insertInQueue(int node, struct packetFormat * aPacket)
{
	struct packetFormat * otherPacket, * morePacket;
	struct nodeJoin * aJoin;
	struct flowRequest * aFlowRequest;
	struct tearDown * aTearDown;
	struct bwRequest * aBwRequest;
	struct voiceFlowRequest * aVoiceFlowRequest;
	struct topologyUpdate * aTopologyUpdate;
	int type, i;
	
	//for join request
	//this copy is necessary since after every event the respective memory allocated is freed
	otherPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));
	otherPacket->ver_type_piggyback = aPacket->ver_type_piggyback;
	otherPacket->source = node;
	otherPacket->destination = nodeID[node].parent;
	
	type = EXTRACT_TYPE(aPacket->ver_type_piggyback);
	
	if(type == JOIN_REQUEST)
	{
		aJoin = (struct nodeJoin *)malloc(sizeof(struct nodeJoin));
		aJoin->node = ((struct nodeJoin *)aPacket->payloadPacket)->node;
 		otherPacket->payloadPacket = (uint8_t *)aJoin; 
 		
 	}

	//portability changes start
	if(type == TOPOLOGY_UPDATE && nodeID[node].state < JOINED)
	{
		return SUCCESS; //changed 5-4-10
	}
	
	if(type == TOPOLOGY_UPDATE)
	{
		aTopologyUpdate = (struct topologyUpdate *)malloc(sizeof(struct topologyUpdate));
		aTopologyUpdate->node = ((struct topologyUpdate *)aPacket->payloadPacket)->node;
		//terrible mistake corrected on 26-4-10
		aTopologyUpdate->type = ((struct topologyUpdate *)aPacket->payloadPacket)->type;
		for(i=0; i < MAX_INFRA_NEIGHBORS; i++)
		{
			aTopologyUpdate->neighbors[i] = ((struct topologyUpdate *)aPacket->payloadPacket)->neighbors[i];
		}
 		otherPacket->payloadPacket = (uint8_t *)aTopologyUpdate; 
 		
 	}	
	//portability changes end
 	
 	if(type == FLOW_REQUEST)
 	{
 		aFlowRequest = (struct flowRequest *)malloc(sizeof(struct flowRequest));
		aFlowRequest->e2eSource = ((struct flowRequest *)aPacket->payloadPacket)->e2eSource;
		aFlowRequest->e2eDestination = ((struct flowRequest *)aPacket->payloadPacket)->e2eDestination;
		aFlowRequest->flowId_bwRequest = ((struct flowRequest *)aPacket->payloadPacket)->flowId_bwRequest;		
 		otherPacket->payloadPacket = (uint8_t *)aFlowRequest; 
 		
 		if(nodeSourceOfThisFlow(node, EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest)))
 		{
			setFlowSentAtTime(EXTRACT_FLOWID(aFlowRequest->flowId_bwRequest)); 			
 		}
 		
 	}
 	if(type == BW_REQUEST)
 	{ 		
 		aBwRequest = (struct bwRequest *)malloc(sizeof(struct bwRequest));
		aBwRequest->flowId_bwRequest = ((struct bwRequest *)aPacket->payloadPacket)->flowId_bwRequest;
		otherPacket->payloadPacket = (uint8_t *)aBwRequest; 
 	}
 	if(type == TEAR_DOWN)
 	{
 		aTearDown = (struct tearDown *)malloc(sizeof(struct tearDown));
		aTearDown->flowId = ((struct tearDown *)aPacket->payloadPacket)->flowId;
		otherPacket->payloadPacket = (uint8_t *)aTearDown; 
 	}	
	//manan change
	if(type == VOICE_FLOW_REQUEST)
 	{
		//FILE *vm;
		//vm = fopen("voiceMsg.txt","a+");
 		aVoiceFlowRequest = (struct voiceFlowRequest *)malloc(sizeof(struct voiceFlowRequest));
		aVoiceFlowRequest->e2eSource = ((struct voiceFlowRequest *)aPacket->payloadPacket)->e2eSource;
		aVoiceFlowRequest->flowId_bwRequest = ((struct voiceFlowRequest *)aPacket->payloadPacket)->flowId_bwRequest;
		aVoiceFlowRequest->overalle2eDestination = ((struct voiceFlowRequest *)aPacket->payloadPacket)->overalle2eDestination;
		aVoiceFlowRequest->voiceMsgDuration = ((struct voiceFlowRequest *)aPacket->payloadPacket)->voiceMsgDuration;
		
 		otherPacket->payloadPacket = (uint8_t *)aVoiceFlowRequest; 
		
		fprintf(vm,"reached till here in TX insertInQueue, also called from RX_END\n");
		//fclose(vm);
		//exit(0);
 		
 	}
	//manan end
	
	//printf("\n contention queue size at node %d: %d to be added",node,nodeID[node].numOfPacketsInQueue);
		
	if(nodeID[node].numOfPacketsInQueue < QUEUE_LIMIT)
	{
		nodeID[node].packetQueue[nodeID[node].controlRear] = otherPacket;
		nodeID[node].controlRear = (nodeID[node].controlRear + 1) % QUEUE_LIMIT;
		nodeID[node].numOfPacketsInQueue++;
		/*if(EXTRACT_TYPE(otherPacket->ver_type_piggyback) == TOPOLOGY_UPDATE && node == 61) // hack 12-4-10
		{
		fprintf(stderr,"\n Topology update packet inserted in control queue of node %d , time %lf , packets in Queue %d \n",node, simTime, nodeID[node].numOfPacketsInQueue);
		//exit(0);
		}*/

		//portability changes
		if(nodeID[node].numOfPacketsInQueue > nodeID[node].maxQueueSize)
		{
			nodeID[node].maxQueueSize = nodeID[node].numOfPacketsInQueue;
		}
		
	}
	else
	{
		//fprintf(stderr,"\n Contention Queue full at node %d contentionProb %lf",node,nodeID[node].contentionProb);
		//printf("\n Contention Queue full at node %d contentionProb %lf",node,nodeID[node].contentionProb);
		
		//node disable change
		/*
		//VJ Start
		
		if(nodeID[nodeID[node].parent].state == DEAD)
		{
		//bypass; Since the parent is dead the topology update and the other requests will keep accumulating
		}
		else
		{
		exit(0); // for now
		}
		
		//VJ End
		
		*/

		if(type == FLOW_REQUEST || type == VOICE_FLOW_REQUEST)
		{
		nodeStat[node].numFlowRequestDrops++;
		//fprintf(stderr,"\n Flow request or voice flow request dropped ");
		}
		nodeStat[node].numQueueControlPacketDrops++;		
	}

}

//insert request packet in control queue
int handleRequestTX(int node, struct packetFormat * aPacket)
{
	insertInQueue(node, aPacket);
}

//***********************************************************************************************************

int handleLinkAck(int node)
{
	if(nodeID[node].expectingLinkAck == TRUE)
	{			
		//nodeID[node].backoff = 1;
		nodeID[node].contentionProb = (double)1;
		nodeID[node].expectingLinkAck = FALSE;
		
		removeFromQueue(node);				
	}
	//handleContentionSlot(node); // portability changes
}

//separate contention probability for each node
int handleLinkAckTimeout(int node)
{
	
	if(nodeID[node].expectingLinkAck == TRUE)
	{
		nodeID[node].expectingLinkAck = FALSE;
		
		//if(nodeID[node].backoff < BACKOFF_LIMIT)
			//nodeID[node].backoff++;
		
		nodeID[node].numOfAttempts++;
		if(nodeID[node].numOfAttempts == ATTEMPT_LIMIT)
		{
			nodeID[node].numOfAttempts = 0;
			nodeID[node].contentionProb = (double)1;
			
			/*
			fprintf(stderr,"\n FORCED REMOVAL FROM CONTENTION QUEUE %d",node);
			exit(0);
			*/
			
			removeFromQueueAccidently(node);	
			//fprintf(stderr,"reached here in handleLinkAckTimeout where no of attempt limit is breached \n");
			//exit(0);
		}	
		else
		{	
		//nodeID[node].contentionProb = (double)(nodeID[node].contentionProb/2);			
	
		//fprintf(stderr,"value of maxinfra nodes is %d \n",MAX_NO_INFRA_NODES);
		nodeID[node].contentionProb = (double)( (1/( (double)(MAX_NO_INFRA_NODES + 1) )) * (1 + maxTreeHops() - getNumOfHops(node)) );

		//fprintf(stderr,"Node %d maxtreehops %d getnumofhops %d contentionprob %lf \n",node, maxTreeHops(), getNumOfHops(node), nodeID[node].contentionProb);
		//exit(0); // for now

		}
	}
	else
	{
		nodeID[node].numOfAttempts = 0;
		nodeID[node].contentionProb = (double)1;
	}
	handleContentionSlot(node);
}

