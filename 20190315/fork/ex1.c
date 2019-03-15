#include <stdio.h>// printf
#include <unistd.h>// fork()
#include <sys/types.h>//pid.t
#include <stdlib.h>


//확인
#define I


//단순한 fork()

#ifdef A
int main(void)
{
	pid_t pid;
	int x=0;

	pid=fork();
	x=1;

	if(pid==0){// 자식과 부모를 판단
	printf ("pid= %ld I am process %ld and getppid=%ld\n",(long)pid,(long)getpid(),(long)getppid());
	sleep(1);
	printf("Done");
	}else
	printf ("pid= %ld I am process %ld and my x is %d \n",(long)pid,(long)getpid(),x);
	return 0;
}

/*
 song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ ./ex1
pid= 3405 I am procesong@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ ./ex1
pid= 3784 I am process 3783 and my x is 1 
pid= 0 I am process 3784 and getppid=3783
Donesong@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ 
ss 3404 and my x is 1 
pid= 0 I am process 3405 and getppid=3404
song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ Done

이런 경우는 부모가 자식프로세서를 관리를 안한다는 것이다.
 */

#endif

//좀더 복잡한 경우의 fork(), wait()의 사용 

#ifdef B
int main(void)
{
	pid_t pid;
	int x=0;
	int status;
	x=1;

	if((pid=fork())==0){// 자식과 부모를 판단
	//자식 프로세서
	printf ("pid= %ld I am process %ld and getppid=%ld\n",(long)pid,(long)getpid(),(long)getppid());
	sleep(1);
	printf("Done");
	return 0;
	}else{
	//부모프로세서
	printf ("pid= %ld I am process %ld and my x is %d \n",(long)pid,(long)getpid(),x);
	}

	wait(&status);	//자식프로세스가 사용종료될때까지 대기,자식 프로세스가  끝났을 경우
					//부모에게 signal 전송(자식프로세스는 좀비 상태) 
	return 0;		//대기 종료
}
/*
song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ ./ex1
pid= 3784 I am process 3783 and my x is 1 
pid= 0 I am process 3784 and getppid=3783
Donesong@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ 
*/

#endif


// 변수의 동기화 문제
#ifdef C
int global =10;
int main (){
int i=20;
pid_t pid;
int status;
if((pid=fork())==0){
	//자식 프로세서
	global=global+10;
	i=i+10;}
else{
	// 부모 프로세서
	global=global+100;
	i=i+100;}

printf("UID:%d global=%d ,i = %d \n",getpid(),global,i);

}


#endif 




#ifdef D

int glob =6;
char buf[]="a write to stdout\n";

int main(){
int var;
pid_t pid;
var =88;
if (write(STDOUT_FILENO,buf, sizeof(buf)-1)!=sizeof(buf)-1)
		perror("write error");
printf("before fork\n");

if((pid=fork())==0){
	//자식프로세서의 경우
	glob++;
	var++;
}else//부모프로세서의 경우
sleep(2);

printf("pid = %d ,glob= %d, var =%d\n",getpid(),glob,var);
}

/*
 <출력>
 a write to stdout
 before fork
 pid = 4071 ,glob= 7, var =89
 pid = 4070 ,glob= 6, var =88
 */


#endif

#ifdef E

int main(){
pid_t pid;
char *message ;
int n;
int temp;

printf("fork program starting\n");

pid =fork();

switch(pid)
{
		case -1 : perror("fork failed");
				  exit(1);
		case 0 	:
				 message ="this is the child";
				 n=5;
				 break;

		default :
				 message ="this is the parent";
				 n=3;
				 break;
}
for(;n>0;n--){
puts(message);
sleep(1);
}
wait(&temp);

}
/*
  <출력 결과>

 -wait 함수가 없는경우-

 fork program starting
 this is the parent
 this is the child
 this is the parent
 this is the child
 this is the parent
 this is the child
 this is the child
 song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ this is the child

-wait 함수가 있는 경우
fork program tarting
this is the parent
this is the child
this is the parent
this is the child
this is the child
this is the parent
this is the child
this is the child
song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ 
 */
#endif

//race condition(경쟁 상태) 의 발생

#ifdef F
static void DisplayChar(char *str){

	char* ptr;
	int c;

	//stdout의 버퍼링을 막는다.
	setbuf(stdout,NULL);
	
	for(ptr=str;c=*ptr++;)
			putc(c,stdout);
}

int main(void){
pid_t pid;

if((pid =fork())<0){
	//오류
	perror("fork error");}
else if (pid==0){
	//자식 프로세서
	DisplayChar("output from child\n");}
	else{
	//부모 프로세서 
	DisplayChar("output from parent\n");}

	exit(1);
}
#endif
/*
 <출력>
 원하는 결과
 song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ ./ex1
 output from parent
 output from child

 실제  결과 중 하나
 song@ubuntusong:~/bit_git/bit_ubuntu/bit/20190315/fork$ ./ex1
 output from paroutput from child
 ent

 
 
 */

#ifdef G
#include <sys/wait.h>
void pr_exit()(int status){
if(WIFEXITED(status))
		printf("normal termination,exit status =%d\n",WEXITSTATUS(status));
else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d %s\n",WTERMSIG(status),SCOREDUMP(status)?
				"core file generated":"");
else if (WIFSTOPPED(status))
		printf("child stopped, signal number =%d \n",WSTOPSIG(status));

}

int main()
{
pid_t pid;
char *message;
int n;
int exit_code;

printf("fork program starting\n");
pid=fork();

switch(pid){

		case -1 :perror("fork failed");
				 exit(1);

		case 0 : message ="this is th child";
			n=5;
			exit_code=37;
			break;
		
		default :message="this is the parent";
				n=3;
				 exit_code-=0;
				 break;
}

for(;n>0;n--){
puts(message);
sleep(1);


}
if(pid!=0){
int stat_val;
pid_t child_pid;

child_pid=wait(&stat_val);
printf("child has finished:PID =%d\n",child_pid);
if(WIFEXITED(stat_val))
{
printf("child exited with code%d \n",WEXITSTATUS(stat_val));
}else
printf("Child tearminated abnormally\n");

}

exit(exit_code);

}

#endif

#ifdef H

void pr_exit()(int status){
if(WIFEXITED(status))
		printf("normal termination,exit status =%d\n",WEXITSTATUS(status));
else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d %s\n",WTERMSIG(status),SCOREDUMP(status)?
				"core file generated":"");
else if (WIFSTOPPED(status))
		printf("child stopped, signal number =%d \n",WSTOPSIG(status));
}

int main (void){
pid_t pid;
int status;
if ((pid=fork())<0)
		perror("fork error");
else if (pid==0) exit(7);

if (wait(&status)==pid)
		pr_exit(status);

if ((pid=fork())<0)
		perror("fork error");
else if (pid== 0) abort();

if ((pid=fork())<0)
		perror("fork error");
else if (pid== 0) status/=0;

if (wait(&status)==0)
		pr_exit(status);
}


#endif
// 좀비 프로세서를 확인 
// ps -al 로 
#ifdef  I

int main(){
pid_t pid;
char *message ;
int n;
int temp;

printf("fork program starting\n");

pid =fork();

switch(pid)
{
		case -1 : perror("fork failed");
				  exit(1);
		case 0 	:
				 message ="this is the child";
				 n=5;
				 break;

		default :
				 message ="this is the parent";
				 n=3;
				 break;
}
for(;n>0;n--){
puts(message);
sleep(1);
}
if(pid>0){
printf("child proc state, about %d sec \n",20);
wait(NULL);

}
}

#endif
