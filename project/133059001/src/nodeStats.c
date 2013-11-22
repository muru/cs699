#include "header.h"

int startLoggingNodeStats()
{
	FILE *fp;
	char *retStr, *convStr, *nodeStr, *fileName;
	int i;

	nodeStr = (char *)malloc(15 * sizeof(char));
	nodeStr = strcpy(nodeStr, NodeLogFilePrefix);
	convStr = (char *)malloc(15 * sizeof(char));

	for(i=0;i<numOfNodesInNetwork;i++)
	{

		sprintf(convStr,"%d",i);
		fileName = strcat(nodeStr, convStr);

		//printf("%s \n",fileName);
		fp = fopen(fileName,"w+");  
		if(fp == NULL)
		{
			fprintf(stderr,"\n File can not be opened");
			exit(0);
		}  
		fclose(fp);
		nodeStr = strcpy(nodeStr, "logs/node"); // this step actually not needed
	}
	free(nodeStr);
	free(convStr);
}

int endLoggingNodeStats()
{
	FILE *fp;
	char *retStr, *convStr, *nodeStr, *fileName;
	int i;

	nodeStr = (char *)malloc(15 * sizeof(char));
	nodeStr = strcpy(nodeStr, NodeLogFilePrefix);

	convStr = (char *)malloc(15 * sizeof(char));
	for(i=0;i<numOfNodesInNetwork;i++)
	{
		sprintf(convStr,"%d",i);
		fileName = strcat(nodeStr, convStr);
		fp = fopen(fileName,"a+");
    
		if(fp == NULL)
		{
			fprintf(stderr,"\n File opening error: endLoggingNodeStats");
		    	exit(0);
		}
    
		//fprintf(fp,"\n Control Packet losses %d",nodeStat[i].numChannelControlPacketLosses);
		//fprintf(fp,"\n Control Packet drops %d",nodeStat[i].numQueueControlPacketDrops);
		fclose(fp);
		nodeStr = strcpy(nodeStr, NodeLogFilePrefix);
  	}  
	free(nodeStr);
	free(convStr);
}

int logNodeStats(int node, enum logType type, double value)
{
 
	FILE *fp;
	char *retStr, *convStr, *nodeStr, *fileName;
	int i;

	nodeStr = (char *)malloc(15 * sizeof(char));
	nodeStr = strcpy(nodeStr, NodeLogFilePrefix);
	convStr = (char *)malloc(15 * sizeof(char));
  
	sprintf(convStr,"%d",node);
	fileName = strcat(nodeStr,convStr);
	fp = fopen(fileName,"a+");

	if(fp == NULL)
    	{
    		fprintf(stderr,"\n File opening error %s: logNodeStats",fileName);
		//return;
    		exit(0);
    	}
   

  	if(type == TIME_SYNC)
    	{
      		fprintf(fp,"\n TIME_SYNC %lf",value);
    	}
  	if(type == TIME_JOIN_REQUEST)
    	{
      		fprintf(fp,"\n TIME_JOIN_REQUEST %lf SPONSOR-DEPTH %d SPONSOR-NODE %d",value,nodeID[node].sponsorDepth,nodeID[node].parent);
	}
  	if(type == TIME_JOIN_GRANT)
    	{
      		fprintf(fp,"\n TIME_JOIN_GRANT %lf",value);
    	}
  	if(type == TIME_FLOW_REQUEST)
    	{
      		fprintf(fp,"\n TIME_FLOW_REQUEST %lf",value);
    	}
  	if(type == TIME_FLOW_GRANT)
    	{
      		fprintf(fp,"\n TIME_FLOW_GRANT %lf",value);
    	}
  	if(type == SIZE_SCHEDULE)
  	{
  		fprintf(fp,"\n SIZE_SCHEDULE %d",(int)value);
  	}  
  	if(type == NUM_SCHEDULE_FRAGMENTS)
  	{
  		fprintf(fp,"\n NUM_SCHEDULE_FRAGMENTS %d",(int)value);
  	}
    	if(type == TIME_NODE_DEAD)
  	{
  		fprintf(fp,"\n TIME_NODE_DEAD %lf",value);
  	}
    	if(type == TIME_NODE_ORPHAN)
      	{
		fprintf(fp,"\n TIME_NODE_ORPHAN %lf",value);
      	}
      	if(type == TIME_SCHEDULE_DROP)
      	{
      		fprintf(fp,"\n TIME_SCHEDULE_DROP %lf",value);
      	}
  	fclose(fp);
	free(nodeStr);
	free(convStr);
}

int logFlowStats(int flowId, char *str, double value)
{
	FILE *fp;
	char *filename, *convStr, *flowStr;	
	flowStr = (char *)malloc(20 * sizeof(char));
	convStr = (char *)malloc(15 * sizeof(char));
	
	strcpy(flowStr, FlowsFilePrefix);
	sprintf(convStr,"%d",flowId);
	filename = strcat(flowStr,convStr);

	fp = fopen(filename,"a+");
	if(fp == NULL)
    	{
    		fprintf(stderr,"\n File opening error: logFlowStats");
    		exit(0);
    	}
	
	fprintf(fp,"%s %lf",str,(double)value);
	fclose(fp);
	free(flowStr);
	free(convStr);
}

//manan change
int logVoiceFlowStats(int flowId, char *str, double value)
{
	FILE *fp;
	char *filename, *convStr, *flowStr;	
	flowStr = (char *)malloc(20 * sizeof(char));
	convStr = (char *)malloc(15 * sizeof(char));
	
	strcpy(flowStr, FlowsFilePrefix);
	sprintf(convStr,"%d",flowId);
	filename = strcat(flowStr,convStr);

	fp = fopen(filename,"a+");
	
	if(fp == NULL)
    	{
    		fprintf(stderr,"\n File opening error: logFlowStats");
    		exit(0);
    	}
	
	fprintf(fp,"%s %lf",str,(double)value);
	
	fclose(fp);
	free(flowStr);
	free(convStr);
}

int logStoreCap(int node, long int store_cap, double time)
{
	FILE *fp;
	char *filename, *convStr, *nodeStr;	
	nodeStr = (char *)malloc(25 * sizeof(char));
	convStr = (char *)malloc(15 * sizeof(char));
	
	strcpy(nodeStr, StoreCapFilePrefix);
	sprintf(convStr,"%d",node);
	filename = strcat(nodeStr,convStr);

	fp = fopen(filename,"a+");
	
	if(fp == NULL)
    	{
    		fprintf(stderr,"\n File opening error: logStoreCap");
    		exit(0);
    	}
	
	fprintf(fp,"%ld \t %lf\n",store_cap,time);
	
	fclose(fp);
	free(nodeStr);
	free(convStr);
}
//manan end

int recordFlow(int flowId)
{
	FILE *fp,*fq;
	char letter;
	char *filename, *convStr, *flowStr;	
	flowStr = (char *)malloc(25 * sizeof(char));
	convStr = (char *)malloc(15 * sizeof(char));
	
	strcpy(flowStr, FlowsFilePrefix);
	sprintf(convStr,"%d",flowId);
	filename = strcat(flowStr,convStr);

	fp = fopen(filename,"r");

	strcpy(flowStr, GlobalFlowFilePrefix);
	sprintf(convStr,"%d",globalLogFlowId);
	filename = strcat(flowStr,convStr);	

	fq = fopen(filename,"w+");
	
	while(1)
	{
		letter = fgetc(fp);
		fprintf(fq,"%c",letter);
		if(letter == EOF)
		{
			break;
		}
	}	
	fclose(fp);
	fclose(fq);
}

//mobility change 3-5-10
void logMobilityStats(enum logType type, int node, int infra, double time)
{
	FILE *fp, *fq, *fr;
	char *convStr, *nodeStr, *fileName, *file_parent, *file_path;
	int i, flowId, src, dst;
	struct routingPath * aPath;

	nodeStr = (char *)malloc(20 * sizeof(char));
	nodeStr = strcpy(nodeStr, MobilityFilePrefix);
	convStr = (char *)malloc(15 * sizeof(char));
	file_parent = (char *)malloc(30 * sizeof(char));
	file_path = (char *)malloc(30 * sizeof(char));
	
	sprintf(convStr,"%d",node);
	fileName = strcat(nodeStr,convStr);
	fp = fopen(fileName,"a+");

	flowId = -1;

	if(fp == NULL)
    	{
    		fprintf(stderr,"\n File opening error %s: logMobilityStats",fileName);
    		exit(0);
    	}
   
  	if(type == EXPECTING_PARENT_CHANGE)
    	{
      		fprintf(fp,"\n\n\n Expecting parent change set to true for node %d with respect to its parent %d at time %lf ", node, infra, time);
		
		sprintf(file_parent,"%s_parent",fileName);
		fq = fopen(file_parent,"a+");

		if(fq == NULL)
    		{
    		fprintf(stderr,"\n File opening error %s: logMobilityStats",file_parent);
    		exit(0);
    		}

		fprintf(fq,"\n\nEXPECTING_PARENT_CHANGE %lf", time);
		fclose(fq);
		
    	}
  	if(type == EXPECTING_PATH_CHANGE)
    	{
		//find out the flow id and the path
		for(i=0; i < nodeID[ROOT].numSchedElem; i++)
		{
			if(globalScheduleElements[i].transmitter == node && globalScheduleElements[i].receiver == infra)
			{
				flowId = EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel);
				break;
			}
		}
		
		if(flowId == -1)
		{
		
		}
		else
		{
		
		for(i=0; i < numOfFlowsInProgress; i++)
		{
			if(flowStat[i].flowId == flowId)
			{
				aPath = flowStat[i].aPath;
				src = flowStat[i].source;
				dst = flowStat[i].destination;
				break;
			}
		}

		fprintf(fp,"\n\n\n\n  Expecting path change for node %d set at time %lf ", node, time);
      		fprintf(fp,"\nFlowid is %d, src is %d, destination is %d ", flowId, src, dst);
		fprintf(fp,"\n Old path is  : ");
		for(i=0; i < aPath->numOfNodesInPath; i++)
		{
			fprintf(fp," %d ", aPath->path[i]);
		}
			
		sprintf(file_path,"%s_path",fileName);
		fr = fopen(file_path,"a+");
		
		if(fr == NULL)
    		{
    		fprintf(stderr,"\n File opening error %s: logMobilityStats",file_path);
    		exit(0);
    		}

		fprintf(fr,"\n\nEXPECTING_PATH_CHANGE %lf", time);
		fclose(fr);
		}
	}
  	if(type == NEW_PARENT_RECEIVED)
    	{
      		fprintf(fp,"\n New parent received for node %d, the new parent is %d , change received at time %lf ", node, infra, time);

		sprintf(file_parent,"%s_parent",fileName);
		fq = fopen(file_parent,"a+");

		if(fq == NULL)
    		{
    		fprintf(stderr,"\n File opening error %s: logMobilityStats",fileName);
    		exit(0);
    		}

		fprintf(fq," NEW_PARENT_RECEIVED %lf", time);
		fclose(fq);
    	}
  	if(type == NEW_PATH_RECEIVED)
    	{
      		//find out the flow id and the path
		for(i=0; i < nodeID[ROOT].numSchedElem; i++)
		{
			if(globalScheduleElements[i].transmitter == node && globalScheduleElements[i].receiver == infra)
			{
				flowId = EXTRACT_FLOWID(globalScheduleElements[i].flowId_channel);
				break;
			}
		}

		if(flowId == -1)
		{

		}
		else
		{
		for(i=0; i < numOfFlowsInProgress; i++)
		{
			if(flowStat[i].flowId == flowId)
			{
				aPath = flowStat[i].aPath;
				src = flowStat[i].source;
				dst = flowStat[i].destination;
				break;
			}
		}

		fprintf(fp,"\n\n New path received for node %d at time %lf ", node, time);
      		fprintf(fp,"\nFlowid is %d, src is %d, destination is %d ", flowId, src, dst);
		fprintf(fp,"\n New path is  : ");
		for(i=0; i < aPath->numOfNodesInPath; i++)
		{
			fprintf(fp," %d ", aPath->path[i]);
		}

		sprintf(file_path,"%s_path",fileName);
		fr = fopen(file_path,"a+");

		if(fr == NULL)
    		{
    		fprintf(stderr,"\n File opening error %s: logMobilityStats",fileName);
    		exit(0);
    		}

		fprintf(fr," NEW_PATH_RECEIVED %lf", time);
		fclose(fr);
		}
    	}
	if(type == HANDOFF_FAIL)
    	{
      		fprintf(fp," \n Handoff failed for node %d with respect to flowid %d at time %lf \n", node, infra, time);
	}
	
	fclose(fp);
	free(nodeStr);
	free(convStr);
	free(file_parent);
}
