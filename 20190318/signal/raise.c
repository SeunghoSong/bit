#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int sig){
printf("raise(): I got signal %d\n",sig);

(void)signal(SIGINT,SIG_DFL);
}

int main(void){
int count =0;
signal(SIGINT,sighandler);

while(1){
printf("hello world\n");
count++;
if(count==3){
raise(SIGINT);
count=0;
}
sleep(1);

}

}
