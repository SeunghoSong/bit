#include <stdio.h>
#include <unistd.h>

int adder(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
float diver(int a, int b);

int main (void){    
	int value1=100, value2=20;
	int result=0;                   
	float result2=0.0;

	 result =adder(value1,value2);
	 printf("%d+%d=%d\n",value1,value2,result);
	 result =sub(value1,value2);
	 printf("%d-%d=%d\n",value1,value2,result);
	 result =mul(value1,value2);
	 printf("%d*%d=%d\n",value1,value2,result);
	 result2 =diver(value1,value2);                
	 printf("%d/%d=%f\n",value1,value2,result2);
	sleep(1000);
}
