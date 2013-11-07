//Shivam Mevawala
//ECE 357 Operating Systems
//ps5 virtual memory

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/resource.h>
extern int errno;

void sigsevhand(int sig){
	fprintf(stderr, "Sigsev occured\n"); 
	exit(1);
}

   
int main (int argc, char **argv) {

	signal(SIGSEGV, sigsevhand);

	printf("Question %c:\n",argv[1][0]);
	switch(argv[1][0]){
		case 'A':{
			int fd;
			FILE *fp;
			char * m;		
			fd=open("testfile", O_RDONLY|O_CREAT);
			if (fd==-1)
				perror("File open error");
			struct stat sb;
			fstat(fd,&sb);
			m=(char*) mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
			if(m==MAP_FAILED)
				perror("Map failed");
			fp=fopen(m,"w");
			fwrite("hello boys",2,sizeof("hello boys"),fp);

			break;
			}
		case 'B':
			{
			int fd;
			char * m;
			char * string;
			string="Lorem ipsum dolor sit amet.";
			fd=open("testfile", O_RDWR);
			if (fd==-1)
				perror("File open error");
			struct stat sb;
			fstat(fd,&sb);
			m=(char*) mmap(0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
			if(m==MAP_FAILED)
				perror("Map failed");
			sprintf(m,"%s\n",string);
			int iread, buff=256;
			char rb[257];
			while((iread=read(fd,rb,buff))>0){
				printf("%s",rb);
			}
				printf("\n");

			break;
		}
		case 'C': 
		{
			int fd;
			char * m;
			char * string;
			string="Lorem ipsum dolor sit amet.";
			fd=open("testfile", O_RDWR);
			if (fd==-1)
				perror("File open error");
			struct stat sb;
			fstat(fd,&sb);
			m=(char*) mmap(0, sb.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
			if(m==MAP_FAILED)
				perror("Map failed");
			sprintf(m,"%s\n",string);
			int iread, buff=256;
			char rb[257];
			while((iread=read(fd,rb,buff))>0){
				printf("%s",rb);
			}
				printf("\n");
			break;
		}
		case 'D':
		{
			int fd;
			char * m;
			char * string;
			string="Lorem ipsum dolor sit amet.";
			fd=open("testfile", O_RDWR);
			if (fd==-1)
				perror("File open error");
			struct stat sb, sa;
			fstat(fd,&sb);
			printf("File size before:%i\n",sb.st_size);
			m=(char*) mmap(0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
			if(m==MAP_FAILED)
				perror("Map failed");
			int n=0;
			while(n<=sb.st_size){
					sprintf(m,"%s",string);
					n=n+sizeof(string);
			}
			fstat(fd,&sa);
			printf("File size after:%i\n",sa.st_size);
			if(sa.st_size==sb.st_size)
				printf("NO, the file size does not change\n");
			break;
		}
		case 'E':
		{
			int fd;
			// FILE *fp;
			char * m;
			char * string;
			string="Lorem ipsum dolor sit amet.";
			fd=open("testfile", O_RDWR);
			if (fd==-1)
				perror("File open error");
			struct stat sb, sa;
			fstat(fd,&sb);
			printf("File size before:%i\n",sb.st_size);
			m=(char*) mmap(0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
			if(m==MAP_FAILED)
				perror("Map failed");

		 	if(lseek(fd,100,SEEK_END) < 0)
		 		perror("lseek error");
		 	write(fd,"hi",4);
			fstat(fd,&sa);
			printf("File size after:%i\n",sa.st_size);
		 	if(lseek(fd,0,SEEK_SET) < 0)
		 		perror("lseek error");
			int iread, buff=256;
			char rb[257];
			while((iread=read(fd,rb,buff))>0){
				printf("%s",rb);
			}
			break;
		}
		case 'F':
		{
			int fd;
			char * m;
			char * string;
			fd=open("testfile", O_RDWR);
			if (fd==-1)
				perror("File open error");
			struct stat sb;
			fstat(fd,&sb);
			printf("File size before:%i\n",sb.st_size);
			m=(char*) mmap(0, 8192, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
			if(m==MAP_FAILED)
				perror("Map failed");
			printf("I can print this:%c\n",m[3000]);
			printf("The error recieved after trying to access the 7th page: ");
			printf("I cant print this:%c\n",m[6000]);

			break;
		}
			break;
		default:
			printf("Please enter a valid question id\n");
			break;
		}
	return 0;
}