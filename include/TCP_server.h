#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <sys/select.h>
#include <queue>

#define PORT 8341
#define BACKLOG 10 //MAX clients waiting accepts in listening

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
    std::queue<int> clients;
    int server_socket;

    void accept_clients();
    void workThread();
    void handle(int);
};

#endif //_TCP_SERVER_H_