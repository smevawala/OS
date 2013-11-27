#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main(){
	int n;
	struct timespec tStart, tEnd, diff;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tStart);
	for(n=0;n<900000000;n++){
		getuid();
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tEnd);
	if ((tEnd.tv_nsec-tStart.tv_nsec)<0) {
		diff.tv_sec = tEnd.tv_sec-tStart.tv_sec-1;
		diff.tv_nsec = 1000000000+tEnd.tv_nsec-tStart.tv_nsec;
	} else {
		diff.tv_sec = tEnd.tv_sec-tStart.tv_sec;
		diff.tv_nsec = tEnd.tv_nsec-tStart.tv_nsec;
	}
	printf("%ld.%ld seconds\n",diff.tv_sec,diff.tv_nsec);
	return 0;
}