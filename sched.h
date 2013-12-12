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

#define SCHED_NPROC 64 // (maximum pid)-1, up to you but >=256
/* Suggested task state values */
#define SCHED_READY XXX
#define SCHED_RUNNING XXX
#define SCHED_SLEEPING XXX
#define SCHED_ZOMBIE XXX
#define STACK_SIZE 65536



struct sched_proc {
	/* use this for each simulated task */
	/* internals are up to you
	*/
	/* probably should include things like the task state */
	/* priority, accumulated, cpu time, stack address, etc. */
	int priority;
	int accumulated;
	unsigned long cpu_time;
	void * stack_addr;
	int pid;
	// struct sched_proc * parent;
	int ppid;
	struct savectx ctx;
};

struct sched_waitq {
	/* use this for each event/wakeup queue */
	/* internals are up to you */
};

void sched_init(void (*init_fn)());

int sched_getppid();

void sched_nice(int niceval);
