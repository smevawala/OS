#include <stdio.h>
#include<string.h>
#include <stdlib.h> 

int main(int argc, char *argv[])
{
    char *ofname; 
    int i, b=255, argo=1;
    if(argc<1){
        printf("no argument\n");
        return 0; 
    }
    if(argv[1][0]=='-'){
        printf("Switch detected\n");
        if(argv[1][1]=='b'){
            b=atoi(argv[2]);
            printf("buffer =%i \n",b);
            if(argv[3][1]=='-' && argv[3][2]=='o'){
                ofname=argv[4];
                argo=5;
                if(argc<5){
                    printf("no argument\n");
                    return 0;
                }
            }
            else{argo=3;}
        }
        else if(argv[1][1]=='o'){
            ofname=argv[2];
            argo=3;
        }
    }
    else{
        printf("Nope\n");
    }
    printf("output=%s\n",ofname);
    printf("%c\n",argv[1][2]);
    FILE *fp;
    char ch; 
    for(i=argo; i<argc;i++){
        printf("got here");

        fp=fopen(argv[i], "r");
        if(fp == NULL) {
            printf("%s: No such file or directory\n", argv[i]);
            return 0;
        }
        while((ch=fgetc(fp)) != EOF){
            putchar(ch);
        }
        fclose(fp);
    }
    return 0;
}
