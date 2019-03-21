#include "mymsg.h"

int main(void){
	struct my_data p_data[10];
	int datanum=0;
	int running =1;
	int msqid; // pid를 담아옴
	//char buffer[BUFSIZ];

	//0 ftok(*) 로 key 생성

	//1.msgget 으로 pid 값을 받는다. key:(key_t)1234, flag 0666|IPC_CREAT
	msqid=msgget((key_t)1234,0666|IPC_CREAT);

	if(msqid==-1)//msgget이 오류시에
	{
		fprintf(stderr,"msgget error. %d\n",errno);
		exit(EXIT_FAILURE);
	}


 while(running&&(datanum<10))
 {
	printf("NAME:");
	fgets(p_data[datanum].name,sizeof(p_data[datanum].name),stdin);//fgets(주소값, size,파일형);
	printf("HEIGHT:");
	scanf("%d",&p_data[datanum].height);
	printf("WEIGHT:");
	scanf("%d",&p_data[datanum].weight);// 입력버퍼에 남아있음
	
	while(getchar()!='\n');// 입력 버퍼를 비우도록 함 
	
	p_data[datanum].msg_type=1;

	if (strncmp(p_data[datanum].name,"end",3)==0)//name 에 end가 입력 되었을때 
		{
			p_data[datanum].msg_type=2; //type에 2를 넣고
			running=0;//반복문을 빠져나오도록한다.
		}
 //2. msgsed
	if(	msgsnd(msqid,&p_data[datanum],sizeof(p_data[datanum])-sizeof(long),0)==-1)//메세지 전달이 안되었을경우
		{
		fprintf(stderr,"msgsed error. %d\n",errno);
		exit(EXIT_FAILURE);
		}		
	datanum++;
 }
exit(EXIT_SUCCESS);
} 
