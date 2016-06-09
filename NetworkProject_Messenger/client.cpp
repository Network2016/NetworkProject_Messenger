/*
 Network Team project
 20113313 이창현
 20113336 채한울
 201133 윤명식
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

static char* hostIP = NULL; // ip
static char* hostport = NULL; // port number
static char* myID = NULL;

int connectToClient(char* cip, char* cport, char *buf);
int connectToServer(char* buf);
void display();
char* touppers(char* str);

int peertcpSocket = -1;	// peer socket
int peertcpSockets = -1;	// peer socket

int main(int argc, char **argv) {
    // 매개변수 1:자기포트 2:서버IP 3:서버포트 4:자기ID
    int tcpServ_sock;
    
    struct sockaddr_in tcpServer_addr;
    struct sockaddr_in tcpClient_addr;
    struct sockaddr_in newTcp_addr;
    
    fd_set reads, temps;
    int fd_max;
    
    char command[1024];
    
    if(argc != 5){
        printf("Usage : %s <myport> <serverIP> <serverport> <myID>\n", argv[0]);
        exit(1);
    }
    
    
    display();
    
    if ((tcpServ_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }// creating socket descriptor for tcp
    
    
    memset((void *) &tcpServer_addr, 0, sizeof (tcpServer_addr));
    tcpServer_addr.sin_family = AF_INET;
    tcpServer_addr.sin_addr.s_addr = INADDR_ANY;
    tcpServer_addr.sin_port = htons((u_short)atoi(argv[1])); // portnum
    fd_max = 10;
    
    if (bind(tcpServ_sock, (struct sockaddr *)&tcpServer_addr, sizeof (tcpServer_addr)) < 0) {
        perror("bind error");
        exit(1);
    } // bind
    
    if (listen(tcpServ_sock, SOMAXCONN) < 0) {
        perror("listen");
        exit(1);
    } // wait connect
    
    // 여기까지 기본 소켓세팅
    hostIP = argv[2]; // ip
    hostport = argv[3]; // port numbere
    myID = argv[4];
    
    char buf[1024];
    strcat(buf, "@connect ");
    strcat(buf, myID);
    //peertcpSockets = connectToServer(buf);
    
    peertcpSockets = connectToServer(buf);
    FD_ZERO(&reads);
    FD_SET(fileno(stdin), &reads);
    FD_SET(tcpServ_sock, &reads);
    FD_SET(peertcpSockets, &reads);
    //FD_SET(peertcpSocket, &reads);
    // initialize the select mask variables and set the
    // mask with stdin and the tcp server socket
    while(1){
        int nfound;
        
        temps = reads;
        nfound = select(fd_max+1, &temps, 0, 0, NULL);
        // nfound?
        
        if(FD_ISSET(fileno(stdin), &temps)) {
            // Input from the keyboard
            char* hostp = NULL;
            char* sin_port = NULL;
            char* file_name = NULL;
            struct hostent *hostps;
            char sendhttp[1024];
            memset(sendhttp, NULL, sizeof(sendhttp));
            
            
            fgets(command, sizeof (command), stdin);
            
            if( command[0] == '@' ){
                // send to server
                if(command[1] == 'e'){ // when exit
                    head = NULL;
                }
                peertcpSockets = connectToServer(command);
                // exit했을때는 나에게 들어있는b 유저목록 다 지워야한다.
            }else{
                node* cnode = head;
                char sendmsg[1024];
                strcat(sendmsg, myID);
                strcat(sendmsg, ") ");
                strcat(sendmsg, command);
                
                while(cnode != NULL){
                    peertcpSocket = connectToClient(head->IP, toArray(head->port), sendmsg);
                    close(peertcpSocket);
                    cnode = cnode->next;
                } // 유저에게 메시지 전송
            }
            fflush(stdin);
        }else if(FD_ISSET(tcpServ_sock, &temps)){
            int addrlen;
            addrlen = sizeof(newTcp_addr);
            peertcpSocket = accept(tcpServ_sock, (struct sockaddr *)&newTcp_addr, (unsigned int*)&addrlen);
            //printf("connection from host %s, port %d, socket %d\n\n",
            //       inet_ntoa(newTcp_addr.sin_addr), ntohs(newTcp_addr.sin_port), peertcpSocket);
            if (peertcpSocket < 0) {
                perror("accept");
                exit(1);
            }
            FD_SET(peertcpSocket, &reads);
            // make connection
        }else if(FD_ISSET(peertcpSocket, &temps)){ // 서버또는 유저에게서 요청받은 내용
            char buf[1024];
            read(peertcpSockets, buf, sizeof(buf)-1);
            char* tbuf;
            strcpy(tbuf, buf);
            char* inif = strtok(tbuf, " ");
            
            if(!strcmp(touppers(inif),"@ADD")){
                char* tempIP = strtok(NULL, " ");
                char* tempPort = strtok(NULL, " ");
                char* tempID = strtok(NULL, " ");
                
                int tport = atoi(tempPort);
                insertFirst(tempIP, tport, tempID, 0);
                // close 소켓 서버에서?유저에서?
            }else if(!strcmp(touppers(inif),"@DELETE")){
                char* tempIP = strtok(NULL, " ");
                char* tempPort = strtok(NULL, " ");
                int tport = atoi(tempPort);
                deletes(tempIP, tport);
            }else{
                printf("%s\n", buf);
            }
            
        }
        else if(FD_ISSET(peertcpSockets, &temps)){ // 서버에서 응답받은 내용
            char buf[1024];
            read(peertcpSockets, buf, sizeof(buf)-1);
            char tbuf[1024];
            strcpy(tbuf, buf);
            
            char* inif = strtok(tbuf, "\n");
            
            if(!strcmp(touppers(inif),"JOIN")){
                char* temp;
                temp = strtok(buf, "\n");
                printf("%s\n", temp); // 입력받은 첫줄 출력
                
                while((temp = strtok(NULL, "\n")) != NULL){
                    char* tempIP = strtok(temp, " ");
                    char* tempPort = strtok(NULL, " ");
                    int tport = atoi(tempPort);
                    char* tempID = strtok(NULL, " ");
                    insertFirst(tempIP, tport, tempID, 0);
                }
            }else if(!strcmp(touppers(inif),"INVITE OK")){
                char* temp;
                temp = strtok(NULL, "\n");
                char* tempIP = strtok(temp, " ");
                char* tempPort = strtok(NULL, " ");
                char* tempID = strtok(NULL, " ");
                
                printf("invite %s\n", tempID);
                int tport = atoi(tempPort);
                insertFirst(tempIP, tport, tempID, 0);
                // invite에 성공했다면 그사람을 저장한다.
            }else{
                printf("%s\n",buf); // print receive msg
                close(peertcpSockets);
                FD_CLR(peertcpSockets, &reads);
            }
        }// receive
        
        printf("> ");
        fflush(stdout);
    }
}//main End


int connectToClient(char* cip, char* cport, char *buf){
    int peertcpsocket;
    // 매개변수 1:자기포트 2:서버IP 3:서버포트 4:자기ID
    if ((peertcpsocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }// create socket description
    
    int tcpServ_sock;
    
    struct sockaddr_in tcpServer_addr;
    struct sockaddr_in tcpClient_addr;
    struct sockaddr_in newTcp_addr;
    
    struct hostent *hostpss;
    char sendhttp[1024];
    memset(sendhttp, NULL, sizeof(sendhttp));
    
    if ((hostpss = gethostbyname(cip)) == 0) {
        fprintf(stderr,"%s: unknown host\n",cip);
    } // host ip
    
    memset((void *) &tcpClient_addr, 0, sizeof (tcpClient_addr));
    tcpClient_addr.sin_family = AF_INET;
    memcpy((void *) &tcpClient_addr.sin_addr, hostpss->h_addr, hostpss->h_length);
    
    tcpClient_addr.sin_port = htons((u_short)atoi(cport));
    if (connect(peertcpsocket, (struct sockaddr *)&tcpClient_addr, sizeof (tcpClient_addr)) < 0) {
        perror("connect error\n");
        exit(1);
    } // connect client to server
    
    // tcp connection
    strcpy(sendhttp, buf);
    // give my ID to server
    
    //sleep(1000);
    write(peertcpsocket, sendhttp, strlen(sendhttp));
    // create&send HTTPmsg
    return peertcpsocket;
}

int connectToServer(char* buf){
    int peertcpsocket=-1;
    // 매개변수 1:자기포트 2:서버IP 3:서버포트 4:자기ID
    if ((peertcpsocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }// create socket description
    
    int tcpServ_sock;
    
    struct sockaddr_in tcpServer_addr;
    struct sockaddr_in tcpClient_addr;
    struct sockaddr_in newTcp_addr;
    
    struct hostent *hostpss;
    char sendhttp[1024];
    memset(sendhttp, NULL, sizeof(sendhttp));
    
    if ((hostpss = gethostbyname(hostIP)) == 0) {
        fprintf(stderr,"%s: unknown host\n",hostIP);
    } // host ip
    
    memset((void *) &tcpClient_addr, 0, sizeof (tcpClient_addr));
    tcpClient_addr.sin_family = AF_INET;
    memcpy((void *) &tcpClient_addr.sin_addr, hostpss->h_addr, hostpss->h_length);
    
    tcpClient_addr.sin_port = htons((u_short)atoi(hostport));
    if (connect(peertcpsocket, (struct sockaddr *)&tcpClient_addr, sizeof (tcpClient_addr)) < 0) {
        perror("connect error\n");
        exit(1);
    } // connect client to server
    
    // tcp connection
    strcpy(sendhttp, buf);
    // give my ID to server
    
    //sleep(1000);
    write(peertcpsocket, sendhttp, strlen(sendhttp));
    // create&send HTTPmsg
    return peertcpsocket;
}

char* touppers(char* str){
    int i;
    for(i=0;str[i] !='\0';i++)
        str[i]=toupper(str[i]);
    return str;
}
void display() {
    fflush(stdin);
    printf("Student ID : 20113313 \n");
    printf("Name : changhyeonLee  \n\n");
    printf("> ");
    fflush(stdout);
}


