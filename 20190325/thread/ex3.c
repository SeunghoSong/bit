#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>

#include <sys/ipc.h>
#include <sys/sem.h>

//#define DEBUG
long long temp;
static int semid;
clock_t start1,start2,end1,end2;

struct numset {
	int start;
	int end;
};

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
}arg;

void p()
 {
      struct sembuf pbuf;
      pbuf.sem_num = 0;
      pbuf.sem_op  = -1;
      pbuf.sem_flg = SEM_UNDO;
     if (semop(semid, &pbuf, 1) == -1)// semop(세마포어 id,data 주소,1= 바이너리,1 이상 카운팅 세>
          perror("p:semop()\n");
 }
  
 
  void v()
 {
      struct sembuf vbuf;
      vbuf.sem_num = 0;
      vbuf.sem_op  = 1;
      vbuf.sem_flg = SEM_UNDO;
      if (semop(semid, &vbuf, 1) == -1)
          perror("v:semop()\n");
 }


void* t1(void* data){
	struct numset argv =*((struct numset*)data);
	int i=argv.start;
	long long t_temp=0;
	start1=clock();
	while(i<argv.end)
	{ 
		t_temp+=i;
		i++;
	}
	p();
	temp+=t_temp;
	v();
	end1=clock();
}

void* t2(void* data){
	struct numset argv =*((struct numset*)data);
	int i=argv.start;
	long long t_temp=0;
	start2=clock();
	while(i<=argv.end)
	{ 
		t_temp+=i;
		i++;
	}
	p();
	temp+=t_temp;
	v();
	end2=clock();
}

int main(int arvc,char* arvg[])
{
	if(arvc<2){
	printf("arg error\n");
	return -1;
	}

	pthread_t p_thread[2];
	
	unsigned long long status1=0,status2=0;
	int ret1,ret2;
	
	struct numset arg_num[2];

	arg_num[0].start=atoi(arvg[1]);
	arg_num[0].end=((atoi(arvg[1])+atoi(arvg[2]))/2);
	arg_num[1].start=((atoi(arvg[1])+atoi(arvg[2]))/2);
	arg_num[1].end=atoi(arvg[2]);

#ifdef DEBUG
printf("%d\n",arg_num[0].start);
printf("%d\n",arg_num[0].end);
printf("%d\n",arg_num[1].start);
printf("%d\n",arg_num[1].end);
#endif

 	if((semid=semget((key_t)1234,1, IPC_CREAT|0666))==-1) 
 	{
         perror("Error:semget()a\n");
          return -1;
  	}
	
	arg.val=1;
 	if (semctl(semid, 0, SETVAL,arg) == -1)
	{
         perror("Error:semctl():SETVAL\n");
         return -1;
	}
	if((ret1=pthread_create(&p_thread[0],NULL,t1,(void*)&arg_num[0]))<0)
	{
		perror("ERROR: create\n");
		return -1;
	}
	if((ret2=pthread_create(&p_thread[1],NULL,t2,(void*)&arg_num[1]))<0)
	{
		perror("ERROR: create\n");
		return -2;
	}

	pthread_join(p_thread[1],(void**)&status2);
	printf("stop thread2\n");
	
	pthread_join(p_thread[0],(void**)&status1);
	printf("stop thread1\n");
	
	if (semctl(semid, 0, IPC_RMID,arg) == -1)
	{
		perror("semctl():IPC_RMID");
		 return -1;
	}
	

	printf("sum:%lld \n",temp);
	printf("1:쓰레드 소요된 시간 : %0.f ms \n",(float)(end1-start1));
	printf("2:쓰레드  소요된 시간 : %0.f ms \n",(float)(end2-start2));

	return 0;
}
