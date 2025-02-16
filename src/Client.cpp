#include "Client.h"

#include <cstring>
#include <unistd.h>

Client::Client(int sockfd, const sockaddr_storage& addr, socklen_t len) 
    : sockfd(sockfd), clilen(len), id(-1)
{
    memcpy(&cliaddr, &addr, sizeof(sockaddr_storage));
}

Client::Client(const Client& obj) : sockfd(obj.sockfd), clilen(obj.clilen),
    id(obj.id)
{
    memcpy(&cliaddr, &obj.cliaddr, sizeof(sockaddr_storage));
}

Client::Client(Client&& obj) noexcept : sockfd(obj.sockfd), clilen(obj.clilen), id(obj.id)
{
    memmove(&cliaddr, &obj.cliaddr, sizeof(sockaddr_storage));
    obj.sockfd = -1;
    obj.clilen = 0;
    obj.id = 0;
}

Client& Client::operator=(Client&& obj) noexcept
{
    if(this == &obj)
        return *this;

    sockfd = obj.sockfd;
    clilen = obj.clilen;
    id = obj.id;
    memmove(&cliaddr, &obj.cliaddr, sizeof(sockaddr_storage));
    obj.sockfd = -1;

    return *this;
}

Client& Client::operator=(const Client& obj)
{
    if(this == &obj)
        return *this;

    memcpy(&cliaddr, &obj.cliaddr, sizeof(sockaddr_storage));
    sockfd = obj.sockfd;
    clilen = obj.clilen;
    id = obj.id;
    return *this;
}

Client::Client(int sockfd) : sockfd(sockfd) {}

bool Client::operator==(const Client& obj) const
{
    return sockfd == obj.sockfd;
}