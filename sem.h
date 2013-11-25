#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/resource.h>

void sem_init (struct sem *s, int count);


int sem_try(struct sem *s);


void sem_wait(struct sem *s);

void sem_inc(struct sem *s);
