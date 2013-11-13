#include "header.h"

#define PRINT_UNIT (1*60*1000)
#define RAND_INIT 007

int flowRequestReceptionCounter = 0;

int flowDropDueToE2EPacketLosses = 0;

double percentageCallDrop = (double)0;

int countUniqueCallsOriginated= 0;
int uniqueCallsOriginated[MAX_FLOW_ID];
int uniqueCallsDropped[MAX_FLOW_ID];

//global variables, usage is documented in header.h
struct event * queueHead = NULL;

struct scheduleElement globalScheduleElements[MAX_SCHED_ELEMENTS];
//so the schedule element contains the transmitter,recvr,flowid_channel,slot_tostart,and no of slots req. 

int routingTree[MAX_NO_NODES*2];
//why *2? parent child relationships

struct flowRequest * lastFlowRequest = NULL;
//manan change 
struct voiceFlowRequest * lastVoiceFlowRequest = NULL;
//manan end

//the flow req contains e2e source,dest and flowid_bwrequest
struct tearDown * lastTearDown = NULL; //contains the flowid
struct nodeJoin * lastJoinRequest = NULL; // contains the node number

double nodeSoftStateAtRoot[MAX_NO_NODES]; // portability changes
int uniq_time[MAX_NO_NODES];
int uniq_time_counter;

int flowsPerNode[MAX_NO_NODES]; //number of flows per node, entries number
//manan change
int voiceFlowsPerNode[MAX_NO_NODES];
//manan end
int phasesPerNode[MAX_NO_NODES][MAX_PHASES]; //which node occupies what phases, entries 0 and 1
int slotsPerNode[MAX_NO_NODES][NO_OF_DATA_SLOTS]; //which node occupies which data slot, entries 0 and 1?thinking of adding 2
int slotsPerNodeSC[MAX_NO_NODES]; //SC was for single channel, no longer being used
int slotChannelNodes[NO_OF_DATA_SLOTS][MAX_NO_CHANNELS][MAX_NO_NODES]; //if a node transmits in particular slot-channel, entries 0 and 1

int globalLogFlowId = 1;//for storing statistics of nodes
int globalUniqueFlowId = 1; //whats this for? for each unique flow
int numOfFlowsInProgress = 0;

//VJ Start
int globalNumSchedElem = 0;
//VJ End

//manan change
int globalUniqueVoiceFlowId = 1;
int numOfVoiceFlowsInProgress = 0;
FILE * dump;
FILE * vm;
FILE * at;
FILE * voice_end;
FILE * voice_start;
FILE * voice_log;
//manan end

int scheduleSlots = NO_OF_DATA_SLOTS; //this gives the logical view of data slots across frames
double scheduleInterval = FRAME_DURATION; //shouldnt it be the time taken for the schedule to go down the tree? its the time required to acomodate all the flows

// tree book keeping
int numOfNodesInTree = 0;
int numOfInfraNodesInTree = 0;

int numOfPresentNodesInTree = 0;
int numOfPresentInfraNodesInTree = 0;  // vijay had said that this are for the local views of the nodes in the tree. but then why arent they arrays??

int numOfNodesInNetwork;
int numOfInfraNodesInNetwork;
int numOfMobileNodesInNetwork;

// schedule book keeping

int numOfPresentSchedElem = 0;
int max_no_fragments = 0; // node disable hack

// call book keeping
int globalCallsInProgress = 0;

//manan change
int globalVoiceCallsInProgress = 0;
int numSuccessVoiceCalls[MAX_FLOW_ID];
int numVoiceFlowRequestsReached[MAX_FLOW_ID];
//int numTimedOutCalls[MAX_FLOW_ID];//a voice message will also never be timed out

int voiceFlowRequestsReceived[MAX_SIM_CALLS];
int voiceFlowRequestsRejected[MAX_SIM_CALLS]; // this is for logging that given a succesful number of calls, what are the flow requests received
int voiceFlowRequestsAdmitted[MAX_SIM_CALLS];

int noOfVoiceCallsAdmitted = 0; //used
int noOfTotalVoiceFlowRequestsReceived = 0; //used
int noOfTotalVoiceFlowRequestsRejected = 0; //used
int noOfVoiceCallsOriginated = 0; //used
//int noOfVoiceCallsErrDropped = 0; // a voice message will never be dropped

//local
int noOfVoiceCallsTimedOut = 0; 
int noOfSuccessVoiceCalls = 0;
int noOfUniqueVoiceFlowRequestsReceived = 0;

//int bottleneck_node = -1;
int numPathsInVoiceQueue = 0;
int numPathsInVoiceCurrQueue = 0;
int real_time_call_counter = 0;
int revoke_flag = 0;
//int voicee2esource = -1;
//int voicee2edest = -1;
//manan end

int numSuccessCalls[MAX_FLOW_ID]; 
int numFlowRequestsReached[MAX_FLOW_ID];
int numTimedOutCalls[MAX_FLOW_ID];

int flowRequestsReceived[MAX_SIM_CALLS];
int flowRequestsRejected[MAX_SIM_CALLS]; // this is for logging that given a succesful number of calls, what are the flow requests received
int flowRequestsAdmitted[MAX_SIM_CALLS];

int hopDistribution[MAX_NO_HOPS]; //still to know hte function of this variable?logging how many flows had how many hops

int noOfCallsAdmitted = 0; //used
int noOfTotalFlowRequestsReceived = 0; //used
int noOfTotalFlowRequestsRejected = 0; //used
int noOfCallsOriginated = 0; //used
int noOfCallsErrDropped = 0; //used

//local
int noOfCallsTimedOut = 0; 
int noOfSuccessCalls = 0;
int noOfUniqueFlowRequestsReceived = 0;

int callsNotHandedOff = 0; // mobility change 6-5-10 
// misc

int schedulePacketChange = FALSE; // i guess this is for change in the schedule
double currentScheduleTime = 0;

double lastScheduleTime = 0; // likely to be changed
double lastScheduleChange = 0; // likely to be changed

double MEAN_CALL_DURATION = 0;
double MEAN_CALL_FREQUENCY = 0;
//manan change start
double MEAN_VOICE_CALL_DURATION = 0;
//manan change end

double simTime = 0;
double endSimTime = 4*60*60*1000; // i guess this is the default value of 4 hours
double errorRate = 0.0;
double contentionProbability = 1.0;

int globalSlotNo = UNDEFINED; //wats the purpose of this
double globalNextEventTime = UNDEFINED;

//************* VJ change start *************
double transmissionRangeI = (double) 250, interferenceRangeI = (double) 350;
double transmissionRangeM = (double) 150, interferenceRangeM = (double) 250;
//************* VJ change end *************

//manan change for logging in case of stored voice messaging
int noOfCallsAdmitted_Unique[MAX_FLOW_ID]; // manan change, for calculating the number of unique calls admitted
//int noOfVoiceFrags[5]; // used for calculating fragmentation of the voice messages
int noOfCallsAdmitted_Unique_Calc = 0;//manan change, calculates the number of unique calls admitted
int numVoiceMsg = 0; // used to calculate the number of stored voice messages generated
int voiceHopDistribution[MAX_NO_HOPS+10]; //used for calculating the hop distribution of the voice messages, that is for paths of what no. of hops are the voice messages generated.

//manan end


double activeFlowIDTime[2000];



//initialize queue, time, tree
//initialize root state
//insert non-consequential events
int initializeSimulation()
{	
	
	int index;
	int i,j,k;
	double wakeupTime, tempTime;
	
	double timeToStart;
	int flowId, source, destination;
	double callDuration;
	
	char ch;
	struct event * anEvent; 
	
	int numFlowEvents = 0, limitFlowEvents = 3;
	
	FILE *fp;
	
	queueHead = NULL;
	simTime = 0;

	numOfNodesInTree = 0;

	routingTree[0] = ROOT;
	nodeID[ROOT].broadcastTurn = 0;
	nodeID[ROOT].depth = 0;
	
	//********Rajesh change*****

	flowDropDueToE2EPacketLosses = 0;

	//********Rajesh change ends******

	
	for(index = 1; index < MAX_NO_NODES*2; index++)
	{
		routingTree[index] = UNDEFINED;		
	}
	
	for(i=0; i<MAX_NO_NODES; i++)
	{
		flowsPerNode[i] = 0;
		//if(i == 3 || i == 1)
		//flowsPerNode[i] = 4;
		//manan change hack
		//voiceFlowsPerNode[i] = 0;
		//manan end
	}
	
	for(i=0; i<MAX_NO_NODES; i++)
	{
		for(j=0; j<MAX_PHASES; j++)
		{
			phasesPerNode[i][j] = 0; 
			//if(i == 3 || i == 1)
			//phasesPerNode[i][j] = 1; // hack 
		}
	}

	for(i=0; i<MAX_NO_NODES; i++)
	{
		for(j=0; j<NO_OF_DATA_SLOTS; j++)
		{
			slotsPerNode[i][j] = 0; 
		}
		//slotsPerNodeSC[i] = 0;
	}
	
	for(i=0; i<NO_OF_DATA_SLOTS; i++)
	{
		for(j=0; j<MAX_NO_CHANNELS; j++)
		{
			for(k=0; k<MAX_NO_NODES; k++)
			{
				slotChannelNodes[i][j][k] = 0;
			}
		}
	}

	for(i=0; i<MAX_FLOW_ID; i++) 
	{
		uniqueCallsOriginated[i] = 0;
		uniqueCallsDropped[i] = 0;
		numSuccessCalls[i] = 0;
		numFlowRequestsReached[i] = 0;
		numTimedOutCalls[i] = 0;
	
		//manan change
		numSuccessVoiceCalls[i] = 0;
		numVoiceFlowRequestsReached[i] = 0;
		noOfCallsAdmitted_Unique[i] = 0;
		//manan end
	}
	
	for(i=0; i<MAX_SIM_CALLS; i++)
	{
		flowRequestsRejected[i] = 0;
		flowRequestsReceived[i] = 0;
		flowRequestsAdmitted[i] = 0;

		//manan change
		voiceFlowRequestsRejected[i] = 0;
		voiceFlowRequestsReceived[i] = 0;
		voiceFlowRequestsAdmitted[i] = 0;

		//manan end
	}
	
	for(i=0;i<MAX_NO_HOPS;i++)
	{
		hopDistribution[i] = 0;

		//manan change
		voiceHopDistribution[i] = 0;
		//manan end
	}

	//portability changes start
	/*for(i=0; i<MAX_NO_INFRA_NODES; i++)
	{
		//infra_topology_update[i].infra_number = i+1;

		for(j=0; j<50; j++)
		{
		infra_topology_update[i].mobile_list[j].mobile_number = 0;
		infra_topology_update[i].mobile_list[j].numTopologyUpdateReceived = 0;
		infra_topology_update[i].mobile_list[j].nodeSoftState = 0;
		infra_topology_update[i].mobile_list[j].node_observed = FALSE;
		}
	}*/
	for(i=0; i<MAX_NO_NODES; i++)
	{
		nodeSoftStateAtRoot[i] = 0;
	}

	//mobility change 16-4-10
	for(i=0; i < MAX_MOVING_NODES; i++)
	{
		moving_nodes[i].mobile_node = -1;
		moving_nodes[i].direction = 0;
		moving_nodes[i].started_at = 0;
	}

	uniq_time_counter = 0;
	//portability changes end
	
	for(i=0; i<2000; i++)
	{
		activeFlowIDTime[i] = 0;
	}
	
	
	startLoggingNodeStats();
	
	anEvent = getScheduleEvent(simTime, ROOT, ROOT, TX); // for broadcasting the first schedule.
	insertEvent(anEvent);
	anEvent = getContentionSlotEvent(simTime+(NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION), ROOT);
	insertEvent(anEvent);

	//manan change voice message hack
	anEvent = getVoiceMsgCheckEvent(simTime);
	insertEvent(anEvent);

	//anEvent = getNodeDisablingEvent(1200000); // node disable change
	//insertEvent(anEvent);

	
	//VJ start: To enable mobility
	//anEvent = getMobilityEvent(1200000);
	//insertEvent(anEvent);
	//VJ end

	//anEvent = getStorageCapacityEvent(1200000);
	//insertEvent(anEvent);
	//manan end

	//portability changes start
	anEvent = getTopologyCheckEvent(simTime+(NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION)+(NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION));
	insertEvent(anEvent);
	//portability changes end
	
	printf("\n numOfNodesInNetwork %d",numOfNodesInNetwork);
	printf("\n numOfInfraNodesInNetwork %d",numOfInfraNodesInNetwork);
	printf("\n numOfMobileNodesInNetwork %d",numOfMobileNodesInNetwork);
	
	initrand((unsigned) RAND_INIT);
	
	tempTime = (double) 0;
	
	for(i=1; i<numOfInfraNodesInNetwork; i++)
	{

		wakeupTime = tempTime + randdouble((double) 1500);
		fprintf(stderr,"\n wakeup %d %lf",i,wakeupTime);
		anEvent = getAliveEvent(wakeupTime,i);
		insertEvent(anEvent);
		tempTime = tempTime + (double) 3000; //why add 3000? waking up the nodes layer by layer in an orderly fashion
	}
	
	for(i=0; i<numOfMobileNodesInNetwork; i++)
	{

		wakeupTime = tempTime + randdouble((double) 1500);
		fprintf(stderr,"\n wakeup %d %lf",numOfInfraNodesInNetwork + i,wakeupTime);
		anEvent = getAliveEvent(wakeupTime,numOfInfraNodesInNetwork + i);
		insertEvent(anEvent);
		tempTime = tempTime + (double) 3000;
	}
	
	#if GENERATE_REQUESTS_FROM_FILE
	
	fp = fopen("recordFlow.txt", "r");
	
	while((ch = fgetc(fp)) != EOF)
	{
		if(ch == '(')
		{
			fscanf(fp,"%lf",&timeToStart);
			fscanf(fp,"%d",&flowId);
			
			fprintf(stderr,"%lf %d\n",timeToStart, flowId);
			
			fscanf(fp,"%d",&source);
			fscanf(fp,"%d",&destination);
			fscanf(fp,"%lf",&callDuration);
			
			anEvent = getGenerateFlowRequestEvent(timeToStart, flowId);
			insertEvent(anEvent);
			
			numFlowEvents++;
			
			//if(numFlowEvents >= limitFlowEvents)
				//break;
		}	
	}
	
	fclose(fp);
	
	#endif

	//exit(0);
	return SUCCESS;
}

//manan change start
void resolve_name(int event_no,char *event_name)
{

switch(event_no)
	{
	case 0:
		strcpy(event_name,"TX");
		break;
	case 1:
		strcpy(event_name,"RX");
		break;
	case 2:
		strcpy(event_name,"RX_BEGIN");
		break;
	case 3:
		strcpy(event_name,"RX_END");
		break;
	case 4:
		strcpy(event_name,"CONTENTION_RX_BEGIN");
		break;
	case 5:
		strcpy(event_name,"CONTENTION_RX_END");
		break;
	case 6:
		strcpy(event_name,"DATA_RX_BEGIN");
		break;
	case 7:
		strcpy(event_name,"DATA_RX_END");
		break;
	case 8:
		strcpy(event_name,"HANDLE_CONTENTION_SLOT");
		break;
	case 9:
		strcpy(event_name,"STOP_TRANSMISSION");
		break;
	case 10:
		strcpy(event_name,"MAKE_ALIVE");
		break;
	case 11:
		strcpy(event_name,"MAKE_DEAD");
		break;
	case 12:
		strcpy(event_name,"DATA_SLOT_TRANSMIT");
		break;
	case 13:
		strcpy(event_name,"CONTENTION_SLOT_BEGINS");
		break;
	case 14:
		strcpy(event_name,"CONTENTION_SLOT_BACKOFF");
		break;
	case 15:
		strcpy(event_name,"JOIN_REQUEST_TIMEOUT");
		break;
	case 16:
		strcpy(event_name,"FLOW_REQUEST_TIMEOUT");
		break;
	case 17:
		strcpy(event_name,"KEEP_JOINED_TIMEOUT");
		break;
	case 18:
		strcpy(event_name,"SCHEDULE_DISSEMINATE");
		break;
	case 19:
		strcpy(event_name,"SCHEDULE_RECEPTION_TIMEOUT");
		break;
	case 20:
		strcpy(event_name,"NEXT_FLOW_REQUEST");
		break;
	case 21:
		strcpy(event_name,"CAN_START_FLOW");
		break;
	case 22:
		strcpy(event_name,"START_NEW_SCHEDULE");
		break;
	case 23:
		strcpy(event_name,"BW_REQUEST_TIMEOUT");
		break;
	case 24:
		strcpy(event_name,"LINK_ACK_TIMEOUT");
		break;
	case 25:
		strcpy(event_name,"CALL_REQUEST");
		break;	
	case 26:
		strcpy(event_name,"VOICE_MSG_CHECK");
		break;	
	case 27:
		strcpy(event_name,"STORE_CAP");
		break;
	case 28:
		strcpy(event_name,"TOPOLOGY_UPDATE_CHECK");
		break;
	case 29:
		strcpy(event_name,"NODE_DISABLE");
		break;
	case 30:
		strcpy(event_name,"VOICE_MSG_RELAY");
		break;
	case 31:
		strcpy(event_name,"FRAGMENT_RECEPTION_TIMEOUT");
		break;
	case 32:
		strcpy(event_name,"MOBILITY");
		break;
	case 33:
		strcpy(event_name,"START_MOBILE");
		break;
	case 34:
		strcpy(event_name,"STOP_MOBILE");
		break;
	
	}
}


void resolve_packet(int packet, char *packet_type)
{

switch(packet)
	{
	case 0:
		strcpy(packet_type,"SCHEDULE");
		break;
	case 1:
		strcpy(packet_type,"SCHEDULE_ELEMENT");
		break;
	case 2:
		strcpy(packet_type,"DATA");
		break;
	case 3:
		strcpy(packet_type,"JOIN_REQUEST");
		break;
	case 4:
		strcpy(packet_type,"FLOW_REQUEST");
		break;
	case 5:
		strcpy(packet_type,"BW_REQUEST");
		break;
	case 6:
		strcpy(packet_type,"TEAR_DOWN");
		break;
	case 7:
		strcpy(packet_type,"LINK_LEVEL_ACK");
		break;
	case 8:
		strcpy(packet_type,"CONTENTION_FORWARD");
		break;
	case 9:
		strcpy(packet_type,"VOICE_FLOW_REQUEST");
		break;
	case 10:
		strcpy(packet_type,"TOPOLOGY_UPDATE");
		break;
	case 11:
		strcpy(packet_type,"EXPECTED_FRAGMENT");
		break;
	case 12:
		strcpy(packet_type,"MOBILE_DIRECTION");
		break;
	}
}
//manan change end

//update local clocks of alive nodes
int updateLocalTimings()
{
	int index;
	for(index = 1; index <= numOfNodesInNetwork; index++)
	{
		if(nodeID[index].synchronized == TRUE)
			nodeID[index].localTime = simTime - nodeID[index].offset; 
	}
	return SUCCESS;
}

void printNeighborInfo(int node)
{
	int i,j;

	fprintf(stderr,"\n Neighbors of node %d are \n", node);
			for(i=0; i<nodeID[node].numNeighbors; i++)
			{
			fprintf(stderr,"%d  ", nodeID[node].neighborList[i]);
			}
		fprintf(stderr,"\n Interferers of node %d are \n", node);
			for(i=0; i<nodeID[node].numInterferringNeighbors; i++)
			{
			fprintf(stderr,"%d  ", nodeID[node].interferenceList[i]);
			}

		fprintf(stderr,"\n Nodes for which node %d is a neighbor \n", node);
			for(i=0; i<numOfNodesInNetwork; i++)
			{
				for(j=0; j<nodeID[i].numNeighbors; j++)
				{
					if(nodeID[i].neighborList[j] == node)
					{
					fprintf(stderr,"node %d is a neighbor of %d \n", node, i);
					}
				}
			}
		fprintf(stderr,"\n Nodes for which node %d is an interferer \n", node);
			for(i=0; i<numOfNodesInNetwork; i++)
			{
				for(j=0; j < nodeID[i].numInterferringNeighbors; j++)
				{
					if(nodeID[i].interferenceList[j] == node)
					{
					fprintf(stderr,"node %d is an interferer of %d \n", node, i);
					}
				}
			}
		
}
//initialize and execute events
int main(int argc, char *argv[])
{
	struct event * anEvent;//contains time of event, type, node of event, packet of event, pointer to next event
	struct packetFormat * otherPacket;
	
	double expectedTime = (double) PRINT_UNIT; // so this assigns 1*60*1000 to expectedTime

	FILE *fv,*fx,*fy,*fz,*fk,*fc,*fn,*event_queue_dump;
        //need to write down purpose of each 
	
	int type;
	int node;
	int i,j;
	int noOfSuccessCalls = 0;
	int minutes;

	//manan change start
	int event_counter,cycle_counter=0;
	struct event * curr_queue_event;
	char * event_name;
	event_name = (char *)malloc(30*sizeof(char));
	char * packet_type;
	packet_type = (char *)malloc(30*sizeof(char));

	//dump = fopen("flowSequence.txt","w+"); // for dumping sequence of events
	//vm = fopen("voiceMsg.txt","w+");
	//manan change end
	
	countNodesFlow = 0;

	#if GENERATE_REQUESTS_FROM_FILE
	
	if (argc < 3)
	{
		fprintf(stderr, "\n\n usage: ./lo3mac <errorrate_between(0,1)> <hoursofsim> > temp.dump\n\n");
		exit(0);
       
	}
	else
	{
		errorRate = (double) atof(argv[1]);
		endSimTime = (double) atof(argv[2]) * 60 * 60 * 1000;
		
		//no effect
		MEAN_CALL_FREQUENCY = (double) (1); 
		MEAN_CALL_DURATION = (double) (1); 
		MEAN_VOICE_CALL_DURATION = (double) (1);
	}
	
	#else
	
	if (argc < 6)
	{
		fprintf(stderr, "\n\n usage: ./lo3mac <errorrate_between(0,1)> <hoursofsim> callDuration[0,4)minutes interCallDuration[0,3)hours storedVoiceDuration[0,4)minutes > temp.dump\n\n");
		exit(0);
       
	}
	else
	{
		errorRate = (double) atof(argv[1]);
		endSimTime = (double) atof(argv[2]) * 60 * 60 * 1000;

		MEAN_CALL_FREQUENCY = (double) atof(argv[3]) * 60 * 60 * 1000; //converting to milliseconds
		MEAN_CALL_DURATION = (double) atof(argv[4]) *  60 * 1000; // same here
		//manan change start
		MEAN_VOICE_CALL_DURATION = (double) atof(argv[5]) * 60 * 1000;
		//manan change end

		fprintf(stderr, "%lf %lf %lf %lf %lf", MEAN_CALL_FREQUENCY, MEAN_CALL_DURATION, errorRate, endSimTime,MEAN_VOICE_CALL_DURATION);
		printf("%lf %lf %lf %lf %lf", MEAN_CALL_FREQUENCY, MEAN_CALL_DURATION, errorRate, endSimTime,MEAN_VOICE_CALL_DURATION);	
		
	}
	
	#endif
	
	FILE *fp, *fq, *fr;
	
	fp = fopen("outputFiles/results.txt","w+");	
	if(fp == NULL)
	{
		fprintf(stderr,"\n error in opening file");
		exit(0);
	}
	
	fq = fopen("outputFiles/simCalls.txt","w+");
	if(fq == NULL)
	{
		fprintf(stderr,"\n error in opening file");
		exit(0);
	}
	else
	fprintf(fq,"if 3 col, then globalCallsInProgress 0 simTime \n if 4 coloumns then flowid globalCallsInProgress ? simTime\n\n");
	
	fv = fopen("outputFiles/callsDistribution.txt","w+");
	if(fv == NULL)
	{
		fprintf(stderr,"\n error in opening file");
		exit(0);
	}
	
	fr = fopen("outputFiles/scheduleInfo.txt","w+");
	if(fr == NULL)
	{
		fprintf(stderr,"\n error in opening file");
		exit(0);
		
	}
	
	fc = fopen("outputFiles/hopDistribution.txt","w+");
	if(fc == NULL)
	{
		fprintf(stderr,"\n error in opening file");
		exit(0);
	}
	
	fn = fopen("outputFiles/nodeUpTime.txt","w+");
	if(fn == NULL)
	{
		fprintf(stderr,"\n error in opening file");
		exit(0);
	}
	
	//manan change 
	event_queue_dump = fopen("outputFiles/event_queue_dump.txt","a+");
	if(event_queue_dump == NULL)
	{	
	fprintf(stderr,"\n error in opening file event queue dump");
	exit(0);
	}
	
	vm = fopen("outputFiles/voiceMsg.txt","w+");
	if(vm == NULL)
	{	
	fprintf(stderr,"\n error in opening file voiceMsg.txt");
	exit(0);
	}

	dump = fopen("outputFiles/flowSequence.txt","w+"); // for dumping sequence of events
	if(dump == NULL)
	{	
	fprintf(stderr,"\n error in opening file flowSequence.txt");
	exit(0);
	}
	
	at = fopen("outputFiles/automatic_teardown.txt","w+");
	if(at == NULL)
	{
	fprintf(stderr,"\n error in opening file automatic_teardown.txt");
	exit(0);
	}

	voice_end = fopen("outputFiles/Voice_End.txt","w+");
	if(voice_end == NULL)
	{
	fprintf(stderr,"\n error in opening file Voice_End.txt");
	exit(0);
	}

	voice_start = fopen("outputFiles/Voice_Start.txt","w+");
	if(voice_start == NULL)
	{
	fprintf(stderr,"\n error in opening file Voice_Start.txt");
	exit(0);
	}

	voice_log = fopen("outputFiles/Voice_Log.txt","w+");
	if(voice_log == NULL)
	{
	fprintf(stderr,"\n error in opening file Voice_Log.txt");
	}
	
	//manan end
	
	fx = fopen("outputFiles/flowStartTimes.txt","w+");
	fy = fopen("outputFiles/admitCalls.txt","w+");
	fprintf(fy,"outputFiles/noOfCallsAdmitted, flowId, globalCallsInProgress, simTime\n\n");
	fz = fopen("outputFiles/callAccepted.txt","w+");
		
	fk = fopen("outputFiles/channelInfo.txt","w+");	
		
	fclose(fx);
	fclose(fy);
	fclose(fk);
	fclose(fz);
	
	minutes = 0;	
	
	//the functions initialising the simulation...........................................................
	initializeNodes();
	parseCoordinateFile();
	extractInput("networkGraph.txt");
	initializeSimulation();

	//manan change

	for(i=0;i<numOfNodesInNetwork;i++)
	{
	fprintf(stderr,"Neighbors of Node %d: ",i);
		for(j=0;j<nodeID[i].numNeighbors;j++)
		{
		fprintf(stderr,"%d  ",nodeID[i].neighborList[j]);
		}
		fprintf(stderr,"\n");
	}

	fprintf(stderr,"\n Details of network graph: For a given node, which nodes are connected \n");
	for(i=0;i<numOfNodesInNetwork;i++)
	{
	fprintf(stderr," Node %d: ",i);
		for(j=0;j<numOfNodesInNetwork;j++)
		{
		if(networkGraph[i][j]!=0 && networkGraph[i][j]!=UNDEFINED)
		fprintf(stderr,"%d  ",j);
		}
		fprintf(stderr,"\n");
	}
	fprintf(stderr,"\n");
	//manan end


	while(eventQueueNotEmpty())
	{
		
		anEvent = nextEvent();

		#ifdef DEBUG_ENABLE
		
			/*if(anEvent->type == TX || anEvent->type == RX || anEvent->type == RX_BEGIN || anEvent->type == RX_END)
			{
				//why did we write this if condition??
				type = EXTRACT_TYPE(anEvent->aPacket->ver_type_piggyback);
				//this if condition is not needed , we are neways extracting type in handlePriority queue
				//so this is extracting the type of the packet . will have to see what use it has
				/*
				code 1	
			*/
				
			//}
			/*
			code 2
			*/
		#endif

		if(anEvent->time < 0 || anEvent->time < simTime)
		{
			fprintf(stderr,"\n Santiy check failed, event time negative or in past");
			fprintf(stderr,"\n event time %lf simTime %lf",anEvent->time, simTime);
			exit(0);
		}
		
		if(anEvent->type != BW_REQUEST_TIMEOUT && anEvent->type != GENERATE_FLOW_REQUEST && anEvent->node > numOfNodesInNetwork)
		{
			//fprintf(stderr,"\n Unexpected node %d numOfNodesInNetwork %d type %d",anEvent->node,numOfNodesInNetwork,anEvent->type);
			destroyEvent(anEvent);
			continue; //ok, so this breaks out from the while loop and basically ends the simulation
			//exit(0);
		}
		
		simTime = anEvent->time; 
		// this is actually a very important statement. This is where the simtime gets updated from 
		
		if(simTime > expectedTime)
		{
			minutes++;
			expectedTime += (double) PRINT_UNIT;
			fprintf(stderr,"\n >>> %d minutes over",minutes);
			printf("\n %d minutes over",minutes);	
		}
		
		updateLocalTimings();	// for the first call i suppose the synchronised of all the nodes are false; so this wont work for the first time, infact lets see if this function is bring used at all or not
		
		//the following if condition is for checking the revoking of the voice flow abruptly due to a real time voice call hack
		/*if(simTime > 1630000 && revoke_flag == 0)
		{
		//fprintf(stderr,"Inside processEvent for CAN_START_FLOW for node %d\n",thisEvent->node);
		canStartFlowRequest(13);
		revoke_flag = 1;
		}*/
		
		processEvent(anEvent);
		destroyEvent(anEvent);

		// manan change start //code for going through the contents of the event queue
		//if((curr_sim_time % 10000)>=9950 && (curr_sim_time % 10000)<=10050)
		//{ the above if was for dumping every 10 seconds	
		curr_queue_event = queueHead;
		event_counter=0;
		
		/*if(simTime>=960000)
		{
		while(curr_queue_event != NULL)
		{
		resolve_name(curr_queue_event->type,event_name);
		
		if(strcmp(event_name,"TX")==0||strcmp(event_name,"RX")==0||strcmp(event_name,"RX_END")==0)
			{
			//fprintf(stderr,"event name is %s\n",event_name);
			resolve_packet(EXTRACT_TYPE(curr_queue_event->aPacket->ver_type_piggyback), packet_type);
			fprintf(event_queue_dump,"Event no: %d   Name: %s   Node: %d   Time: %f   Packet type: %s \n ",event_counter,event_name,curr_queue_event->node,curr_queue_event->time,packet_type);
			}
		else
		{
			
			fprintf(event_queue_dump,"Event no: %d   Name: %s   Node: %d   Time: %f\n ",event_counter,event_name,curr_queue_event->node,curr_queue_event->time);	
			
		}
		event_counter++;
		curr_queue_event = curr_queue_event->next;
		if(cycle_counter>=5000)
			{
			fprintf(stderr,"exiting for test purposes from event queue dump\n");
			exit(0);
			}
		}//while loop ends here
		fprintf(event_queue_dump,"\n\n\nCycle number %d over \n",cycle_counter);
		fprintf(event_queue_dump,"Simulation time is %f over \n\n\n",simTime);
		cycle_counter++;
		}*/
		//}
		// manan change end event queue dumping code ends here
		
		//mobility change hack 13-4-10
		/*if(simTime > 600000)
		{
		printNeighborInfo(25);
		exit(0);
		}*/
		
		if(simTime > endSimTime)
			break;	
		
	}
	
	fclose(event_queue_dump);	

	printEventQueue();
	endLoggingNodeStats();
	
	fprintf(stderr,"\n Gracefully Exiting\n");

	//fprintf(fp,"\n No of calls Originated %d", noOfCallsOriginated);
	
	for(i=0; i<MAX_FLOW_ID; i++)
	{
		countUniqueCallsOriginated += uniqueCallsOriginated[i];
	}
	
	//fprintf(fp,"\n Unique calls originated %d", countUniqueCallsOriginated);	
	
	flowDropDueToE2EPacketLosses = 0;
	for(i=0; i<MAX_FLOW_ID; i++)
	{
		flowDropDueToE2EPacketLosses += uniqueCallsDropped[i];
	}
	
	noOfUniqueFlowRequestsReceived = 0;
	for(i=0;i<noOfCallsOriginated; i++)
	{
		noOfUniqueFlowRequestsReceived += numFlowRequestsReached[i+1];
	}
	
	for(i=0;i<noOfCallsOriginated; i++)
	{
		noOfCallsAdmitted_Unique_Calc += noOfCallsAdmitted_Unique[i+1];
		/*
		if(noOfCallsAdmitted_Unique[i+1] == 0)
			fprintf(stderr,"%d call was rejected",i+1);
		else
			fprintf(stderr,"%d call was accepted",i+1);
		*/	
	}
	
		
	//(this may be less than the no of calls originated as the simulation does not consider calls in last few minutes of run, also there may be calls whose source or destination or both could be busy as calls are generated independently and offline. As a consequence, some of the originated calls may be discarded at the root due to filtering check.)	
	fprintf(fp,"\n Unique flow requests received at root %d", noOfUniqueFlowRequestsReceived);
	//fprintf(fp,"\n No of flow requests not received at root %d (these are due to the fact that simulation does not allow calls to be processed in last 10 minutes otherwise there is no packet loss in contention slots and all requests reach root)", (countUniqueCallsOriginated - noOfUniqueFlowRequestsReceived));

	//fprintf(fp,"\n No of calls admitted by scheduler %d", noOfCallsAdmitted);
	fprintf(fp,"\n Unique calls admitted (out of flow requests received) by scheduler %d", noOfCallsAdmitted_Unique_Calc);
	fprintf(fp,"\n No of calls rejected by scheduler %d", noOfUniqueFlowRequestsReceived - noOfCallsAdmitted_Unique_Calc);
	//fp corresponds to results.txt

	noOfSuccessCalls = 0;
	for(i=0;i<noOfCallsOriginated; i++)
	{
		if(numSuccessCalls[i+1] == 0)
		{
			//fprintf(stderr,"\n %d call failed",i+1);
		}	
		noOfSuccessCalls += numSuccessCalls[i+1];
	}
	fprintf(fp,"\n Out of Admitted calls, no of calls successful at client %d", noOfSuccessCalls);
	
	
	noOfCallsTimedOut = 0;
	for(i=0;i<noOfCallsOriginated; i++)
	{
		if(numTimedOutCalls[i+1] == 1)
		{
			//fprintf(stderr,"\n %d call timed out",i+1);
		}	
		noOfCallsTimedOut += numTimedOutCalls[i+1];
	}
	pendingTasks();	
	
	fprintf(fp,"\n Out of Originated calls, no of calls unsuccessful at client: (Timed Out) %d should be (Originated - Successful) %d", noOfCallsTimedOut, noOfUniqueFlowRequestsReceived - noOfSuccessCalls);
	
	fprintf(fp,"\n No of calls Dropped after root due to Time out (this is due to control overhead) (Admitted - Successful): %d ",noOfCallsAdmitted_Unique_Calc - noOfSuccessCalls);
	fprintf(fp,"\n Calls Dropped due to insufficient packets at receiver %d",flowDropDueToE2EPacketLosses);
	
	//fprintf(fp,"\n No of calls Err Dropped (at root) due to BW Time out: %d", noOfCallsErrDropped);
	//fprintf(fp,"\n Starting logs for voice messages \n");
	//fprintf(fp,"\n Number of Voice Messages generated %d ",numVoiceMsg);

	for(i=0;i<noOfCallsOriginated; i++)
	{
		if(numFlowRequestsReached[i+1] == 0)
		{
			//fprintf(stderr,"\n flow %d could not reach root",i+1);
		}	
	}	

	fprintf(fv,"Node Flow_req_recvd Flow_req_rej Flow_req_admt\n");
	
	for(i=0;i<MAX_SIM_CALLS;i++)
	{
		fprintf(fv,"\n%d \t %d \t %d \t %d",i,flowRequestsReceived[i], flowRequestsRejected[i], flowRequestsAdmitted[i]);
	}
	//fv corresponds to calldistribution.txt
	
	fprintf(fc,"Hops Calls with hops",i,hopDistribution[i]);
	for(i=0;i<MAX_NO_HOPS;i++)
	{
		fprintf(fc,"\n%d \t %d",i,hopDistribution[i]);
	}//fc corresponds to hop distribution.txt

	fprintf(fc, "\n\nHops Voice Calls with hops\n\n");
	for(i=0;i<(MAX_NO_HOPS+10);i++)
	{
		fprintf(fc,"\n%d \t %d",i,voiceHopDistribution[i]);
	}//fc corresponds to hop distribution.txt

	for(i=0;i<numOfNodesInNetwork;i++)
	{
		fprintf(fn,"\n %d \t %lf", i, (double) nodeStat[i].upTime + (double)endSimTime * (double)((NO_OF_CONTROL_SLOTS * CONTROL_SLOT_DURATION + NO_OF_CONTENTION_SLOTS * CONTENTION_SLOT_DURATION) / FRAME_DURATION));
	}//fn corresponds to nodeUptime.txt

	fclose(fp);
	fclose(fq);	
	fclose(fr);
	fclose(fv);
	fclose(fc);
	fclose(fn);
	
	//manan change
	/*
	for(i=0;i<(numOfNodesInNetwork);i++)
	fprintf(stderr,"Parent Node %d Child Node %d\n",routingTree[2*i+1],routingTree[2*i]);

	fprintf(stderr,"\n Printing Routing Tree \n");
	for(i=0; i<(2*numOfNodesInTree); i++)
	fprintf(stderr,"%d ",routingTree[i]);	
	fprintf(stderr,"\n");
	*/
	
	for(i=0;i<numOfFlowsInProgress;i++)
	fprintf(stderr,"FLow id %d source %d destination %d\n",flowStat[i].flowId,flowStat[i].source,flowStat[i].destination);
	//manan end

	
	for(i=0; i<numVoiceMsg; i++)
	{
	fprintf(voice_log,"flowId %d source %d destination %d start_time %lf end_time %lf hops %d revoked %d fragmented %d\n",voiceFlowStat[i].voiceFlowId, voiceFlowStat[i].source, voiceFlowStat[i].destination, voiceFlowStat[i].start_time, voiceFlowStat[i].end_time, voiceFlowStat[i].hops, voiceFlowStat[i].revoked, voiceFlowStat[i].fragmented+1);
	}

	//portability changes start
	/*
	for(i=0;i<numOfNodesInNetwork;i++)
	{
		fprintf(stderr,"\n TP sent %d TP received %d maxQueueSize %d",nodeID[i].numOfTopologyUpdatesSent, nodeID[i].numOfTopologyUpdatesReceived, nodeID[i].maxQueueSize);
		
	}
	*/
	//portability changes end

	fprintf(stderr,"\n countNodesFlow %d\n", countNodesFlow);
	
	fprintf(stderr,"\n FlowRequestReception %d\n", flowRequestReceptionCounter);
	
	
	//fprintf(stderr,"\n No of flows terminated %d",flowDropDueToE2EPacketLosses);
	
	fprintf(stderr,"\n closing down \n");
	
	fclose(dump);
	fclose(vm);
	fclose(voice_end);
	fclose(voice_start);
	return SUCCESS;
	
}
