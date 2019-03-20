
#define UPPER

#ifdef  UPPER
#include <stdio.h>
#include <ctype.h>

int main()
{
int ch;
// 문자 입력을 받는다. 버퍼의 형태로 한글자씩 담긴다.
while((ch=getchar())!=EOF){ //EOF(End - of -file) 파일의 끝. 값은 -1 즉 getchar 가 NULL 값을 리턴했을때에 종료  
putchar(toupper(ch)); // toupper() 소문자를 대문자로 출력한다. putchar()에서 한글자씩 출력
}
exit(0);


//phj790122.blog.me
}
#endif

//위에 upper 를 먼저 컴파일 후에 이 main 함수는 userupper.
#ifdef USERUPPER
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
char *filename;

if(argc!=2){ //쉘에 파일 이름  외에 적힌게 있을경우
fprintf(stderr,"usage:useupper file\n");
exit(1);
}
filename = argv[1]; // 파일이름을 저장한다:
if(!freopen(filename,"r",stdin)){ //파일 입력으로 파일을 읽어 온다.
fprintf(stderr,"could not readirect stdin form file %s\n",filename);
exit(2);
}
execl("./upper","upper",0);//upper 프로그램으로 덮어 씌어진다. 하지만 스트림 영역은 덮어지지 않고 남아있다.
perror("could not extec ./upper"); // upeer 실행 파일 이 없을 경우
exit(3);
}
#endif
