#include "mymsg.h"



int main(void){
	struct my_data p_data[10];
	struct msqid_ds msqstat;//메세지큐에 남은 메세지개수를 확인
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


 while(running){
		if((msgrcv(msqid,&p_data[datanum],sizeof(p_data[datanum])-sizeof(long),0,0))==-1)//msgget이 오류시에
		{
			fprintf(stderr,"msqid error. %d\n",errno);
			exit(EXIT_FAILURE);
		}
		if(p_data[datanum].msg_type==2)
			running==0;
		else{
			printf("Recevie data[%d]\n",datanum);
			printf("NAME: %s \n",p_data[datanum].name);
			printf("HEIGHT: %d \n",p_data[datanum].height);
			printf("WEIGHT: %d \n",p_data[datanum].weight);
			}

		if(msgctl(msqid,IPC_STAT,&msqstat)==-1){
			fprintf(stderr,"msctl error. %d\n",errno);
			exit(EXIT_FAILURE);
		}

		printf("remain message count:%ld\n",msqstat.msg_qnum);
		if(msqstat.msg_qnum>0)
			running=1;
		else
			running=0;
	datanum++;
	}

	if(msgctl(msqid,IPC_RMID,NULL)==-1)
	{
		fprintf(stderr,"msctl error. %d\n",errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
