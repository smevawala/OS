//Shivam Mevawala
//udp_server.c

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
#include <sys/sysinfo.h>

int main(int argc, char ** argv){
	int port, sd, fd, len;
	char buf[INET6_ADDRSTRLEN];
	struct sysinfo info;
	sysinfo(&info);
	port=atoi(argv[1]);
	if((sd=socket(AF_INET, SOCK_DGRAM, 0))<0){
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

	int rc;
	char rb[1024], t[512], u[512];
	time_t start;
	while(1){
		memset(rb,0,sizeof rb);
		memset(u,0,sizeof u);
		memset(t,0,sizeof t);
		rc=recvfrom(sd, rb, 1024, 0, (struct sockaddr*) &sender, &len);
		if(rc<0)
			perror("receive error");
		printf("%s\n",rb);
		if(!strcmp(rb, "DATE")){
			ctime_r(&start,t);
			printf("Sending date: %s\n",t);
			sendto(sd,t,sizeof t, 0, (struct sockaddr*) &sender, len);
		}
		if(!strcmp(rb, "UPTIME")){
			sprintf(u, "%ld", info.uptime);
			printf("Sending Uptime: %s\n",t);
			sendto(sd,u,sizeof u, 0, (struct sockaddr*) &sender, len);
		}	
	}
	return 0;
}