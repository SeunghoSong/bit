#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
int fd;
 fd=creat("dup_result",0644);
 printf("fd=%d",fd);
int fd2 = dup2(fd,STDOUT_FILENO);
printf("fd2=%d",fd2);
close(fd); 
printf("hello world\n");
 close(fd2);
 return 0;

}
