#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

/*
뮤텍스 (mutex)
공유 자원에 대한 접근 권한, lock 이라는 키를 한개만
가지고 있는것 한 자원을 한 쓰레드만 접근이 가능하다.

header
pthread.h 
-쓰레드 관련 (세마포어,뮤텍스 ..등) 함수및 구조체
				
struct

pthread_mutex_t 
	-thread 에서 뮤텍스를 사용하기 위한 구조체 
	(구조체 내부 속성을 직접 지정해서 사용하지 않아도 함수를 이용하여  초기화된다.)

-attribute
	(자세히 알고 싶다면)
	https://stackoverflow.com/questions/30585375/what-is-the-type-of-pthread-mutex-t


fuc

pthread_mutex_init(pthread_mutex_t *mutex,const pthread_mutex_attr *attr)
뮤텍스를 사용하기 위한 초기화 작업 
pthread_mutex_t 구조체에 뮤텍스 사용을 위한 정보와 초기값이 들어간다.

-매개변수
(&pthread_mutex_t 구조체 변수,뮤텍스 정적 설정값(필요없을시 NULL))// 수정 필요

pthread_mutex_lock(pthread_mutex_t *mutex)
뮤텍스를 이용하여 해당 자원의 접근을 막는다. 
실행한 스레드만 접근이 가능해진다.

pthread_mutex_unlock(pthread_mutex_t *mutex)
뮤텍스를 이용하여 해당 자원의 접근을 허용한다.
실행하지 않을경우 다른 스레드는 접근이 불가한다.

pthread_mutex_destroy(pthread_mutex_t *mutex)
뮤텍스  해제한다.

*/
int cnt=0;

pthread_mutex_t crit_cnt; // 세마 포어 의  OP역할 겸


void* inc_func(void* hi){
	
	pthread_mutex_lock(&crit_cnt);
	printf("Before INC:%d\n",cnt);
	cnt++;
	printf("After INC:%d\n",cnt);
	pthread_mutex_unlock(&crit_cnt);
}

void* dec_func(void* hi){

	pthread_mutex_lock(&crit_cnt);
	printf("Before DEC:%d\n",cnt);
	cnt--;
	printf("After DEC:%d\n",cnt);
	pthread_mutex_unlock(&crit_cnt);
}

int main (int argc, char* argv[]){
	
	pthread_t p_thread[2];
	
	pthread_mutex_init(&crit_cnt,NULL);

	pthread_create(&p_thread[0],NULL,inc_func,NULL);
	pthread_create(&p_thread[1],NULL,dec_func,NULL);
	
	pthread_join(p_thread[0],NULL);
	pthread_join(p_thread[1],NULL);

	pthread_mutex_destroy(&crit_cnt);
	return 0;
}
