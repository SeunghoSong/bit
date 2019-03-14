#include <sys/wait.h>
//#include <tlpi_hdr.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CMD_LEN 200 


int main (int argc, char *argv[]){
char str [MAX_CMD_LEN];	//system 함수에서 실행할 명령을 저장하기 위한 문자열 배열
int status;		//뇬ㅅ드 함수가 실행후 종료시 실행 결과 값을 저장할 변수

for(;;){// ==while(1)
printf("commond: ");
fflush(stdout); // 비워질때까지대기-> 처리와 동시에 입력값이 들어가지 않도록 하기 위함
if(fgets(str,MAX_CMD_LEN,stdin)==NULL) //200자 까지 받음
		break;
//입력이 정상적일 경우
status= system(str);
printf("system( return ed : status =0x%04x (%d,%d)\n)",
				(unsigned int )status,status >> 8,status &0xff);
//실행 결과에 대한 구문처리
if(status==-1){
printf("system\n");

}else{
if(WIFEXITED(status)&&WEXITSTATUS(status)==127)
		printf("(Probably)could not invoke shell\n");
else
		printf("status:%d ",status);

}

}
exit(0);

}
