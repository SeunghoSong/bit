#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

/*
 exec 함수 
프로세스를 그대로 유지하면서 
새로운 프로그램을 기존 실행 프로그램에 덮어쓴다.(기존 실행 프로그램은 없어져 버린다.)

[exec 함수 종류]

execl(const char *path,const char *arg,...);
execlp(const char *file,const char *arg,...);
execle(const char *path,const char *arg,...,char * const envp[]);
execv(const char *path,char *const argv[]);
execvp(const char *file,char *const argv[]);
execve(const char path,char *const argv[],char *const envp[]);

--매개변수 명칭--
const char *path	: 실행파일 경로
const char *file	: 실행파일
const char *arg		: 명령어또는 옵션
const char *argv[]	: 명령어 배열
const char *envp[]	: 검색 할 환경변수 배열


--exec 뒤에 붙은 글자 의미--
l : argv(명령어 집합)가 list로 나열된다. ex) arg1, arg2 ....,NULL
v : argv가 vector(배열)로 나열한다. ex) argv[]{arg1,arg2....,NULL}
p : 명령어/실행 파일이 PATH로 지정된 디렉토리에 있다면 PATH를 적지 않아도 된다.
e : 설정할 환경변수를 매개변수로 받는다.

-- exec 함수 사이의 관계 --
┌──────┐         ┌──────┐       ┌──────┐
│execlp│         │execl │       │execle│
└──────┘         └──────┘       └──────┘
    │                │              │
  argv[]           argv[]         argv[]
    │                │              │
    │                │              │
┌──────┐  파일   ┌──────┐  환경  ┌──────┐
│execvp│ ────▶   │execv │ ────▶  │execvp│
└──────┘ 경로로  └──────┘ 변수로 └──────┘

*/





// #define 을 이용해 원하는 함수를 테스트 하도록한다.

#define EXECLP


/////////////////////////////////////////////////////////////////// execl 예제

#ifdef EXECL

int main(void){
printf("Running ps with execl\n");
//execlp("파일 경로","명령어","옵션"...,"NULL")
execl("/bin/ps","ps","-ax",NULL);   

// 이 이하로는 실행이 안됨
printf("Done.\n");
exit(0);
}
// +문자열 -> execv
#endif


////////////////////////////////////////////////////////////////// execlp 예제

#ifdef EXECLP

int main(void){
printf("Running ps with execlp\n");
//execlp("파일이름","명령어","옵션"..."NULL")
execlp("ps","ps","-ax",NULL);   

// 이 이하로는 실행이 안됨
printf("Done.\n");
exit(0);
}
//+문자열(v) -> execvp
#endif


//////////////////////////////////////////////////////////////////// execle 예제

#ifdef EXECLE

int main(void){
// 환경변수 검색 배열 = {"PATH=파일이 존재하는 디렉터리","NULL"}
char*const ps_envp[]={"PATH=/bin",NULL};
printf("Running ps with execle\n");
//execv("파일이름","명령어","옵션",..."환경변수 검색 배열명")
execve("ps","ps","-rf","ps_envp");   

// 이 이하로는 실행이 안됨
printf("Done.\n");
exit(0);
}
//+명령어 배열(v) ->execve
#endif


/////////////////////////////////////////////////////////////////// execv 예제

#ifdef EXECV

int main(void){
//명령어리스트 배열 ={"명령어","옵션",..."NULL"}
char*const arg[]={"ps", "-ax",NULL}; 
printf("Running ps with execv\n");
//execv("경로","명령어리스트 문자열 배열명")
execv("/bin/ps","-ax",NULL);   

// 이 이하로는 실행이 안됨
printf("Done.\n");
exit(0);
}
//+환경 변수(e) ->execve
#endif


/////////////////////////////////////////////////////////////////// execvp 예제

#ifdef EXECVP

int main(void){
//명령어리스트 배열 ={"명령어","옵션",..."NULL"}
char*const arg[]={"ps", "-ax",NULL}; 
printf("Running ps with execvp\n");
//execv("파일이름","명령어리스트  배열명")
execvp("ps","arg");   

// 이 이하로는 실행이 안됨
printf("Done.\n");
exit(0);
}
//+경로(p) ->execv
#endif


//////////////////////////////////////////////////////////////////// execve 예제

#ifdef EXECVE

int main(void){
//명령어리스트 배열 ={"명령어","옵션",..."NULL"}
char*const arg[]={"ps", "-ax",NULL};
//환경변수 검색 배열 = {"PATH=파일이 존재하는 디렉터리","NULL"}
char*const ps_envp[]={"PATH=/bin",NULL};
printf("Running ps with execve\n");
//execv("파일이름","명령어리스트 배열명","환경변수 검색 배열명")
execve("/bin/ps","arg","ps_envp");   

// 이 이하로는 실행이 안됨
printf("Done.\n");
exit(0);
}

#endif
