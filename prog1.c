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
	int waitTime;
};

struct proccess proccesses[7] = {
	{'1',8,10,0},
	{'2',10,3,0},
	{'3',14,7,0},
	{'4',9,5,0},
	{'5',16,4,0},
	{'6',21,6,0},
	{'7',26,2,0}
	};


void *A();
void *B();

pthread_t tid1,tid2;
pthread_attr_t attr;

char * fifo = "fifo";



int main()
{
        if (mkfifo(fifo, 0666) < 0)
                printf("Error creating named pipe.\n");
        else
                printf("pipe made");



	
	pthread_attr_init(&attr);
	pthread_create(&tid1,&attr,A,NULL);
	pthread_create(&tid2,&attr,B,NULL);
	pthread_join(tid2,NULL);
//	pthread_join(tid2,NULL);
//	pthread_join(tid1,NULL);
}


void *A()
{
	int initialise = 0;
	int clock;
	int totalWait=0;
	int averageWait;
	int fd;	
        
        printf("Thread A FLAG\n\n");
	//Open FIFO for write only proccess
	//fd = open(fifo, O_WRONLY);

	for (clock=0;clock<10;clock++)
	{
		int smallestBustTime = 15;
		int proc;
		for(proc=0;proc<7;proc++)
		{
		
		 	//intf("Proc: %c, ArriveTime: %d, Clock: %d, bustTime: %d, smallestBustTime: %d, waitTime: %d\n", proccesses[proc].id,proccesses[proc].arriveTime,clock,proccesses[proc].bustTime, smallestBustTime, proccesses[proc].waitTime);	
			if(proccesses[proc].arriveTime <= clock)
			{
				if(proccesses[proc].bustTime > 0)
					proccesses[proc].waitTime = proccesses[proc].waitTime + 1;
				
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
			printf("%c : %i\n",proccesses[3].id,proccesses[3].bustTime);
		 	proccesses[smallestBustTime].bustTime = proccesses[smallestBustTime].bustTime - 1;
			printf("%c : %i\n", proccesses[smallestBustTime].id, proccesses[smallestBustTime].bustTime);
		}
	}
	
        int proc;
	for(proc=0;proc<sizeof(proccesses);proc++)
	{
		totalWait = totalWait + proccesses[proc].waitTime;		
	}
	averageWait = totalWait / sizeof(proccesses);
        while(1)
	{
	if (fd=open(fifo, O_WRONLY) == -1)
		printf("PIPE OPEN ERROR");	

	write(fd,"balls",strlen("balls"));
	 
	close(fd);
	}
	return;
}

void *B()
{
	char string[10];
	int fd;
	printf("THread B baby\n\n");
	//fd = open(fifo,O_RDONLY);
	
	while(1)
	{
	        fd = open(fifo,O_RDONLY);
		read(fd,&string,strlen("balls"));
		printf("string: %s\n\n", string);
		//close(fd);
		if(string == "bls")
			break;
		//printf("%s",string);
	}
return;



}
