//Shivam Mevawala
//tcp-rcv.c

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
	int port, sd, sd2, fd, len;
	char buf[INET6_ADDRSTRLEN];

	port=atoi(argv[1]);

	if((sd=socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("socket error");
		return -1;
	}

	struct sockaddr_in sockaddr;
	struct sockaddr_storage sender;
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

	if((sd2=accept(sd,(struct sockaddr *) &sender, &len))<0){
		perror("accept error");
		return -1;
	}
	char * hostname;
	inet_ntop(sender.ss_family, &(((struct sockaddr_in *)&sender)->sin_addr), buf, sizeof buf );
	printf("%s\n", buf);
	struct hostent *addr;
	addr=gethostbyaddr(buf, sizeof buf, AF_INET);
	if(addr==NULL){
		herror(" ");
	} 
	else{
		hostname=addr->h_name;
		// printf("%s\n",hostname);
	}
	int rc,wc, totalc=0;
	char rb[1024];
	clock_t start=clock();
	while((rc=read(sd2, rb, 1024))>0){
	wc=write(1,rb, rc);
	totalc=totalc+wc;
	if(wc<0)
		perror("write error");
	}
	if(rc<0)
		perror("read error");
	clock_t end=clock();
	close(sd);
	close(sd2);
	fprintf(stderr,"\n\n--------------------------------------------------------\nTotal bytes sent: %i B\nThroughput is %f MB/S",totalc,((totalc)/(((double)end-(double)start)/CLOCKS_PER_SEC))/1000000);
	fprintf(stderr,"hostname is %s\n", hostname);
	return 0;
}