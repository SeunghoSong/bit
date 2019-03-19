#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
FILE *read_fp;//파일 포인터 생성
char buffer[BUFSIZ+1]; //sizeof(BUFSIZ)+NULL 만큼 생성
printf("%ld\n",BUFSIZ); //BUFSIZ =8192
int chars_read;
memset(buffer,'\0',sizeof(buffer));
read_fp=popen("uname -a", "r");
if(read_fp!=NULL){
	chars_read=fread(buffer,sizeof(char),BUFSIZ,read_fp);
 	if(chars_read>0){
	printf("Output was: -\n%s\n",buffer);
	}
	pclose(read_fp);
	exit(EXIT_SUCCESS);
}
exit(EXIT_FAILURE);
}
