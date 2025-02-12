#ifndef _NEW_TCP_SERVER_H_
#define _NEW_TCP_SERVER_H_

#include "Handle.h"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <sys/select.h>
#include <queue>
#include <sys/socket.h>
#include <unordered_map>

#define PORT 8341
#define BACKLOG 10 //MAX clients waiting accepts in listening
#define MAXLINE 1024
#define MAX_CLIENTS_NOW 100

//returns in successful IPv4 or IPv6 addr, in failure return ""
std::string sock_ntop(sockaddr *addr);

struct Client{
    int sockfd, id, bad_auth_tries;
    sockaddr_storage cliaddr;
    socklen_t clilen;

    Client(int, const sockaddr_storage&, socklen_t);
    Client(int);
    Client& operator=(const Client&);
    Client(const Client&);

    int get_socket() const;

    bool operator==(const Client&);
};

namespace std
{
    template<>
    struct hash<Client>
    {
        std::size_t operator()(const Client& c) const noexcept {
            return std::hash<int>()(c.sockfd);
        }
    };
}

class new_TCP_server{
public:
    new_TCP_server();

    void socket();
    void bind();
    void listen();

    void start();

private:
    std::unordered_map<Client, Action> handle_clients;
    std::unordered_map<Action, std::unique_ptr<Handle>> map_handle;

    fd_set master_fd;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::queue<Client> clients;
    int server_socket;
    int maxfd;

    void accept_clients();
    void workThread();
    void handle(const Client&);
};



#endif //_NEW_TCP_SERVER_H_