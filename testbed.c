//Shivam Mevawala
 
 #include "sched.h"


void child1()
{
 	fprintf(stderr,"HIIII child here! my pid is %d and my parent is %d\n",sched_getpid(),sched_getppid());
 	long long i;
 	for (i = 0; i < 1000000000; i++)
 	{
 		
 		if (i%100000000==0)
 		{
 			fprintf(stderr,"pid=%d, i=%lld, ticks=%d\n",sched_getpid(),i,sched_gettick());
 		}
 		
 	}
 	sched_exit(85);
 }
void child2()
{
 	fprintf(stderr,"HIIII child here! my pid is %d and my parent is %d\n",sched_getpid(),sched_getppid());
 	long long i;
 	for (i = 0; i < 1000000000; i++)
 	{
 		
 		if (i%100000000==0)
 		{
 			fprintf(stderr,"pid=%d, i=%lld, ticks=%d\n",sched_getpid(),i,sched_gettick());
 		}
 		
 	}
 	sched_exit(20);
 }
 
void parent()
{
 	fprintf(stderr,"parent hello world my pid is %d and my parent is %d\n",sched_getpid(),sched_getppid());
 	long long i;
 	for (i = 0; i < 1000000000; i++)
 	{
 		
 		if (i%100000000==0)
 		{
 			fprintf(stderr,"pid=%d, i=%lld, ticks=%d\n",sched_getpid(),i,sched_gettick());
 		}
 		
 	}
 	int ret;
 	sched_wait(&ret);
 	printf("one child returned %d\n",ret);
 	sched_wait(&ret);
 	printf("another child returned %d\n",ret);
 	sched_exit(0);
 }
 
void init_fn()
{
	int j;
	for (j = 0; j < 2; j++)
	{
	 	switch(sched_fork())
	 	{
	 		case -1:
	 			fprintf(stderr, "fork error!\n");
	 			exit(1);
	 		case 0:
	 			if (!j)
	 			{
	 				sched_nice(-20);
	 				child1();
	 			}
	 			else
	 			{
	 				sched_nice(20);
	 				child2();
	 			}
	 			
	 			exit(0);
	 	}
	}
	//sched_nice(5);
 	parent();
 	exit(0);
 }
 
 int main(int argc, char** argv)
 {
 	sched_init(init_fn);
 	return 0;
 }
 

