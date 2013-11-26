#include "sem.h"

void sighand(int i){
	// fprintf(stderr,"Waking up\n");
};

void sem_init(struct sem *s, int count){
	s->count=count;
	int i;
	for(i=0;i<N_PROC;i++){
		s->semqueue[i]=0;
		s->waiting[i]=0;
	}
	s->lock=0;
}

int sem_try(struct sem *s){
	while(tas(&(s->lock))!=0){
		
	}
	if(s->count>0){
		(s->count)--;
		s->lock=0;
		return 1;
	}
	else{
		s->lock=0;
		return 0;
	}

}

void sem_wait(struct sem *s){
	
	while(1){
		while(tas(&(s->lock))!=0){
		}
		if(s->count>0){
			// printf("got here\n");
			(s->count)--;
			s->lock=0;
			return;
		}
		else{
			//set signal
			// printf("waiting with procnum:%i\n",my_procnum);
			s->waiting[my_procnum]=1;
			s->semqueue[my_procnum]=getpid();
			sigset_t mask;
	     	sigfillset (&mask);
	 		sigdelset (&mask, SIGUSR1);
	 		sigdelset (&mask, SIGINT);
		    sigprocmask (SIG_BLOCK, &mask, NULL);
			signal(SIGUSR1, sighand);	
			s->lock=0;
		    sigsuspend (&mask);
			sigprocmask (SIG_UNBLOCK, &mask, NULL);
		}
	}
}

void sem_inc(struct sem *s){
	while(tas(&(s->lock))!=0){}
	(s->count)++;
	// printf("the count is:%i\n",s->count);
	int i;
	for(i=0;i<N_PROC;i++){
		if(s->waiting[i]==1){
			s->waiting[i]=0;
			// fprintf(stderr,"sending signal\n");
			kill(s->semqueue[i],SIGUSR1);
			break;
		}
	}
	s->lock=0;
}
