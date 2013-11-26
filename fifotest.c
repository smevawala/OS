#include "fifo.h"

int my_procnum;
int main(int argc, char **argv) {
	if(argc!=3){
		printf("Enter correct number of arguments\n");
		exit(0);
	}
	int numProc=atoi(argv[1]);
	int numLong=atoi(argv[2]);
	if(numProc>N_PROC){
		fprintf(stderr,"Enter correct number of arguments\n");
	}
	pid_t pid;
	int i,j;
	struct fifo *f;
	f= mmap (NULL, sizeof(struct fifo), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	fifo_init(f);
	// j=0;
	unsigned long r;
	for(i=0; i<numProc; i++){
		switch(pid=fork()){
	    	case -1:
	    		perror("Fork failed");
	    		exit(1);
	    		break;
    		case 0:
    			my_procnum=i;
    			for(j=0; j<numLong; j++){
    				unsigned long d = (j << 6) + i;
    				printf("wrote from process:%d, seq num:%d\n",i,j);
    				fifo_wr(f,d);
    			}
	    			exit(0);
    		default:
    			for(j=0; j<numLong; j++){
					r=fifo_rd(f);
					printf("read from process:%lu, seq num:%lu\n",(r&63),r>>6);
    			}
    			break;
   		}	

	}


}
