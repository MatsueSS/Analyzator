#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "TCP_server.h"

class Handle{
public:
    virtual ~Handle() = default;

    virtual int handle(const Client&) = 0;

protected:
    unsigned int id;
    char buf[MAXLINE];
    ssize_t n;

    bool read_fd(int);
    void write_str(const std::string&, int) const;
};

#endif //_HANDLE_H_