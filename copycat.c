     #include <ctype.h>
     #include <stdio.h>
     #include <stdlib.h>
     #include <unistd.h>
     #include <fcntl.h> 
     #include <string.h> 
     #include <errno.h> 
    
    extern int errno;
    
    int main (int argc, char **argv)
    {
    	int i, j;
    	int c, buff=256, wb, iread, ofile=1, infile=0;
    	char *rb; 
    	rb=malloc(256);
    	opterr = 0;
     
    	while ((c = getopt (argc, argv, "b:o:")) != -1)
		switch (c)
        	{
        	case 'b':
            	buff=atoi(optarg);
            	rb=malloc(buff);
            	break;
           	case 'o':
             	ofile= open(optarg, O_CREAT| O_WRONLY| O_TRUNC, S_IRUSR | S_IWUSR ); 
             	if(ofile<0){
                	fprintf(stderr,"Error opening file %s : %s\n",optarg,strerror(errno));
        			return -1;
            	}
            	break;
           	case '?':
             	if (optopt == 'b'|| optopt =='o')
               		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             	else if (isprint (optopt))
               		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             	else
               		fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
             	return -1;
           	default:
             	abort ();
           	}
     
       	printf ("buff= %i\n",buff);
      	i = optind;
       	j=argc;
       	if(argc==optind){
	        j++;
		} 
       	while( i < j){
        infile=0;

        if(j==argc && argv[i][0]!='-'){
            infile= open(argv[i],O_RDONLY);
            if(infile<0){
				fprintf(stderr,"Error opening file %s : %s\n",argv[i],strerror(errno));
                if(ofile!=1){
                    if(close(ofile)==-1){
                        fprintf(stderr,"Error closing output : %s\n",strerror(errno));
                    }
                }
                return -1;
            }
        }
        while((iread=read(infile,rb,buff))>0){
         
            wb=write(ofile,rb,iread);
            if(wb!=iread){
                if(wb<0){
                    fprintf(stderr,"Error: %s\n", strerror(errno));
                }
                else{
                    fprintf(stderr,"Error: Partial write occured\n");
                }
                if(ofile!=1){
                    if(close(ofile)==-1){
                        fprintf(stderr,"Error closing output : %s\n",strerror(errno));
                    }
                }
                if(infile!=0){
                    if(close(infile)==-1)
                        fprintf(stderr,"Error closing %s : $s\n",argv[i],strerror(errno));
                }
                return -1;
            }
         
        }
         if(iread<0){
            fprintf(stderr,"Error reading file %s : %s\n",argv[i],strerror(errno));
            if(ofile!=1){
                if(close(ofile)==-1)
            	    fprintf(stderr,"Error closing output : %s\n",strerror(errno));
            }
            if(infile!=0){
                if(close(infile)==-1)
                    fprintf(stderr,"Error closing %s : $s\n",argv[i],strerror(errno));
            }
            return -1;
        }
       i++;
       if(infile!=0){
            if(close(infile)==-1)
            fprintf(stderr,"Error closing %s : $s\n",argv[i],strerror(errno));
      	}
    }
    if(ofile!=1){
        if(close(ofile)==-1)
        	fprintf(stderr,"Error closing output : %s\n",strerror(errno));
    }

       return 0;
     }

