#include "new_TCP_server.h"
#include "Registration.h"
#include "PostgresDB.h"
#include "Authentification.h"
#include "Get.h"
#include "Add.h"
#include "Del.h"
#include "Edit.h"
#include "Log.h"

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

new_TCP_server::new_TCP_server()
{
    handle_clients.reserve(MAX_CLIENTS_NOW);
    map_handle[greetings] = std::make_unique<Reg_or_Auth>();
    map_handle[registration] = std::make_unique<Registration>();
    map_handle[authentification] = std::make_unique<Authentification>();
    map_handle[command_checker] = std::make_unique<Command_checker>();
    map_handle[get] = std::make_unique<Get>();
    map_handle[add] = std::make_unique<Add>();
    map_handle[del] = std::make_unique<Del>();
    map_handle[edit] = std::make_unique<Edit>();
}

void new_TCP_server::socket()
{
    if((server_socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0){
        Log::make_note("101");
        return;
    }
    const int on = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void new_TCP_server::bind()
{
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(::bind(server_socket, (sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        Log::make_note("102");
        return;
    }
}

void new_TCP_server::listen()
{
    if(::listen(server_socket, BACKLOG) < -1){
        Log::make_note("103");
        return;
    }
}

void new_TCP_server::start()
{
    for(int i = 0; i < std::thread::hardware_concurrency()-2;i++){
        workers.emplace_back(&new_TCP_server::workThread, this);
    }

    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 100;

    FD_ZERO(&master_fd);
    maxfd = server_socket;
    FD_SET(server_socket, &master_fd);
    while(true){
        fd_set readset = master_fd;
        ssize_t n;
        if((n = select(maxfd+1, &readset, nullptr, nullptr, &time)) > 0){
            if(FD_ISSET(server_socket, &readset)){
                accept_clients();
                if(--n <= 0)
                    continue;
            }
            for(int i = 0; i <= maxfd; i++){
                if(FD_ISSET(i, &readset)){
                    FD_CLR(i, &master_fd);
                    std::unique_lock<std::mutex> ul(mtx);
                    clients.push(handle_clients.find(i)->first);
                    ul.unlock();
                    cv.notify_one();
                    if(--n <= 0)
                        break;
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
        if(errno == ENFILE || errno == EMFILE)
            Log::make_note("107");
        else if(errno == ENOMEM || errno == ENOBUFS)
            Log::make_note("106");
        else if(errno == EPROTO || errno == EPERM)
            Log::make_note("1002");
        return;
    }
    Log::make_note("100001 " + sock_ntop((sockaddr*)&cliaddr));
    FD_SET(sockfd, &master_fd);
    maxfd = maxfd > sockfd ? maxfd : sockfd;
    handle_clients.insert(std::make_pair(Client(sockfd, cliaddr, clilen), greetings));
    write_str("Hello, enter needed action: registration or authentification\n", sockfd);
}

void new_TCP_server::close_connect(const Client& obj)
{
    Log::make_note("100002 " + sock_ntop((sockaddr*)&obj.cliaddr));
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
    std::unique_ptr<PostgresDB> conn = std::make_unique<PostgresDB>("dbname=loganalyzer user=matsuess password=731177889232 host=localhost port=5432");
    conn->connect();
    while(true)
    {
        std::unique_lock<std::mutex> ul(mtx);
        cv.wait(ul, [this]{ return !clients.empty(); });
        Client client = clients.front();
        clients.pop();
        ul.unlock();

        int result = map_handle.find(handle_clients.find(client)->second)->second->handle(client, conn);

        switch(handle_clients.at(client)){
        case greetings:
            if(result == NOONE){
                write_str("You entered false action. Try again\n", client.sockfd);
                break;
            }
            else if(result == disconnect){
                close_connect(client);
                break;
            }
            handle_clients[client] = Action(result);
            write_str("Entered name and password for continue\n", client.sockfd);
            break;
        case registration:
            if(result == disconnect){
                close_connect(client);
                break;
            }
            else if(result == existing){
                write_str("User with this name is exist. Please, enter other name\n", client.sockfd);
                break;
            }
            else{
                write_str("Your registration was success. You can use this app\n", client.sockfd);
                write_str("For next just enter command: (get) password, (add) password, (del) password, (edit) auth password\nJust enter command, which write in the brackets\n", client.sockfd);
                client.id = result;
                handle_clients[client] = command_checker;
            }
            break;
        case authentification:
            if(result == bad_auth){
                write_str("You make a mistake. Try again\n", client.sockfd);
                break;
            }
            else if(result == disconnect){
                close_connect(client);
            }
            else if(result == blocked){
                write_str("You will blocked on 10 minutes.\n", client.sockfd);
                close_connect(client);
            }
            else{
                write_str("Your authorization was successful. You can use this app.\n", client.sockfd);
                write_str("For next just enter command: (get) password, (add) password, (del) password, (edit) auth password\nJust enter command, which write in the brackets\n", client.sockfd);
                client.id = result;
                handle_clients[client] = command_checker;
            }
            break;
        case command_checker:
            if(result == NOONE){
                write_str("You entered bad actions. Try again.\n", client.sockfd);
            }
            else if(result == disconnect){
                close_connect(client);
            }
            else{
                handle_clients[client] = Action(result);
            }
            break;
        case get:
            if(result == disconnect){
                close_connect(client);
                break;
            }
            if(result == not_exist)
                write_str("Resourse with this name isn't exist. Try again\n", client.sockfd);
            handle_clients[client] = Action(command_checker);
            break;
        case add:
            if(result == disconnect){
                close_connect(client);
                break;
            }
            if(result == existing){
                write_str("You entered exist resourse in your account. Enter another name.\n", client.sockfd);
            }
            else if(result == empty_values){
                write_str("You entered empty values\n", client.sockfd);
            }
            else{
                write_str("New data was added\n", client.sockfd);
            }
            handle_clients[client] = command_checker;
            break;
        case edit:
            if(result == disconnect){
                close_connect(client);
                break;
            }
            handle_clients[client] = command_checker;
            break;
        }
        if(result != disconnect)
            FD_SET(client.sockfd, &master_fd);
    }
}