#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h> //getpwuid()
#include <grp.h> //getgrgid()
#include <time.h>
#include <errno.h>
#define DEBUG
#define PATH_LENGTH 80
#define PERM_LENGTH 11
//=========================================
// dirent 구조체
//=========================================
/*
struct dirent
{
	long d_ino; //inode 번호
	off_t d_off; // offset
	unsigned short d_reclen; //d_name 길이
	char d_name[NAME_MAX+1]; //파일 이름
};
*/

//=========================================
// stat 구조체
//=========================================
/*
struct stat {
    dev_t     st_dev;     // ID of device containing file 
    ino_t     st_ino;     // inode number 
    mode_t    st_mode;    // 파일의 종류 및 접근권한
    nlink_t   st_nlink;   // hardlink 된 횟수
    uid_t     st_uid;     // 파일의 owner
    gid_t     st_gid;     // group ID of owner
    dev_t     st_rdev;    // device ID (if special file)
    off_t     st_size;    // 파일의 크기(bytes)
    blksize_t st_blksize; // blocksize for file system I/O
    blkcnt_t  st_blocks;  // number of 512B blocks allocated
    time_t    st_atime;   // time of last access
    time_t    st_mtime;   // time of last modification
    time_t    st_ctime;   // time of last status change
};
*/


//=========================================
// Function Name : access Name
// arg1 : permission value
// arg2 : mode 
// return : void
// Description : 
//=========================================


void access_perm(char *perm, mode_t mode)
{
	int i;
	char permchar[] = "rwx";
	memset(perm, '-', 10);
	perm[10] = '\0';

	if (S_ISDIR(mode)) perm[0] = 'd';
	else if (S_ISCHR(mode)) perm[0] = 'c';
	else if (S_ISBLK(mode)) perm[0] = 'b';
	else if (S_ISFIFO(mode)) perm[0] = 'p';
	else if (S_ISLNK(mode)) perm[0] = 'l';

	for (i = 0; i < 9; i++) 
	{
		if ((mode << i) & 0x100)
			perm[i+1] = permchar[i%3];
	}

	if (mode & S_ISUID) perm[3] = 's';
	if (mode & S_ISGID) perm[6] = 's';
	if (mode & S_ISVTX) perm[9] = 't';
}


int main(int argc, char *argv[])
{
	DIR *dp;			// DIR pointer
	struct stat statbuf;		// inode info
	struct dirent **dent;		//
	struct group *group_entry; 	//
	struct passwd *user_entry;	//
	struct tm *tm; 			//time 
	char perm[PERM_LENGTH]; 		//파일권한정보를 위함 10btye +NULL
	char pathname[PATH_LENGTH];			//파일 주소 
	char temp[20];
	char dirname[PATH_LENGTH];


	//flag
	int flag =0;

	int count=0;
	// 옵션 값 확인
	if (argc==1){ //ls
			flag=0;
	strcpy(dirname,".");
	}

	if(argc>1){
	sprintf(temp,"-al");

	if(!strcmp(temp,argv[1])){
		flag =1; //ls -al
		strcpy(dirname,".");
	}else{
		strcpy(dirname,argv[1]);
		flag=2; //ls path
		}
	}

	if(argc>2){
	strcpy(dirname,argv[2]);
	flag=3;//ls path -al
	}
	
	
#ifdef DEBUG
	printf("flag=%d\n",flag);
	printf("dir=%s\n",dirname);
#endif
			
	//디렉터리의 inode정보 읽기
	stat(dirname, &statbuf);
	if (!S_ISDIR(statbuf.st_mode)) //디렉터리가 아닌 경우 
	{
		fprintf(stderr, "%s is not directory\n",dirname);
		printf("a");
		exit(1);
	}
	//디렉터리 열기
	if ((dp = opendir(dirname)) == NULL) //디렉터리가 정상적으로 열리지 않은 경우
	{
		perror("Error:");
		printf("b");
		exit(1);
	}

	//정상적으로 열렸을 경우 디렉터리 명 출력
	if((count=scandir(dirname,&dent,NULL,alphasort))== -1){
	fprintf(stderr,"%s Directory Scan Error: %s \n",pathname,strerror(errno));								
	exit(1);				
	}
	printf("Lists of Directory(%s):\n", dirname);
	printf("==================================================\n\n");
	//디렉터리 정보 (inode)를 읽어 온다.
	for(int a=0;count>a;a++) // 디렉터리 내용이 없을 때까지 파일목록 끝에가면 NULL
	{
		if(flag==1|flag==3){
			printf("d");
			sprintf(pathname, "%s/%s", dirname, dent[a]->d_name);
			lstat(pathname, &statbuf);
			user_entry=getpwuid(statbuf.st_uid);
			group_entry=getgrgid(statbuf.st_gid);
			access_perm(perm, statbuf.st_mode);
			tm = localtime(&statbuf.st_mtime);
			strftime(temp, sizeof(temp), "%m월 %e일 %H:%M", tm); // 사용자 정의 문자열 지정
			printf("%s ",perm);
			printf("%ld",statbuf.st_nlink);
			printf("%s",user_entry->pw_name)
			printf("%s",group_entry->gr_name)
			printf("%8ld",statbuf.st_size)
			printf("%s" ,temp);
			printf("%s\n",dent[a]->d_name);
		}

		else
		{
		printf(" %s ",dent[a]->d_name);
			
		}

	free(dent[a]);
	}
	free(dent);	
printf("\n\n=====================================================\n");

	return 0;
}
