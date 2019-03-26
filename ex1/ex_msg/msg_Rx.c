#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//IPC
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define P_MAX 10

struct my_score {
int long type ;
char name[20];
int korean;
int math;
int english;
int sum;
};

int main(void){
	struct my_score scores[P_MAX];
	int pNum=0;
	int msqid;
	int running=1;
	int score=0;
	//ftok () 로 키생성
	//msgget 으로 키값과 권한으로 msqid 값 반환
	//while문에서 데이터 입력 받기
	//msgsnd 로 전송
	
	
	msqid=msgget((key_t)1234,0666|IPC_CREAT);

	if(msqid==-1){
		fprintf(stderr,"msgget error %d\n",errno);
		exit(EXIT_FAILURE);
	}

	printf("성적 출력\n");	
	while(running&&(pNum<(P_MAX))){
	
		if(msgrcv(msqid,&scores[pNum],sizeof(scores[pNum])-sizeof(long),0,0)==-1){
			fprintf(stderr,"msgrcv  error %d\n",errno);
			exit(EXIT_FAILURE);
		}

		if(!(strncmp(scores[pNum].name,"end",3)))
			running=0;
		else
		pNum++;
	}
	puts("-----------------------------------------------------------------------------");
	printf("No.\t 이름\t 국어\t 수학\t 영어\t 총점\t 평균\n");
	for(int i=0;i<pNum;i++){
	printf("%d.\t %s\t %d\t %d\t %d\t %d\t %d \n",i+1,scores[i].name,scores[i].korean,scores[i].math,scores[i].english,scores[i].sum,((scores[i].sum)/3));
	
	}
	puts("-----------------------------------------------------------------------------");
	
	if((msgctl(msqid,IPC_RMID,0))==-1){
		fprintf(stderr,"msgctl error %d\n",errno);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
	}
