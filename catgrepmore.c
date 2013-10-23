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


int execgrepmore(char *filepath, char *pattern){
    pid_t pidgrep,cpidgrep, pidmore, cpidmore;
  	char grepstr[8];
    unsigned status;
    int fdsi[2], fdsj[2], ofds;
	char *command[3];

    command[0]="grep";
    command[1]=pattern;
    command[2]=NULL;
    ofds= open(filepath,O_RDONLY);
    // printf("%s\n",command[0]);


    switch(pidgrep = fork()) {
    	case -1:
    		perror("Fork failed");
    		exit(1);
    		break;
    	case 0:
    		//in grep child
    		printf("grepping %s for pattern %s\n", filepath, command[1]);
    		dup2(ofds,fileno(stdin));
    		if(execvp(command[0],command)<0)
    			perror("execution error");
    		
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
	int i;

	for(i=2;i<argc;i++){
		execgrepmore(argv[i],argv[1]);
	}



	return 0;
}