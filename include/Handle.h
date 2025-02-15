#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "PostgresDB.h"
#include "Client.h"

#include <memory>

#define MAXLINE 1024

enum Action{
    greetings = 0, registration = 1, authentification = 2, command_checker = 3,
    get = 4, add = 5, del = 6, edit = 7
};

enum Error{
    disconnect = -1, blocked = -2, lose_tries = -3, existing = -4, bad_auth = -5,
    not_exist = -6, empty_values = -7
};

class Handle{
public:
    virtual ~Handle() = default;

    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) = 0;

protected:
    char buf[MAXLINE];
    ssize_t n;

    //Read socket buffer
    bool read_fd(int);
};

#endif //_HANDLE_H_