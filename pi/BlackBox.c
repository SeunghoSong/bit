#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

#define DEBUG
#define VIDEOTIME 60000 //ms
#define p_width 320
#define p_height 260
#define DIR_PATH "/home/pi/BlackBox/"
/*
1.카메라 sd 카드 확인
2.옵션 설정
3. 카메라 작동


*/

//시그널 핸들러

 pid_t pid;


static void sig_handler(int signum){
    kill(pid,SIGKILL);


}


//파일 관련 함수
const char *MMOUNT = "/proc/mounts";//파티션정보를 담고 있는곳

struct f_size //
{
    long blocks; //전체 블럭수 
    long avail;  //사용 가능한 블럭수
};

typedef struct 
{
    FILE *fp;               // 파일 스트림 포인터    
    char devname[80];       // 장치 이름
    char mountdir[80];      // 마운트 디렉토리 이름
    char fstype[12];        // 파일 시스템 타입
    long f_type;
    long f_blocks;	    // Total Block count
    long f_bavail;	    // unused block count(Normal User)
    
	struct f_size size;     // 파일 시스템의 총크기/사용율 
} MOUNTP;

MOUNTP *dfopen()
{
    MOUNTP *MP;

    // /proc/mounts 파일을 연다.
    MP = (MOUNTP *)malloc(sizeof(MOUNTP));
    if(!(MP->fp = fopen(MMOUNT, "r")))
    {
        return NULL;
    }
    else
        return MP;
}

MOUNTP *dfget(MOUNTP *MP)
{
    char buf[256];
    char *bname;
    char null[16];
    struct statfs lstatfs;
    struct stat lstat; 
    int is_root = 0;

    // /proc/mounts로 부터 마운트된 파티션의 정보를 한줄씩 얻어온다.
    while(fgets(buf, 256, MP->fp))
    {
       // is_root = 0;
        sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);//버퍼로 부터 문자입력을 받아옴
         if (strcmp(MP->mountdir,"/") == 0) is_root=1;
        if (stat(MP->devname, &lstat) == 0 || is_root) 
        {
            if (strstr(buf, MP->mountdir) && S_ISBLK(lstat.st_mode) || is_root)//문자열에서 임의의 문자열을 검색
            {
                	// 파일시스템의 총 할당된 크기와 사용량을 구한다.        
                statfs(MP->mountdir, &lstatfs);
                MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024); //한블럭 사이즈가 1024 byte
                MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024);
                return MP;
            }
        }
    }
    rewind(MP->fp);
    return NULL;
}

int dfclose(MOUNTP *MP)
{
    fclose(MP->fp);
}




int main (int argc, char* argv[] ){
    time_t UTCtime; // time()->
    struct tm *tm;

    char dirnamebuf[BUFSIZ];
    char filenamebuf[BUFSIZ];
    char tempbuf[25];
    char cmdbuf[BUFSIZ];
    char fullpath[BUFSIZ];

    int width;
    int height;
    int status;
    
	MOUNTP *MP;

	float remain;
    
    //옵션선택 0 :FHD 1920*1080  1 : HD 1280*720

  if(!(strcmp("0",argv[1]))){//FHD 
    width =1920;
    height=1080;
  }else if(!(strcmp("1",argv[1]))){//HD
    width =1280;
    height=720;
  }else{            //ERROR
     printf("Error: argv[1] is not valid\n");
    return -1;
  }


#ifdef DEBUG
    printf("width :%d hight:%d\n",width,height);
#endif



    while (1){
    //시간 확인
        // 커널로 부터 시간을 읽어온다. 비정형화된 데이터로 읽어옴
        time(&UTCtime);
        // tm 구조체에 년월일 시분초로 분해된 시간으로 저장 
        tm=localtime(&UTCtime);

       strftime(dirnamebuf,sizeof(dirnamebuf),"%Y%m%d%H",tm);
#ifdef DEBUG
    printf("time:%s\n",dirnamebuf);
#endif

    //disk 사이즈 확인 
        struct dirent *de=NULL; //쓰레기 값이 들어가지 않도록 오류 방지
            
		
	if(((MP=dfopen())==NULL)){

		printf("Error:dfopen()\n");
		return -2;
		}
	dfget(MP);
	

	//남아있는 공간 확인

	remain=(((float)(MP->size.avail))/((float)(MP->size.blocks)))*100;
	//남아있는 공간이 5% 미만 일 경우
#ifdef DEBUG
 	printf("block :%ld\n",MP->size.blocks);
 	printf("avail : %ld\n",MP->size.avail);
	printf("remain : %0.f\n",remain);
#endif
	if(remain<5){
//TODO : 가장 오래된 디렉토리를 찾아서 삭제
	}
	dfclose(MP);	

//TODO 디렉터리 생성

	strftime(dirnamebuf,sizeof(dirnamebuf),"%Y%m%d%H",tm); //디렉터리 명
	
#ifdef DEBUG
printf ("dirname: %s",dirnamebuf);
#endif 

    
    //디렉토리 명을 현재  시간으로  디렉토리 생성
    /*
     디렉터리에 대한 에러 처리
    EACCES:생성 하려는 부모가 쓰기 권한이 없음
    EFAULT:
    ENOTDIR:
    EEXIST :동일한 디렉터리가 존재 
     */
	if((mkdir(dirnamebuf,0666))==-1){
        if (errno !=EEXIST)
        {
        return -1;
        }
    }
	chmod(dirnamebuf,0777); //권한 재설정
	

    
//TODO 파일 생성

    strftime(tempbuf,sizeof(dirnamebuf),"%Y%m%d_%H%M%S.h264",tm);// 파일 명
	
    /*파일 생성시 주의점
    파일을 디텍터리에 넣어 생성 하고자 한다면
	파일의 상위 디렉터리도 알고 있어야한다. */
    sprintf(filenamebuf,"%s/%s", dirnamebuf, tempbuf); // 상위 디렉터리와 파일 명을 합친다.
    
    //파일 생성을 위해 카메라 관련  명령어 실행 
    sprintf(cmdbuf,"raspivid -p 0,0,%d,%d -w %d -h %d -t %d -o %s",p_width,p_height,width,height,VIDEOTIME,filenamebuf); 

#ifdef DEBUG
	printf("cmd: %s\n",cmdbuf);
#endif

/*
 부모 fork():-----> 자식 sysfuc(cmd); 
 disk 확인;              명령어 실행

 */

    pid=fork();

    if(pid>0) //부모
        {
    
    
        }else if (pid==0)// 자식
            {
                sprintf(fullpath,"/home/pi/blackbox/%s",cmdbuf);
                execlp(DIR_PATH,cmdbuf,NULL);
                exit(1);
            }
	wait(NULL);
    }   
	

return 0;
}
