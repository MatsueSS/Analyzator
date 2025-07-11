#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>

#include <stdbool.h>
#include "cipher.h"
#include "decipher.h"

#define SIZE 1024

void str_cli(FILE* fp, int sockfd){
    // char sendline[SIZE], recvline[SIZE];
    // while(fgets(sendline, SIZE, fp) != NULL){
    //     write(sockfd, sendline, strlen(sendline));
    //     char recvline[SIZE];
    //     read(sockfd, recvline, SIZE);
    //     fputs(recvline, stdout);
    //     memset(recvline, 0, SIZE);
    // }
    int maxfdpl, stdineof = 0;
    fd_set rset;
    char buf[SIZE];
    int n;
    bool encrypt = false, crypt = false, precrypt = false;;
    char* data;
    char name[128];

    FD_ZERO(&rset);
    for(;;){
        if(stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdpl = sockfd > fileno(fp) ? sockfd + 1 : fileno(fp) + 1;
        select(maxfdpl, &rset, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &rset)){
            if( (n = read(sockfd, buf, SIZE)) == 0){
                if(stdineof == 1) return;
                else {
                    fprintf(stderr, "str_cli server terminated prematurely\n");
                    return;
                }
            }
            if(crypt){
                precrypt = false;
                crypt = false;
                buf[n-1] = '\0';
                printf("%s", buf);
                char* str = decipher(name, buf);
                write(fileno(stdout), str, strlen(str));
                write(fileno(stdout), "\n", 1);
                free(str);
            }
            else
                write(fileno(stdout), buf, n);
        }
        if(FD_ISSET(fileno(fp), &rset)){
            if( (n = read(fileno(fp), buf, SIZE)) == 0){
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            if(precrypt){
                memset(name, 0, sizeof(name));
                buf[n-1] = '\0';
                strncpy(name, buf, n);
                crypt = true;
                write(sockfd, buf, n);
                continue;
            }

            if(encrypt){
                char *new_name = (char *)malloc(128*sizeof(char));
                char *psw = (char *)malloc(128*sizeof(char));
                bool flag_space = false, flag_enter = false;

                memset(new_name, 0, 128);
                memset(psw, 0, 128);

                for(int i = 0, j = 0, k = 0; i < n; i++){
                    if(buf[i] == ' '){
                        flag_space = true;
                        continue;
                    }
                    if(buf[i] == '\n'){
                        flag_enter = true;
                        break;
                    }
                    if(flag_space == false){
                        new_name[j++] = buf[i];
                    }
                    else if(flag_enter == false){
                        psw[k++] = buf[i];
                    }
                }

                char* enc_psw = cipher(new_name, psw);
                free(psw);
                char* result = (char *)malloc(512*sizeof(char));
                memset(result, 0, 512);
                strncpy(result, new_name, strlen(new_name));
                free(new_name);
                strcat(result, " ");
                strncat(result, enc_psw, strlen(enc_psw));
                write(sockfd, result, strlen(result));
                free(result);
                free(enc_psw);
            }
            else
                write(sockfd, buf, n);

            if(strncmp(buf, "add", 3) == 0){
                encrypt = true;
            }
            else if(strncmp(buf, "get", 3) == 0){
                precrypt = true;
            }
            else{
                precrypt = false;
                encrypt = false;
            }
        }
    }
}

void sig_pipe(int signo){
    char buff[SIZE] = "Signal is good!";
    write(STDOUT_FILENO, buff, strlen(buff));
    return;
}

int main(int argc, char *argv[]){
    struct sockaddr_in addr;
    int client_fd;
    struct linger ling;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_port = htons(8341);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);

    signal(SIGPIPE, sig_pipe);

    connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));

    str_cli(stdin, client_fd);

    exit(0);
}
