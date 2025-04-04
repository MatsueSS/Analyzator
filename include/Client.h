#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sys/socket.h>
#include <string>

struct Client{
    int sockfd; //socket client 
    mutable int id; //user-id in database
    sockaddr_storage cliaddr;
    socklen_t clilen;

    Client(int, const sockaddr_storage&, socklen_t);
    Client(int);
    Client& operator=(const Client&);
    Client(const Client&);
    Client(Client&&) noexcept;
    Client& operator=(Client&&) noexcept;

    bool operator==(const Client&) const;
};

namespace std
{
    template<>
    struct hash<Client>
    {
        std::size_t operator()(const Client& c) const {
            return std::hash<int>()(c.sockfd);
        }
    };
}

#endif //_CLIENT_H_