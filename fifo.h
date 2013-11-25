#include "sem.h"

void fifo_init(struct fifo *f);

void fifo_wr(struct fifo *f, unsigned long d);

unsigned long fifo_rd(struct fifo *f);