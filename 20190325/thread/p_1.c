#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//POSIX Thread
#include <pthread.h>

int g_data= 99;
int * h_addr;
//Thread function
void* t_function(void* data){
	int argData;
	pthread_t t_id;

	argData=*((int*)data);//void 형태의 'data'를 int* 로 캐스팅(형변환)하여 데이터값을 참조

	t_id=pthread_self(); // 자기자신의 TID  리턴
	sleep(argData);
	printf("getpid= %d, t_id=%dlu, argData=%d\n",getpid(),t_id,argData);
	//heap , global_data test
	printf("heap:\n");
	for(int i=0;i<4;i++)
	{
		printf("%d\n",*g_addr++);
	}
	sleep(5);
	
	return (void*) (argData*argData);
}



int main (void){

	// POSIX thread id data type;
	pthread_t p_thread[2];
	int ret; //return value
	int a=1,b=2;
	int status;
	int * h_addr;
	h_addr=malloc(sizeof(int)*4);

	for(int i=0;i<4;i++)
	{
		h_addr*++=i;
	
	
	}
	printf("PID= %d\n",getpid());

	//(P_thread[]의 주소(생성된 쓰레드 주소값 리턴),NULL,쓰레드 함수 네임,쓰레드에 넘겨줄값,)
	if((ret=pthread_create(&p_thread[0],NULL,t_function,(void*)&a))<0) 
	{
		perror("ERROR : pthread_creat1()");
		return -1;
	}

	if((ret=pthread_create(&p_thread[1],NULL,t_function,(void*)&b))<0)
	{
		perror("ERROR : pthread_creat2()");
		return -2;
	}
	
	//thread waiting
	pthread_join(p_thread[0],(void **)&status); 
	printf("thread_join1:%d\n",status);

	pthread_join(p_thread[1],(void **)&status);
	printf("thread_join2:%d\n",status);

}
