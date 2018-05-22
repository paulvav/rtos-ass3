/*
Name: Paul Vavich ID: 11685726
This program demonstrates the usage of the deadlock algorithm.
Please provide an input file in the following format:

Process	Allocation	Request	Available
		A B C		A B C	A B C
P0		0 1 0		0 1 2	0 1 2
P1		2 0 0		2 0 2   
P2		3 0 3		0 0 5  

Output will be a file with the processes causing a deadlock
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Structure defining Resources, Spit into two arrays for Allocation and Request

struct resource
{
	int Allo[3],Req[3];
	char id[2];
	int done;  	               //done flag
};


//initialising array for current available resources
int avail[3];

//initialising input file
FILE * fileName;

//initialising output file
FILE * outputFile;

//initialise functions
int numberOfLines();
void getProcessFromFile();
int deadlockDetector(struct resource * process);

//MAIN opens files according to user input, then calls the getProcess function
int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		fprintf(stderr, "USAGE: Inputfile Outputfile\n");
		return-1;
	}

	fileName = fopen(argv[1],"r");
	outputFile = fopen(argv[2],"w");
	fclose(outputFile);
	outputFile = fopen(argv[2],"a");

getProcessFromFile();

}

//numberOfLines returns the number of processes in the input file - allows for different file types
int numberOfLines()
{
	//initialise number of lines variable
	int lines =0;
	//initialise char for \n detection
	char ch;

	while(!feof(fileName))
		{
  		ch = fgetc(fileName);
  		
		//increment number of lines if new line detected
		if(ch == '\n')
    			lines++;
		}
	//reset file
	rewind(fileName);

	//remove header lines from line calulation
	lines = lines -2;
	return lines;
}

//getProcessFromFile() loads a struct with all the process information from a txt file
void getProcessFromFile()
{
	//Char buffer to hold line from file
	char lineFromFile[100];

	//variable used for detection of first process
	int initialise = 0;
	
	//int used to iterated through processes
	int proc = 0;

	//Amount of proceses
        int linestotal = numberOfLines();

	//Initialising process struct
	struct resource process[linestotal];
	
	while(fgets(lineFromFile,1000,fileName))
        {			
	
		//if first process has already been found, load the line into the struct array
		if (initialise==1)
		{
			sscanf(lineFromFile,"%s%d%d%d%d%d%d",process[proc].id,&process[proc].Allo[0],&process[proc].Allo[1],&process[proc].Allo[2],&process[proc].Req[0],&process[proc].Req[1],&process[proc].Req[2]);	
			process[0].done = 0;
			proc = proc + 1;
		}		
		
		//if first process is found, load first line and set initialise flag
	        if (strstr(lineFromFile,"P0"))
        	{
          		sscanf(lineFromFile,"%s%d%d%d%d%d%d%d%d%d",process[proc].id,&process[proc].Allo[0],&process[proc].Allo[1],&process[proc].Allo[2],&process[proc].Req[0],&process[proc].Req[1],&process[proc].Req[2],&avail[0],&avail[1],&avail[2]);
	  
	 	 	process[proc].done=0;
	  		proc = proc + 1;
          		initialise = 1;
        	}	
	}
	
	rewind(fileName);
	
	//call deadlock algorithm and pass the struct array of processes
	deadlockDetector(process);

return;
}


//deadlockDetector takes a struct array of resources and detects if the process will cause a 
//deadlock. If there is a deadlock deadlockDetector will output to the output file as specified by the user

int deadlockDetector(struct resource * process)
{
	int finish = 0;
	int doneCount = 0;
	int previousDoneCount = 0;
	int proc = 0;
	int resourceID;
	int unAvail;
        int numOfLines = numberOfLines();
	
	//Loop runs until finish flag is set
	while(1)
	{
		//reset resource unavailable flag to 0
		unAvail = 0;
		
		//if the process is not done run calulation to determind if it can be 	
		if(process[proc].done==0)
		{

			//Check if there are enough available resources for process		
			for(resourceID = 0;resourceID<3;resourceID++)
			{
				//if there aren't enough of any of the three resources set the unavailable flag
				if(process[proc].Req[resourceID] > avail[resourceID])
					unAvail = 1;		
			}

			//if there are enough resources to run the process
			if(unAvail==0)
			{
				//set done flag
				process[proc].done = 1;
				
				//add allocation resources to available
				for(resourceID = 0; resourceID<3;resourceID++)
				{
					avail[resourceID] = avail[resourceID] + process[proc].Allo[resourceID];
				}
			}
		}
			
		//if the process is done increment done count
		else
		{
			doneCount = doneCount + 1;
		}

		//if all processes have been checked, check for completion
		if(proc==numOfLines)
		{
			//No deadlock
			if(doneCount == 0)
				printf("NO DEADLOCK");

			//if done count has been the same twice - indicates completion
			else if(doneCount == previousDoneCount)
			{
				fprintf(outputFile, "\nDEADLOCK DETECTED: ");
				
				//print processes in deadlock to file
				for(proc = 0; proc < numOfLines; proc++)
				{
					if(process[proc].done==0)
						fprintf(outputFile," %s",process[proc].id);
				
				}
			break;
			}
			//set previous done count to present
			previousDoneCount = doneCount;
		
			//reset done count
			doneCount = 0;
			
			//reset process number
			proc = -1;			
		}
	
		//increment process number
	  	proc = proc + 1;		
	}
}

