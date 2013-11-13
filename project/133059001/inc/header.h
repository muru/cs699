#ifndef __HEADER_H
#define __HEADER_H 1

#define PHASE_SCHEDULER 1

#define GENERATE_REQUESTS_FROM_FILE 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
//***********************************************************************************************************************
#define VERSION 1

#define SCHEDULE_MACRO(a,b,c,d) ((a << 3) | (b << 2) | (c << 1) | d) 
//constructs schedule contents

//so the schedule macro is of say, 16 bits. so the size of the scheduling element should be 13 bits since we are anding it with 0x01fff. oh yes got it, this is just for counting the number of scheduling elements present in the schedule

#define EXTRACT_SCHEDELEM(input) ((input >> 3) & 0x1fff)
#define EXTRACT_FRAG_BIT(input) ((input >> 2) & 0x0001)
#define EXTRACT_RTREE_BIT(input) ((input >> 1) & 0x0001)
#define EXTRACT_SCHEDREPEAT_BIT(input) (input & 0x0001)


#define VER_TYPE_PIGGYBACK(a,b,c) ( (a << 5) | (b << 1) | c ) 
//constructs packet contents
#define EXTRACT_VER(input) ((input >> 5) & 0x07)
#define EXTRACT_TYPE(input) ((input >> 1) & 0x0f)
#define EXTRACT_PIGGYBACK(input) (input & 0x01)

//ok, so ver_type_piggyback is of 8 bits. of which version is of 3 bits, type is of 4 bits and piggyback is of 1 bit

#define FLOWID_CHANNEL(a,b) ((a << 4) | b )
#define FLOWID_BW(a,b) ((a << 4) | b )
#define EXTRACT_FLOWID(input) ((input >> 4) & 0x0fff)
#define EXTRACT_CHANNEL(input) (input & 0x000f)
#define EXTRACT_BW(input) (input & 0x000f)

//by this means, i think flowid is of 12 bits in FLOWID_channel and channel is of 4 bits (right, there are 16 channels in 15.4)
//by this means even in extract_bw it should be anded with 0x000f

//general macros
#define PRESENT 1
#define ABSENT 0
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAILURE 1

#define PIGGYBACK 1 
//whether a packet is piggybacked or not
#define NO_PIGGYBACK 0

#define MAX_FLOW_RETRY 9

#define NEGATIVE_UNDEFINED (-1)

#define RAJESH_CONSTANT 64000

#define UNDEFINED 255
#define CONST_RSSI 88 
//not used in current implementation

#define QUEUE_LIMIT 32 
//ideally, data queue has queue limit = no of data slots, control queue stores contention type packets, currently both data packet and control queues are of same limit
#define MAX_QUEUE_EVENTS 5000 
//event structures are freed when done with, this is the limit on maximum events at a time
#define MAX_NO_NODES 150
#define MAX_NO_INFRA_NODES 25
#define MAX_NO_SCHEDULE_FRAGMENTS 30
#define ROOT 0 
#define BROADCAST 255 
//broadcast destination

#define MAX_NO_HOPS 15 
#define MAX_SIM_CALLS 100 
// maximum number of simultaneous calls
#define MAX_NO_FLOWS 12000
#define MAX_FLOW_ID 12000

//****************************************************
//configuration parameters
 
//in kbps, when you change this value, change the slot duration, max_backoff, ack_timeout and control slots accordingly
#define MAX_PACKET_SIZE 112   
// in bytes

#define PACKET_TX_TIME 2 //1
//maximum packet transmit time in milliseconds, used when taking transmission decision in contention slot, changed from 0.5 to 1 

#define ACK_TIMEOUT  0.4 //1.4 //0.7
//link level ack time out, depends on data rate

#define MAX_NO_CHANNELS 3 //3
#define DATA_RATE 250 //1000 //125 //(250)
#define CONTROL_SLOT_DURATION (4.5) //(1.125)//(1.5) //(13)//(4.5)
#define CONTENTION_SLOT_DURATION (3) //(0.75)//(1) //(11)//(3)
#define DATA_SLOT_DURATION (3) //(0.75)//(0.8) //(7)//(3)
#define NO_OF_SLOTS 19 //67 //92 //10 //19
#define NO_OF_CONTROL_SLOTS 2 //8 //1 //2
#define NO_OF_CONTENTION_SLOTS 1 //4 //1 //1
#define NO_OF_DATA_SLOTS (NO_OF_SLOTS-(NO_OF_CONTROL_SLOTS + NO_OF_CONTENTION_SLOTS))
#define FRAME_DURATION (NO_OF_DATA_SLOTS * DATA_SLOT_DURATION + NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION)

//60ms 	frame

//not used in current implementation
#define MAX_BACKOFF (0.2)//1 //0.5 
//specifies a window, use in conjuction with backoff variable, depends on data rate

//****************************************************
#define MAX_PHASES (NO_OF_DATA_SLOTS/4)
#define MAX_SCHED_ELEMENTS (MAX_NO_CHANNELS * NO_OF_DATA_SLOTS * MAX_NO_HOPS)

#define TREE_UPDATE_REPEAT 3

//not used in current implementation
#define BACKOFF_LIMIT 5 
//limit on node.backoff, note that backoff is not the part of current implementation

#define JOIN_WAIT_TIME (5 * FRAME_DURATION)
#define JOIN_TIMEOUT (5 * MAX_NO_INFRA_NODES * FRAME_DURATION * MAX_NO_SCHEDULE_FRAGMENTS) 
#define FLOW_TIMEOUT (5 * MAX_NO_INFRA_NODES * FRAME_DURATION * MAX_NO_SCHEDULE_FRAGMENTS)
#define SCHEDULE_MISS_THRESHOLD 10
//#define SCHEDULE_TIMEOUT (10 * MAX_NO_INFRA_NODES * FRAME_DURATION * MAX_NO_SCHEDULE_FRAGMENTS)
// node disable change, schedule timeout is one minute
#define SCHEDULE_TIMEOUT 60000
//#define MAX_SCHEDULE_SLOTS 40 //we can have at most 40 slots as a logical group, not part of current implementation

#define BW_INTERVAL (5*1000)
#define BW_TIMEOUT (MAX_FLOW_RETRY * BW_INTERVAL)

#define DATA_PACKET_SIZE 45//60 //45
#define LINK_LEVEL_ACK_SIZE 2 
// i presume this is in bytes

//portability changes start
#define SECOND 1000
#define TOPOLOGY_UPDATE_PERIOD (40)
#define ROOT_TOPOLOGY_UPDATE_REFRESH (5*TOPOLOGY_UPDATE_PERIOD)
#define ATTEMPT_LIMIT 20
//portability changes end

//mobility change 14-4-10
#define INFRA_RSSI -86
#define PARENT_RSSI -84
#define CONNECTIVITY_THRESHOLD -82
//this threshold will be used while making handoff decisions 
#define INFRA_HEARD_THRESHOLD 5 
#define MAX_INFRA_NEIGHBORS 10 
//#define FRAGMENT_HEARING(a,b) ((a << 5) | b ) // here 'a' will be the mobile node and 'b' will be the infra node
//#define EXTRACT_NODE(input) ((input >> 5) & 0x0fff) // this things need to be changed if the topology is changed to big grid
//#define EXTRACT_INFRA(input) (input & 0x001f) // this is a very important point. don't forget it. 
#define FRAGMENT_TIMEOUT 5000  
// 5 seconds = 5000 milliseconds
#define MAX_MOVING_NODES 10 
// maximum number of mobiles moving simultaneously
#define PEDESTRIAN_SPEED 1.5 
// speed in pedestrian mobility 
#define LENGTH 90 
// mobility length hardcoded at 90 meters 
#define REGULAR 1
#define REACTIVE 0
#define HANDOFF 2
#define MAX_DATA_HISTORY 30  
// number of data packets to be used for averaging, 25-4-10
#define HANDOFF_WAIT 15000
// wait for 15 seconds before sending another handoff request


//********Rajesh change******

//! specify sliding window size for call monitoring purpose
#define DATA_PACKET_SLIDING_WINDOW 100
//! maximum number of packet drops loss in sliding window allowed before dropping a flow
#define MAX_END_TO_END_DATA_PACKET_LOSS_THRESHOLD 10

extern int flowDropDueToE2EPacketLosses;

//********Rajesh change ends******

//***********************************************************************************************************************

int countNodesFlow;

struct packetFormat //general packet structure, the payload contains various MAC packets
{
	uint16_t ver_type_piggyback;
	uint16_t source;
	uint16_t destination;	
	int8_t rssi; // portability change
	uint8_t *payloadPacket;
};

struct schedule
{
	double timestamp;	//global time
	double validFrom;	//last schedule change   
	uint16_t fragmentNo;
	uint16_t schedElem_frag_rTree_repeat;	//schedule descriptor 	
	//schedule element structure to be added
};

struct scheduleElement
{
	uint16_t transmitter;
	uint16_t receiver;
	uint16_t flowId_channel;
	uint16_t slotToStart;
	uint16_t numSlots;
};

struct flowRequest
{
	uint16_t e2eSource;
	uint16_t e2eDestination;
	uint16_t flowId_bwRequest;
};

//manan change
struct voiceFlowRequest
{
	uint16_t e2eSource;
	//uint16_t e2eDestination;
	//uint16_t overalle2eSource;
	uint16_t overalle2eDestination;
	uint16_t flowId_bwRequest;
	double voiceMsgDuration;
	//uint16_t carryForward;
};
//manan end

struct bwRequest
{
	uint16_t flowId_bwRequest;
};

struct tearDown
{
	uint16_t flowId;
	//manan change
	//int voiceMsg;
	//manan end
};

struct nodeJoin
{
	uint16_t node;
	//uint16_t *neighborRSSI;
};

struct data
{
	uint16_t length;
	uint16_t seqNumber;		// Rajesh added for e2e packet drop based call drop	
	uint16_t e2eSource;
	uint16_t e2eDestination;
	uint16_t flowId;
	uint16_t slotNo; //for internal purpose
	uint16_t *payloadData;
	//manan change
	//uint16_t seq_no;
	//manan end
};

extern int uniqueCallsDropped[MAX_FLOW_ID];
extern int noOfCallsAdmitted_Unique_Calc;
extern int noOfSuccessCalls;

//************* VJ change start *************
struct nodePos
{
	uint16_t x;
	uint16_t y;
}nodeLocation[MAX_NO_NODES];

extern double transmissionRangeI,interferenceRangeI;
extern double transmissionRangeM,interferenceRangeM;

//************* VJ change end *************

//portability change start
struct neighborhood_info
{
	uint8_t infra_node;
	int8_t rssi;
};

struct topologyUpdate
{
	uint16_t node;
	uint8_t type;
	struct neighborhood_info neighbors[MAX_INFRA_NEIGHBORS];
	//uint8_t no_of_neighbors;
	//struct neighborhood_info * neighbors;
};

struct infrastructure_list
{
	int infra_node;
	int times_observed;
	int missed; // may be used in averaging
	int rssi[5]; // portability hack
	int is_neighbor; //boolean variable
	float average; 
	double timeLastHeard; 
};

//mobility change 15-4-10
struct expected_fragment
{
	uint8_t infra_node;
};

//mobility change 16-4-10
struct moving_mobiles
{
	int mobile_node;
	int direction;
	double started_at; 
}moving_nodes[MAX_MOVING_NODES];

//mobility change 18-4-10
struct mobile_direction
{
	uint8_t direction;
};

/*struct temp_elements
{
	struct scheduleElement tempScheduleElements[MAX_SCHED_ELEMENTS];
	uint8_t no_elements;
};*/

/*
struct infra_update
{
	//int infra_number;
	struct mobile_update mobile_list[50]; // assuming not more than 50 mobiles at a time under an infrastructure node

}infra_topology_update[MAX_NO_INFRA_NODES];
*/

extern int uniq_time[MAX_NO_NODES]; 
extern int uniq_time_counter;
extern double percentageCallDrop;
//portability changes end


//manan change begin // this structure is not being used currently
/*struct voice_data
{
	uint16_t length;
	uint16_t e2eSource;
	uint16_t e2eDestination;
	uint16_t flowId;
	uint16_t slotNo; //for internal purpose
	uint16_t *payloadData;
	uint16_t seq_no;
	uint16_t prev_seq_no;
	uint16_t next_seq_no;
};*/
//manan change ends

enum packetType
{
	SCHEDULE,
	SCHEDULE_ELEMENT, //for internal purpose
	DATA,
	JOIN_REQUEST,
	FLOW_REQUEST,
	BW_REQUEST,
	TEAR_DOWN,
	LINK_LEVEL_ACK,
	CONTENTION_FORWARD,
	//manan change
	VOICE_FLOW_REQUEST,
	TOPOLOGY_UPDATE, // portability change
	EXPECTED_FRAGMENT,  // mobility change 16-4-10
	MOBILE_DIRECTION    // mobility change 18-4-10
	//manan end	
};

enum eventType
{	
	TX, //for contention free slots
	RX, 	
	RX_BEGIN, ////for contention slots, for detecting collision
	RX_END,
	CONTENTION_RX_BEGIN,
	CONTENTION_RX_END,
	DATA_RX_BEGIN,
	DATA_RX_END,
	HANDLE_CONTENTION_SLOT,
	STOP_TRANSMISSION,	
	MAKE_ALIVE, //non-consequential events
	MAKE_DEAD,
	DATA_SLOT_TRANSMIT,
	CONTENTION_SLOT_BEGINS,	//a contention slot event is inserted for every node
	CONTENTION_SLOT_BACKOFF,	//a contention slot event is inserted for every node
	JOIN_REQUEST_TIMEOUT,	//time out event when join request is sent
	FLOW_REQUEST_TIMEOUT,//may not be used
	KEEP_JOINED_TIMEOUT,
	SCHEDULE_DISSEMINATE,
	SCHEDULE_RECEPTION_TIMEOUT,
	NEXT_FLOW_REQUEST,
	GENERATE_FLOW_REQUEST,
	CAN_START_FLOW,
	START_NEW_SCHEDULE,
	BW_REQUEST_TIMEOUT,
	LINK_ACK_TIMEOUT,	//time out event when packet is sent in contention slot
	CALL_REQUEST, //not used currently, call requests are initiated when a node join in
	VOICE_MSG_CHECK,
	STORE_CAP,
	TOPOLOGY_UPDATE_CHECK, // portability change
	NODE_DISABLE, // node disable change
	VOICE_MSG_RELAY,
	FRAGMENT_RECEPTION_TIMEOUT,  // mobility change 15-4-10
	MOBILITY,   //mobility change 17-4-10 
	START_MOBILE,  // mobility change 16-4-10
	STOP_MOBILE,    // mobility change 16-4-10
	SET_UP_NEW_SCHEDULE
};

enum nodeType
{
	INFRA,
	MOBILE
};

enum nodeState
{
	ALIVE,
	DEAD,
	NOT_JOINED,
	JOINED //implies synchronized as well
};

enum allocation
{
	FREE,
	BUSY
};

enum logType
{
	TIME_SYNC,
	TIME_JOIN_REQUEST,
	TIME_JOIN_GRANT,
	TIME_FLOW_REQUEST,
	TIME_FLOW_GRANT,
	SIZE_SCHEDULE,
	NUM_SCHEDULE_FRAGMENTS,
	TIME_NODE_DEAD,
	TIME_NODE_ORPHAN,
	TIME_SCHEDULE_DROP,
	EXPECTING_PARENT_CHANGE,
	NEW_PARENT_RECEIVED,
	EXPECTING_PATH_CHANGE,
	NEW_PATH_RECEIVED,
	HANDOFF_FAIL
};

struct event
{
	double time;	//time of the event
	int node;	//with which node this event deals
	struct packetFormat * aPacket;	//corresponding packet if event demands	contains src,dest,ver_type_piggyback and pointer to payload
	enum eventType type;
	struct event *next;
};

struct nodeInfo
{		
	enum nodeType type;	
	enum nodeState state;

	//CSMA info
	int packetBeingReceived;	//for detecting collision, not used
	int packetBeingTransmitted;	//for detecting whether channel is busy by checking neighboring nodes
	int collision;	//no of overlapped receptions
	int backoff;	//backoff exponent, controls window to choose random number from, i suppose not being used for 15.4
	//int contentionPacketType;
	int expectingLinkAck;
	int numOfAttempts; //portability changes
	
	//node info
	int numInterferringNeighbors;
	int numNeighbors;

	int interferenceList[MAX_NO_NODES];
	int neighborList[MAX_NO_NODES];	
	int sponsorDepth; // whats this?? depth of the parent of which the node is the child
	int sponsor; // mobility change 2-5-10
	int parent;
	int depth;	//used for orphan node time out
	int broadcastTurn;	//if infrastructure node, the turn to broadcast schedule with respect schedule packet reception
	int numOfInfraNodesInTree;
	
	int controlReceptionCount;
	int controlReceptionCollision;
		
	int currentFlowId;
	//manan change
	int voiceFlow; // boolean variable to check whether its a voice msg going on or not
	int numOfTopologyUpdatesReceived; // portability change
	int numOfTopologyUpdatesSent; // portability change
	int maxQueueSize;
	//manan end
	int flowTearedDown;		
	
	int expectingJoinRequestAck;
	int expectingFlowRequestAck;

	//int nodeRSSI[MAX_NO_NODES]; //not used in current implementation
	struct infrastructure_list infra_list[MAX_NO_INFRA_NODES];  
	int data_history[MAX_DATA_HISTORY + 10]; // mobility change 25-4-10, mobility hack 6-5-10 

	//packet queues
	struct packetFormat * dataPacketQueue[QUEUE_LIMIT];
	int dataFront, dataRear;
	struct packetFormat * packetQueue[QUEUE_LIMIT]; // so is packetQueue actually a control packet queue or comprises both the data and control packets???
	int controlFront, controlRear;	
	int numOfPacketsInQueue;	
	int numOfPacketsInDataQueue;

	double callDuration; 
	int callInProgress; //not to invoke generate event when call is already in progress
	//manan change
	//int voiceCallInProgress;
	//manan end
	//time sync info
	int synchronized;
	double globalTime;
	double localTime;
	double offset;
	
	//******************************************
	//schedule info, local view of the node
	double lastScheduleTime;
	double scheduleInterval;	
	//******************************************
	
	int scheduleSlots;
	int lastScheduleMissed;
	double timeLastScheduleReception;
		
	//*******************************************
	int numSchedElem;
	int treeUpdate;	//ON and OFF based on formation of new routing tree
	
	int treeUpdateBroadcast;
	
	int remSchedElem;
	int currSchedElem;
	int remTreeNodes;
	int currTreeNodes;
	int moreFragment;
	int broadcastSchedFragmentNo;
	double scheduleValidFrom;
		
	int expectedSchedFragmentNo;
	int lastFragmentNo;
	
	double contentionProb;
	int scheduleNumOfFragments;
	
	int wasOrphan;
	//*******************************************
	
	int schedRepeat;
	int numScheduleMissed;
	int newScheduleReceived;
	
	int flowRequestGranted;
	int flowRetryCount;

	int callRequestSent;   //flag to indicate call in progress when set to 1
	long int store_cap; // 19-11-09
	int expectingParentChange; // mobility change 17-4-10  
	int expectingPathChange; // mobility change 26-4-10
	int next_hop;
	double timeLastHandoffSent; // mobility change 27-4-10
	struct scheduleElement localScheduleElements[MAX_SCHED_ELEMENTS];
	//struct scheduleElement newScheduleElements[MAX_SCHED_ELEMENTS];
	//int tempNumSchedElem;
	int scheduleMissed;

	//********Rajesh change******

	//! used as a circular queue to to keep record of lost packets in a sliding window of 100 packets. 0 represent packet loss and 1 represents packet received.
	int slidingWindow[DATA_PACKET_SLIDING_WINDOW];
	int windowCount;
	//! used to keep track of data packet sequence number during flow (0 - 255)
	int seqNumber;
	//! used to maintain sequence number of data packet received previously for active flow at mobile node
	int prevSeqNumber;

	//********Rajesh change ends******
	
	int flowScheduleSet[2000];

}nodeID[MAX_NO_NODES];

struct nodeStats
{
	double timeJoinRequestSent;
	double timeJoinGrantReceived;
	double timeFlowRequestSent;
	double timeFlowGrantReceived;
	unsigned int numChannelControlPacketLosses;
	unsigned int numQueueControlPacketDrops;
	unsigned int numFlowRequestDrops;
	
	double upTime;
	double endCallTime;
	
}nodeStat[MAX_NO_NODES];

extern double activeFlowIDTime[2000];


struct flowStats
{
	int flowId;
	int source;
	int destination;
	int flowScheduleSet;
	int phase;
	double timeoutTime;
	int slotArray[MAX_NO_HOPS * 2];
	int voiceFlow; //manan change to check whether the flow is a voice flow or not
	double flowSentAt;
	struct routingPath * aPath; // portability change
	
}flowStat[MAX_NO_FLOWS];

struct channelResourceArray
{
	enum allocation state;
	int source;	//these fields are necessary to deal with primary and secondary interferences
	int destination;
	int flowId;
};

struct routingPath
{
	int *path;
	int numOfNodesInPath;
};

extern int uniqueCallsOriginated[MAX_FLOW_ID];

//connectivity graph
extern double nodeSoftStateAtRoot[MAX_NO_NODES]; // portability changes
extern int networkGraph[MAX_NO_NODES][MAX_NO_NODES];
extern int incomingEdge[MAX_NO_NODES];

extern int globalUniqueFlowId;

//input vars

extern double MEAN_CALL_DURATION;
extern double MEAN_CALL_FREQUENCY;
//manan change
extern double MEAN_VOICE_CALL_DURATION;
//manan end

//call stats
extern int noOfCallsOriginated; //employed
extern int noOfCallsAdmitted; //employed
extern int noOfCallsErrDropped; //employed

//At scheduler
extern int noOfTotalFlowRequestsReceived; //employed
extern int noOfTotalFlowRequestsRejected; //employed

//global variables

extern int numFlowRequestsReached[MAX_FLOW_ID];
extern int numTimedOutCalls[MAX_FLOW_ID];
extern int numSuccessCalls[MAX_FLOW_ID];

extern int flowRequestsReceived[MAX_SIM_CALLS];
extern int flowRequestsRejected[MAX_SIM_CALLS];
extern int flowRequestsAdmitted[MAX_SIM_CALLS];

extern int hopDistribution[MAX_NO_HOPS];

extern int globalCallsInProgress;       // count for simultaneous calls. increment/decrement at root
extern int globalLogFlowId;
extern struct event * queueHead;	//event queue head
extern double simTime;	//current time
extern double endSimTime;	//time to end simulation

//error rate
extern double errorRate;

//global schedule info
extern struct schedule * globalSchedule;	//still to be used, not sure if required at first place

//VJ Start
extern int globalNumSchedElem;
//VJ End

extern struct scheduleElement globalScheduleElements[MAX_SCHED_ELEMENTS];
extern int scheduleSlots;	//to view data slots as logically contigous slots across frames
extern double scheduleInterval;

extern int flowsPerNode[MAX_NO_NODES];
extern int phasesPerNode[MAX_NO_NODES][MAX_PHASES];
extern int slotsPerNode[MAX_NO_NODES][NO_OF_DATA_SLOTS]; // am doubtful of this being used
extern int slotsPerNodeSC[MAX_NO_NODES];
extern int slotChannelNodes[NO_OF_DATA_SLOTS][MAX_NO_CHANNELS][MAX_NO_NODES];

extern int schedulePacketChange;	//if any of the below 4 variables change, this gets ON

//global routing tree info
extern int numOfFlowsInProgress;
extern int numOfNodesInTree;
extern int numOfPresentNodesInTree;
extern int numOfPresentInfraNodesInTree;
extern int numOfInfraNodesInTree;
extern int numOfNodesInNetwork;
extern int numOfInfraNodesInNetwork;
extern int numOfMobileNodesInNetwork;
extern int routingTree[MAX_NO_NODES*2];

extern int numOfPresentSchedElem;
extern int max_no_fragments; // node disable hack to calculate max number of fragments in the simulation

//manan change
extern int globalUniqueVoiceFlowId;
extern int numOfVoiceFlowsInProgress;
extern int voiceFlowsPerNode[MAX_NO_NODES];
extern int globalVoiceCallsInProgress;
extern int numSuccessVoiceCalls[MAX_FLOW_ID];
extern int numVoiceFlowRequestsReached[MAX_FLOW_ID];
extern int voiceFlowRequestsReceived[MAX_SIM_CALLS];
extern int voiceFlowRequestsRejected[MAX_SIM_CALLS]; // this is for logging that given a succesful number of calls, what are the flow requests received
extern int voiceFlowRequestsAdmitted[MAX_SIM_CALLS];

extern int noOfVoiceCallsAdmitted; //used
extern int noOfTotalVoiceFlowRequestsReceived; //used
extern int noOfTotalVoiceFlowRequestsRejected; //used
extern int noOfVoiceCallsOriginated; //used

//local
extern int noOfVoiceCallsTimedOut; 
extern int noOfSuccessVoiceCalls;
extern int noOfUniqueVoiceFlowRequestsReceived;

extern int callsNotHandedOff ; // mobility change 6-5-10

struct voiceMsgQueue
{
	int source;
	int destination;
	int overalle2eDestination;
	int voiceQueueFlowId;
	double callDuration;
	int voice_phase;
	//double voiceCopyCallDuration;

}voiceQueue[50];

struct voiceCurrentQueue
{
	int source;
	int destination;
	int overalle2eDestination;
	int voice_phase;
	int voiceFlowId;
	double voiceMsgDuration;
	double voiceCopyCallDuration;
	struct routingPath * rPath;

}voiceCurrQueue[50];

struct voiceFlowStats
{
	int voiceFlowId;
	int source;
	int destination;
	int hops;
	double start_time;
	double end_time;
	int revoked;
	int fragmented;
	
}voiceFlowStat[MAX_NO_FLOWS];

extern int numPathsInVoiceQueue;
extern int numPathsInVoiceCurrQueue;
//for stored voice messaging
//extern int bottleneck_node;
extern int real_time_call_counter;
extern int noOfCallsAdmitted_Unique[MAX_FLOW_ID]; // manan change, for calculating the number of unique calls admitted
//extern int noOfVoiceFrags[5]; // used for calculating fragmentation of the voice messages
extern int noOfCallsAdmitted_Unique_Calc;//manan change, calculates the number of unique calls admitted
extern int numVoiceMsg; // used to calculate the number of stored voice messages generated
extern int voiceHopDistribution[MAX_NO_HOPS+10]; //used for calculating the hop distribution of the voice messages, that is for paths of what no. of hops are the voice messages generated.


//extern int voicee2esource;
//extern int voicee2edest;
//manan end

//for detecting duplicate receptions (scenario: packet received in contention slot but link ack got dropped)
extern struct flowRequest * lastFlowRequest;
//manan change
extern struct voiceFlowRequest * lastVoiceFlowRequest;
//manan end
extern struct tearDown * lastTearDown;
extern struct nodeJoin * lastJoinRequest;
//manan change
extern FILE * dump;
extern FILE * vm;
extern FILE * at;
extern FILE * voice_end;
extern FILE * voice_start;
extern FILE * voice_log;
//manan end

extern flowRequestReceptionCounter;

//extern int scheduleElementsExist; // likely to be removed
//extern int lastScheduleElementFragment; //likely to be removed


//temporary variables for debugging here

//***********************************************************************************************************
//function declarations
double randdouble();
struct event * getScheduleEvent(double time, int tx, int rx, enum eventType type);
struct event * getAliveEvent(double time, int node);
struct event * getFlowRequestEvent(double time, int flowSource, int flowDestination, int flowId_bwRequest, int node, int destination, enum eventType type);
//manan change
struct event * getVoiceFlowRequestEvent(double time, int flowSource, int flowId_bwRequest, int node, int destination, int e2eDest, double voiceMsgDuration, enum eventType type);
struct event * getVoiceMsgCheckEvent(double time);
struct event * getStorageCapacityEvent(double time);
//manan end
struct event * getJoinRequestEvent(double time, int joinSource, int node, int destination, enum eventType type);
int extractSizeof(struct packetFormat * aPacket);
struct event * getLinkLevelAckEvent(double time, int node, int destination, int type);
struct event * getJoinRequestTimeoutEvent(double time, int node);
struct event * getFlowRequestTimeoutEvent(double time, int node);
struct event * getAckTimeoutEvent(double time, int node);
struct event * getContentionSlotEvent(double time, int node);
struct event * getBackoffContentionSlotEvent(double time, int node);
struct event * getDataSendEvent(double time, int node, struct scheduleElement inputElement);
struct event * getDataSourceSendEvent(double time, int node, int length, int seqNumber, int e2eSource, int e2eDestination, int flowId, int source, int destination, int slotNo, enum eventType type);
//manan change
struct event * getTearDownEvent(double time, int flowId, int node, int destination, enum eventType type);
//manan end
struct event * getBandwidthRequestEvent(double time, int flowId_bwRequest, int node, int destination, enum eventType type);
struct event * getBandwidthTimeoutEvent(double time, int flowId);
struct event * getScheduleReceptionTimeout(double time, int node);
struct event * getNewScheduleParamEvent(double time, int node);
struct event * getNextExpectedScheduleEvent(double time, int node);
struct event * getDeadEvent(double time, int node);
struct event * getNextFlowRequestEvent(double time, int node);
struct event * getContentionReceptionEvent(double time, int node, enum eventType type);
struct event * getDataReceptionEvent(double time, int node, enum eventType type);
struct event * getStopTransmissionEvent(double time, int node);
struct event * getHandleContentionSlotEvent(double time, int node);
struct event * getCanStartFlowRequestEvent(double time, int node);

struct event * nextEvent();
double exponCallDuration(double x);
double getNextExpectedScheduleTime(double thisTime, int numOfFragments, int setTest);

struct routingPath * getRoutingPath(int source, int destination);

struct event * getTopologyCheckEvent(double time); // portability changes
struct event * getTopologyUpdateEvent(double time, int source, int node, int destination, enum eventType type, int update_type); // portability changes
struct event * getNodeDisablingEvent(double time); // portability changes
struct event * getMobilityEvent(double time); // mobility change 17-4-10
struct event * getStartMobilityEvent(double time, int mobile_node, int direction); // mobility change 17-4-10
struct event * getStopMobilityEvent(double time, int mobile_node); // mobility change 17-4-10
struct event * getVoiceMsgRelayEvent(int node, double time); // portability change, node disable change
struct event * getNextExpectedFragmentEvent(double time, int node, int infra); // mobility change 15-4-10
double generateTopologyUpdateTime(double time); // portability changes
void resolve_packet(int , char *);
int maxTreeHops();
int getNumOfHops(int);
int getParent(int);
int getRSSI(int src, int dst, double time); // mobility change
void getPosition(int node, int *x, int *y, double time); // mobility change
int anyAncestorIsDead(int node); 
int isPartOfAnyFlow(int node);
void updateNeighborInfo(int node, int infra, int rssi); // mobility change 14-4-10 , the infra here will the source of the schedule
void purgeNeighborInfo(int node); // mobility change 16-4-10
void rebuildNeighborInfo(int node); // mobility change 17-4-10 
void printNeighborInfo(int node); // mobility change 16-4-10 
void performAveraging(int node, int infra, int rssi); // mobility change 16-4-10 
void getCurrentTopology(int infra); // mobility change 17-4-10   
int chooseAppropriateDirection(int x_coord, int y_coord); // mobility change 19-4-10
int flowInProgress(int flowId); // mobility change on 24-4-10 
int updateConnectivityInfo(int node, int infra, int rssi); // mobility change 25-4-10
void performDataAveraging(int node, int infra, int rssi); // mobility change 25-4-10
void clearDataHistory(int node); // mobility change 26-4-10
int setSchedulingElements(int source, int destination, int flowId, int incomingPhase, int carryForward, int handoff, struct routingPath * aPath); //mobility change 26-4-10
int canInsertNextEvent(int node, struct scheduleElement anElement, double nextEventTiming); // mobility change 29-4-10
int canSendMoreData(int node, int flowId, int pkt_src, int pkt_dst); // mobility change 30-4-10
int nodeIsMobile(int node, int flowId, int pkt_src); // mobility change 30-4-10
int getNewNextHop(int node, int flowId, int pkt_src); // mobility change 30-4-10
int getNewSlot(int node, int flowId, int pkt_src); // mobility change 30-4-10
void logMobilityStats(enum logType type, int node, int infra, double simTime); // mobility change 4-5-10
void refreshDataQueue(int infra); //mobility change 5-5-10
void printDataQueue(int infra); // mobility change 6-5-10 
struct event * getSetUpNewScheduleEvent(double time, int node);
struct event * getGenerateFlowRequestEvent(double time, int flowId);
//***********************************************************************************************************************

#endif
