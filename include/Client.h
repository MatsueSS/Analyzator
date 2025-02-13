#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sys/socket.h>
#include <string>

struct Client{
    int sockfd, id, bad_auth_tries;
    sockaddr_storage cliaddr;
    socklen_t clilen;

    Client(int, const sockaddr_storage&, socklen_t);
    Client(int);
    Client& operator=(const Client&);
    Client(const Client&);

    bool operator==(const Client&) const;
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

#endif //_CLIENT_H_