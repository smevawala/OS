#include "fifo.h"
#include <sys/mman.h>
#include <stdlib.h>


void fifo_init(struct fifo *f){
	sem_init(&(f->s_a), 1);
	sem_init(&(f->s_r), 0);
	sem_init(&(f->s_w), MYBUFFSIZE);
	printf("%i\n",MYBUFFSIZE);
	f->r=0;
	f->w=0;
}

void fifo_wr(struct fifo *f, unsigned long d){
	sem_wait(&(f->s_w));
	sem_wait(&(f->s_a));
	f->ds[f->w]=d;
	f->w=(f->w+1)%MYBUFFSIZE;
	sem_inc(&(f->s_r));
	sem_inc(&(f->s_a));
}

unsigned long fifo_rd(struct fifo *f){
	sem_wait(&(f->s_r));
	sem_wait(&(f->s_a));
	unsigned long d=f->ds[f->r];
	f->r=(f->r+1)%MYBUFFSIZE;
	sem_inc(&(f->s_w));
	sem_inc(&(f->s_a));
	return d;
	}