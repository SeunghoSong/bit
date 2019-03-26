#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


void* t_F(void){
	unsigned long long temp=0;
	int i=0;
	while(i<=200000)
	{
		temp+=i;
		i++;
	}

	return(void*)temp;
}

int main(){
	clock_t start1,end1;
	pthread_t p_thread;
	
	unsigned long long status=0;
	int ret;
	start1=clock();
	if((ret=pthread_create(&p_thread,NULL,t_F,(void*)NULL))<0){
	
		perror("ERROR: create\n");
		return -1;
	}

	pthread_join(p_thread,(void**)&status);
	end1=clock();
	printf("sum:%lld \n",status);
	printf("일반함수 소요된 시간 : %.3f ms \n",(float)(end1-start1));


	return 0;
}
