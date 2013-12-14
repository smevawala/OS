#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include "savectx64.h"
#include "jmpbuf-offsets64.h"

#define SCHED_NPROC 256 // (maximum pid)-1, up to you but >=256
/* Suggested task state values */
#define SCHED_READY 0
#define SCHED_RUNNING 1
#define SCHED_SLEEPING 2
#define SCHED_ZOMBIE 3
#define STACK_SIZE 65536



struct sched_proc {
	/* use this for each simulated task */
	/* internals are up to you
	*/
	/* probably should include things like the task state */
	/* priority, accumulated, cpu time, stack address, etc. */
	int priority;
	int state;
	int accumulated;
	unsigned long cpu_time;
	void * stack_addr;
	int pid;
	// struct sched_proc * parent;
	int ppid;
	int exit_code;
	struct savectx ctx;
};

struct sched_waitq {
	/* use this for each event/wakeup queue */
	/* internals are up to you */
};

void sched_init(void (*init_fn)());

int sched_fork();

int sched_getppid();

void sched_nice(int niceval);

void sched_exit(int code);

void sched_switch();

int find_lowest_pid();


void adjstack(void *lim0,void *lim1,unsigned long adj);
