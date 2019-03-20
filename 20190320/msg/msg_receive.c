#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
/*
// msqid_ds 구조체 :msq_id
struct msqid_ds {
    struct ipc_perm msg_perm; 메세지큐의 허가사항 정보 
    struct msg *msg_first;  // first message on queue 큐의 처음 메세지
    struct msg *msg_last;   // last message in queue 큐의 마지막 메세지
    time_t msg_stime;       // last msgsnd time 마지막으로 msgsnd가 수행된 시간
    time_t msg_rtime;       // last msgrcv time 마지막으로 msgrcv가 수행된 시간
    time_t msg_ctime;       // last change time 마지막으로 change가 수행된 시간
    struct wait_queue *wwait;//and
    struct wait_queue *rwait;//커널의 대기큐를 가르키는 포인터
    ushort msg_cbytes;    	// 큐에 보내진 마지막 메세지의 시간 기록
    ushort msg_qnum;     	// 메세지 큐에 담긴 메세지 수
    ushort msg_qbytes;      // max number of bytes on queue 큐의 최대 바이트 수
    ushort msg_lspid;       // pid of last msgsnd 마지막으로 msgsnd를 수행한 pid
    ushort msg_lrpid;       // last receive pid 마지막으로 받은 pid
};
*/

//메세지큐 수신 프로그램

//(송신을 먼저 컴파일 후에 수신을 컴파일하는것을 추천한다.)

struct my_msg_st{//수신된 메세지큐를 받기위한 구조체
long int my_msg_type;// 메세지 타입
char some_text[BUFSIZ]; // 수신된 데이터
};

int main ()
{
	int running=1;
	int msgid;
	struct msqid_ds msqstat;//메세지큐에 남아있는값을 체크하기 위한 구조체
	struct my_msg_st some_data;// 수신을 받기위함

	msgid=msgget((key_t)1234,0666|IPC_CREAT);//키값을 통해 msgid 확인 해당 메세지큐에 접근하기 위함
	printf("수신메세지 큐의 값 :%d\n",msgid);//msgid 출력

	if(msgid==-1) //메세지큐가 생성에 실패 했을경우
	{
		fprintf(stderr,"msgget failed with error: %d\n",errno);
		exit(EXIT_FAILURE);
	}
 	//메세지큐에 접근하여 msgrcv()함수를 이용하여 데이터를 가져옴 
	while (running)
	{	//my_msg_st구조체에 데이터가 담김,해당 구조체에구조체 담을수 있는 만큼 만 받아오도록한다.
		if(msgrcv(msgid,(void*)&some_data,sizeof(some_data)-sizeof(long), 0, 0)==-1)
		{
			fprintf(stderr, "msgrcv failed with error: %d\n",errno);
			exit(EXIT_FAILURE);
		}

		if(strncmp(some_data.some_text,"end",3)==0)// 받아온 내용이 end일 경우
		{
			//msgctl함수가 IPC_STAT플래그를 이용해, msqid_ds 구조체의  msgid에 따른 메세지큐의 속성상태값을 받아온다.
			if(-1== (msgctl( msgid, IPC_STAT, &msqstat)))//msgctl이 오류 났을경우
   			{
      			perror( "msgctl() error");
      			exit(EXIT_FAILURE);
			}
			if(msqstat.msg_qnum==0)//메세지큐에 담긴 데이터가 없을 경우 (msg_qnum :메세지큐의 데이터수)
				running=0;
		
		}else
			printf("you worte: %s",some_data.some_text);

	}

	if(msgctl(msgid,IPC_RMID,0)==-1) //해당 메세지 큐를 비운다.(flag:IPC_RMID: 해당 메세지큐 삭제)
	{
		fprintf(stderr,"msgctl(IPC_RMID) failed \n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
