#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>
#include <stdlib.h>

//posix_spawn

/*
//헤더//
spawn.h
 
함수
int posix_spawn(pid_t *restrict pid,
				const char *restrict path,
       			const posix_spawn_file_actions_t *file_actions,
       			const posix_spawnattr_t *restrict attrp,
       			char *const argv[restrict],
				 char *const envp[restrict]);

int posix_spawnp(pid_t *restrict pid,
				const char *restrict file,
       			const posix_spawn_file_actions_t *file_actions,
       			const posix_spawnattr_t *restrict attrp,
       			char *const argv[restrict], 
				char *const envp[restrict]);

 
*/

/*참고*/
/*
my_github

 
fork-exec 를 쓰지말고 posix_spawn으로 써야 하는이유
https://sunyzero.tistory.com/191
fork-exec 에서 posix_spawn으로 바꾸기
https://koyo.kr/post/posix_spawn-instead-of-fork-exec          
*/



/*예제*/

int my_posix_spawn(const char *path,char *const argv[]){
int ref;
pid_t pid;

pid=posix_spawn(&pid,path,NULL,NULL,argv,NULL);

int status;
waitpid(pid,&status,0);//해당 pid를 가진 
return ref;
}

int main(){
char path[]= "/bin/ls";
char* argv_ls[]={"ls","-al",NULL};
my_posix_spawn(path,  argv_ls);
return 0;
}
