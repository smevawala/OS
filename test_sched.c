//Shivam Mevawala
//OS scheduler
#include "sched.h"

void init_fn(){
	printf("init_fn\n");
	int pid1, pid2, retval;
	printf("Forking now\n");
	switch(pid1=sched_fork()){
		case 0:
			printf("in child\n");
			int k=0;
			int n;
			for(n=0;n<100000000;n++){
				k=k+n*2;
			}
			sched_exit(5);
			break;
		default:
			switch(pid2=sched_fork()){
				case 0:
					printf("in second child\n");
					int k=0;
					int n;
					for(n=0;n<1000000;n++){
						k=k+n*2;
					}
					sched_exit(3);
				default:
					printf("in parent now\n");
			}
			sched_wait(&retval);
			printf("retValue is %i\n\n",retval);
			sched_wait(&retval);
			printf("retValue is %i\n\n",retval);
			break;
		}

	// return 0;
	exit(0);
}
int main(){
	printf("hello\n");
	sched_init(init_fn);

}

