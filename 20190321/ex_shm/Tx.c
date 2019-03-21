#include "my_shm.h"

int main(void){
	struct my_score scores[M_MAX];
	struct my_score *s_addr;
	void* addr =(void*)0;
	int pNum=0;
	int shmid;
	int running=1;
	int score=0;
	//ftok () 로 키생성
	//msgget 으로 키값과 권한으로 msqid 값 반환
	//while문에서 데이터 입력 받기
	//msgsnd 로 전송
	
	
	shmid=shmget((key_t)1234,sizeof(struct my_score)*M_MAX,0666|IPC_CREAT);
	if(shmid==-1){
		fprintf(stderr,"shmid error %d\n",errno);
		exit(EXIT_FAILURE);
	}
	
	addr=shmat(shmid,(void*)0,0);
	
	s_addr=(struct my_score*)addr;
	printf("성적을 입력하세요\n");	
	while(running&&(pNum<(P_MAX))){
		printf("%d.No \n",pNum+1);
		puts("이름");
		scanf("%s",&scores[pNum].name);
		puts("국어");
		scanf(" %d",&scores[pNum].korean);
		puts("수학");
		scanf(" %d",&scores[pNum].math);
		puts("영어");
		scanf(" %d",&scores[pNum].english);
		// while (getchar()!='\n');
		scores[pNum].sum=scores[pNum].korean+scores[pNum].math+scores[pNum].english;
		s_addr+pNum=scores[pNum];	
		if(!(strncmp(scores[pNum].name,"end",3)))
			running=0;
		pNum++;
	}


	exit(EXIT_SUCCESS);
	}
