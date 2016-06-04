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
    peertcpSockets = connectToServer(buf);
    
    FD_ZERO(&reads);
    FD_SET(fileno(stdin), &reads);
    FD_SET(tcpServ_sock, &reads);
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
                peertcpSocket = connectToServer(command);
            }else{
                //connectToClient()
            }
//            else if(!strcmp(touppers(strtok(command, " ")),"DOWNLOAD")){ // input download
//                if ((peertcpSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//                    perror("socket");
//                    exit(1);
//                }// create socket description
//                
//                hostp = strtok(NULL, " "); // ip
//                sin_port = strtok(NULL, " "); // port number
//                file_name = strtok(NULL, " "); // file name
//                file_name[strlen(file_name)-1] = '\0';// delete \n
//                
//                if ((hostps = gethostbyname(hostp)) == 0) {
//                    fprintf(stderr,"%s: unknown host\n",hostp);
//                    continue;
//                } // host ip
//                
//                memset((void *) &tcpClient_addr, 0, sizeof (tcpClient_addr));
//                tcpClient_addr.sin_family = AF_INET;
//                memcpy((void *) &tcpClient_addr.sin_addr, hostps->h_addr, hostps->h_length);
//                
//                tcpClient_addr.sin_port = htons((u_short)atoi(sin_port));
//                if (connect(peertcpSocket, (struct sockaddr *)&tcpClient_addr, sizeof (tcpClient_addr)) < 0) {
//                    perror("connect error\n");
//                    exit(1);
//                } // connect client to server
//                FD_SET(peertcpSocket, &reads);
//                // tcp connection
//                
//                
//                strcat(sendhttp, "GET ");
//                strcat(sendhttp, file_name);
//                strcat(sendhttp, " HTTP/1.0\n");
//                strcat(sendhttp, "HOST: ");
//                strcat(sendhttp, hostp);
//                strcat(sendhttp, "\nUser-agent: Mozila/5.0\n");
//                strcat(sendhttp, "connection: close\n");
//                
//                //sleep(1000);
//                write(peertcpSocket, sendhttp, strlen(sendhttp));
//                // create&send HTTPmsg
//                
//                
//            }else if(!strcmp(touppers(strtok(command, " ")),"QUIT\n")){ // input quit -> quit process
//                exit(1);
//            }
            
            fflush(stdin);
        }
        else if(FD_ISSET(tcpServ_sock, &temps))
        {
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
        } // make connection
        else if(FD_ISSET(peertcpSocket, &temps))
        {
            char buf[1024];
            char file_name[1024];
            FILE *file = 0;
            int bytesread = 0;
            char sendhttp[1024];
            char send_file[1024];
            char file_length[1024];
            
            memset(buf, NULL, sizeof(buf));
            memset(file_name, NULL, sizeof(file_name));
            memset(sendhttp, NULL, sizeof(sendhttp));
            memset(send_file, NULL, sizeof(send_file));
            memset(file_length, NULL, sizeof(file_length));
            
            
            bytesread = read(peertcpSocket, buf, sizeof(buf)-1);
            if(bytesread <= 0){
                FD_CLR(peertcpSocket , &reads);
                //printf("Connetion closed : %d \n", peertcpSocket);
                close(peertcpSocket);
                continue;
            }
            printf("\n%s\n",buf); // print receive msg
            if(!strcmp(touppers(strtok(buf, " ")),"GET")){
                strcat(file_name, ".");
                strcat(file_name ,strtok(NULL, " "));
                
                
                file = fopen(file_name, "r"); // read mode
                if(!file){  // if file not exist
                    printf("Server Error: No such file %s \n", file_name);
                    fclose(file);
                    
                    strcat(sendhttp, "HTTP/1.0 404 NOT FOUND\n");
                    strcat(sendhttp, "Connection: close\n");
                    strcat(sendhttp, "Content-Length: 0\n");
                    strcat(sendhttp, "Content-Type: text/html\n");
                    write(peertcpSocket, sendhttp, strlen(sendhttp)); // send http msg
                }else{
                    char temp[1024];
                    while(!feof(file)){
                        fscanf(file, "%s", temp);
                        strcat(send_file, temp);
                        strcat(send_file, "\n");
                    }// read file
                    
                    snprintf(file_length, sizeof(file_length), "%d", strlen(send_file));
                    
                    
                    strcat(sendhttp, "HTTP/1.0 200 OK\n");
                    strcat(sendhttp, "Connection: close\n");
                    strcat(sendhttp, "Content-Length: ");
                    strcat(sendhttp, file_length);
                    strcat(sendhttp, "\nContent-Type: text/html\n");
                    
                    printf("finish %s %s\n\n", file_length, file_length);
                    
                    strcat(sendhttp, "\n");
                    strcat(sendhttp, send_file);
                    write(peertcpSocket, sendhttp, strlen(sendhttp));
                }
                
            }else if(!strcmp(touppers(strtok(buf, " ")),"HTTP/1.0")){
                close(peertcpSocket);
                FD_CLR(peertcpSocket, &reads);
                peertcpSocket = -1;
            } // get response, connection close
            
        }else if(FD_ISSET(peertcpSockets, &temps)){
            char buf[1024];
            read(peertcpSocket, buf, sizeof(buf)-1);
            printf("\n%s\n",buf); // print receive msg
            close(peertcpSocket);
            if(!strcmp(touppers(strtok(command, " ")),"DUPLICATE")){
                exit(1);
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



