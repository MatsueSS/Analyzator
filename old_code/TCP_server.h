#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <sys/select.h>
#include <queue>
#include <sys/socket.h>

#define PORT 8341
#define BACKLOG 10 //MAX clients waiting accepts in listening
#define MAXLINE 1024

//In global network its values need to up
#define KEEPIDLE 60
#define KEEPINTVL 10
#define KEEPCNT 3

//returns in successful IPv4 or IPv6 addr, in failure return ""
std::string sock_ntop(sockaddr *addr);

struct Client{
    int sockfd;
    sockaddr_storage cliaddr;
    socklen_t clilen;

    Client(int, const sockaddr_storage&, socklen_t);
    Client& operator=(const Client&);
    Client(const Client&);
};

class TCP_server{
public:
    void socket();
    void bind();
    void listen();

    void start();

private:
    fd_set master_fd;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::queue<Client> clients;
    int server_socket;

    void accept_clients();
    void workThread();
    void handle(const Client&);
};

#endif //_TCP_SERVER_H_