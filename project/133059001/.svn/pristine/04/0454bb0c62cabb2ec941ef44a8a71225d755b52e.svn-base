#ifndef __CONFIG_H
#define __CONFIG_H 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct number
{		
	int type;
	double d;
	long l;
} number; 

struct number nsym[64];
char* ssym[64];

typedef enum NUM_VARS {
	eRAND_INIT = 1,
	eMAX_FLOW_RETRY,
	eMAX_PACKET_SIZE,
	ePACKET_TX_TIME,
	eACK_TIMEOUT,
	eDATA_RATE,
	eCONTROL_SLOT_DURATION,
	eCONTENTION_SLOT_DURATION,
	eDATA_SLOT_DURATION,
	eNO_OF_SLOTS,
	eNO_OF_CONTROL_SLOTS,
	eNO_OF_CONTENTION_SLOTS,
	eNO_OF_DATA_SLOTS,
	eFRAME_DURATION,
	eDATA_PACKET_SIZE,
	eLINK_LEVEL_ACK_SIZE,
} NVARS;

typedef enum eSVARS {
	eCallAcceptedFile = 1,
	eFlowsFilePrefix,
	eFlowStartTimesFile,
	eNodeLogFilePrefix,
	eStoreCapFilePrefix,
	eGlobalFlowFilePrefix,
	eMobilityFilePrefix,
	eChannelInfoFile,
	eSimulatedCallsFile,
	eAdmittedCallsFile,
	eScheduleInfoFile,
	eResultsFile,
	eCallsDistributionFile,
	eHopDistributionFile,
	eNodeUpTimeFile,
	eEventQueueDumpFile,
	eVoiceMessageFile,
	eFlowSequenceFile,
	eAutoTearDownFile,
	eVoiceEndFile,
	eVoiceStartFile,
	eVoiceLogFile,
} SVARS;
NVARS var_name_to_nenum(char *);
SVARS var_name_to_Senum(char *);

#endif
