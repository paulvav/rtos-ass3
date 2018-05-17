#include <stdio.h>
#include <pthread.h>





//Declare struct used for procceses
struct proccess
{
        char id;
        int arriveTime;
        int bustTime;
};

struct proccess proccesses[7] = {
	{'1',8,10},
	{'2',10,3},
	{'3',14,7},
	{'4',9,5},
	{'5',16,4},
	{'6',21,6},
	{'7',26,2}
	};

void *A();
pthread_t tid1;
pthread_attr_t attr;
int main()
{
	pthread_attr_init(&attr);
	pthread_create(&tid1,&attr,A,NULL);
	pthread_join(tid1,NULL);
}

void *A()
{
	printf("DEBUG: We have arrived at thread A");
	int initialise = 0;
	int clock;
	for (clock=0;clock<60;clock++)
	{
		int smallestBustTime = 15;
		for(int proc=0;proc<7;proc++)
		{
		
		 	printf("Proc: %c, ArriveTime: %d, Clock: %d, bustTime: %d, smallestBustTime: %d\n", proccesses[proc].id,proccesses[proc].arriveTime,clock,proccesses[proc].bustTime, smallestBustTime);	
			if(proccesses[proc].arriveTime <= clock && proccesses[proc].bustTime < smallestBustTime && proccesses[proc].bustTime > 0)
			{
				printf("%i", proc);
				smallestBustTime = proc;
				initialise = 1;	
			}
		}
		
		if(initialise == 1)
		{
			printf("%c : %i\n",proccesses[3].id,proccesses[3].bustTime);
		 	proccesses[smallestBustTime].bustTime = proccesses[smallestBustTime].bustTime - 1;
			printf("%c : %i\n", proccesses[smallestBustTime].id, proccesses[smallestBustTime].bustTime);
		}
	}



}
