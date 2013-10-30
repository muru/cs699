#include "header.h"

//portability change 
void getPosition(int node, int *x, int *y, double time)
{
	int i,j,k;
	int mobile_moving; // boolean value to check whether the mobile is moving or not
	int direction;	
	double start_time;
	double time_diff, temp_sec;
	int seconds_elapsed, floor_ceil;
	int new_x, new_y;
	
	mobile_moving = FALSE;
	for(i=0; i < MAX_MOVING_NODES; i++)
	{
		if(moving_nodes[i].mobile_node == node)
		{
		mobile_moving = TRUE;
		direction = moving_nodes[i].direction;
		start_time = moving_nodes[i].started_at;
		break;
		}
	}

	if(mobile_moving == TRUE)
	{
	//the mobile is on move, need to calculate the current position
	//fprintf(stderr,"The condition of mobile moving is true for mobile %d, its in direction %d, it started moving at %lf \n", node, direction, start_time);
	
	time_diff = simTime - start_time;
	seconds_elapsed = (int) (time_diff/1000);
	//floor_ceil = fmod((temp_sec * 10),10); 
	//if less than 0.5 seconds, then floor else ceiling
	
		switch(direction)
		{
			case 1:
				new_x = (uint16_t) nodeLocation[node].x;
				new_y = (uint16_t) (nodeLocation[node].y + (seconds_elapsed * PEDESTRIAN_SPEED));
				*x = new_x;
				*y = new_y;	
				break;
			case 2:
				new_x = (uint16_t) (nodeLocation[node].x + (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				new_y = (uint16_t) (nodeLocation[node].y + (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				*x = new_x;
				*y = new_y;	
				break;
			case 3:
				new_x = (uint16_t) (nodeLocation[node].x + (seconds_elapsed * PEDESTRIAN_SPEED));
				new_y = (uint16_t) nodeLocation[node].y;
				*x = new_x;
				*y = new_y;	
				break;
			case 4:
				new_x = (uint16_t) (nodeLocation[node].x + (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				new_y = (uint16_t) (nodeLocation[node].y - (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				*x = new_x;
				*y = new_y;	
				break;
			case 5:
				new_x = (uint16_t) nodeLocation[node].x;
				new_y = (uint16_t) (nodeLocation[node].y - (seconds_elapsed * PEDESTRIAN_SPEED));
				*x = new_x;
				*y = new_y;	
				break;
			case 6:
				new_x = (uint16_t) (nodeLocation[node].x - (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				new_y = (uint16_t) (nodeLocation[node].y - (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				*x = new_x;
				*y = new_y;	
				break;
			case 7:
				new_x = (uint16_t) (nodeLocation[node].x - (seconds_elapsed * PEDESTRIAN_SPEED));
				new_y = (uint16_t) nodeLocation[node].y;
				*x = new_x;
				*y = new_y;	
				//fprintf(stderr,"X coord is %d , simTime %lf \n", *x, simTime);
				//fprintf(stderr,"Y coord is %d \n", *y);
				//hack
					/*if((fmod(seconds_elapsed,20) == 0) && seconds_elapsed > 0)
					{
					exit(0);
					}*/
				break;
			case 8:
				new_x = (uint16_t) (nodeLocation[node].x - (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				new_y = (uint16_t) (nodeLocation[node].y + (seconds_elapsed * (PEDESTRIAN_SPEED/sqrt(2))));
				*x = new_x;
				*y = new_y;	
				break;
		}
		
	//exit(0);	
	
	}
	else if(mobile_moving == FALSE)
	{
	*x = nodeLocation[node].x;
	*y = nodeLocation[node].y;
	}
}


int getRSSI(int source, int destination, double time)
{
	double distance;
	int src_x, src_y, dst_x, dst_y;

	getPosition(source, &src_x, &src_y, time);
	getPosition(destination, &dst_x, &dst_y, time);

	if(source == 2 && simTime > 600000) // mobility hack
	{
	//fprintf(stderr,"Source %d x_coord %d ycoord %d \n", source, src_x, src_y);
	//fprintf(stderr,"Destination %d x_coord %d ycoord %d \n", destination, dst_x, dst_y);
	}
	distance = (double)sqrt(pow(src_x - dst_x, 2) + pow(src_y - dst_y, 2));

	if(distance >= 0 && distance < 5)
	return -53;
	else if(distance >= 5 && distance < 10)
	return -57;
	else if(distance >= 10 && distance < 15)
	return -60;
	else if(distance >= 15 && distance < 20)
	return -63;
	else if(distance >= 20 && distance < 25)
	return -65;
	else if(distance >= 25 && distance < 30)
	return -67;
	else if(distance >= 30 && distance < 35)
	return -68;
	else if(distance >= 35 && distance < 40)
	return -69;
	else if(distance >= 40 && distance < 45)
	return -70;
	else if(distance >= 45 && distance < 50)
	return -72;
	else if(distance >= 50 && distance < 55)
	return -73;
	else if(distance >= 55 && distance < 65)
	return -74;
	//else if(distance >= 60 && distance < 65)
	//return 74;
	else if(distance >= 65 && distance < 75)
	return -75;
	//else if(distance >= 70 && distance < 75)
	//return 75;
	else if(distance >= 75 && distance < 85)
	return -76;
	//else if(distance >= 80 && distance < 85)
	//return 76;
	else if(distance >= 85 && distance < 95)
	return -77;
	//else if(distance >= 90 && distance < 95)
	//return 77;
	else if(distance >= 95 && distance < 105)
	return -78;
	//else if(distance >= 100 && distance < 105)
	//return 78;
	else if(distance >= 105 && distance < 120)
	return -79;
	/*else if(distance >= 110 && distance < 115)
	return 79;
	else if(distance >= 115 && distance < 120)
	return 79;*/
	else if(distance >= 120 && distance < 135)
	return -80;
	/*else if(distance >= 125 && distance < 130)
	return 80;
	else if(distance >= 130 && distance < 135)
	return 80;*/
	else if(distance >= 135 && distance < 155)
	return -81;
	/*else if(distance >= 140 && distance < 145)
	return 81;
	else if(distance >= 145 && distance < 150)
	return 81;
	else if(distance >= 150 && distance < 155)
	return 81;*/
	else if(distance >= 155 && distance < 170)
	return -82;
	/*else if(distance >= 160 && distance < 165)
	return 82;
	else if(distance >= 165 && distance < 170)
	return 82;*/
	else if(distance >= 170 && distance < 185)
	return -83;
	/*else if(distance >= 175 && distance < 180)
	return 83;
	else if(distance >= 180 && distance < 185)
	return 83;*/
	else if(distance >= 185 && distance < 205)
	return -84;
	/*else if(distance >= 190 && distance < 195)
	return 84;
	else if(distance >= 195 && distance < 200)
	return 84;
	else if(distance >= 200 && distance < 205)
	return 84;*/
	else if(distance >= 205 && distance < 225)
	return -85;
	/*else if(distance >= 210 && distance < 215)
	return 85;
	else if(distance >= 215 && distance < 220)
	return 85;
	else if(distance >= 220 && distance < 225)
	return 85;*/
	else if(distance >= 225 && distance < 245)
	return -86;
	/*else if(distance >= 230 && distance < 235)
	return 86;
	else if(distance >= 235 && distance < 240)
	return 86;
	else if(distance >= 240 && distance < 245)
	return 86;*/
	else if(distance >= 245 && distance < 255)
	return -87;
	//else if(distance >= 250 && distance < 255)
	//return 87;
	else if(distance >= 255 && distance < 275)
	return -88;
	/*else if(distance >= 260 && distance < 265)
	return 88;
	else if(distance >= 265 && distance < 270)
	return 88;
	else if(distance >= 270 && distance < 275)
	return 88;*/
	else if(distance >= 275 && distance < 295)
	return -89;
	/*else if(distance >= 280 && distance < 285)
	return 89;
	else if(distance >= 285 && distance < 290)
	return 89;
	else if(distance >= 290 && distance < 295)
	return 89;*/
	else if(distance >= 295 && distance < 300)
	return -90;
	else 
	return -95;
	
}


struct schedule * getSchedulePacket(double time, int node)
{
	struct schedule * aSchedule;
	aSchedule = (struct schedule *)malloc(sizeof(struct schedule));
	
	aSchedule->timestamp = time;
	aSchedule->validFrom = nodeID[node].scheduleValidFrom;
	aSchedule->fragmentNo = nodeID[node].broadcastSchedFragmentNo;
	
	//appropriate schedElem
	
	aSchedule->schedElem_frag_rTree_repeat = SCHEDULE_MACRO(nodeID[node].currSchedElem, nodeID[node].moreFragment, nodeID[node].treeUpdate, nodeID[node].schedRepeat);

	return aSchedule;
}

//returns schedule event for root as well as for infrastructure nodes
struct event * getScheduleEvent(double time, int tx, int rx, enum eventType type)
{
	struct schedule * aSchedule;
	struct packetFormat * aPacket;
	struct event * anEvent;


	aSchedule = getSchedulePacket(time, tx);
	
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,SCHEDULE,NO_PIGGYBACK);
	aPacket->source = tx;
	aPacket->destination = BROADCAST;
	aPacket->payloadPacket = (uint8_t *)aSchedule;
	
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = rx;
	anEvent->type = type;
	//mobility change
	if(type == TX)
	aPacket->rssi = 0;
	else
	{
	aPacket->rssi = getRSSI(tx, rx, time);
	//if(tx == 2 && simTime > 600000) // mobility hack
	//fprintf(stderr,"Src %d Dst %d Rssi %d \n", tx, rx, aPacket->rssi);
	}

	anEvent->aPacket = aPacket;
	anEvent->next = NULL;

	return anEvent;
}

//***********************************************************************************************************************
//Switch state of node from deat to alive
struct event * getAliveEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = MAKE_ALIVE;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}

//switch state of node from alive to dead
struct event * getDeadEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = MAKE_DEAD;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}
//***********************************************************************************************************************
//Link level ack events
struct event * getLinkLevelAckEvent(double time, int node, int destination, int type)
{
	struct event * anEvent;
	struct packetFormat * aPacket;

	anEvent = (struct event *)malloc(sizeof(struct event));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,LINK_LEVEL_ACK,NO_PIGGYBACK);
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = NULL;

	anEvent->time = time;
	anEvent->node = destination;
	anEvent->type = type;
	if(type == RX_END)
	{
	aPacket->rssi = getRSSI(node, destination, time);
	//fprintf(stderr,"Link Level ack Node %d Destination %d Rssi %d \n", node, destination, aPacket->rssi);
	}
	else
	{
	aPacket->rssi = 0;
	}

	anEvent->aPacket = aPacket;
	anEvent->next = NULL;

	return anEvent;
}


struct event * getAckTimeoutEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = LINK_ACK_TIMEOUT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}
//***********************************************************************************************************************
//join request events
//returns join request event, changes link level destination based on type
struct event * getJoinRequestEvent(double time, int joinSource, int node, int destination, enum eventType type)
{
	struct nodeJoin * aJoinRequest;
	struct packetFormat * aPacket;
	struct event * anEvent;
	
	aJoinRequest = (struct nodeJoin *)malloc(sizeof(struct nodeJoin));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aJoinRequest->node = joinSource;
	//aJoinRequest->neighborRSSI = NULL; //ignore RSSI

	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,JOIN_REQUEST,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aJoinRequest;

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Join request for %d : Node %d Destination %d Rssi %d \n", joinSource, node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;
	
	return anEvent;
}

//portability changes start *********************************************
struct event * getTopologyUpdateEvent(double time, int source, int node, int destination, enum eventType type, int update_type)
{
	struct topologyUpdate * aTopologyUpdate;
	struct packetFormat * aPacket;
	struct event * anEvent;
	
	aTopologyUpdate = (struct topologyUpdate *)malloc(sizeof(struct topologyUpdate));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aTopologyUpdate->node = source;	
	aTopologyUpdate->type = update_type;

	if(update_type != REGULAR && update_type != REACTIVE)  // this is a check hack
	{
		//fprintf(stderr,"handoff for %d , type %d \n", source, update_type);
	}

	//calculate the information to be sent here take care to put -1 information for the remaining neighbors
	//cannot put -1, its unsigned integer 8 bit. So instead it will be max infra nodes +1
	int i,j,k;
	i=0; // for how many neighbors filled
	for(j=0; j < MAX_NO_INFRA_NODES; j++)
	{
		if(nodeID[source].infra_list[j].is_neighbor == TRUE)
		{
		aTopologyUpdate->neighbors[i].infra_node = nodeID[source].infra_list[j].infra_node;
		aTopologyUpdate->neighbors[i].rssi = (int) nodeID[source].infra_list[j].average;
		i++;
		//fprintf(stderr,"Node %d included in neighborhood");
		}
	}
	for(;i < MAX_INFRA_NEIGHBORS; i++)
	{
		aTopologyUpdate->neighbors[i].infra_node = MAX_NO_INFRA_NODES + 1;
		aTopologyUpdate->neighbors[i].rssi = 0;
	}

	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,TOPOLOGY_UPDATE,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aTopologyUpdate;

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
				//hack 12-4-10
				/*if(node == 61 && simTime > 2041186)
				{
				fprintf(stderr,"\n Time for type %d is %lf \n ", type, time);
				//exit(0);
				}*/

	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}	
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Topology update for %d : Node %d Destination %d Rssi %d \n", source, node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;
	
	return anEvent;
}

//portability changes
struct event * getVoiceMsgRelayEvent(int node, double time)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = VOICE_MSG_RELAY;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
	
}


struct event * getNodeDisablingEvent(double time)
{

	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = ROOT; // It is not supposed to be root, it is actually NULL. 
	anEvent->type = NODE_DISABLE;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
	
}

//portability changes end***************************************************

//mobility change 17-4-10
struct event * getMobilityEvent(double time)
{

	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = ROOT; // It is not supposed to be root, it is actually NULL. 
	anEvent->type = MOBILITY;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;	
}

//mobility change 17-4-10
struct event * getStartMobilityEvent(double time, int mobile_node, int direction)
{

	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = mobile_node; 
	anEvent->type = START_MOBILE;
	anEvent->next = NULL;

	//need to add information to moving_nodes, moving this to handle priority queue as well 
	struct mobile_direction * aMobile_Direction;
	struct packetFormat * aPacket;
	
	aMobile_Direction = (struct mobile_direction *)malloc(sizeof(struct mobile_direction));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aMobile_Direction->direction = (uint8_t) direction;
	
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,MOBILE_DIRECTION,NO_PIGGYBACK);	
	aPacket->source = mobile_node;
	aPacket->destination = mobile_node;
	aPacket->payloadPacket = (uint8_t *)aMobile_Direction;

	anEvent->aPacket = aPacket;

	return anEvent;
}

struct event * getStopMobilityEvent(double time, int mobile_node)
{

	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = mobile_node; 
	anEvent->type = STOP_MOBILE;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	//need to delete information from moving_nodes, maybe we can move this to handlePriorityQueue. Doing that 
	
	return anEvent;
}

//sets join request timeout
struct event * getJoinRequestTimeoutEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = JOIN_REQUEST_TIMEOUT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}

//***********************************************************************************************************************
//flow request events
struct event * getFlowRequestTimeoutEvent(double time, int node)
{
	//fprintf(dump,"Inside getFlowRequestTimeoutEvent for node %d time %lf\n",node,time);
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = JOIN_REQUEST_TIMEOUT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}
//returns flow request event, changes link level destination based on type
struct event * getFlowRequestEvent(double time, int flowSource, int flowDestination, int flowId_bwRequest, int node, int destination, enum eventType type)
{
	//fprintf(dump,"Inside getFlowRequestEvent for flowSource %d flowDestination %d flowId_bwRequest %d current node %d node's parent %d event type %d\n",flowSource,flowDestination,flowId_bwRequest,node,destination,type);
	struct flowRequest * aFlowRequest;
	struct packetFormat * aPacket;
	struct event * anEvent;
	
	aFlowRequest = (struct flowRequest *)malloc(sizeof(struct flowRequest));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aFlowRequest->e2eSource = flowSource;
	aFlowRequest->e2eDestination = flowDestination;
	aFlowRequest->flowId_bwRequest = flowId_bwRequest;
		
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,FLOW_REQUEST,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aFlowRequest; // why is this an integer pointer? whys isnt it a pointer of type packetformat??

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Flow request for %d : Node %d Destination %d Rssi %d \n", flowSource, node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;
	
	return anEvent;
}

//manan change
//***********************************************************************************************************************

struct event * getVoiceFlowRequestEvent(double time, int flowSource, int flowId_bwRequest, int node, int destination, int e2eDest, double voiceMsgDuration, enum eventType type)
{
	struct  voiceFlowRequest * aFlowRequest;
	struct packetFormat * aPacket;
	struct event * anEvent;
	//FILE *vm;
	//vm=fopen("voiceMsg.txt","a+");
	
	aFlowRequest = (struct voiceFlowRequest *)malloc(sizeof(struct voiceFlowRequest));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aFlowRequest->e2eSource = flowSource;
	//aFlowRequest->e2eDestination = flowDestination;
	aFlowRequest->flowId_bwRequest = flowId_bwRequest;
	aFlowRequest->overalle2eDestination = e2eDest;
	aFlowRequest->voiceMsgDuration = voiceMsgDuration;
	//aFlowRequest->carryForward = carryForward;
		
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,VOICE_FLOW_REQUEST,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aFlowRequest; // why is this an integer pointer? whys isnt it a pointer of type packetformat??

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Voice flow request for %d : Node %d Destination %d Rssi %d \n", EXTRACT_FLOWID(flowId_bwRequest), node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;

	fprintf(vm,"\n getVoiceFlowRequestEvent function type %d by node %d \n",type, node);
	
	//fclose(vm);
	return anEvent;
	
}

//***********************************************************************************************************************
struct event * getVoiceMsgCheckEvent(double time)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));

	anEvent->time = time;
	anEvent->type = VOICE_MSG_CHECK;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;
	anEvent->node = ROOT;
}


struct event * getStorageCapacityEvent(double time)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));

	anEvent->time = time;
	anEvent->type = STORE_CAP;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;
	anEvent->node = ROOT;
}


//manan end

//***********************************************************************************************************************
struct event * getBandwidthRequestEvent(double time, int flowId_bwRequest, int node, int destination, enum eventType type)
{
	//fprintf(dump,"Inside getBandwidthRequestEvent for node %d\n",node);
	struct bwRequest * aBwRequest;
	struct packetFormat * aPacket;
	struct event * anEvent;
	
	aBwRequest = (struct bwRequest *)malloc(sizeof(struct bwRequest));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aBwRequest->flowId_bwRequest = flowId_bwRequest;
	
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,BW_REQUEST,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aBwRequest;

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Bw request for %d : Node %d Destination %d Rssi %d \n", EXTRACT_FLOWID(flowId_bwRequest), node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;
	
	return anEvent;
}

struct event * getBandwidthTimeoutEvent(double time, int flowId)
{
	struct event * anEvent;

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = flowId;
	anEvent->type = BW_REQUEST_TIMEOUT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;
	
	return anEvent;
}

//***********************************************************************************************************************
//lets the node know the start of contention slot. In contention slot, any pending contention packets in contention queue are sent using CSMA/CA or ALOHA
struct event * getContentionSlotEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = CONTENTION_SLOT_BEGINS;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}
//If neighbor is busy in sending, backoff of a while. This event will invoke the same function as contention slot event keeping backoff freezed to chosen value.
struct event * getBackoffContentionSlotEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = CONTENTION_SLOT_BACKOFF;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}
//***********************************************************************************************************************
//returns data generate event, changes link level destination based on type
struct event * getDataSourceSendEvent(double time, int node, int length, int seqNumber, int e2eSource, int e2eDestination, int flowId, int source, int destination, int slotNo, enum eventType type)
{
	struct data * aData;
	struct packetFormat * aPacket;
	struct event * anEvent;
	
	aData = (struct data *)malloc(sizeof(struct data));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aData->length = length;
	aData->seqNumber = seqNumber;
	aData->e2eSource = e2eSource;
	aData->e2eDestination = e2eDestination;
	aData->flowId = flowId;
	aData->slotNo = slotNo;
	
	if(flowId == 193)
	{
		//fprintf(stderr,"\n getEvent: e2eDestination of flow 193 %d", aData->e2eDestination);
	}
	
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,DATA,NO_PIGGYBACK);	
	aPacket->source = source;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aData;

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Data Packet Node %d Destination %d Rssi %d \n", node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;

	//fprintf(stderr,"getDataSourceSendEvent set with source %d destination %d time %lf event type %d simtime %lf \n", source, destination, time, type, simTime);	

	return anEvent;

}
//returns data send event, type is data_slot_transmit which signifies that this node is not data generator
//This event just looks into the data packet queue and selects proper packet for a slot.
struct event * getDataSendEvent(double time, int node, struct scheduleElement inputElement)
{
	struct event * anEvent;
	struct packetFormat * aPacket;
	struct scheduleElement * anElement;
		
	anElement = (struct scheduleElement *)malloc(sizeof(struct scheduleElement));	
	anElement->transmitter = inputElement.transmitter;
	anElement->receiver = inputElement.receiver;
	anElement->flowId_channel = inputElement.flowId_channel;
	anElement->slotToStart = inputElement.slotToStart;
	anElement->numSlots = inputElement.numSlots;

	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,SCHEDULE_ELEMENT,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = node;
	aPacket->payloadPacket = (uint8_t *)anElement;
	
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = DATA_SLOT_TRANSMIT;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;

	//fprintf(stderr,"getDataSendEvent set with transmitter %d receiver %d time %lf simtime %lf node %d :: Schedulevalidfrom %lf \n", inputElement.transmitter, inputElement.receiver, time, simTime, node, nodeID[node].scheduleValidFrom);

	return anEvent;
}
//***********************************************************************************************************************
//returns tear down event, changes link level destination based on type
struct event * getTearDownEvent(double time, int flowId, int node, int destination, enum eventType type)
{
	//manan change
	int i;
	for(i=0;i<numOfFlowsInProgress;i++)
	{
		if(flowStat[i].flowId == flowId && flowStat[i].voiceFlow == 1)
		{
		//FILE *vm;
		//vm = fopen("voiceMsg.txt","a+");
		fprintf(vm,"get tear down event called for voice flow id %d type %d\n",flowStat[i].flowId,type);
		//fclose(vm);
		}
	}
		
	//manan end

	struct tearDown * aTearDown;
	struct packetFormat * aPacket;
	struct event * anEvent;
	
	aTearDown = (struct tearDown *)malloc(sizeof(struct tearDown));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aTearDown->flowId = flowId;
	//aTearDown->voiceMsg = voiceMsg;
	
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,TEAR_DOWN,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = destination;
	aPacket->payloadPacket = (uint8_t *)aTearDown;

	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	if(type == TX)
	{
		anEvent->node = node;
		aPacket->rssi = 0;
	}
	else
	{
		anEvent->node = destination;
		aPacket->rssi = getRSSI(node, destination, time);
		//fprintf(stderr,"Tear Down request for %d : Node %d Destination %d Rssi %d \n", flowId, node, destination, aPacket->rssi);
	}	
	anEvent->type = type;
	anEvent->aPacket = aPacket;
	anEvent->next = NULL;
	
	return anEvent;
}
//***********************************************************************************************************************
struct event * getScheduleReceptionTimeout(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = KEEP_JOINED_TIMEOUT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}

struct event * getNextExpectedScheduleEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = SCHEDULE_RECEPTION_TIMEOUT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}

//mobility change 15-4-10
struct event * getNextExpectedFragmentEvent(double time, int node, int infra)
{
	//the packet created for this event is a dummy packet

	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = FRAGMENT_RECEPTION_TIMEOUT;

	struct expected_fragment * aExpected_Fragment;
	struct packetFormat * aPacket;
	
	aExpected_Fragment = (struct expected_fragment *)malloc(sizeof(struct expected_fragment));
	aPacket = (struct packetFormat *)malloc(sizeof(struct packetFormat));

	aExpected_Fragment->infra_node = (uint8_t) infra;
	
	aPacket->ver_type_piggyback = VER_TYPE_PIGGYBACK(VERSION,EXPECTED_FRAGMENT,NO_PIGGYBACK);	
	aPacket->source = node;
	aPacket->destination = node;
	aPacket->payloadPacket = (uint8_t *)aExpected_Fragment;

	anEvent->aPacket = aPacket;
	anEvent->next = NULL;
	
	//fprintf(stderr,"Reached here to insert event: Node %d time %lf simTime %lf \n", anEvent->node, anEvent->time, simTime);
	return anEvent;
}


struct event * getNewScheduleParamEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = START_NEW_SCHEDULE;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}
//***********************************************************************************************************************

struct event * getContentionReceptionEvent(double time, int node, enum eventType type)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	
	if(type == RX_BEGIN)
		anEvent->type = CONTENTION_RX_BEGIN;
	if(type == RX_END)
		anEvent->type = CONTENTION_RX_END;
			
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;

}

struct event * getDataReceptionEvent(double time, int node, enum eventType type)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	
	if(type == RX_BEGIN)
		anEvent->type = DATA_RX_BEGIN;
	if(type == RX_END)
		anEvent->type = DATA_RX_END;
			
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;

}
struct event * getStopTransmissionEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = STOP_TRANSMISSION;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;	
}

struct event * getHandleContentionSlotEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = HANDLE_CONTENTION_SLOT;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;
}

struct event * getCanStartFlowRequestEvent(double time, int node)
{
	fprintf(dump,"Node joined, inside getCanStartFlowRequestEvent for node %d\n",node);
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = CAN_START_FLOW;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;

	return anEvent;

}

struct event * getGenerateFlowRequestEvent(double time, int flowId)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = flowId;
	anEvent->type = GENERATE_FLOW_REQUEST;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;
	fprintf(dump,"NEXT_FLOW_REQUEST event generated for node %d\n",flowId);
	return anEvent;
}

struct event * getNextFlowRequestEvent(double time, int node)
{
	struct event * anEvent;
	anEvent = (struct event *)malloc(sizeof(struct event));
	anEvent->time = time;
	anEvent->node = node;
	anEvent->type = NEXT_FLOW_REQUEST;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;
	fprintf(dump,"NEXT_FLOW_REQUEST event generated for node %d\n",node);
	return anEvent;
}

//portability changes start************************************************
struct event * getTopologyCheckEvent(double time)
{
	struct event * anEvent;
	anEvent = (struct event*)malloc(sizeof(struct event));
	
	anEvent->time = time;
	anEvent->node = ROOT; 
	anEvent->type = TOPOLOGY_UPDATE_CHECK;
	anEvent->aPacket = NULL;
	anEvent->next = NULL;
	
	return anEvent;
}	
//portability changes end*********************************************************

//VJ Start: Following event is unnecessary
/*
struct event * getSetUpNewScheduleEvent(double time, int node)
{
	int i;
	struct event * anEvent;
	anEvent = (struct event*)malloc(sizeof(struct event));
	
	anEvent->time = time;
	anEvent->node = node; 
	anEvent->type = SET_UP_NEW_SCHEDULE;
	anEvent->next = NULL;
	
	if(node == ROOT)
	{
	//no updation required
	}
	else
	{
		for(i=0; i < nodeID[ROOT].numSchedElem; i++)
		{
		nodeID[node].newScheduleElements[i] = globalScheduleElements[i];
		}
	}

	nodeID[node].tempNumSchedElem = nodeID[ROOT].numSchedElem;
	
	for(i=0; i < nodeID[ROOT].numSchedElem; i++)
	{
	//fprintf(stderr,"Element %d : Tx %d Rx %d fc %d ss %d \n", i, globalScheduleElements[i].transmitter, globalScheduleElements[i].receiver, globalScheduleElements[i].flowId_channel, globalScheduleElements[i].slotToStart);
	}

	anEvent->aPacket = NULL;

	return anEvent;
}	
*/
//VJ End
