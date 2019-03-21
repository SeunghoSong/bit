#include "my_msg.h"

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

	printf("성적을 입력하세요\n");	
	while(running&&(pNum<(P_MAX))){
		scores[pNum].type=1;
		printf("%d.No \n",pNum+1);
		puts("이름");
		scanf("%s",&scores[pNum].name);
		puts("국어");
		scanf("%d",&scores[pNum].korean);
		puts("수학");
		scanf("%d",&scores[pNum].math);
		puts("영어");
		scanf("%d",&scores[pNum].english);
		while (getchar()!='\n');
		scores[pNum].sum=scores[pNum].korean+scores[pNum].math+scores[pNum].english;
		if(msgsnd(msqid,&scores[pNum],sizeof(scores[pNum])-sizeof(long),0)==-1){
			fprintf(stderr,"msgqid  error %d\n",errno);
			exit(EXIT_FAILURE);
		}
		if(!(strncmp(scores[pNum].name,"end",3)))
			running=0;
		pNum++;
	}


	exit(EXIT_SUCCESS);
	}
