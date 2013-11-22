#ifndef __CONFIG_H
#define __CONFIG_H 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct number
{		
	int type; // = 0 for long/int, 1 for double/float
	double d;
	long l;
} number; 

typedef struct config
{	
	long rand_init;
	long max_flow_retry;
	long max_packet_size;
	long packet_tx_time;
	double ack_timeout;
	long data_rate;
	double control_slot_duration;
	double contention_slot_duration;
	double data_slot_duration;
	long no_of_slots;
	long no_of_control_slots;
	long no_of_contention_slots;
	long no_of_data_slots;
	double frame_duration;
	long data_packet_size;
	long link_level_ack_size;
	char* callacceptedfile;
	char folder_callacceptedfile[50];
	char* flowsfileprefix;
	char* flowstarttimesfile;
	char folder_flowstarttimesfile[50];
	char* nodelogfileprefix;
	char* storecapfileprefix;
	char* globalflowfileprefix;
	char* mobilityfileprefix;
	char* channelinfofile;
	char folder_channelinfofile[50];
	char* simulatedcallsfile;
	char folder_simulatedcallsfile[50];
	char* admittedcallsfile;
	char folder_admittedcallsfile[50];
	char* scheduleinfofile;
	char folder_scheduleinfofile[50];
	char* resultsfile;
	char folder_resultsfile[50];
	char* callsdistributionfile;
	char folder_callsdistributionfile[50];
	char* hopdistributionfile;
	char folder_hopdistributionfile[50];
	char* nodeuptimefile;
	char folder_nodeuptimefile[50];
	char* eventqueuedumpfile;
	char folder_eventqueuedumpfile[50];
	char* voicemessagefile;
	char folder_voicemessagefile[50];
	char* flowsequencefile;
	char folder_flowsequencefile[50];
	char* autoteardownfile;
	char folder_autoteardownfile[50];
	char* voiceendfile;
	char folder_voiceendfile[50];
	char* voicestartfile;
	char folder_voicestartfile[50];
	char* voicelogfile;
	char folder_voicelogfile[50];
} configs;

configs config;

#ifdef __HEADER_H
#undef RAND_INIT
#undef MAX_PACKET_SIZE
#undef MAX_FLOW_RETRY
#undef PACKET_TX_TIME
#undef ACK_TIMEOUT
#undef DATA_RATE
#undef CONTROL_SLOT_DURATION
#undef CONTENTION_SLOT_DURATION
#undef DATA_SLOT_DURATION
#undef NO_OF_SLOTS
#undef NO_OF_CONTROL_SLOTS
#undef NO_OF_CONTENTION_SLOTS
#undef NO_OF_DATA_SLOTS
#undef FRAME_DURATION
#undef DATA_PACKET_SIZE
#undef LINK_LEVEL_ACK_SIZE

#undef FolderCallAcceptedFile
#undef CallAcceptedFile
#undef FlowsFilePrefix
#undef FolderFlowStartTimesFile
#undef FlowStartTimesFile
#undef NodeLogFilePrefix
#undef StoreCapFilePrefix
#undef GlobalFlowFilePrefix
#undef MobilityFilePrefix
#undef FolderChannelInfoFile
#undef ChannelInfoFile
#undef FolderSimulatedCallsFile
#undef SimulatedCallsFile
#undef FolderAdmittedCallsFile
#undef AdmittedCallsFile
#undef FolderScheduleInfoFile
#undef ScheduleInfoFile
#undef FolderResultsFile
#undef ResultsFile
#undef FolderCallsDistributionFile
#undef CallsDistributionFile
#undef FolderHopDistributionFile
#undef HopDistributionFile
#undef FolderNodeUpTimeFile
#undef NodeUpTimeFile
#undef FolderEventQueueDumpFile
#undef EventQueueDumpFile
#undef FolderVoiceMessageFile
#undef VoiceMessageFile
#undef FolderFlowSequenceFile
#undef FlowSequenceFile
#undef FolderAutoTearDownFile
#undef AutoTearDownFile
#undef FolderVoiceEndFile
#undef VoiceEndFile
#undef FolderVoiceStartFile
#undef VoiceStartFile
#undef FolderVoiceLogFile
#undef VoiceLogFile
#endif

#define MAX_FLOW_RETRY (config.max_flow_retry)
#define MAX_PACKET_SIZE (config.max_packet_size)
#define PACKET_TX_TIME (config.packet_tx_time)
#define ACK_TIMEOUT (config.ack_timeout)
#define DATA_RATE (config.data_rate)
#define CONTROL_SLOT_DURATION (config.control_slot_duration)
#define CONTENTION_SLOT_DURATION (config.contention_slot_duration)
#define DATA_SLOT_DURATION (config.data_slot_duration)
#define NO_OF_SLOTS (config.no_of_slots)
#define NO_OF_CONTROL_SLOTS (config.no_of_control_slots)
#define NO_OF_CONTENTION_SLOTS (config.no_of_contention_slots)
#define NO_OF_DATA_SLOTS (config.no_of_data_slots)
#define FRAME_DURATION (config.frame_duration)
#define DATA_PACKET_SIZE (config.data_packet_size)
#define LINK_LEVEL_ACK_SIZE (config.link_level_ack_size)

#define FolderCallAcceptedFile (config.folder_callacceptedfile)
#define CallAcceptedFile (config.callacceptedfile)
#define FlowsFilePrefix (config.flowsfileprefix)
#define FolderFlowStartTimesFile (config.folder_flowstarttimesfile)
#define FlowStartTimesFile (config.flowstarttimesfile)
#define NodeLogFilePrefix (config.nodelogfileprefix)
#define StoreCapFilePrefix (config.storecapfileprefix)
#define GlobalFlowFilePrefix (config.globalflowfileprefix)
#define MobilityFilePrefix (config.mobilityfileprefix)
#define FolderChannelInfoFile (config.folder_channelinfofile)
#define ChannelInfoFile (config.channelinfofile)
#define FolderSimulatedCallsFile (config.folder_simulatedcallsfile)
#define SimulatedCallsFile (config.simulatedcallsfile)
#define FolderAdmittedCallsFile (config.folder_admittedcallsfile)
#define AdmittedCallsFile (config.admittedcallsfile)
#define FolderScheduleInfoFile (config.folder_scheduleinfofile)
#define ScheduleInfoFile (config.scheduleinfofile)
#define FolderResultsFile (config.folder_resultsfile)
#define ResultsFile (config.resultsfile)
#define FolderCallsDistributionFile (config.folder_callsdistributionfile)
#define CallsDistributionFile (config.callsdistributionfile)
#define FolderHopDistributionFile (config.folder_hopdistributionfile)
#define HopDistributionFile (config.hopdistributionfile)
#define FolderNodeUpTimeFile (config.folder_nodeuptimefile)
#define NodeUpTimeFile (config.nodeuptimefile)
#define FolderEventQueueDumpFile (config.folder_eventqueuedumpfile)
#define EventQueueDumpFile (config.eventqueuedumpfile)
#define FolderVoiceMessageFile (config.folder_voicemessagefile)
#define VoiceMessageFile (config.voicemessagefile)
#define FolderFlowSequenceFile (config.folder_flowsequencefile)
#define FlowSequenceFile (config.flowsequencefile)
#define FolderAutoTearDownFile (config.folder_autoteardownfile)
#define AutoTearDownFile (config.autoteardownfile)
#define FolderVoiceEndFile (config.folder_voiceendfile)
#define VoiceEndFile (config.voiceendfile)
#define FolderVoiceStartFile (config.folder_voicestartfile)
#define VoiceStartFile (config.voicestartfile)
#define FolderVoiceLogFile (config.folder_voicelogfile)
#define VoiceLogFile (config.voicelogfile)

#endif
