#include <stdio.h> //printf
#include <fcntl.h> //O_RDONLY,O_WRONLY,O_RDWR
#include <unistd.h>//write,read,colse
#define BUF_SIZE 1

int main(int argc, char *argv[])
{
	int fd1, fd2, len;

	char buf[BUF_SIZE];

	fd1 =open("a.txt",O_RDONLY); //open read
	fd2 =open("b.txt",O_WRONLY|O_CREAT|O_TRUNC);//open write

	while((len=read(fd1,buf,sizeof(buf)))>0)
			write(fd2,buf,len);

	close(fd1);
	close(fd2);

	return 0;


}
