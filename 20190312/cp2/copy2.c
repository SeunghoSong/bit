#include <stdio.h> //printf
#include <fcntl.h> //O_RDONLY,O_WRONLY,O_RDWR
#include <unistd.h>//write,read,colse
#define BUF_SIZE 10
#define DEBUG
int main(int argc, char *argv[])
{
#ifdef DEBUG // debug
		printf("argc%d\n",argc);
		
		printf("argv[1]%s\n",argv[1]);
		printf("argv[2]%s\n",argv[2]);
#endif		
		
	int fd1, fd2, len;

	char buf[BUF_SIZE];

	fd1 =open(argv[1],O_RDONLY); //open read
	if(fd1==-1)
			printf("%s not open\n",argv[1]);
	fd2 =open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);//open write

	if(fd1==-1)
			printf("%s not open\n",argv[2]);
	while((len=read(fd1,buf,sizeof(buf)))>0)
			write(fd2,buf,len);

	close(fd1);
	close(fd2);

	return 0;


}
