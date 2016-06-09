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

#define PROMPT() {printf("\n>");fflush(stdout);}
#define GETCMD "download"
#define QUITCMD "quit"

void connectToClient(char* cip, char* cport, char *buf);



int room_size;
int portnum;



//
//
//
int main(int argc, char *argv[]){
    
    struct sockaddr_in server;
    int new_sock, server_sock = -1;
    int addrlen;
    char command[128], buf[128];
    
    if (argc != 2) {
        (void) fprintf(stderr,"usage: %s port# \n",argv[0]);
        exit(1);
    }
    
    portnum = atoi(argv[1]);
    
    // Create a Server Socket
    if((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("socket");
        exit(1);
    }
    
    memset((void*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons((u_short)atoi(argv[1]));
    
    if(bind(server_sock, (struct sockaddr *)&server, sizeof server) < 0) {
        perror("bind");
        exit(1);
    }
    
    if(listen(server_sock, SOMAXCONN) < 0) {
        perror("listen");
        exit(1);
    }
    
    
    
    //
    //
    //
    while(1){
        // Input from the keyboard
        memset(command, 0x00, sizeof command);
        
        addrlen = sizeof server;
        new_sock = accept(server_sock,
                          (struct sockaddr *)&server,
                          (socklen_t *)&addrlen);
        
        if (new_sock < 0) {
            perror("accept");
            exit(1);
        }
        
        printf("connection : Host IP %s, Port %d, socket %d\n",
               inet_ntoa(server.sin_addr), ntohs(server.sin_port), new_sock);
        
        read(new_sock, command, sizeof command);	// read user command
        printf("%s", command);
        
        if(!strncmp(command, "@quit", 5)){			// quit
            memset(buf, 0x00, sizeof(buf));
            
            strcat(buf, "@delete ");
            strcat(buf, inet_ntoa(server.sin_addr));
            strcat(buf, " ");
            strcat(buf, toArray(ntohs(server.sin_port)));
            
            struct node* delNode = find(inet_ntoa(server.sin_addr), ntohs(server.sin_port));
            int roomNum = delNode->room;
            delNode->room = 0;
            
            current = head;
            while(current != NULL){
                if(current->room == roomNum)
                    connectToClient(current->IP, toArray(current->port), buf);
                current = current->next;
            }
        }
        else if(!strncmp(command, "@exit", 5)){		// exit
            memset(buf, 0x00, sizeof(buf));
            
            strcat(buf, "@delete ");
            strcat(buf, inet_ntoa(server.sin_addr));
            strcat(buf, " ");
            strcat(buf, toArray(ntohs(server.sin_port)));
            strcat(buf, "\n");
            
            int roomNum = find(inet_ntoa(server.sin_addr), ntohs(server.sin_port))->room;
            deletes(inet_ntoa(server.sin_addr), ntohs(server.sin_port));
            if(roomNum != 0){
                current = head;
                while(current != NULL){
                    if(current->room == roomNum)
                        connectToClient(current->IP, toArray(current->port), buf);
                    current = current->next;
                }
            }
        }
        else if(!strncmp(command, "@connect", 8)){	// connect
            insertFirst(inet_ntoa(server.sin_addr), ntohs(server.sin_port), &command[9], 0);
            printf("%s is connected\n", &command[9]);
            
            if (write(new_sock, "connection ok", strlen("connection ok")) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@getlist", 8)){		// user & room list
            memset(buf, 0x00, sizeof(buf));
            for(int i=0; i<room_size; i++){
                if(i == 0){
                    strcat(buf, "\n==========waiting room ");
                    strcat(buf, toArray(i));
                    strcat(buf, "==========\n");
                }
                else{
                    strcat(buf, "==========room# ");
                    strcat(buf, toArray(i));
                    strcat(buf, "==========\n");
                }
                
                current = head;
                while(current != NULL){
                    if(current->room == i){
                        strcat(buf, current->IP);
                        strcat(buf, " ");
                        strcat(buf, toArray(current->port));
                        strcat(buf, " ");
                        strcat(buf, current->ID);
                        strcat(buf, "\n");
                    }
                    current = current->next;
                }
            }
            
            if (write(new_sock, buf, strlen(buf)) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@invite", 7)){	// exit
            char* ip = strtok(&command[8], " ");
            char* port = strtok(NULL, " ");
            
            struct node* guest = find(ip, atoi(port));
            printf("\n\ninvite %d %d\n", find(inet_ntoa(server.sin_addr), ntohs(server.sin_port))->room, guest->room);
            guest->room = find(inet_ntoa(server.sin_addr), ntohs(server.sin_port))->room;
            
            memset(buf, 0x00, sizeof(buf));
            strcat(buf, "invite ");
            strcat(buf, inet_ntoa(server.sin_addr));
            strcat(buf, " ");
            strcat(buf, toArray(ntohs(server.sin_port)));
            
            if (write(new_sock, buf, strlen(buf)) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@mkroom", 7)){	// mkroom
            room_size++;
            memset(buf, 0x00, sizeof(buf));
            strcat(buf, "room ");
            strcat(buf, toArray(room_size));
            strcat(buf, " is created\n");
            
            if (write(new_sock, buf, strlen(buf)) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@join", 5)){		// join
            int roomNum = atoi(&command[6]);
            
            memset(buf, 0x00, sizeof(buf));
            strcat(buf, "@add ");
            strcat(buf, inet_ntoa(server.sin_addr));
            strcat(buf, " ");
            strcat(buf, toArray(ntohs(server.sin_port)));
            strcat(buf, "\n");
            
            char join[1024];
            memset(join, 0x00, sizeof(join));
            strcat(join, "@join\n");
            
            int i = 0;
            current = head;
            while(current != NULL){
                
                if(current->room == roomNum){
                    connectToClient(current->IP, toArray(current->port), buf);
                    strcat(join, current->IP);
                    strcat(join, " ");
                    strcat(join, toArray(current->port));
                    strcat(join, " ");
                    strcat(join, current->ID);
                    strcat(join, "\n");
                }
                current = current->next;
                
            }
            
            if (write(new_sock, join, strlen(join)) < 0) {
                perror("write");
                exit(1);
            }
            find(inet_ntoa(server.sin_addr), ntohs(server.sin_port))->room = atoi(&command[6]);
        }
    }
}// main



void connectToClient(char* cip, char* cport, char *buf){
    struct sockaddr_in tcpClient_addr;
    struct hostent *hostp;
    
    int peertcpsocket;
    if ((peertcpsocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    
    if ((hostp = gethostbyname(cip)) == 0)
        fprintf(stderr,"%s: unknown host\n",cip);
    
    memset((void *) &tcpClient_addr, 0, sizeof (tcpClient_addr));
    tcpClient_addr.sin_family = AF_INET;
    memcpy((void *) &tcpClient_addr.sin_addr, hostp->h_addr, hostp->h_length);
    tcpClient_addr.sin_port = htons((u_short)atoi(cport));
    
    if (connect(peertcpsocket, (struct sockaddr *)&tcpClient_addr, sizeof (tcpClient_addr)) < 0) {
        perror("connect error\n");
        exit(1);
    }
    
    if (write(peertcpsocket, buf, strlen(buf)) < 0) {
        perror("write");
        exit(1);
    }
}
