#include "fifo.h"


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
	struct fifo f;
	struct fifo *fif=&f;
	fif = mmap (NULL, sizeof(struct fifo), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	fifo_init(&f);
	// j=0;
	for(i=0; i<numProc; i++){
		switch(pid=fork()){
	    	case -1:
	    		perror("Fork failed");
	    		exit(1);
	    		break;
    		case 0:
    			// printf("i:%i\tj:%i\n",i,j);
    			my_procnum=i;
    			for(j=0; j<numLong; j++){
    				unsigned long d = (j << 6) + i;
    				// printf("%x\n",d);
    				fifo_wr(&f,d);
    			}
    			printf("%x\n",fifo_rd(&f));
    			exit(0);
    		default:
    			break;
    }


	}









}
