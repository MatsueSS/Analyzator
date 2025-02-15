#ifndef _NEW_TCP_SERVER_H_
#define _NEW_TCP_SERVER_H_

#include "Handle.h"
#include "Client.h"

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
#define MAX_CLIENTS_NOW 100

//returns in successful IPv4 or IPv6 addr, in failure return ""
std::string sock_ntop(sockaddr *addr);

class new_TCP_server{
public:
    new_TCP_server();

    void socket();
    void bind();
    void listen();

    void start();

private:
    std::unordered_map<Client, std::pair<Action,int>> handle_clients;
    std::unordered_map<Action, std::unique_ptr<Handle>> map_handle;

    fd_set master_fd; //All set for select
    std::mutex mtx; //Blocked access for Queue ready clients
    std::condition_variable cv; //Notify thread about new ready clients
    std::vector<std::thread> workers; //Container threads
    std::queue<Client> clients; //Queue ready clients
    int server_socket;
    int maxfd;

    void accept_clients();
    void workThread();
    
    void close_connect(const Client&);
    void write_str(const std::string&, int) const;
};

#endif //_NEW_TCP_SERVER_H_