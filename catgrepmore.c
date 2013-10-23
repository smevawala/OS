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
	char *commandg[3], *commandm[2];

    commandg[0]="grep";
    commandg[1]=pattern;
    commandg[2]=NULL;
    commandm[0]="more";
    commandm[1]=NULL;
    if((ofds= open(filepath,O_RDONLY))<0){
    	perror("Error opening file");
    	return 1;
    }
    if(pipe(fdsi)<0){
    	perror("Cannot create pipe");
    	return 1;
    }
    // printf("%s\n",commandg[0]);


    switch(pidgrep = fork()) {
    	case -1:
    		perror("Fork failed");
    		exit(1);
    		break;
    	case 0:
    		//in grep child
    		printf("grepping %s for pattern %s\n", filepath, commandg[1]);
		    if((ofds= open(filepath,O_RDONLY))<0){
    			perror("Error opening file");
    			return 1;
    		}
    		dup2(ofds,0);
    		dup2(fdsi[1],1);
    		if(execvp(commandg[0],commandg)<0){
    			perror("Execution error");
    			return 1;
    		}
    		if(close(ofds)<0)
    			perror("File closing error");
			if(close(fdsi[1])<0)
				perror("Pipe closing  error");
    		break;

    	default:
			while ((cpidgrep=wait(&status)) != pidgrep){
				if (cpidgrep<0){
					perror("Wait failed");
					break;
				}
			};
			if(close(fdsi[1])<0)
				perror("Pipe closing  error");
			switch(pidmore = fork()) {
		    	case -1:
		    		perror("Fork failed");
		    		exit(1);
		    		break;
		    	case 0:
		    		//in more child
		    		dup2(fdsi[0],0);
		    		if(execvp(commandm[0],commandm)<0){
    					perror("Execution error");
    					return 1;
    				}
    				if(close(fdsi[0]<0)){
    					perror("Pipe closing error");
    				}
		    		break;
		    	default:
					while ((cpidmore=wait(&status)) != pidmore){
						if (cpidmore<0){
							perror("Wait failed");
							break;
						}
					}
		    		if(close(fdsi[0])<0)
    					perror("Pipe closing error");
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


