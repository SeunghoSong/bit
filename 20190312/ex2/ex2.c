#include <stdio.h>
#include <string.h>
#define DEBUG

void insert (char* a);
void del (char* a);
void change(char* a);
void change2(char* a,char* b);

int main (void){
int loop=0;
int num=0;
int temp=0;
char getchar[51]={0,};
char get1[26]={0,};
char get2[26]={0,};
gets(getchar);
temp=strlen(getchar);
num=(temp>>1);

	for(int i=0;(temp-(num))>1;temp--,i++) //get2
			get2[i]=getchar[temp-1];

	strncpy(get1,getchar,num); //get1
do{


#ifdef DEBUG
puts(get1);
puts(get2);
printf("%d",strcmp(get2,get1));
#endif 

}while(strcmp(get1,get2))	




return 0;
}

void insert (char* a){




}

void del (char* a){


}

void change(char* a){



}

void change2(char* a,char* b){



}
