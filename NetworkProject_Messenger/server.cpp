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

#include "list.h"

#define PROMPT() {printf("\n>");fflush(stdout);}
#define GETCMD "download"
#define QUITCMD "quit"

int room_size;
int portnum = 0;



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
        printf("waiting...\n");
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
        
        
        //	for test
//        if (write(new_sock, "connection ok", strlen("connection ok")) < 0) {
//            perror("write");
//            exit(1);
//        }
//        close(new_sock);
//        printf("after write\n");
        
        
        if(!strncmp(command, "@quit", 5)){			// quit
        }
        else if(!strncmp(command, "@exit", 5)){		// exit
            current = head;
            memset(buf, 0x00, sizeof(buf));
            while(current != NULL){
                strcat(buf, current->ID);
                strcat(buf, current->IP);
                strcat(buf, toArray(current->port));
                strcat(buf, toArray(current->room));
                strcat(buf, "\n");
                
                if (write(new_sock, buf, strlen(buf)) < 0) {
                    perror("write");
                    exit(1);
                }
            }
        }
        else if(!strncmp(command, "@connect", 8)){	// connect
            printf("in connection...\n");
            
            insertFirst(inet_ntoa(server.sin_addr), ntohs(server.sin_port), &command[10], 0);
            
            if (write(new_sock, "connection ok", strlen("connection ok")) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@getlist", 8)){		// user & room list
            getList(buf);
            if (write(new_sock, buf, strlen(buf)) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@invite", 6)){	// exit
            // invite function
        }
        else if(!strncmp(command, "@mkroom", 6)){	// mkroom
            room_size++;
        }
        else{										// error
            
        }
    }
} /* main - hw2.c */
