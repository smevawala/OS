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

int main(int argc, char ** argv){
	char * hostname;
	int port, sd;

	hostname = argv[1];
	port=atoi(argv[2]);
	printf("%s\n",hostname);
	if(!isdigit(hostname[0])){
		struct hostent *addr;
		addr=gethostbyname(hostname); 
		hostname=inet_ntoa(*((struct in_addr *)addr->h_addr));

	}
	printf("%s\n",hostname);
	sd=socket(PF_INET, SOCK_STREAM, 0);
	if(sd<0){
		perror("socket error");
		exit(1);
	}
	// struct in_addr inaddr;
	// inaddr.s_addr
	struct sockaddr_in sockaddr;
	sockaddr.sin_addr.s_addr.s_addr=inet_addr(hostname);
	sockaddr.sin_port=htons(port);


	bind( sd, &sockaddr, sizeof sockaddr);




}