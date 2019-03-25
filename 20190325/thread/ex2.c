#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#include <time.h>

long long temp;

void* t_F_1(void){
long long temp;
	int i=1;
	while(i<100000)
	{
		temp+=i;
		i++;
	}

	return(void*)temp;
}

void* t_F_2(void){
long long temp;
	int i=100000;
	while(i<=200000)
	{
		temp+=i;
		i++;
	}

	return(void*)temp;
}


int main(){
	clock_t start1,start2,end1,end2;
	pthread_t p_thread[2];
	
	unsigned long long status1=0,status2=0;
	int ret1,ret2;
	start1=clock();
	if((ret1=pthread_create(&p_thread[0],NULL,t_F_1,(void*)NULL))<0){
	
		perror("ERROR: create\n");
		return -1;
	}

	start2=clock();
	if((ret2=pthread_create(&p_thread[1],NULL,t_F_2,(void*)NULL))<0){
	
		perror("ERROR: create\n");
		return -1;
	}
	pthread_join(p_thread[0],(void**)&status1);
	end1=clock();	
	pthread_join(p_thread[1],(void**)&status2);
	end2=clock();
	sleep(5);
	printf("sum:%lld \n",status1+status2);
	printf("1:쓰레드 소요된 시간 : %.3f ms \n",(float)(end1-start1));
	printf("2:쓰레드  소요된 시간 : %.3f ms \n",(float)(end2-start2));

	return 0;
}
