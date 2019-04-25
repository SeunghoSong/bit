/*
소켓 프로그래밍

서버

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

void error_handling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

int carculate(int opnum, int opnds[], char oper){
    int result=opnds[0];
    int i;
    
    switch (oper)
    {
        case: '+' : for(i=1;i<opnum;i++)
                    result+=opnds[i];
                    break;
        case: '-' : for(i=1;i<opnum;i++)
                    result-=opnds[i];
                    break;
        case: '*' : for(i=1;i<opnum;i++)
                    result*=opnds[i];
                    break;
       
        default: 
        printf("잘못된 매개변수 입니다.\n")
        return 0;
    }
    return result;
}

int main(int argc,char* argv[]){
    //소켓도 파일 처럼 선언
    int serv_sock; //socket =fd
    int clnt_sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[]="Hello World!";
    
    struct calcul cal;

    int result;

    int str_len;
    // 서버는 자기 자신의 IP 주소는 알고 있음 어떤 포트로 들어오는지만 알면됨
    if(argc!=2)
    {   
        // ./helloServer [port_Num]
        printf("Usage : %s <PORT>\n",argv[0]);
        exit(1);
    }

    //step 1 socket을 생성한다. file의  open() 과 동일
    //PF_INET =ip_v4 ,SOCK_STREAM=TCP
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1)
    {
        error_handling("socket() error");
    }
    //접속할 서버의 IP 주소, 포트 번호, 프로토콜을 정의
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET; //ip v4
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//long 형의 ip 주소를 숫자로 변형
    server_addr.sin_port=htons(atoi(argv[1]));//문자열로 된 포트번호를 숫자로 변경후 htons 함수 short 형으로 변형
    
    //step 3. BIND  서버 접속 허용
    if(bind(serv_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
        error_handling("bind() error");
    
    //step 4. 접속 요청 버퍼 설정 
    //listen(소켓,버퍼 크기(동시에 접속할수 있는 수)) :여러개의 접속요청을 처리 하기위해 여러개의 버퍼가 필요
    if(listen(serv_sock,5)==-1)
         error_handling("bind() error");


    //step 5. 클라이언트 접속 처리
    //client: connect() --> server: accept()  서버소켓이 accept()를 통해 클라이언트 소켓을 생성
    clnt_addr_size =sizeof(clnt_addr);
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");

    //step 6. 데이터 수신
    str_len=read(clnt_sock,&cal, sizeof(cal)-1);
    if(str_len==-1)
    {
        error_handling("read() error");
    }
    result=calculate(cal.opnd_cnt,cal.opnd,cal.oper);
    printf("%d \n",result);
    sleep(2);
    write(clnt_sock,&result,sizeof(result));
    //step 7. 소켓 종료 
    //close 는 ack 신호가 없다 그래서 time_out 시간내에는 close를 해도 일정시간 포트가 열려 있다. 
    //시간이 지나면 반납
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
