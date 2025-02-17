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
            write(fileno(stdout), buf, n);
        }
        if(FD_ISSET(fileno(fp), &rset)){
            if( (n = read(fileno(fp), buf, SIZE)) == 0){
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            write(sockfd, buf, n);
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
