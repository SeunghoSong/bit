#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h> //time_ex.c 
#include <string.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>

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
    if (mode & S_ISVTX) perm[9] = 't'; }   

int main(int argc, char *argv[]){

DIR *dp;
struct stat sb;
struct dirent *dent;
struct tm *tm;
struct passwd *pw;
struct group *gr;
char buf[BUFSIZ];

time_t UTCtime;
char* path ="./";
char perm[11];
char pathname[80]={"\0",};
int all_flag=0;

 for(int a=argc;a>0;a-- ){
		 printf("%s\n",argv[a-1]);
	 if((strcmp(argv[a-1],"-al")&strcmp(argv[a-1],"-l"))==0)
		 all_flag=1;
	 }

if((argv[2]==NULL)||(argv[1]==NULL)){
	dp=opendir(path);
	stat(path,&sb);
	printf("Path :%s\n",path);
}else{
dp=opendir(argv[1]);
stat(argv[1],&sb);
printf("Path :%s\n",argv[1]);
}


printf("=====================================\n");
if(all_flag==1){
	while ((dent =readdir(dp))!=NULL)
	{
			time_t make_t=sb.st_ctime;
			tm=localtime(&make_t);
			strftime(buf,sizeof(buf),"%m %e %H:%m",tm);
			sprintf(pathname,"%s/%s",argv[1],dent->d_name);
			lstat(pathname,&sb);
			access_perm(perm,sb.st_mode);
			
			pw=getpwuid(sb.st_uid);
			gr=getgrgid(sb.st_gid);
			
			printf("%s\t",perm);
			printf("%ld\t",sb.st_nlink);
			printf("%s\t",pw->pw_name);
			printf("%s\t",gr->gr_name);
			printf("%8ld\t",sb.st_size);
			printf("%s\t",buf);
			printf("%s\n",dent->d_name);
	}
}else{
while ((dent =readdir(dp))!=NULL)
	    {
		sprintf(pathname,"%s/%s",path,dent->d_name);
		 printf("%s ",dent->d_name);
		}
printf("\n");
}
printf("=====================================\n");
closedir(dp);

return 0;



}
