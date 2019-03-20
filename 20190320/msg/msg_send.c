#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>


//메세지큐 송신 프로그램
//(송신을 먼저 컴파일 후에 수신을 컴파일하는것을 추천한다.)


#define MAX_TEXT 512
struct my_msg_st{ //메세지 큐에 담길 데이터 묶음
long int my_msg_type; //전송한 메세지큐 속성 (msg처리에 필수적으로 있어야한다.)
char some_text[MAX_TEXT];//메세지 큐에 담길 데이터
};

int main()
{
	int running =1;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];

	msgid =msgget((key_t)1234, 0666 |IPC_CREAT) ;//메세지 큐를 생성 또는 호출 한다. msqid를 리턴
	printf("송신 측 메세지큐 값 : %d \n",msgid); //msqid를 출력
	
	if(msgid==-1) //msgget 함수가 오류가 났을경우 (오류시 리턴 -1)
	{
		fprintf(stderr,"msgget failed with error%d\n",errno);
		exit(EXIT_FAILURE);
	}

	//문장을 받아 메세지 큐에 저장한다.
	while (running)
	{
		printf("Enter some text:");
		fgets(buffer,BUFSIZ,stdin);//문장을 받는다.
		some_data.my_msg_type =1;//  
		strcpy(some_data.some_text,buffer); //받은 문장을 my_msg_st 구조체의 some_text에 저장한다.
		//msqid 값을 이용하여 해당 메세지큐에 데이터를 집어 넣도록한다.
		if(msgsnd(msgid,(void*)&some_data,MAX_TEXT,0)==-1)//메세지큐에 보내는것을 실패한 경우 
		{
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
		}
		if(strncmp(buffer,"end",3)==0)//end 값을 만났을경우 반복문을 종료한다.
			running=0;
	}

	exit(EXIT_SUCCESS);
}

