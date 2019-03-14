#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>  //open()

#define DEBUG

int main (){
int fd;
int val;
fd =open("exec_copy.txt",O_CREAT|O_TRUNC,0644);
val=fcntl(fd,F_GETFD,0); //get FD_CLOSEXEC

#ifdef DEBUG
if(val&FD_CLOEXEC)
		printf("Before : close-in-exc bit on\n");
else
		printf("Before : close-in-exc bit off\n");
#endif

val|=FD_CLOEXEC;  //modify FD_CLOSEXEC

#ifdef DEBUG
if(val&FD_CLOEXEC)
		printf("After : close-in-exc bit on\n");
else
		printf("After : close-in-exc bit off\n");
#endif

execl("./loop","./loop",(char*)0);

return 0;


}
