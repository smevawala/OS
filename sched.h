

#define SCHED_NPROC XXX // (maximum pid)-1, up to you but >=256
/* Suggested task state values */
#define SCHED_READY XXX
#define SCHED_RUNNING XXX
#define SCHED_SLEEPING XXX
#define SCHED_ZOMBIE XXX

struct sched_proc {
/* use this for each simulated task */
/* internals are up to you
*/
/* probably should include things like the task state */
/* priority, accumulated, cpu time, stack address, etc. */
};
struct sched_waitq {
/* use this for each event/wakeup queue */
/* internals are up to you */
};