/*
소켓 프로그래밍

클라이언트 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//네트워크 관련 헤더파일
#include <arpa/inet.h>
#include <sys/socket.h>
#include "mysocket.h"

#define MSG_SIZE 100

//#define DEBUG

error_handling (char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

int main(int argc,char* argv[]){
    //소켓도 파일 처럼 선언
    int sock; //socket =fd
    struct sockaddr_in server_addr;
    char message[MSG_SIZE];
    int str_len;

    int i;

   

    struct calcul cal;


    if(argc!=3)
    {   
        // ./helloClinet [serverIp_addr] [port_Num]
        printf("Usage : %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }

   
    //step 1 socket을 생성한다. file의  open() 과 동일
    //PF_INET =ip_v4 ,SOCK_STREAM=TCP
    sock =socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        error_handling("socket() error");
    }
    //접속할 서버의 IP 주소, 포트 번호, 프로토콜을 정의
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET; //ip v4
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);//문자형으로 작성된 ip 주소를 숫자로 변형
    server_addr.sin_port=htons(atoi(argv[2]));//문자열로 된 포트번호를 숫자로 변경후 htons 함수 short 형으로 변형

    //step 3. 접속 요청
    //              명시적 형변환. 구조체 sockaddr 과 sockaddr_in 구조는 비슷하나 다른점이 있다.
    if(connect(sock,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
    {
        error_handling("connect() error");
    }

     printf("입력 할 수를 입력하시오 \n");
    scanf("%d",&cal.opnd_cnt);
    
    

    for(i=0;cal.opnd_cnt<i;i++)
    {
        printf("Operand %d\n",i+1);
        scanf("%d",&cal.opnd[i]);
    }

    printf("연산자를 입력하시오 \n");
    scanf("%s",&cal.oper);

    #ifdef DEBUG
    printf("count : %d \n",cal.opnd_cnt);
    for(i=0;i<cal.opnd_cnt;i++){
    printf("num %d : %d \n",i+1,cal.opnd[i]);
    }
    printf("op : %s \n",cal.oper);
    #endif
    //step 4. 데이터 수신
    write(sock,&cal,sizeof(cal));
    printf("Message from server: %s",message);

str_len=read(sock,&cal,sizeof(cal));  //값이 들어올때까지 blocking
 if(str_len==-1)
    {
        error_handling("read() error");
    }

    //step 5. 소켓 종료
    close(sock);
    return 0;
}

