#include "header.h"

int networkGraph[MAX_NO_NODES][MAX_NO_NODES];
int incomingEdge[MAX_NO_NODES];

//takes node co-ordinates through coordinateFile.txt
//builds network graph in networkGraph.txt

int parseCoordinateFile()
{
	FILE *fp, *fq;
	int node, currentNode;
	char currentType;
	double currentX, currentY;
	int scanNode;
	char scanType;
	double scanX, scanY;
	int currentNodeFlag;
	char ch;
	double transmissionRange, interferenceRange;
	//double transmissionRangeI = (double) 250,interferenceRangeI = (double) 350;
	//double transmissionRangeM = (double) 150,interferenceRangeM = (double) 250;
	double distance;
		
	int connectivityMatrix[MAX_NO_NODES][MAX_NO_NODES];
	
	int i,j;
	
	for(i=0;i<MAX_NO_NODES;i++)
	{
		incomingEdge[i] = 0;
	}
	
	for(i=0;i<MAX_NO_NODES;i++)
	{
		for(j=0;j<MAX_NO_NODES;j++)
		{
			networkGraph[i][j] = UNDEFINED;
		}
	}
	
	
	fp = fopen("coordinateFile.txt", "r");
	fq = fopen("networkGraph.txt", "w+");
	currentNode = 0;
	
	while(currentNode < MAX_NO_NODES)
	{
		rewind(fp);
		currentNodeFlag = 0;
		while((ch=fgetc(fp)) != EOF)
		{
			if(ch == '(')
			{
				fscanf(fp,"%d",&node);
				
				if(currentNode == node)
				{
					currentNodeFlag = 1; //once set, this variable remains on till the entire coordinate.txt file has been scanned establishing relationships between this node and the rest of the nodes in the network
					fgetc(fp);
					fscanf(fp,"%c", &currentType);
					fgetc(fp);
					fscanf(fp,"%lf", &currentX);
					fgetc(fp);
					fscanf(fp,"%lf", &currentY);										
					//************ VJ change start ************
					nodeLocation[currentNode].x = currentX;
					nodeLocation[currentNode].y = currentY;
					//************ VJ change end ************

					//printf("\n current node %d type %c x %lf y %lf",node, currentType, currentX, currentY);

					if(currentType == 'I')
					{
						transmissionRange = transmissionRangeI;
						interferenceRange = interferenceRangeI;	
					}
					else if(currentType == 'M')
					{
						transmissionRange = transmissionRangeM;
						interferenceRange = interferenceRangeM;
					}
					else
					{
					
					}
					rewind(fp);
					
					while((ch=fgetc(fp)) != EOF) // For each given node, this while loop traverses the entire coordinatefile.txt establishing relationships between the current node and the rest of the nodes in the network
					{
						if(ch == '(')
						{
							fscanf(fp, "%d", &scanNode);
							fgetc(fp);
							fscanf(fp,"%c", &scanType);
							fgetc(fp);
							fscanf(fp, "%lf", &scanX);
							fgetc(fp);
							fscanf(fp, "%lf", &scanY);
							
							//printf("\n scan node %d type %c x %lf y %lf",scanNode, scanType, scanX, scanY);
							if(scanType == 'I')
							{
								transmissionRange = transmissionRangeI;
								interferenceRange = interferenceRangeI;
							}
							
							if(scanNode == currentNode)
							{
								connectivityMatrix[currentNode][scanNode] = 0;
								networkGraph[currentNode][scanNode] = 0;
								continue;
							}
							
							distance = (double)sqrt(pow(scanX - currentX, 2) + pow(scanY - currentY, 2));
							
							//printf("\n distance (%d,%d) %lf",currentNode, scanNode, distance);
							
							if(distance <= transmissionRange)
							{
								connectivityMatrix[currentNode][scanNode] = 1;
								if(scanType == 'M' && currentType == 'M')
									networkGraph[currentNode][scanNode] = UNDEFINED;
								else
								{
									networkGraph[currentNode][scanNode] = 1;
									incomingEdge[currentNode]++;
								}	
							}
							else if(distance <= interferenceRange)
							{
								connectivityMatrix[currentNode][scanNode] = 2;
								networkGraph[currentNode][scanNode] = UNDEFINED;
							}
							else
							{
								connectivityMatrix[currentNode][scanNode] = 0;
								networkGraph[currentNode][scanNode] = UNDEFINED;
							}
						}	
					}					
				}			
			}
		}	
		
		if(currentNodeFlag == 0) // this if condition will come to be false only when for the last node also every relationship is established and now there is no other node left to match
		{
			break;
		}
		else
		{
			currentNode++;
		}
	}
	
//so i think the above while structure basically passes through the file coordinatefile.txt twice and establishes relationships between the various nodes

	currentNode = 0;
	while(currentNode < MAX_NO_NODES) // now this while loop is for writing the output to networkgraph.txt in the form of first the node type and then a series of 0s and 1s specifying the relationship of the given node with the other nodes in the network
	{
		rewind(fp);
		currentNodeFlag = 0;
		while((ch=fgetc(fp)) != EOF)
		{
			if(ch == '(')
			{
				fscanf(fp,"%d",&node);
				if(currentNode == node)
				{
					currentNodeFlag = 1;
					fgetc(fp);
					fscanf(fp,"%c", &currentType);
					
					fprintf(fq,"%c ",currentType);
					
					for(i=0; i<MAX_NO_NODES; i++)
					{
						fprintf(fq,"%d ",connectivityMatrix[currentNode][i]);
					}
				}
			}
		}
		if(currentNodeFlag == 0)
			break;
		else
		{
			fprintf(fq,"\n");
			currentNode++;
		}	
	}

	fclose(fp);
	fclose(fq);				
}
