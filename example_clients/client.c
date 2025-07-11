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

// #include <stdbool.h>
// #include <openssl/evp.h>
// #include <openssl/rand.h>

#define SIZE 1024

// int generate_key_from_password(const char *password, unsigned char *salt, int salt_len,
//                                unsigned char *key, int key_len) {
//     int iterations = 10000; // Количество итераций, чем больше — тем безопаснее, но дольше
//     return PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), salt, salt_len, iterations, key_len, key);
// }

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
    // bool encrypt = false, crypt = false;

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
            // char* passw;

            // if(encrypt){
            //     char* name = (char *)malloc(SIZE/2*sizeof(char));
            //     bool flag_space = false, flag_enter = false;

            //     int j = 0, k = 0;
            //     for(int i = 0; i < n; i++){
            //         if(buf[i] == ' '){
            //             name[j] = '\0';
            //             flag_space = true;
            //             continue;
            //         }
            //         if(buf[i] == '\n'){
            //             passw[k] = '\0';
            //             flag_enter = true;
            //             break;
            //         }
            //         if(flag_space == false){
            //             name[j++] = buf[i];
            //         }
            //         else if(flag_enter == false){
            //             passw[k++] = buf[i];
            //         }
            //     }
            //     if(flag_space == false)
            //         name[j] = '\0';
            //     if(flag_enter == false)
            //         passw[k] = '\0';

                

            // }

            // if(crypt){
            //     unsigned char salt[16];
            //     if(1 != RAND_bytes(salt, sizeof(salt))){
            //         fprintf(stderr, "Error generating salt\n");
            //         return 1;
            //     }

            //     unsigned char key[32];
            //     if(!generate_key_from_password(passw, salt, sizeof(salt), key, sizeof(key))) {
            //         fprintf(stderr, "Error generating key\n");
            //         return 1;
            //     }
            // }

            // if(strncmp(buf, "add", 3) == 0){
            //     encrypt = true;
            // }
            // else if(strncmp(buf, "del", 3) == 0){
            //     crypt = true;
            // } else {
            //     encrypt = false;
            //     crypt = false;
            // }
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
