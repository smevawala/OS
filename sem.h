// #include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define N_PROC 64

extern int my_procnum;

struct sem
{
	int count;
	volatile char lock;
	pid_t semqueue[N_PROC];
	int waiting[N_PROC]; 
};

int tas(volatile char *lock);

void sem_init (struct sem *s, int count);

int sem_try(struct sem *s);

void sem_wait(struct sem *s);

void sem_inc(struct sem *s);