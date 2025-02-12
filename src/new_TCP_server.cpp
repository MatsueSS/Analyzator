#include "new_TCP_server.h"
#include "Reg_or_Auth.h"
#include "Registration.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

Client::Client(int sockfd, const sockaddr_storage& addr, socklen_t len) 
    : sockfd(sockfd), clilen(len), id(-1)
{
    memcpy(&cliaddr, &addr, sizeof(sockaddr_storage));
}

Client::Client(const Client& obj)
{
    memcpy(&cliaddr, &obj.cliaddr, sizeof(sockaddr_storage));
}

Client& Client::operator=(const Client& obj)
{
    if(this == &obj)
        return *this;

    memcpy(&cliaddr, &obj.cliaddr, sizeof(sockaddr_storage));
    sockfd = obj.sockfd;
    clilen = obj.clilen;
    return *this;
}

Client::Client(int sockfd) : sockfd(sockfd) {}

bool Client::operator==(const Client& obj)
{
    return sockfd == obj.sockfd;
}

new_TCP_server::new_TCP_server()
{
    handle_clients.reserve(MAX_CLIENTS_NOW);
    map_handle[greetings] = std::make_unique<Reg_or_Auth>();
    map_handle[registration] = std::make_unique<Registration>();
}

void new_TCP_server::socket()
{
    server_socket = ::socket(AF_INET, SOCK_STREAM, 0);
}

void new_TCP_server::bind()
{
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    ::bind(server_socket, (sockaddr *)&servaddr, sizeof(servaddr));
}

void new_TCP_server::listen()
{
    ::listen(server_socket, BACKLOG);
}

void new_TCP_server::start()
{
    for(int i = 0; i < std::thread::hardware_concurrency()-2;i++)
        workers.emplace_back(&new_TCP_server::workThread, this);

    FD_ZERO(&master_fd);
    maxfd = server_socket;
    FD_SET(server_socket, &master_fd);
    while(true){
        fd_set readset = master_fd;
        if(select(maxfd+1, &readset, nullptr, nullptr, nullptr) > 0){
            if(FD_ISSET(server_socket, &readset)){
                accept_clients();
                continue;
            }
            for(int i = 0; i <= maxfd; i++){
                if(FD_ISSET(i, &readset)){
                    clients.push(handle_clients.find(i)->first);
                }
            }
        }
    }
}

void new_TCP_server::accept_clients()
{
    sockaddr_storage cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int sockfd = accept(server_socket, (sockaddr *)&cliaddr, &clilen);
    FD_SET(sockfd, &master_fd);
    maxfd = maxfd > sockfd ? maxfd : sockfd;
    handle_clients.insert(std::make_pair(Client(sockfd, cliaddr, clilen), greetings));
    std::string str = "Hello, enter needed action: registration or authentification\n";
    write(sockfd, str.c_str(), str.size());
}

void new_TCP_server::workThread()
{
    while(true)
    {
        Client client(-1, sockaddr_storage(), sizeof(sockaddr_storage));

        {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, [this]{ return !clients.empty(); });
            client = clients.front();
            clients.pop();
        }

        // switch(client.epoch_handle){
        // case 0:
        //     int result = handle_clients.find(client)->second->handle(client.sockfd);
        //     if(result == CLIENT_DISCONNECTED){
        //         FD_CLR(client.sockfd, &master_fd);
        //         handle_clients.erase(client);
        //         return;
        //     }
        //     else if(result == NOONE){
        //         write
        //     }

        // case 1:
        // }

    }
}