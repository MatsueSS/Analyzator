#include "new_TCP_server.h"
#include "Registration.h"
#include "PostgresDB.h"
#include "Authentification.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

std::string sock_ntop(sockaddr *addr)
{
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

Client::Client(int sockfd, const sockaddr_storage& addr, socklen_t len) 
    : sockfd(sockfd), clilen(len), id(-1), bad_auth_tries(0)
{
    memcpy(&cliaddr, &addr, sizeof(sockaddr_storage));
}

Client::Client(const Client& obj) : sockfd(obj.sockfd), clilen(obj.clilen),
    id(obj.id), bad_auth_tries(obj.bad_auth_tries)
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
    id = obj.id;
    bad_auth_tries = obj.bad_auth_tries;
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
    map_handle[authentification] = std::make_unique<Authentification>();
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
                    {
                        std::lock_guard<std::mutex> gl(mtx);
                        clients.push(handle_clients.find(i)->first);
                    }
                    cv.notify_one();
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
    if(sockfd < 0){
        //error
        return;
    }
    FD_SET(sockfd, &master_fd);
    maxfd = maxfd > sockfd ? maxfd : sockfd;
    handle_clients.insert(std::make_pair(Client(sockfd, cliaddr, clilen), greetings));
    write_str("Hello, enter needed action: registration or authentification\n", sockfd);
}

void new_TCP_server::close_connect(const Client& obj)
{
    close(obj.sockfd);
    handle_clients.erase(obj);
    FD_CLR(obj.sockfd, &master_fd);
}

void new_TCP_server::write_str(const std::string& str, int sockfd) const
{
    write(sockfd, str.c_str(), str.size());
}

void new_TCP_server::workThread()
{
    while(true)
    {
        std::unique_ptr<PostgresDB> conn = std::make_unique<PostgresDB>("dbname=loganalyzer user=matsuess password=731177889232 host=localhost port=5432");
        conn->connect();
        Client client(-1, sockaddr_storage(), sizeof(sockaddr_storage));

        {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, [this]{ return !clients.empty(); });
            client = clients.front();
            clients.pop();
        }

        int result = map_handle.find(handle_clients.find(client)->second)->second->handle(client.sockfd, conn);
        
        switch(handle_clients.at(client)){
        case greetings:
            if(result == NOONE){
                write_str("You entered false action. Try again\n", client.sockfd);
                break;
            }
            handle_clients.find(client)->second = Action(result);
            break;
        case registration:
            if(result == disconnect){
                close_connect(client);
            }
            else if(result == existing){
                write_str("You entered existing data. Please, enter a new data\n", client.sockfd);
            }
            else{
                write_str("Your registration was success. You can use this app\n", client.sockfd);
                client.id = result;
                handle_clients.at(client) = command_checker;
            }
            break;
        case authentification:

        }
    }
}