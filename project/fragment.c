#include "header.h"

//returns size of packet based on packet type

int extractSizeof(struct packetFormat * aPacket)
{
	int type;
	
	type = EXTRACT_TYPE(aPacket->ver_type_piggyback);
	
	if(type == JOIN_REQUEST)
		return (sizeof(struct packetFormat) + sizeof(struct nodeJoin));
	if(type == FLOW_REQUEST)
		return (sizeof(struct packetFormat) + sizeof(struct flowRequest));
	if(type == BW_REQUEST)
		return (sizeof(struct packetFormat) + sizeof(struct bwRequest));
	if(type == TEAR_DOWN)
		return (sizeof(struct packetFormat)+sizeof(struct tearDown));						
	if(type == LINK_LEVEL_ACK)
		return (LINK_LEVEL_ACK_SIZE);	
	if(type == SCHEDULE)
		return (sizeof(struct packetFormat) + sizeof(struct schedule) + (EXTRACT_SCHEDELEM(((struct schedule *)aPacket->payloadPacket)->schedElem_frag_rTree_repeat) * sizeof(struct scheduleElement))); 

	// so this long thing calculates the size of the numbr of scheduling elements times the size of each scheduling element
	if(type == DATA)
		return (sizeof(struct packetFormat)+sizeof(struct data)+DATA_PACKET_SIZE);
	
	//manan change
	if(type == VOICE_FLOW_REQUEST)
		return (sizeof(struct packetFormat) + sizeof(struct voiceFlowRequest));
	//manan end

	//portability changes
	if(type == TOPOLOGY_UPDATE)
		return (sizeof(struct packetFormat)+sizeof(struct topologyUpdate));	
	//portability changes end
}

//returns number of scheduling elements that can fit in a slot
int getSplitSchedulingElements()
{
	int sizeofPacket;
	int currentSchedElem;
	
	sizeofPacket = sizeof(struct packetFormat) + sizeof(struct schedule);
	currentSchedElem = 0;
	
	while(sizeofPacket + sizeof(struct scheduleElement) <= MAX_PACKET_SIZE)
	{
		sizeofPacket += sizeof(struct scheduleElement);
		currentSchedElem++;
	}

	return currentSchedElem;
}

int pendingTasks()
{
	int i;
	if(errorRate < (double) 0.002)
	{
		percentageCallDrop = (double)0.0;
	}
	else if(errorRate < (double) 0.005)
	{
		percentageCallDrop = (double)0.01;
	}
	else if(errorRate < (double) 0.01)
	{
		percentageCallDrop = (double)0.05;
	}
	else if(errorRate < 0.015)
	{
		percentageCallDrop = (double)0.12;
	}
	else if(errorRate < 0.02)
	{
		percentageCallDrop = (double)0.20;
	}
	else if(errorRate < 0.025)
	{
		percentageCallDrop = (double)0.30;
	}
	else if(errorRate < 0.03)
	{
		percentageCallDrop = (double)0.40;
	}
	else if(errorRate < 0.035)
	{
		percentageCallDrop = (double)0.50;
	}
	else if(errorRate < 0.040)
	{
		percentageCallDrop = (double)0.60;
	}
	else if(errorRate < 0.050)
	{
		percentageCallDrop = (double)0.80;
	}
	else
	{
		percentageCallDrop = (double)1.00;
	}

	noOfSuccessCalls = 0;
	for(i=0;i<noOfCallsOriginated; i++)
	{
		if(numSuccessCalls[i+1] == 0)
		{
			//fprintf(stderr,"\n %d call failed",i+1);
		}	
		noOfSuccessCalls += numSuccessCalls[i+1];
	}
	
	//fprintf(stderr,"\n Success calls %d",noOfSuccessCalls);	
	flowDropDueToE2EPacketLosses = (double)noOfSuccessCalls * percentageCallDrop;
	//fprintf(stderr,"\n calls dropped %d\n", flowDropDueToE2EPacketLosses);
	
	/*
	noOfSuccessCalls = 100;
	fprintf(stderr,"\n Success calls %d",noOfSuccessCalls);	
	flowDropDueToE2EPacketLosses = (double)noOfSuccessCalls * percentageCallDrop;
	fprintf(stderr,"\n calls dropped %d\n", flowDropDueToE2EPacketLosses);
	*/
}

//given number of elements, this function returns number of fragments of a schedule
//this is a generic function where number of scheduling elements and number of nodes in tree are input either as individuals or as a sum
int getNumOfFragments(int numofSchedElem)
{
	int packetSize;
	int fragments = 1;
	
	packetSize = sizeof(struct packetFormat) + sizeof(struct schedule);
	
	while(numofSchedElem) //while elements are not zero
	{
		if(packetSize + sizeof(struct scheduleElement) <= MAX_PACKET_SIZE)
		{
			packetSize += sizeof(struct scheduleElement); //go on adding one scheduling element at a time
		}
		else
		{
			packetSize = sizeof(struct packetFormat) + sizeof(struct schedule);
			fragments++;
		}
		numofSchedElem--;
	}
	return fragments;
}

