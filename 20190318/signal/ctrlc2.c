#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
//만약 핸들러로 구조체 act 를 가져오고 싶은경우 
//전역변수로 주소값을 가져온다.
void ouch(int sig)
{
printf("OUCH- i got signal %d \n",sig);
signal(SIGINT,SIG_DFL);
}

int main(void){
struct sigaction act;
act.sa_handler =ouch;
sigemptyset(&act.sa_mask);
act.sa_flags=0;
sigaction(SIGINT,&act,0);
while(1){
printf("hello world\n");
sleep(1);
}


}
