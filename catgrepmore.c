#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
extern int errno;
    

int execgrepmore(){
    pid_t pidgrep,cpidgrep, pidmore, cpidmore;
    unsigned status;
    switch(pidgrep = fork()) {
    	case -1:
    		perror("Fork failed");
    		exit(1);
    		break;
    	case 0:
    		//in grep child
    		
    		break;
    	default:
			while ((cpidgrep=wait(&status)) != pidgrep){
				if (cpidgrep<0){
					perror("Wait failed");
					break;
				}
			};
			switch(pidmore = fork()) {
		    	case -1:
		    		perror("Fork failed");
		    		exit(1);
		    		break;
		    	case 0:
		    		//in more child
		    		break;
		    	default:
					while ((cpidmore=wait(&status)) != pidmore){
						if (cpidmore<0){
							perror("Wait failed");
							break;
						}
					};
			}
    		break;
	}
return 0;

}
    
int main (int argc, char **argv) {




	return 0;
}