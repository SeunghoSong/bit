#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


static void user_sig2_handler(int sig_num){

printf("World\n");


}

static void user_sig1_handler(int sig_num){
printf("hello");

}



int main(void){
int count=0;
int status=0;
struct sigaction act1,act2;
pid_t pid;
sigset_t old,old2;

if((pid=fork())<0){
perror("fork error\n");
return -1;
}else if(pid==0){
act1.sa_handler=user_sig1_handler;
act2.sa_handler=user_sig2_handler;

sigemptyset(&act1.sa_mask);
sigemptyset(&act2.sa_mask);

sigaddset(&act1.sa_mask,SIGUSR1);
sigaddset(&act2.sa_mask,SIGUSR2);

if(sigprocmask(SIG_BLOCK,&act1.sa_mask,&old)<-1)
		perror("act1 BLOCK error\n");
if(sigprocmask(SIG_BLOCK,&act2.sa_mask,&old2)<-1)
		perror("act2 BLOCK error\n");
	
if(sigaction(SIGUSR1,&act1,NULL)==-1)
perror("act1 act error");

if(sigaction(SIGUSR2,&act2,NULL)==-1)
perror("act2 act error");


		sleep(5);
		//fflush(stdout);
		printf("go !\n");

		if(sigprocmask(SIG_SETMASK,&old,NULL)<-1)
			perror("act1 UNBLOCK  error\n");
		if(sigprocmask(SIG_SETMASK,&old2,NULL)<-1)
			perror("act2 UNBLOCK  error\n");
		while(1)
		pause();
		}else if(pid>0){
		for(int a =0 ;a<6;a++){
			kill(pid,SIGUSR1);
			sleep(1);
			kill(pid,SIGUSR2);
			sleep(1);
			}
		kill(pid,SIGKILL);
		}

wait(&status);
exit(0);
}
