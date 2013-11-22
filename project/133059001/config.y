%{
#include "config.h"

extern FILE * yyin;

void yyerror(const char *str)
{
	fprintf(stderr, "Error: %s\n", str);
}

int yywrap()
{
	return 1;
}

void set_var_num(char *var, number num)
{
	if (strcmp(var, "RAND_INIT") == 0) config.rand_init = num.type ? num.d : num.l;
	else if (strcmp(var, "MAX_FLOW_RETRY") == 0) config.max_flow_retry = num.type ? num.d : num.l;
	else if (strcmp(var, "MAX_PACKET_SIZE") == 0) config.max_packet_size = num.type ? num.d : num.l;
	else if (strcmp(var, "PACKET_TX_TIME") == 0) config.packet_tx_time = num.type ? num.d : num.l;
	else if (strcmp(var, "ACK_TIMEOUT") == 0) config.ack_timeout = num.type ? num.d : num.l;
	else if (strcmp(var, "DATA_RATE") == 0) config.data_rate = num.type ? num.d : num.l;
	else if (strcmp(var, "CONTROL_SLOT_DURATION") == 0) config.control_slot_duration = num.type ? num.d : num.l;
	else if (strcmp(var, "CONTENTION_SLOT_DURATION") == 0) config.contention_slot_duration = num.type ? num.d : num.l;
	else if (strcmp(var, "DATA_SLOT_DURATION") == 0) config.data_slot_duration = num.type ? num.d : num.l;
	else if (strcmp(var, "NO_OF_SLOTS") == 0) config.no_of_slots = num.type ? num.d : num.l;
	else if (strcmp(var, "NO_OF_CONTROL_SLOTS") == 0) config.no_of_control_slots = num.type ? num.d : num.l;
	else if (strcmp(var, "NO_OF_CONTENTION_SLOTS") == 0) config.no_of_contention_slots = num.type ? num.d : num.l;
	else if (strcmp(var, "NO_OF_DATA_SLOTS") == 0) config.no_of_data_slots = num.type ? num.d : num.l;
	else if (strcmp(var, "FRAME_DURATION") == 0) config.frame_duration = num.type ? num.d : num.l;
	else if (strcmp(var, "DATA_PACKET_SIZE") == 0) config.data_packet_size = num.type ? num.d : num.l;
	else if (strcmp(var, "LINK_LEVEL_ACK_SIZE") == 0) config.link_level_ack_size = num.type ? num.d : num.l;
	else yyerror(var);
}

void set_var_string(char *var, char* string)
{
	if (strcmp(var, "CallAcceptedFile") == 0) config.callacceptedfile = string;
	else if (strcmp(var, "FlowsFilePrefix") == 0) config.flowsfileprefix = string;
	else if (strcmp(var, "FlowStartTimesFile") == 0) config.flowstarttimesfile = string;
	else if (strcmp(var, "NodeLogFilePrefix") == 0) config.nodelogfileprefix = string;
	else if (strcmp(var, "StoreCapFilePrefix") == 0) config.storecapfileprefix = string;
	else if (strcmp(var, "GlobalFlowFilePrefix") == 0) config.globalflowfileprefix = string;
	else if (strcmp(var, "MobilityFilePrefix") == 0) config.mobilityfileprefix = string;
	else if (strcmp(var, "ChannelInfoFile") == 0) config.channelinfofile = string;
	else if (strcmp(var, "SimulatedCallsFile") == 0) config.simulatedcallsfile = string;
	else if (strcmp(var, "AdmittedCallsFile") == 0) config.admittedcallsfile = string;
	else if (strcmp(var, "ScheduleInfoFile") == 0) config.scheduleinfofile = string;
	else if (strcmp(var, "ResultsFile") == 0) config.resultsfile = string;
	else if (strcmp(var, "CallsDistributionFile") == 0) config.callsdistributionfile = string;
	else if (strcmp(var, "HopDistributionFile") == 0) config.hopdistributionfile = string;
	else if (strcmp(var, "NodeUpTimeFile") == 0) config.nodeuptimefile = string;
	else if (strcmp(var, "EventQueueDumpFile") == 0) config.eventqueuedumpfile = string;
	else if (strcmp(var, "VoiceMessageFile") == 0) config.voicemessagefile = string;
	else if (strcmp(var, "FlowSequenceFile") == 0) config.flowsequencefile = string;
	else if (strcmp(var, "AutoTearDownFile") == 0) config.autoteardownfile = string;
	else if (strcmp(var, "VoiceEndFile") == 0) config.voiceendfile = string;
	else if (strcmp(var, "VoiceStartFile") == 0) config.voicestartfile = string;
	else if (strcmp(var, "VoiceLogFile") == 0) config.voicelogfile = string;
	else yyerror(var);
	
}

number var_name_to_num(char *var)
{
	number num = {0, 0, 0};
	if (strcmp(var, "RAND_INIT") == 0) { num.type = 0; num.d = (num.l = config.rand_init); }
	else if (strcmp(var, "MAX_FLOW_RETRY") == 0) { num.type = 0; num.d = (num.l = config.max_flow_retry); }
	else if (strcmp(var, "MAX_PACKET_SIZE") == 0) { num.type = 0; num.d = (num.l = config.max_packet_size); }
	else if (strcmp(var, "PACKET_TX_TIME") == 0) { num.type = 0; num.d = (num.l = config.packet_tx_time); }
	else if (strcmp(var, "ACK_TIMEOUT") == 0) { num.type = 1; num.d = config.ack_timeout; }
	else if (strcmp(var, "DATA_RATE") == 0) { num.type = 0; num.d = ( num.l = config.data_rate); }
	else if (strcmp(var, "CONTROL_SLOT_DURATION") == 0) { num.type = 1; num.d = config.control_slot_duration; }
	else if (strcmp(var, "CONTENTION_SLOT_DURATION") == 0) { num.type = 1; num.d = config.contention_slot_duration; }
	else if (strcmp(var, "DATA_SLOT_DURATION") == 0) { num.type = 1; num.d = config.data_slot_duration; }
	else if (strcmp(var, "NO_OF_SLOTS") == 0) { num.type = 0; num.d = ( num.l = config.no_of_slots); }
	else if (strcmp(var, "NO_OF_CONTROL_SLOTS") == 0) { num.type = 0; num.d = ( num.l = config.no_of_control_slots); }
	else if (strcmp(var, "NO_OF_CONTENTION_SLOTS") == 0) { num.type = 0; num.d = ( num.l = config.no_of_contention_slots); }
	else if (strcmp(var, "NO_OF_DATA_SLOTS") == 0) { num.type = 0; num.d = ( num.l = config.no_of_data_slots); }
	else if (strcmp(var, "FRAME_DURATION") == 0) { num.type = 1; num.d = config.frame_duration; }
	else if (strcmp(var, "DATA_PACKET_SIZE") == 0) { num.type = 0; num.d = ( num.l = config.data_packet_size); }
	else if (strcmp(var, "LINK_LEVEL_ACK_SIZE") == 0) { num.type = 0; num.d = ( num.l = config.link_level_ack_size); }
	else yyerror(var);

	return num;
}

char* var_name_to_string(char *var)
{
	if (strcmp(var, "CallAcceptedFile") == 0) return config.callacceptedfile;
	if (strcmp(var, "FlowsFilePrefix") == 0) return config.flowsfileprefix;
	if (strcmp(var, "FlowStartTimesFile") == 0) return config.flowstarttimesfile;
	if (strcmp(var, "NodeLogFilePrefix") == 0) return config.nodelogfileprefix;
	if (strcmp(var, "StoreCapFilePrefix") == 0) return config.storecapfileprefix;
	if (strcmp(var, "GlobalFlowFilePrefix") == 0) return config.globalflowfileprefix;
	if (strcmp(var, "MobilityFilePrefix") == 0) return config.mobilityfileprefix;
	if (strcmp(var, "ChannelInfoFile") == 0) return config.channelinfofile;
	if (strcmp(var, "SimulatedCallsFile") == 0) return config.simulatedcallsfile;
	if (strcmp(var, "AdmittedCallsFile") == 0) return config.admittedcallsfile;
	if (strcmp(var, "ScheduleInfoFile") == 0) return config.scheduleinfofile;
	if (strcmp(var, "ResultsFile") == 0) return config.resultsfile;
	if (strcmp(var, "CallsDistributionFile") == 0) return config.callsdistributionfile;
	if (strcmp(var, "HopDistributionFile") == 0) return config.hopdistributionfile;
	if (strcmp(var, "NodeUpTimeFile") == 0) return config.nodeuptimefile;
	if (strcmp(var, "EventQueueDumpFile") == 0) return config.eventqueuedumpfile;
	if (strcmp(var, "VoiceMessageFile") == 0) return config.voicemessagefile;
	if (strcmp(var, "FlowSequenceFile") == 0) return config.flowsequencefile;
	if (strcmp(var, "AutoTearDownFile") == 0) return config.autoteardownfile;
	if (strcmp(var, "VoiceEndFile") == 0) return config.voiceendfile;
	if (strcmp(var, "VoiceStartFile") == 0) return config.voicestartfile;
	if (strcmp(var, "VoiceLogFile") == 0) return config.voicelogfile;
	yyerror(var);
	return NULL;
}

void print(number num)
{
	if (num.type)
		printf("%lf", num.d);
	else
		printf("%ld", num.l);
}
                
%}

%union {
	struct number nval;
	char *sval, *kval;
}

%token <nval> NUMBER
%token <sval> KEY
%token <sval> STRING
%token IncludeCfg
%type <nval> expr

%left '+' '-' 
%left '*' '/'

%%

config: config config_line
	  |
	  ;

config_line: KEY '=' expr '\n'	
			{
				set_var_num($1, $3);
				printf("%s = ", $1);
				print($3);
				printf("\n");
			}
		   | KEY '=' '"' STRING '"' '\n'
			{ 
				set_var_string($1, $4);
				printf("%s = %s", $1, $4);
				printf("\n");
			}
		   ;

expr: '(' expr ')' 		{ $$ = $2; }
	| expr '+' expr 	
		{ 
			$$.type = $1.type || $3.type; 
			if (!$$.type) 
				$$.d = ($$.l = $1.l + $3.l); 
			else
				$$.d = $1.d + $3.d;
		}
	| expr '-' expr
		{ 
			$$.type = $1.type || $3.type; 
			if (!$$.type) 
				$$.d = ($$.l = $1.l - $3.l); 
			else
				$$.d = $1.d - $3.d;
		}
	| expr '*' expr
		{ 
			$$.type = $1.type || $3.type; 
			if (!$$.type) 
				$$.d = ($$.l = $1.l * $3.l); 
			else
				$$.d = $1.d * $3.d;
		}
	| expr '/' expr
		{ 
			$$.type = $1.type || $3.type; 
			if (!$$.type) 
				$$.d = ($$.l = $1.l / $3.l); 
			else
				$$.d = $1.d / $3.d;
		}
	| KEY				{ $$ = var_name_to_num($1); }
	| NUMBER 			{ $$ = $1; }
	;

%%             
int parse_config(void) 
{	
	config.rand_init = 007;
	config.max_flow_retry = 9;
	config.max_packet_size = 112;
	config.packet_tx_time = 2;
	config.ack_timeout = 0.4;
	config.data_rate = 250;
	config.control_slot_duration = 4.5;
	config.contention_slot_duration = 3;
	config.data_slot_duration = 3;
	config.no_of_slots = 19;
	config.no_of_control_slots = 2;
	config.no_of_contention_slots = 1;
	config.no_of_data_slots = (config.no_of_slots-(config.no_of_control_slots + config.no_of_contention_slots));
	config.frame_duration = (config.no_of_data_slots * config.data_slot_duration + config.no_of_control_slots * config.control_slot_duration + config.no_of_contention_slots * config.contention_slot_duration);
	config.data_packet_size = 45;
	config.link_level_ack_size = 2;
	config.callacceptedfile = "callAccepted.txt";
	config.flowsfileprefix = "flows/flow";
	config.flowstarttimesfile = "flowStartTimes.txt";
	config.nodelogfileprefix = "logs/node";;
	config.storecapfileprefix = "store_cap/node";
	config.globalflowfileprefix = "flows/globalFlow";
	config.mobilityfileprefix = "mobility/node";
	config.channelinfofile = "channelInfo.txt";
	config.simulatedcallsfile = "simCalls.txt";
	config.admittedcallsfile = "admitCalls.txt";
	config.scheduleinfofile = "scheduleInfo.txt";
	config.resultsfile = "results.txt";
	config.callsdistributionfile = "callsDistribution.txt";
	config.hopdistributionfile = "hopDistribution.txt";
	config.nodeuptimefile = "nodeUpTime.txt";
	config.eventqueuedumpfile = "event_queue_dump.txt";
	config.voicemessagefile = "voiceMsg.txt";
	config.flowsequencefile = "flowSequence.txt";
	config.autoteardownfile = "automatic_teardown.txt";
	config.voiceendfile = "Voice_End.txt";
	config.voicestartfile = "Voice_Start.txt";
	config.voicelogfile = "Voice_Log.txt";
	if ((yyin = fopen("lo3sim.cfg","r")) != NULL)
		yyparse(); 
	else
		fprintf(stderr, "Couldn't open lo3sim.cfg.\n");

	strcpy(config.folder_callacceptedfile, "outputFiles/");
	strcpy(config.folder_flowstarttimesfile, "outputFiles/");
	strcpy(config.folder_channelinfofile, "outputFiles/");
	strcpy(config.folder_simulatedcallsfile, "outputFiles/");
	strcpy(config.folder_admittedcallsfile, "outputFiles/");
	strcpy(config.folder_scheduleinfofile, "outputFiles/");
	strcpy(config.folder_resultsfile, "outputFiles/");
	strcpy(config.folder_callsdistributionfile, "outputFiles/");
	strcpy(config.folder_hopdistributionfile, "outputFiles/");
	strcpy(config.folder_nodeuptimefile, "outputFiles/");
	strcpy(config.folder_eventqueuedumpfile, "outputFiles/");
	strcpy(config.folder_voicemessagefile, "outputFiles/");
	strcpy(config.folder_flowsequencefile, "outputFiles/");
	strcpy(config.folder_autoteardownfile, "outputFiles/");
	strcpy(config.folder_voiceendfile, "outputFiles/");
	strcpy(config.folder_voicestartfile, "outputFiles/");
	strcpy(config.folder_voicelogfile, "outputFiles/");

	strcat(config.folder_callacceptedfile, config.callacceptedfile);
	strcat(config.folder_flowstarttimesfile, config.flowstarttimesfile);
	strcat(config.folder_channelinfofile, config.channelinfofile);
	strcat(config.folder_simulatedcallsfile, config.simulatedcallsfile);
	strcat(config.folder_admittedcallsfile, config.admittedcallsfile);
	strcat(config.folder_scheduleinfofile, config.scheduleinfofile);
	strcat(config.folder_resultsfile, config.resultsfile);
	strcat(config.folder_callsdistributionfile, config.callsdistributionfile);
	strcat(config.folder_hopdistributionfile, config.hopdistributionfile);
	strcat(config.folder_nodeuptimefile, config.nodeuptimefile);
	strcat(config.folder_eventqueuedumpfile, config.eventqueuedumpfile);
	strcat(config.folder_voicemessagefile, config.voicemessagefile);
	strcat(config.folder_flowsequencefile, config.flowsequencefile);
	strcat(config.folder_autoteardownfile, config.autoteardownfile);
	strcat(config.folder_voiceendfile, config.voiceendfile);
	strcat(config.folder_voicestartfile, config.voicestartfile);
	strcat(config.folder_voicelogfile, config.voicelogfile);
	fclose(yyin);

    return 0;  
}              
 
