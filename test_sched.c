#include "sched.h"

void init_fn(){
	printf("init_fn\n");
	int pid, retval=5;
	printf("Forking now\n");
	switch(pid=sched_fork()){
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
			sched_wait(&retval);
			printf("in parent, child pid is %i\n",pid);
			break;
		}
	printf("retValue is %i",retval);
	// return 0;
	exit(0);
}
int main(){
	printf("hello\n");
	sched_init(init_fn);

}

