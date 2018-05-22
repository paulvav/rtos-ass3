/*
Name: Paul Vavich ID: 11685726
This program demonstrates the Shortest Time first algorithm, and utilises a FIFO and threads
To print the data to an output file output.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//Declare struct used for procceses
struct proccess
{
        char id;
        int arriveTime;
        int bustTime;
	int responseTime;
	int waitTime;
};

struct proccess proccesses[7] = {
	{'1',8,10,0,0},
	{'2',10,3,0,0},
	{'3',14,7,0,0},
	{'4',9,5,0,0},
	{'5',16,4,0,0},
	{'6',21,6,0,0},
	{'7',26,2,0,0}
	};


void *A();
void *B();

pthread_t tid1,tid2;
pthread_attr_t attr;

char * fifoWT = "fifoWT";
char * fifoRT = "fifoRT";


int main()
{
	//initialise both FIFOs
        if (mkfifo(fifoWT, 0666) < 0)
                printf("Error creating Wait Time named pipe. May have already been created\n");         
	if (mkfifo(fifoRT, 0666) < 0)
		printf("Error creating Response Time named pipe. May have already been created\n");
	
	pthread_attr_init(&attr);
	pthread_create(&tid2,&attr,B,NULL);
	pthread_create(&tid1,&attr,A,NULL);
	pthread_join(tid2,NULL);
}


//Thread A performs SPF algorithm and prints average response time and wait time to fifo 

void *A()
{
	//initialise variable
	int initialise = 0;
	//used to iterate "clock" to simulate cpu cycles
	int clock;

	//used to calulate average later
	int totalWait=0, totalRT =0;
	
	//for fifos
	int fd_RT, fd_WT;	
	double averageWait=0,averageRT=0;       
 	int smallestBustTime;
	
	//Open FIFOs for write only proccess
 	fd_RT = open(fifoRT, O_WRONLY);
	fd_WT = open(fifoWT, O_WRONLY);

	//for 50 clock cycles
	for (clock=0;clock<50;clock++)
	{
		int proc;

		//iterate through 7 processes
		for(proc=0;proc<7;proc++)
		{
			//if processes' arrive time is less than clock continue calulation
			if(proccesses[proc].arriveTime <= clock)
			{
				//If process is not complete
				if(proccesses[proc].bustTime > 0)
				{
					//Response time increases
					proccesses[proc].responseTime = proccesses[proc].responseTime + 1;
					//If to detect first process
					if(proc==0)
					{
						//set smallest Bust time to first process
						smallestBustTime = proc;
					}
				}
				
				//if busttime of process is smallest so far
				if(proccesses[proc].bustTime < proccesses[smallestBustTime].bustTime && proccesses[proc].bustTime > 0)
				{	//set it as smallest busttime
					smallestBustTime = proc;

					//initialise flag
					initialise = 1;	
				}
			}
		}
		
		//if a smallest proccess has been found
		if(initialise == 1)
		{
			//subtract one busttime
			proccesses[smallestBustTime].bustTime = proccesses[smallestBustTime].bustTime - 1;			
		}

		int proc1;		
		//loop to increment wait time on all remaining active functions
		for(proc1=0;proc1<7;proc1++)
		{
			if(proccesses[proc1].bustTime == proccesses[smallestBustTime].bustTime);
			else if(proccesses[proc1].bustTime > 0 && proccesses[proc1].arriveTime <= clock)
				proccesses[proc1].waitTime = proccesses[proc1].waitTime + 1;
		}
	}

        int proc; 
	//After algorithm is over, calculate total Response time and wait time - for use with average
	for(proc=0;proc<7;proc++)
	{
		totalRT = totalRT + proccesses[proc].responseTime;
		totalWait = totalWait + proccesses[proc].waitTime;		
	}

	//calculate wait	
	averageWait = ((double)totalWait)/7.0;

	//calculate Responsetime average
	averageRT = ((double)totalRT)/7.0;

	//write to FIFO
	write(fd_RT,&averageRT, sizeof(averageRT));
	write(fd_WT,&averageWait,sizeof(averageWait));
	
	//Close FIFO
	close(fd_RT);
	close(fd_WT);
	return;

}

void *B()
{
	//initialise variables
	double averageWait, averageRT;
	int fdRT, fdWT;	
	char answer[50];
	//Open FIFOs for read only operation
	fdRT = open(fifoRT,O_RDONLY);
        fdWT = open(fifoWT,O_RDONLY);
	
	//extract WAit and response time data from FIFO
	read(fdRT,&averageRT,sizeof(averageRT));
	read(fdWT,&averageWait,sizeof(averageWait));
    	//close FIFOs
	close(fdRT);
	close(fdWT);
	
	//Print data to output.txt file
	fprintf(fopen("output.txt","w"),"Average Wait: %lf, Average Response Time: %lf",averageWait,averageRT);
return;



}
