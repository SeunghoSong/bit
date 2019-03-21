#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//IPC
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//Rx
#define SHMSIZE 100 //int*100
int main()
	{// (void*)0 초기화의 이유 쓰레기 값이 들어갈수 있기때문에 명시적으로 적어 오류를 회피하도록한다. 
	void *shared_Mem=(void*)0;// void 포인터는 주소값만 받을경우에만 사용 자료형이 정해져 있지 않기 때문
	int shmid;
	int rddata;
	int *shmaddr;
	//0. ftok()
	//1. shmget()
	shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	printf("memory attach :%p\n",(unsigned int)shared_Mem);
	
	//2. shmat

	shared_Mem=shmat(shmid,(void*)0,0);
	if(shared_Mem==-1)
	{
		fprintf(stderr,"shmat error %d\n",errno);
		exit(EXIT_FAILURE);
	}
	printf("memory attach :%p\n",(unsigned int)shared_Mem);
	shmaddr=(int*)shared_Mem;
	//3. memory access
	for(int i=0;i<SHMSIZE;i++)
	{
	rddata=*(shmaddr+i);
	printf("add:%p, data %d\n",(unsigned int)shmaddr+i,rddata);
	}
	//4. shmdt
	if(shmdt(shared_Mem)==-1)
	{
		fprintf(stderr,"shmdt error %d\n",errno);
		exit(EXIT_FAILURE);
	}
	//5. shmctl
	if(shmctl(shmid, IPC_RMID,0)==-1)
	{
		fprintf(stderr,"shmctl error %d\n",errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
	}
