#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//IPC
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define P_MAX 10 //최대로 적을수 있는 수
#define M_MAX 100
struct my_score {
char name[20];
int korean;
int math;
int english;
int sum;
};




int main(void)
{
// 데이터를 담을 구조체
	struct my_score scores[P_MAX];
	
//공유 메모리 주소값
	void* shared_mem=(void*)0;//shmat에서 공유메모리 주소값을 받아옴
	struct my_score* shmaddr;	//my_score 구조체를 담을수 있는 장소의 주소값을 받을 수 있어야한다. 
	
// 공유메모리 id
	int shmid;
	

	int pNum=0; 	//구조체 배열 번호
	int running=1; //반복분 제어
	
//shmget
	shmid=shmget((key_t)1234,sizeof(struct my_score)*P_MAX,0666|IPC_CREAT);

	if(shmid==-1) //shmget error
	{
		fprintf(stderr,"shmid error %d\n",errno);
		exit(EXIT_FAILURE);
	}
//shmat
	shared_mem=shmat(shmid,(void*)0,0);
	
	if(shared_mem==-1) // shmat error
	{
		fprintf(stderr,"shmat error %d\n",errno);
		exit(EXIT_FAILURE);
	}

//구조체 포인터 로 주소 이동
	shmaddr=(struct my_score*)shared_mem;

// 성적 출력
	printf("성적 출력\n");	
	while(running&&(pNum<(P_MAX)))
	{
		//shared memory access
		scores[pNum]=shmaddr[pNum];

		if(!(strncmp(scores[pNum].name,"end",3)))
				running=0;
			else
				pNum++;
	}


	puts("-----------------------------------------------------------------------------");
	printf("No.\t 이름\t 국어\t 수학\t 영어\t 총점\t 평균\n");

	for(int i=0;i<pNum;i++)
	{
		printf("%d.\t %s\t %d\t %d\t %d\t %d\t %d \n",i+1,scores[i].name,scores[i].korean,scores[i].math,scores[i].english,scores[i].sum,((scores[i].sum)/3));
	
	}
	puts("-----------------------------------------------------------------------------");
	
//shmdt()
	if((shmdt(shared_mem))==-1)// shmdt error
	{
		fprintf(stderr,"shmctl error %d\n",errno);
		exit(EXIT_FAILURE);
	}

//shmctl()
	if((shmctl(shmid,IPC_RMID,0))==-1) //shmctl error
	{
		fprintf(stderr,"shmctl error %d\n",errno);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
	}
