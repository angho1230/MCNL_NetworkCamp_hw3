#include <stdio.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include "file.h"
#include "socket.h"
#include "shell.h"

void * clnt_handle(void * arg);

int main(int argc, char * argv[]){
    if(argc != 3){
        printf("Usage %s <sever ip> <port>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in serv_adr;
    int sock = tcp_client_create(argv[2], argv[1], &serv_adr);
    printf("%d", serv_adr.sin_port);
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error!");
    }
    comm_st cst;
    while(1){
        char * buf;
        size_t bufsize = 1024;
        res_st rst;
        getline(&buf, &bufsize, stdin);
        if(strlen(buf) == 0){
            continue;
        }
        fflush(stdin);
        char * args = strtok(buf, " ");
        if(args == 0x0){
            if(strcmp(buf, "ls") == 0){
                cst.c = LS;
                cst.arg[0] = '\0';
                write(sock, &cst, sizeof(cst));
                read_full(sock, &rst, sizeof(rst));
                file* files = (file *)malloc(rst.size);
                read_full(sock, files, rst.size);
                print_files(files, rst.size/sizeof(file));  
            }
            else if(strcmp(buf, "exit") == 0){
                cst.c = EXIT;
                cst.arg[0] = '\0';
                write(sock, &cst, sizeof(cst));
            }else if(strcmp(buf, "cd") == 0 ||
                        strcmp(buf, "up") == 0 ||
                        strcmp(buf, "dl") == 0){
                printf("you need argument for %s\n", buf);
            }
            else{
                printf("no command %s\n", buf);
            }
            free(buf);
            continue;
        }
        if(strcmp(buf, "up") == 0){
            cst.c = UP;
            strcpy(cst.arg, args); 
            write(sock, &cst, sizeof(cst));
            file finfo;
            get_file(".", &finfo, args);
            write(sock, &finfo, sizeof(finfo));
            write_from_file(sock, ".", finfo);
        }
        else if(strcmp(buf, "dl") == 0){
            cst.c = DL;
            write(sock, &cst, sizeof(cst));
            file finfo;
            read_full(sock, &finfo, sizeof(finfo));
            read_to_file(sock, ".", finfo);
        }
        else if(strcmp(buf, "exit") == 0){
            cst.c = EXIT;
            cst.arg[0] = '\0';
            write(sock, &cst, sizeof(cst));
        }else if(strcmp(buf, "cd") == 0){
            cst.c = CD;
            strcpy(cst.arg, args);
            write(sock, &cst, sizeof(cst));
        }
        else{
            printf("no command %s with argument\n", buf);
        }
        free(buf);
    }
}