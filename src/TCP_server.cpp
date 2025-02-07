#include "TCP_server.h"

#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

void TCP_server::socket()
{
    if((server_socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0){
        //error
        return;
    }
    const int on = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void TCP_server::bind()
{
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(::bind(server_socket, (sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        //error
    }
}

void TCP_server::listen()
{
    if(::listen(server_socket, BACKLOG) < -1){
        //error
    }
}

void TCP_server::start()
{
    for(int i = 0; i < std::thread::hardware_concurrency() - 2; i++)
        workers.emplace_back(&TCP_server::workThread, this);

    FD_ZERO(&master_fd);
    FD_SET(server_socket, &master_fd);
    while(true){
        fd_set read_set = master_fd;
        if(select(server_socket+1, &read_set, nullptr, nullptr, nullptr) > 0)
            if(FD_ISSET(server_socket, &read_set))
                accept_clients();
    }
}

void TCP_server::handle(int sockfd)
{
    char buf[MAXLINE];
    ssize_t n = read(sockfd, buf, MAXLINE);
    write(sockfd, buf, n);
}

void TCP_server::accept_clients()
{
    int sockfd = accept(server_socket, nullptr, nullptr);
    if(sockfd > 0){
        std::unique_lock<std::mutex> ul(mtx);
        clients.push(sockfd);
        ul.unlock();
        cv.notify_one();
    }
}

void TCP_server::workThread()
{
    while(true){
        int sockfd = -1;

        {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, [this]{ return !clients.empty(); });
            sockfd = clients.front();
            clients.pop();
        }

        if(sockfd != -1){
            handle(sockfd);
            close(sockfd);
        }
    }
}