#include "sched.h"

void init_fn(){
	printf("init_fn\n");
	// return 0;
	exit(0);
}
int main(){
	printf("hello\n");
	sched_init(init_fn);

}

