#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
static void sig_killhandler(int signum)// 접근이 안된다.
{
	printf("sig_killhandler : signum=%d\n",signum);
	exit(0);
}

static void sig_inthandler(int signum)
{
	printf("sig_inthandler : signum=%d\n",signum);
	exit(0);
}

void pr_exit(int status)
{
	// 정상 종료인 경우
	if (WIFEXITED(status))
	    printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

	// 시그널에 의해서 종료된 경우
	else if (WIFSIGNALED(status))
	    printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),
				WCOREDUMP(status) ? "(core file generated)" : "");

	// 시그널에 의해 STOP된 경우
	else if (WIFSTOPPED(status))
	    printf("child stopped, signal number = %d\n", WSTOPSIG(status));

}

static sig_handler(int signum)
{
printf("I got signal : signum=%d\n",signum);
exit(0); //시그널 종료




}

int main(void){

pid_t pid;

int status;

pid=fork();
if(pid==-1){
printf("fork error!\n");
return -1;
}

else if(pid==0){// 자식 프로세스만 시그날 처리

if(signal(SIGKILL,sig_killhandler)==SIG_ERR) 
			printf("\ncan't catch SIGKILL\n");

		if(signal(SIGINT,sig_inthandler)==SIG_ERR)
			printf("\ncan't catch SIGINT\n");

while (1){
printf("HELLO!\n");
sleep(1);
}
}else{
sleep(10);
kill(pid,SIGKILL); 
}
wait(&status);//자식 프로세스종료 까지 기다림, 자식프로세스의 상태값 확인
pr_exit(status);


return 0;
}
