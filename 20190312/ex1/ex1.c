#include <stdio.h>
#include <string.h>

int main (void){
int num=0;
int temp=0;
char getchar[100]={0,};
char get1[50]={0,};
char get2[50]={0,};
gets(getchar);
if((strlen(getchar)%2)==1){
	temp=strlen(getchar);
	num=temp/2;
	for(int i=0;(temp-(num+1))>0;temp--,i++)
			get2[i]=getchar[temp-1];
	strncpy(get1,getchar,num);
	printf("%s\n",get1);
	printf("%s\n",get2);
	if(strcmp(get1,get2)==0)
		printf("yes");
	else
		printf("no");
}else
	printf("no");


}
