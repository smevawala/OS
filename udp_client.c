#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

int main(int argc, char ** argv){
	char * hostname;
	int port, sd;

	hostname = argv[1];
	port=atoi(argv[2]);
	printf("%s\n",hostname);
	if(!isdigit(hostname[0])){
		struct hostent *addr;
		if(!(addr=gethostbyname(hostname))){
			herror(" ");
			return -1;
		} 
		hostname=inet_ntoa(*((struct in_addr *)addr->h_addr));

	}
	printf("%s\n",hostname);
	sd=socket(PF_INET, SOCK_DGRAM, 0);
	if(sd<0){
		perror("socket error");
		return -1;
	}

	struct sockaddr_in sockaddr;
	sockaddr.sin_family=PF_INET;
	sockaddr.sin_addr.s_addr=inet_addr(hostname);
	sockaddr.sin_port=htons(port);

	// if(bind(sd,(struct sockaddr *)&sockaddr,sizeof sockaddr)<0){
	// 	perror("bind error");
	// 	close(sd);
	// 	return -1;
	// }
	int rc,wc,totalc=0;
	char rb[1024];
	clock_t start=clock();
	while((rc=read(0, rb, 1024))>0){
	wc=sendto(sd,rb, rc, 0, (struct sockaddr *)sockaddr.sin_addr.s_addr, sizeof sockaddr.sin_addr.s_addr);
	totalc = totalc+rc;
	if(wc<0)
		perror("write error");
	}
	clock_t end=clock();	
	if(rc<0)
		perror("read error");
	close(sd);
	fprintf(stderr,"\n\n--------------------------------------------------------\nTotal bytes sent: %i B\nThroughput is %f MB/S",totalc,((totalc)/(((double)end-(double)start)/CLOCKS_PER_SEC))/1000000);
	return 0;
}