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
	// char * hostname;
	int port, sd, sd2, fd, len;

	// hostname = argv[1];
	port=atoi(argv[1]);
	// printf("%s\n",hostname);
	// if(!isdigit(hostname[0])){
	// 	struct hostent *addr;
	// 	addr=gethostbyname(hostname); 
	// 	hostname=inet_ntoa(*((struct in_addr *)addr->h_addr));

	// }
	// printf("%s\n",hostname);
	if((sd=socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("socket error");
		return -1;
	}
	// struct in_addr inaddr;
	// inaddr.s_addr
	struct sockaddr_in sockaddr;
	sockaddr.sin_family=AF_INET;
	sockaddr.sin_addr.s_addr=INADDR_ANY;
	sockaddr.sin_port=htons(port);
	len=sizeof sockaddr;

	if(bind(sd,(struct sockaddr *)&sockaddr,len)<0){
		perror("bind error");
		return -1;
	}
	if(listen(sd,128)<0){
		perror("listen error");
		return -1;
	}
	if((sd2=accept(sd,(struct sockaddr *) &sockaddr, &len))<0){
		perror("accept error");
		return -1;
	}

	int rc,wc;
	char rb[1024];
	while((rc=read(sd2, rb, 1024))>0){
	wc=write(1,rb, 1024);
	if(wc<0)
		perror("write error");
	}
	if(rc<0)
		perror("read error");
	close(sd);
	close(sd2);
}