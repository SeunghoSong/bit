#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//IPC
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define P_MAX 10 //입력 최대값

struct my_score {	// 공유 메모리에 저장할 구조체
char name[20];
int korean;
int math;
int english;
int sum;
};

int main(void)
{
	struct my_score scores[P_MAX]; // 구조체 배열
	struct my_score *s_addr;		// 구조체 형태의 데이터의 주소를 받기 위한 포인터
	void* addr =(void*)0;
	int pNum=0;
	int shmid;
	int running=1;

//shmget()	
	shmid=shmget((key_t)1234,sizeof(struct my_score)*P_MAX,0666|IPC_CREAT);
	if(shmid==-1){
		fprintf(stderr,"shmid error %d\n",errno);
		exit(EXIT_FAILURE);
	}
//shmat()	
	addr=shmat(shmid,(void*)0,0);
//my_scroe 구조체 포인터 형변환	
	s_addr=(struct my_score*)addr;

//공유 메모리에 데이터 입력
	printf("성적을 입력하세요\n");	
	while(running&&(pNum<(P_MAX)))
	{
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
		s_addr[pNum]=scores[pNum];	
		if(!(strncmp(scores[pNum].name,"end",3)))
			running=0;
		pNum++;
	}


	exit(EXIT_SUCCESS);
}
