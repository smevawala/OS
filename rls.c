#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// #include <fcntl.h> 
#include <string.h> 
#include <errno.h> 
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

uid_t guid=0;
int mtime =0;
time_t curtime= 0;
int rdir(char * path, int uflag, int mflag){    
	DIR * dirp=opendir(path);
	if(dirp==NULL){perror("CANNOT OPEN DIRECTORY");return 1;}
	char apath[256], time[80], perm[11]="----------", *linkname;
	int serr=0;
	strcpy(apath,path);
	struct dirent *entry;
	struct stat info;
	struct passwd *uid;
	struct group *gid;
	while ((entry = readdir (dirp)) != NULL) {
		if( strcmp(entry->d_name,".") && strcmp(entry->d_name,"..") ){
			sprintf(apath,"%s/%s",path,(entry->d_name));
			serr=lstat(apath,&info);
			if(serr==-1) {perror("STAT ERROR"); return 1;}
			if((!uflag) || (info.st_uid==guid)){
				if((!mflag) || ((curtime-info.st_mtime>mtime) && mtime>0) || ((info.st_mtime-curtime>mtime) && mtime<0)){
		           switch (info.st_mode & S_IFMT) {
			           case S_IFBLK:  perm[0]='b';break;
			           case S_IFCHR:  perm[0]='c';break;
			           case S_IFDIR:  perm[0]='d';break;
			           case S_IFSOCK: perm[0]='s';break;
			           case S_IFIFO:  perm[0]='p';break;
			           case S_IFLNK:  perm[0]='l';break;
			           case S_IFREG:  perm[0]='-';break;
					}
					if(info.st_mode&S_IRUSR)
						perm[1]='r';
					if(info.st_mode&S_IWUSR)
						perm[2]='w';
					if(info.st_mode&S_IXUSR){
						if(info.st_mode&S_ISUID)
							perm[3]='s';
						else
							perm[3]='x';
					}
					if(info.st_mode&S_IRGRP)
						perm[4]='r';
					if(info.st_mode&S_IWGRP)
						perm[5]='w';
					if(info.st_mode&S_IXGRP){
						if(info.st_mode&S_ISGID)
							perm[6]='s';
						else
							perm[6]='x';
					}
					if(info.st_mode&S_IROTH)
						perm[7]='r';
					if(info.st_mode&S_IWOTH)
						perm[8]='w';
					if(info.st_mode&S_IXOTH){
						if(info.st_mode&S_ISVTX)
							perm[9]='t';
						else
							perm[9]='x';
					}
					printf ("%ld/%ld\t%s %ld\t", info.st_dev, info.st_ino, perm, info.st_nlink);
					if((uid=getpwuid(info.st_uid))!=NULL)
						printf("%s\t",uid->pw_name);
					else
						printf("%i\t",info.st_uid);
					if ((gid = getgrgid(info.st_gid)) != NULL)
						printf("%s\t",gid->gr_name);
					else
						printf("%i\t",info.st_gid);
					if (((info.st_mode&S_IFMT)==S_IFCHR)||((info.st_mode&S_IFMT)==S_IFBLK))
						printf("%x\t", info.st_dev);
					else
						printf("%lld\t", info.st_size);
					strftime(time, 80,"%b %d %Y %I:%M%p",  localtime(&info.st_mtime) );
					printf ("%s\t%s/%s", time, path, entry->d_name);
					if((info.st_mode&S_IFMT)==S_IFLNK){
						linkname = (char *)malloc(info.st_size + 1);
						if (linkname<0){perror("MEMORY ERROR");return 1;}						
						readlink(apath,linkname, info.st_size + 1);
						linkname[info.st_size] = '\0';
						printf(" -> %s",linkname);   
					}
					printf("\n");
					sprintf(perm,"----------");
					if((info.st_mode&S_IFMT)==S_IFDIR){
						if(rdir(apath,uflag, mflag)==1)
							return 1;   
					}
				}
			}
		}

	}
closedir(dirp); 
return 0;
}

int main (int argc, char **argv)
{
	time(&curtime);
	int uflag =0, mflag=0, c;
	char * path, * user;
	opterr = 0;
 	struct passwd * ustruct;
	while ((c = getopt (argc, argv, "u:m:")) != -1)
	switch (c)
		{
		case 'u':
			user=optarg;
			uflag=1;
            if(!isdigit(user[0])){
			    ustruct=getpwnam(user);
			    guid=ustruct->pw_uid;
            }
            else
                guid=atoi(optarg);
			break;
		case 'm':
			mtime=atoi(optarg);
			mflag=1;
			break;
		case '?':
			if (optopt == 'u'|| optopt =='m')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
			return -1;
		default:
			abort ();
		}
 
	path=argv[optind];
	rdir(argv[optind], uflag, mflag);
	return 0;
}
					// printf ("%ld/%ld\t%s %ld\t%s\t%s\t%lld\t%s\t%s/%s", info.st_dev, info.st_ino, perm, info.st_nlink, user, group, info.st_size, time, path, entry->d_name);
