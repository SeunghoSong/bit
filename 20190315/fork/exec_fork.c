#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>
#include <stdlib.h>

/*

fork-exec
자식 프로세스로 부모와 다른 프로그램을 실행하도록 한다.

*이해*
fork()의 한계성 
자식프로세스는 부모 프로세스와 다른 명령어를 실행하지 못함

fork-exec의 사용
fork()로 자식프로세스를 생성한후 자식프로세스만 exec를 실행하도록한다.

자식프로세스 부모 프로세스 구분 방법
uid_t fork() 함수 의 리턴값을 비교하도록한다.
uid_t uid =-1 :fork() 오류
uid_t uid = 0 :자식프로세스


 *참고*
 my_github

 
 fork-exec 의 이해  
 https://talkingaboutme.tistory.com/674
 
 */


//*예제*

// fork-exec 사용

//fork-exec를 사용자 함수를 이용하여 구성한다.
void my_execv(const char *path,char *const argv[]);

int main(){

char path[]= "/bin/ls"; //해당 파일 위치 배열

char* argv_ls[]={"ls","-al",NULL};// 명령어 배열


my_execv(path,argv_ls);//fork-exec 사용 
return 0;
}

//void my_execv(해당 파일 위치 배열, 명령어  배열)
void my_execv(const char *path,char *const argv[]){
int ret;
pid_t pid;

pid=fork(); //자식 프로세스 생성

if(pid ==-1)// fork 오류
{
perror("fork"); // 오류 정보가 perror로 넘어간다.
exit(EXIT_FAILURE);
}

if(pid ==0)// 자식 프로세스 일 경우
{
ret=execv(path,argv);
	if(ret==-1){	//execv 오류
	perror("execv");
	exit(EXIT_FAILURE);
	}
}
wait(NULL); //자식 프로세서의 상태를 알고 싶으면 int* 형식을 NULL 대신에 넣으면 된다.
}

