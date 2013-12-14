#include "sched.h"

void init_fn(){
	printf("init_fn\n");
	int pid;
	printf("Forking now\n");
	switch(pid=sched_fork()){
		case 0:
			printf("in child\n");
			break;
		default:
			printf("in parent, child pid is %i\n",pid);
			break;
		}
	while(1){

	};
	// return 0;
	exit(0);
}
int main(){
	printf("hello\n");
	sched_init(init_fn);

}

