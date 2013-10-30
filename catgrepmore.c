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

int files_read, bytes_written;

void sighand(int sig){
	fprintf(stderr, "Amount of files read:%i\tAmount of bytes read:%i\n",files_read-1,bytes_written); 
	exit(1);
}
void sigpipehand(int sig){
	fprintf(stderr, "Sigpipe occured\n"); 
	exit(1);
}

int fileread(char * filepath, int fds){
	int infp, wb, iread, buff=1024;
	char *rb[1024];

	if ((infp=open(filepath,O_RDONLY))<0){
		perror("File opening error");
		return 1;
	}
	//&& sofar<65536
	while((iread=read(infp,rb,buff))>0 ){
		wb=write(fds,rb,iread);
		if(wb<0){
			perror("Write failure");
			return 1;
		}
		else if(wb<iread){
			perror("Partial write");
		}
		bytes_written=bytes_written+wb;
		// sofar=sofar+wb;
	}
	if(close(infp)<0){
		perror("File closing error");
	}
	return 0;
}

int execgrepmore(char *filepath, char *pattern){
    pid_t pidgrep,cpidgrep, pidmore, cpidmore;
  	// char grepstr[8];
    unsigned status;
    int fdsi[2], fdsfile[2];
   	char *commandg[3], *commandm[2];

    commandg[0]="grep";
    commandg[1]=pattern;
    commandg[2]=NULL;
    commandm[0]="more";
    commandm[1]=NULL;
    
    if(pipe(fdsfile)<0){
    	perror("Cannot create pipe");
    	return 1;
    }
    if(pipe(fdsi)<0){
    	perror("Cannot create pipe");
    	return 1;
    }



   	switch(pidgrep = fork()) {
    	case -1:
    		perror("Fork failed");
    		exit(1);
    		break;
    	case 0:
    		//in grep child
			if(close(fdsfile[1])<0)
				perror("Pipe closing  error");
			if(close(fdsi[0])<0)
				perror("Pipe closing  error");	

    		printf("grepping %s for pattern %s\n", filepath, commandg[1]);
    		dup2(fdsfile[0],0);
    		dup2(fdsi[1],1);
    		// fprintf(stderr,"starting with grep\n");
    		if(execvp(commandg[0],commandg)<0){
    			perror("Execution error");
    			return 1;
    		}
    		// fprintf(stderr,"done with grep\n");
			if(close(fdsfile[0])<0)
				perror("Pipe closing  error");	
			if(close(fdsi[1])<0)
				perror("Pipe closing  error");
    		break;

    	default:
			switch(pidmore = fork()) {
		    	case -1:
		    		perror("Fork failed");
		    		exit(1);
		    		break;
		    	case 0:
		    		//in more child
					if(close(fdsfile[1])<0)
						perror("Pipe closing  error");
					if(close(fdsfile[0])<0)
						perror("Pipe closing  error");	
					if(close(fdsi[1])<0)
						perror("Pipe closing  error");

		    		dup2(fdsi[0],0);
		    		// fprintf(stderr,"starting with more\n");	
		    		if(execvp(commandm[0],commandm)<0){
    					perror("Execution error");
    					return 1;
    				}
		    		// fprintf(stderr,"done with more\n");	
    				if(close(fdsi[0]<0))
    					perror("Pipe closing error");

		    		break;
		    	default:
					if(close(fdsfile[0])<0)
						perror("Pipe closing  error");	
					if(close(fdsi[1])<0)
						perror("Pipe closing  error");
		    		if(close(fdsi[0])<0)
    					perror("Pipe closing error");
					if((fileread(filepath,fdsfile[1]))==1)
						return 1;	
					if(close(fdsfile[1])<0)
						perror("Pipe closing  error");
					while ((cpidgrep=wait(&status)) != pidgrep){
						if (cpidgrep<0){
							perror("Wait failed");
							break;
						}
					}
					// printf("done with grep process\n");					
					while ((cpidmore=wait(&status)) != pidmore){
						if (cpidmore<0){
							perror("Wait failed");
							break;
						}
					}
					// printf("done with more process\n");
			}


			
    		break;
	}
return 0;
}
   
int main (int argc, char **argv) {

	signal(SIGINT, sighand);
	signal(SIGPIPE, sigpipehand);

	for(files_read=2;files_read<argc;files_read++){
		execgrepmore(argv[files_read],argv[1]);
		// printf("%i\n",files_read);
	}
	return 0;
}