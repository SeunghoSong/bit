#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
static void sig_quit(int);


int main (void)
{
sigset_t newmask, oldmask, pendmask;

if(signal(SIGINT,sig_quit)==SIG_ERR)
		perror("can`t catch SIGINT\n");


//block SIGQUIT
sigemptyset(&newmask);
sigaddset(&newmask,SIGINT);

if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
		perror("SIG_BLOCK error");

sleep(5);

if(sigpending(&pendmask)<0)
		perror("sigpending error\n");
if(sigismember(&pendmask,SIGINT))
		perror("\n SIGINT pendig\n");
if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
		perror("SIG_SETMASK error");
printf("SIGQUIT unblocked\n");

sleep(5);
exit(0);
}

static void sig_quit(int signo){
printf("cought SIGINT");
//if(signal(SIGINT,SIG_DFL)==SIG_ERR)
//perror("cant reset SIGINT");

}
