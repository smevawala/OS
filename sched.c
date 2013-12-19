#include "sched.h"
#include <limits.h>


sigset_t mask;
struct sched_proc * process;
struct sched_proc * proc_list[SCHED_NPROC+2];
int * return_codes[SCHED_NPROC+2];
int tickwindow=0, tickcount=0;
void sched_tick(int i);

void sched_init(void (*init_fn)()) {
	/* This function will be called once by the testbed program.
	It should initialize your scheduling system, including
	setting up a periodic interval timer (see setitimer),
	establishing sched_tick as the signal handler for
	that timer, and creating the initial task which will
	have pid of 1. After doing so, make pid 1 runnable and
	transfer execution to it (including switching to its
	stack) at location init_fn. This function is not
	expected to return and if it does so it is OK to
	have unpredictable results.
	*/
	sigemptyset(&mask);
	sigaddset(&mask, SIGVTALRM);
	signal(SIGVTALRM,sched_tick);
	struct itimerval timer;
	timer.it_value.tv_sec=0;
	timer.it_value.tv_usec=100000;
	timer.it_interval.tv_sec=0;
	timer.it_interval.tv_usec=100000;
	process = (struct sched_proc *) malloc(sizeof(struct sched_proc));

	

	// struct sched_proc *init=process;
	process->pid=1;
	process->ppid=1;
	process->priority=10;
	process->state=SCHED_RUNNING;
	void *newsp;
	if ((newsp=mmap(0,STACK_SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,0,0))==MAP_FAILED){
		perror("mmap failed");
	}
	process->stack_addr=newsp;
	proc_list[process->pid]=process;

	struct savectx ctx;
	ctx.regs[JB_SP]=process->stack_addr+STACK_SIZE;
	ctx.regs[JB_BP]=process->stack_addr+STACK_SIZE;
	ctx.regs[JB_PC]=init_fn;
	process->ctx=ctx;

	sigprocmask(SIG_BLOCK, &mask, NULL);
	setitimer(ITIMER_VIRTUAL, &timer,NULL);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
	restorectx(&ctx, 0);
		
}

int sched_fork() {
	/* Just like the real fork, create a new simulated task which
	is a copy of the caller. Allocate a new pid for the
	child, and make the child runnable and eligible to
	be scheduled. sched_fork returns 0 to the child and
	the child’s pid to the parent. It is not required that
	the relative order of parent vs child being scheduled
	be defined. On error, return -1.
	Unlike the real fork, you do not need to duplicate the
	entire address space. Parent and child will execute in the
	same address space. However, you will need to create a
	new private stack area for the child and initialize it to be
	a copy of the parent’s. See below for discussion on stacks.
	*/
	sigprocmask(SIG_BLOCK, &mask, NULL);
	int newpid=find_lowest_pid();
	printf("newpid is %i\n",newpid);
	struct sched_proc * newprocess;
	newprocess = (struct sched_proc *) malloc(sizeof(struct sched_proc));
	newprocess->ppid=process->pid;
	newprocess->pid=newpid;
	newprocess->priority=process->priority;
	newprocess->state=SCHED_READY;
	void *newsp;
	if ((newsp=mmap(0,STACK_SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,0,0))==MAP_FAILED){
		perror("mmap failed");
		return -1;
	}
	memcpy(newsp, process->stack_addr, STACK_SIZE);
	adjstack(newsp, newsp+STACK_SIZE, newsp-process->stack_addr);
	// struct savectx ctx;
	// ctx.regs[JB_SP]=newprocess->stack_addr+STACK_SIZE;
	// ctx.regs[JB_BP]=newprocess->stack_addr+STACK_SIZE;
	// newprocess->ctx=ctx;
	// process=newprocess;
	proc_list[newpid]=newprocess;
	if(savectx(&(newprocess->ctx))==0){
		newprocess->ctx.regs[JB_SP] += newsp - process->stack_addr;
		newprocess->ctx.regs[JB_BP] += newsp - process->stack_addr;
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		return newpid;
	}

	sigprocmask(SIG_UNBLOCK, &mask, NULL);
    return 0;
}

void sched_exit(int code) {
	/* Terminate the current task, making it a ZOMBIE, and store
	the exit code. If a parent is sleeping in sched_wait(),
	wake it up and return the exit code to it.
	There will be no equivalent of SIGCHLD. sched_exit
	will not return. Another runnable process will be scheduled.
	*/
	sigprocmask(SIG_BLOCK, &mask, NULL);

	process->state=SCHED_ZOMBIE;
	if(proc_list[process->ppid]->state=SCHED_SLEEPING){
		proc_list[process->ppid]->state=SCHED_READY;
		return_codes[process->ppid]=(int *)malloc(sizeof(int));
		*return_codes[process->ppid]=code;
	}
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

}
int sched_wait(int *exit_code) {
	/* Return the exit code of a zombie child and free the
	resources of that child. If there is more
	than one such child, the order in which the codes are
	returned is not defined. If there are no zombie children,
	but the caller does have at least one child, place
	the caller in SLEEPING, to be woken up when a child
	calls sched_exit().
	If there are no children, return
	immediately with -1, otherwise the return value is 0.
	Since there are no simulated signals, the exit code
	is simply the integer from sched_exit().
	*/
	sigprocmask(SIG_BLOCK, &mask, NULL);

	int i;
	int ccount=0;
	int pids[SCHED_NPROC];

	for(i=2;i<SCHED_NPROC-2;i++){
		if(proc_list[i]!= NULL && proc_list[i]->ppid==process->pid){
			if(proc_list[i]->state==SCHED_ZOMBIE){
				exit_code = return_codes[process->pid];
				if (munmap(proc_list[i]->stack_addr, STACK_SIZE) == -1){
					perror("Failed to unmap ");
					break;
				}
				free(proc_list[i]);
				proc_list[i] = NULL;
				sigprocmask(SIG_UNBLOCK, &mask, NULL);
				return 0;
			}
			else{
				pids[ccount]=proc_list[i]->pid;
				ccount++;
			}
		}
	}
	if(ccount==0){//no child
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		return -1;
	}
	else{
		printf("going to sleep\n");
		process->state=SCHED_SLEEPING;
		sched_switch();
		exit_code = return_codes[process->pid];
		munmap(proc_list[pids[0]]->stack_addr, STACK_SIZE);
		free(proc_list[pids[0]]);
		proc_list[pids[0]] = NULL;
	}
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

    return 0;
}
void sched_nice(int niceval) {
	/* Set the current task’s "nice value" to the supplied parameter.
	Nice values may range from +19 (least preferred static
	priority) to -20 (most preferred). Clamp any out-of-range
	values to those limits
	*/
	if(niceval>20)
		niceval=20;
	else if(niceval<0)
		niceval=0;

	process->priority=niceval;
}

int sched_getpid() {
	/* return current task’s pid */
    return process->pid;
}
int sched_getppid() {
	/* return pid of the parent of current task */
	return process->ppid;
}

int sched_gettick() {
	/* return the number of timer ticks since startup */
    return tickcount+tickwindow;
}
void sched_ps() {
	/* output to stdout a listing of all of the current tasks,
	including sleeping and zombie tasks. List the
	following information in tabular form:
	    pid
	    ppid
	    current state
	    base addr of private stack area
	    if SLEEPING, address of wait queue
	    static priority
	    dynamic priority
	    total CPU time used (in ticks)
	*/

	printf("pid \t ppid \t state \t Stack BP \t Wait Queue \t S_P \t D_P \t CPU \n");
	printf("--- \t ---- \t ----- \t -------- \t ---------- \t --- \t --- \t --- \n");
	int n;
	for (n = 1; n < SCHED_NPROC; n++){
		if (proc_list[n] != NULL){
			printf("%i \t %i \t %i\t %p \t %p \t %i \t %lu \t %lu \n",proc_list[n]-> pid, proc_list[n]-> ppid, proc_list[n]-> state, proc_list[n]-> stack_addr, proc_list[n], proc_list[n]-> priority, (proc_list[n]-> priority +proc_list[n]->cpu_time), proc_list[n]->cpu_time);
		}
	}
	/* You should establish sched_ps() as the signal handler
	for SIGABRT so that a ps can be forced at any
	time by sending the testbed SIGABRT */
}
void sched_switch() {
	/* This is the suggested name of a required routine which will
	never be called directly by the testbed. sched_switch()
	should be the sole place where a context switch is made,
	analogous to schedule() within the Linux kernel.
	sched_switch() should place the current task on the run queue
	(assuming it is READY), then select the best READY task from
	the runqueue, taking into account the dynamic priority
	of each task. The selected task should then be placed
	in the RUNNING state and a context switch made to it
	(unless, of course, the best task is also the current task)
	See discussion below on support routines for context switch.
	*/
	printf("starting switching\n");
	//find pid to switch to
	int i, n;
	int min = INT_MAX;
	for(i=0;i<SCHED_NPROC+2;i++){
		if(proc_list[i]!=NULL){
			if(proc_list[i]->state==SCHED_READY){
				printf("i = %i\n",i);
				if(min>(proc_list[i]->priority +proc_list[i]->cpu_time)){
					min=(proc_list[i]->priority +proc_list[i]->cpu_time);
					n=i;
				}
			}
		}
	}
	if(i!=SCHED_NPROC+1){
		printf("n = %i\n",n);
		// struct savectx cur_ctx;
		process->state=SCHED_READY;
		// cur_ctx=process->ctx;
		proc_list[n]->state=SCHED_RUNNING;
		if(savectx(&(process->ctx))==0){
			process=proc_list[n];
			sigprocmask(SIG_UNBLOCK, &mask, NULL);
			restorectx(&(process->ctx), 1);
		}
			sigprocmask(SIG_UNBLOCK, &mask, NULL);
	}

}
void sched_tick(int i) {
	/* This is the suggested name of a required routine which will
	never be called directly by the testbed, but instead will
	be the signal handler for the SIGVTALRM signal generated by
	the periodic timer. Each occurrence of the timer signal
	is considered a tick. The number of ticks since sched_init
	is to be returned by sched_gettick().
	sched_tick should examine the currently running
	task and if its time slice has just expired, mark that
	task as READY, place it on the run queue based on its
	current dynamic priority, and then call sched_switch()
	to cause a new task to be run. Watch out for signal
	mask issues...remember SIGVTALARM will, by default,
	be masked on entry to your signal handler.
	*/
	// printf("tick\n");
	process->cpu_time+=1;
	tickwindow++;
	if(tickwindow>14){
		tickcount=+tickwindow;
		tickwindow=0;
		printf("clearing tickcount\n");
		sched_switch();
	}
}

int find_lowest_pid(){
	int i=0;
	for(i=1;i<SCHED_NPROC+2;i++){
		if(proc_list[i]==NULL)
			break;
	}	
	return i;

}