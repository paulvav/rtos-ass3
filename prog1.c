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
        if (mkfifo(fifoWT, 0666) < 0)
                printf("Error creating named pipe.\n");
        else
                printf("pipe made");


	mkfifo(fifoRT, 0666);
	
	pthread_attr_init(&attr);
//	pthread_create(&tid1,&attr,A,NULL);
	pthread_create(&tid2,&attr,B,NULL);
	pthread_create(&tid1,&attr,A,NULL);
	pthread_join(tid2,NULL);
//	pthread_join(tid2,NULL);
//	pthread_join(tid1,NULL);
}


void *A()
{
	int initialise = 0;
	int clock;
	int totalWait=0, totalRT =0;
	int averageWait, averageRT;
	int fd_RT, fd_WT;	
        
        printf("Thread A FLAG\n\n");
	//Open FIFO for write only proccess
 	fd_RT = open(fifoRT, O_WRONLY);
	fd_WT = open(fifoWT, O_WRONLY);

	for (clock=0;clock<50;clock++)
	{
		int smallestBustTime = 15;
		int proc;
		for(proc=0;proc<7;proc++)
		{
		
		 	printf("Proc: %c, ArriveTime: %d, Clock: %d, bustTime: %d, smallestBustTime: %d, waitTime: %d\n\n", proccesses[proc].id,proccesses[proc].arriveTime,clock,proccesses[proc].bustTime, smallestBustTime, proccesses[proc].waitTime);	
			if(proccesses[proc].arriveTime <= clock)
			{
				if(proccesses[proc].bustTime > 0)
					proccesses[proc].responseTime = proccesses[proc].responseTime + 1;
				
				if(proccesses[proc].bustTime < smallestBustTime && proccesses[proc].bustTime > 0)
				{	
					printf("%i", proc);
					smallestBustTime = proc;
					initialise = 1;	
				}
			

			}
		}
		
		if(initialise == 1)
		{
			proccesses[smallestBustTime].bustTime = proccesses[smallestBustTime].bustTime - 1;			
		}
		int proc1;		
		for(proc1=0;proc1<7;proc1++)
		{
			if(proccesses[proc1].bustTime == smallestBustTime);
			else if(proccesses[proc1].bustTime > 0)
				proccesses[proc1].waitTime = proccesses[proc1].waitTime + 1;
		}
	}
        int proc; 
	for(proc=0;proc<7;proc++)
	{
		totalRT = totalRT + proccesses[proc].responseTime;
		totalWait = totalWait + proccesses[proc].waitTime;		
	}
	
	averageWait = totalWait / 7;
	write(fd_RT,&averageRT, sizeof(averageRT));
	write(fd_WT,&averageWait,sizeof(averageWait));
	
	close(fd_RT);
	close(fd_WT);
	return;

}
void *B()
{
	int averageWait, averageRT;
	int fdRT, fdWT;	
	char answer[50];
	fdRT = open(fifoRT,O_RDONLY);
        fdWT = open(fifoWT,O_RDONLY);
	
	read(fdRT,&averageRT,sizeof(averageRT));
	read(fdWT,&averageWait,sizeof(averageWait));
    	close(fdRT);
	close(fdWT);
	
	fprintf(fopen("output.txt","w"),"Average Wait: %d, Average Response Time: %d",averageWait,averageRT);
return;



}
