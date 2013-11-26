#include "sem.h"

#define MYBUFFSIZE 4096
struct fifo
{
	int r;
	int w;
	unsigned long ds[MYBUFFSIZE];
	struct sem s_a,s_r,s_w;
};

void fifo_init(struct fifo *f);

void fifo_wr(struct fifo *f, unsigned long d);

unsigned long fifo_rd(struct fifo *f);