#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// IPC & semaphore
#include <sys/ipc.h>
#include <sys/sem.h>

struct sembuf {
	short sem_num;   // 세마포어 번호
	short sem_op;    // 세마포어 증감값 op=1 op=-1
	short sem_flg;   // 옵션	IPC_NUWAIT, SEM_NUMDO
}
/*
IPC_NOWAIT : 호출 즉시 실행하지 못했을 경우 기다리지 않고
             실패로 바로 복귀합니다.
SEM_UNDO   : 프로세스가 종료되면 시스템에서 세마포어 설정을
             원래 상태로 되돌립니다.(보통 이 옵션을 사용)
*/


union semun {
	int				val;
	struct semid_ds *buf;
	unsigned short int *array;
}arg;


int cnt = 0;
static int semid;

void p()
{
	struct sembuf pbuf;
	pbuf.sem_num = 0;
	pbuf.sem_op  = -1;
	pbuf.sem_flg = SEM_UNDO;
	if (semop(semid, &pbuf, 1) == -1)// semop(세마포어 id,data 주소,1= 바이너리,1 이상 카운팅 세마포어(사용할 수만큼만))
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

void* fun_thread1(void* arg){//증가
	
	while(1){
	p();
	cnt++;
	v();
	if(cnt>50000)
		break;
	} 


}

void* fun_thread2(void* arg){//감소
	while(1){
	p();
	cnt--;
	v();
	if(cnt>-50000)
		break;


}


void* fun_thread2(void* arg){//감소
	while(1){
	p();
	printf("cnt:%d\n",cnt);
	fflush(stdout);
	v();

}


int main(int argc, char *argv[])
{
	pthread_t p_thread[3];
	//step1. semget 세마포어 생성
	if((semid=semget(IPC_PRIVATE, 1, IPC_CREAT|0666))==-1) 
	{
		perror("Error:semget()a\n");
		return -1;
	}

	arg.val = 1;

	//step2. semctl SETVAL
	if (semctl(semid, 0, SETVAL, arg) == -1)
	{
		perror("Error:semctl():SETVAL\n");
		return -1;
	}
p_thread_create(&p_thread[0],NULL,fun_thread1,NULL);
p_thread_create(&p_thread[1],NULL,fun_thread2,NULL);
p_thread_create(&p_thread[2],NULL,fun_thread3,NULL);

pthread_join(p_thread[0],NULL);
pthread_join(p_thread[1],NULL);
pthread_join(p_thread[2],NULL);


	if (semctl(semid, 0, IPC_RMID, arg) == -1)
	{
		perror("semctl():IPC_RMID");
		return -1;
	}

	//Test Code
	while (1)
	{
		if (cnt < 9)
		{
			p();
			cnt++;
			v();
			printf("increase :%d\n", cnt);
			usleep(100);

		}
		else
		{
			p();
			cnt--;
			v();
			printf("decrease :%d\n", cnt);
			break;
		}
	}
 	return 0;
}

