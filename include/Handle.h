#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "new_TCP_server.h"
#include "PostgresDB.h"

#define CLIENT_DISCONNECTED -1
#define MAXLINE 1024

enum Action{
    greetings = 0, registration = 1, authentification = 2, 
};

enum Error{
    disconnect = -1, blocked = -2, lose_tries = -3, existing = -4
};

class Handle{
public:
    virtual ~Handle() = default;

    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) = 0;

protected:
    unsigned int id;
    char buf[MAXLINE];
    ssize_t n;

    bool read_fd(int);
};

#endif //_HANDLE_H_