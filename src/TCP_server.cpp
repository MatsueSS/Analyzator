#include "TCP_server.h"
#include "Log.h"

#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

std::string sock_ntop(sockaddr *addr){
    thread_local char str[INET6_ADDRSTRLEN];
    switch (addr->sa_family){
    case AF_INET:
        if(inet_ntop(AF_INET, &((sockaddr_in *)addr)->sin_addr, str, sizeof(str)) == nullptr)
            return "";
        return str;
    case AF_INET6:
        if(inet_ntop(AF_INET6, &((sockaddr_in6 *)addr)->sin6_addr, str, sizeof(str)) == nullptr)
            return "";
        return str; 
    default:
        return "";
    }
}

void TCP_server::socket()
{
    if((server_socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0){
        Log::make_note("101");
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
        Log::make_note("102");
        //error
    }
}

void TCP_server::listen()
{
    if(::listen(server_socket, BACKLOG) < -1){
        Log::make_note("103");
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
        if(select(server_socket+1, &read_set, nullptr, nullptr, nullptr) > 0){
            if(FD_ISSET(server_socket, &read_set))
                accept_clients();
        } else {
            Log::make_note("109");
        }
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
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int sockfd = accept(server_socket, (sockaddr *)&addr, &len);
    if(sockfd == -1){
        Log::make_note("1003");
        return;
    }
    Log::make_note("100001 " + sock_ntop((sockaddr *)&addr));
    std::unique_lock<std::mutex> ul(mtx);
    clients.push(sockfd);
    ul.unlock();
    cv.notify_one();
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