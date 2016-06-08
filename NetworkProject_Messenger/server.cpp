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



int main(int argc, char *argv[]){
    List* userList = createList();
    
    
    
    struct sockaddr_in server;
    int new_sock, server_sock = -1;
    int addrlen;
    char buf[102400], command[128], file[128], port[128], ip[128];
    struct hostent *hostp;
    
    
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
        }
        else if(!strncmp(command, "@exit", 5)){		// exit
        }
        else if(!strncmp(command, "@connect", 8)){	// connect
			printf("in connect\n");
            Data user;
            strcpy(user.IP, inet_ntoa(server.sin_addr));
            strcpy(user.ID, &command[10]);
            user.port = ntohs(server.sin_port);
            
            insert(userList, user);
            
            if (write(new_sock, "connection ok", strlen("connecting success!")) < 0) {
                perror("write");
                exit(1);
            }
        }
        else if(!strncmp(command, "@getlist", 8)){		// user & room list
            List* list_ptr = userList;
            //while(list_ptr != null)
                printf("%s %s %d", list_ptr->data.IP, list_ptr->data.IP, list_ptr->data.port);
            
            
//            printf("ID : %s, IP : %s, Port# : %d", user.id, user.ip, user.port); 			// need to fix
        }
        else if(!strncmp(command, "@invite", 6)){	// exit
            close(server_sock);
            return 0;
        }
        else if(!strncmp(command, "@mkroom", 6)){	// mkroom
            room_size++;
        }
        else{										// error
            
        }
        
        printf("pass\n");
    }
} /* main - hw2.c */
        
        
        
        
        
        
        
        
        
        
        
//        else {										// download
//            int i, j;
//            
//            for(i=9; command[i] != ' '; i++)	// 9 = initial ip location
//                ip[i-9] = command[i];
//            
//            for(j=0; command[++i] != ' '; j++)	// port
//                port[j] = command[i];
//            
//            for(j=0; command[++i] != '\n'; j++)	// file
//                file[j] = command[i];
//            
//            
//            if ((request_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
//                perror("socket");
//                exit(1);
//            }
//            
//            if ((hostp = gethostbyname(ip)) == 0) {
//                fprintf(stderr,"%s: unknown host\n", ip);
//                exit(1);
//            }
//            
//            
//            memset((void *) &request, 0, sizeof request);
//            request.sin_family = AF_INET;
//            memcpy((void *) &request.sin_addr, hostp->h_addr, hostp->h_length);
//            request.sin_port = htons((u_short)atoi(port));
//            
//            if (connect(request_sock, (struct sockaddr *)&request, sizeof (request)) < 0) {
//                (void) close(request_sock);
//                perror("connect");
//                exit(1);
//            }
//            
//            
//            FD_SET(request_sock, &mask);			// listen request socket
//            
//            
//            memset(buf, 0x00, sizeof buf);
//            strncpy(buf, "GET ", strlen("GET "));
//            strcat(buf, file);
//            strcat(buf, " HTTP/1.0\nHOST: ");
//            strcat(buf, ip);
//            strcat(buf, "\nUser-agent: HW2/1.0");
//            strcat(buf, "\nConnection: close\n");
//            
//            if (write(request_sock, buf, strlen(buf)) < 0) {
//                perror("write");
//                exit(1);
//            }
//        }// inner else

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
//        
//        
//        if(1) {
//            int i, j;
//            
//            // Input from the keyboard
//            memset(command, 0x00, sizeof command);
//            
//            addrlen = sizeof server;
//            new_sock = accept(server_sock,
//                              (struct sockaddr *)&server,
//                              (socklen_t *)&addrlen);
//            
//            if (new_sock < 0) {
//                perror("accept");
//                exit(1);
//            }
//            
//            printf("connection : Host IP %s, Port %d, socket %d\n",
//                   inet_ntoa(server.sin_addr), ntohs(server.sin_port), new_sock);
//            
//            read(new_sock, command, sizeof command);	// read user command
//            printf("%s", command);
//            
//            
//            
//            
//            if(!strncmp(command, "quit", 4)){			// quit
//                close(server_sock);
//                return 0;
//            }
//            else if(!strncmp(command, "exit", 4)){		// exit
//                close(server_sock);
//                return 0;
//            }
//            else if(!strncmp(command, "connect", 7)){	// connect
//                userInfo user = {inet_ntoa(server.sin_addr), &command[8], 1, 1};
//                user_list.push_back(user);
//                if (write(new_sock, "connecting success!", strlen("connecting success!")) < 0) {
//                    perror("write");
//                    exit(1);
//                }
//            }
//            else if(!strncmp(command, "list", 4)){		// user & room list
//                for(int i=0; i<user_list.size(); i++){
//                    userInfo user = user_list.front();
//                    printf("ID : %s, IP : %s, Port# : %d", user.id, user.ip, user.port); 			// need to fix
//                }
//            }
//            else if(!strncmp(command, "invite", 6)){	// exit
//                close(server_sock);
//                return 0;
//            }
//            else if(!strncmp(command, "mkroom", 6)){	// mkroom
//                room_size++;
//            }
//            else{										// error
//                
//            }
//            
//            
//            
//            if (write(new_sock, "connecting success!", strlen("connecting success!")) < 0) {
//                perror("write");
//                exit(1);
//            }
//            
//            
//            
//            
//            
//            
//            
//            
//            
//            
//            
//            else {										// download
//                int i, j;
//                
//                for(i=9; command[i] != ' '; i++)	// 9 = initial ip location
//                    ip[i-9] = command[i];
//                
//                for(j=0; command[++i] != ' '; j++)	// port
//                    port[j] = command[i];
//                
//                for(j=0; command[++i] != '\n'; j++)	// file
//                    file[j] = command[i];
//                
//                
//                if ((request_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
//                    perror("socket");
//                    exit(1);
//                }
//                
//                if ((hostp = gethostbyname(ip)) == 0) {
//                    fprintf(stderr,"%s: unknown host\n", ip);
//                    exit(1);
//                }
//                
//                
//                memset((void *) &request, 0, sizeof request);
//                request.sin_family = AF_INET;
//                memcpy((void *) &request.sin_addr, hostp->h_addr, hostp->h_length);
//                request.sin_port = htons((u_short)atoi(port));
//                
//                if (connect(request_sock, (struct sockaddr *)&request, sizeof (request)) < 0) {
//                    (void) close(request_sock);
//                    perror("connect");
//                    exit(1);
//                }
//                
//                
//                FD_SET(request_sock, &mask);			// listen request socket
//                
//                
//                memset(buf, 0x00, sizeof buf);
//                strncpy(buf, "GET ", strlen("GET "));
//                strcat(buf, file);
//                strcat(buf, " HTTP/1.0\nHOST: ");
//                strcat(buf, ip);
//                strcat(buf, "\nUser-agent: HW2/1.0");
//                strcat(buf, "\nConnection: close\n");
//                
//                if (write(request_sock, buf, strlen(buf)) < 0) {
//                    perror("write");
//                    exit(1);
//                }
//            }// inner else
//        }

        
        
//        else if(FD_ISSET(server_sock, &rmask)){
//            int i, j;
//            
//            addrlen = sizeof server;
//            new_sock = accept(server_sock,
//                              (struct sockaddr *)&server,
//                              (socklen_t *)&addrlen);
//            
//            if (new_sock < 0) {
//                perror("accept");
//                exit(1);
//            }
//            
//            printf("connection : Host IP %s, Port %d, socket %d\n",
//                   inet_ntoa(server.sin_addr), ntohs(server.sin_port), new_sock);
//            
//            read(new_sock, buf, sizeof buf);
//            printf("%s", buf);
//            
//            memset(file, 0x00, sizeof file);
//            for(i=0; buf[i++] != '/';);
//            for(j=0; buf[i] != ' '; i++)
//                file[j++] = buf[i];
//            
//            memset(buf, 0x00, sizeof buf);	// reuse
//            strncpy(buf, "HTTP/1.0 ", strlen("HTTP/1.0 "));
//            
//            strncpy(path, "./", strlen("./"));
//            strcat(path, file);
//            
//            FILE* inf = fopen(path, "r");
//            if (inf == NULL)
//                strcat(buf, "404 NOT FOUND");
//            else
//                strcat(buf, "200 OK");
//            
//            strcat(buf, "\nConnection: close");
//            if(inf != NULL){
//                memset(html, 0x00, sizeof html);
//                while(fscanf(inf, "%c", tmp) != EOF)
//                    strcat(html, tmp);
//            }
//            
//            sprintf(buf, "%s\nContent-Length: %ld", buf, strlen(html));
//            strcat(buf, "\nContent-Type: text/html\n");
//            
//            if (inf == NULL){
//                puts("p5");
//                if (write(new_sock, buf, strlen(buf)) < 0) {
//                    perror("write");
//                    exit(1);
//                }
//                printf("Server Error: No such file ./%s\n", file);
//            }
//            else{
//                strcat(buf, html);
//                if (write(new_sock, buf, strlen(buf)) < 0) {
//                    perror("write");
//                    exit(1);
//                }
//                printf("finish: %lu %lu\n", strlen(html), strlen(html));
//            }
//        }
//        
//        
//        else if(FD_ISSET(request_sock, &rmask)){
//            memset(buf, 0x00, sizeof buf);
//            read(request_sock, buf, sizeof buf);
//            printf("%s\n", buf);
//            
//            close(request_sock);
//            FD_ZERO(&mask);
//            FD_SET(fileno(stdin), &mask);
//            FD_SET(server_sock, &mask);
//        }
        
    
