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
	char * hostname, * command;
	int port, sd;

	hostname = argv[1];
	port=atoi(argv[2]);
	command=argv[3];
	if(!isdigit(hostname[0])){
		struct hostent *addr;
		if(!(addr=gethostbyname(hostname))){
			herror(" ");
			return -1;
		} 
		hostname=inet_ntoa(*((struct in_addr *)addr->h_addr));
	}
	sd=socket(PF_INET, SOCK_DGRAM, 0);
	if(sd<0){
		perror("socket error");
		return -1;
	}
	struct sockaddr_in sockaddr;
	sockaddr.sin_family=PF_INET;
	sockaddr.sin_addr.s_addr=inet_addr(hostname);
	sockaddr.sin_port=htons(port);
	int wc, len = sizeof sockaddr;
	wc=sendto(sd,command, sizeof command, 0, (struct sockaddr *)&sockaddr, len);
	if(wc<0)
		perror("send error");
	char * s;
	s=(char *)malloc(512);
	if(s==NULL){
		perror("malloc");
		return -1;
	}
	if((recvfrom(sd, s, sizeof s, 0,(struct sockaddr *)&sockaddr, &len))<0)
		perror("recieve error");
	printf("%s\n",s);
	close(sd);
	return 0;
}