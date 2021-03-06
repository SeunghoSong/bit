#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//네트워크 관련 헤더파일
#include <arpa/inet.h>
#include <sys/socket.h>


#define MSG_SIZE 100

int error_handling (char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

int main(int argc,char* argv[]){
    //소켓도 파일 처럼 선언
    int sock; //socket =fd
    struct sockaddr_in server_addr,from_addr; //데이터 송수신할때 주소가 필요
    char message[MSG_SIZE];
    int str_len;

    int i;

   
    socklen_t adr_sz;


    if(argc!=3)
    {   
        // ./helloClinet [serverIp_addr] [port_Num]
        printf("Usage : %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }

   
    //step 1 socket을 생성한다. file의  open() 과 동일
    //PF_INET =ip_v4 ,SOCK_STREAM=TCP
    sock =socket(PF_INET,SOCK_DGRAM,0);
    if(sock==-1)
    {
        error_handling("socket() error");
    }
    //접속할 서버의 IP 주소, 포트 번호, 프로토콜을 정의
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET; //ip v4
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);//문자형으로 작성된 ip 주소를 숫자로 변형
    server_addr.sin_port=htons(atoi(argv[2]));//문자열로 된 포트번호를 숫자로 변경후 htons 함수 short 형으로 변형

    while(1)
    {
        fputs("Insert massage(q to quit):",stdout);
        fgets(message,sizeof(message),stdin);

        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;
       
        sendto(sock,message,strlen(message),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
         adr_sz=sizeof(from_addr);
        str_len=recvfrom(sock,message,MSG_SIZE,0,(struct sockaddr*)&from_addr,&adr_sz);

        message[str_len]=0;
        printf("MEssage form server :%s", message);
    }
    close(sock);

}