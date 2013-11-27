#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void emtpyFunction(){};
int main(){
	int n, iterations=900000000;
	struct timespec tStart, tEnd, diff;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tStart);
	for(n=0;n<iterations;n++){
		emtpyFunction();
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tEnd);
	if ((tEnd.tv_nsec-tStart.tv_nsec)<0) {
		diff.tv_sec = tEnd.tv_sec-tStart.tv_sec-1;
		diff.tv_nsec = 1000000000+tEnd.tv_nsec-tStart.tv_nsec;
	} else {
		diff.tv_sec = tEnd.tv_sec-tStart.tv_sec;
		diff.tv_nsec = tEnd.tv_nsec-tStart.tv_nsec;
	}
	printf("%ld.%ld seconds for %i interations\n",diff.tv_sec,diff.tv_nsec, iterations);
	double pi=((double)diff.tv_sec*((double)1000000000)+(double)diff.tv_nsec)/((double)iterations);
	printf("%f nanoseconds per interation\n",pi);

	return 0;
}
