#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "TCP_server.h"

class Handle{
public:
    virtual ~Handle() = default;

    virtual int handle(const Client&) = 0;
};

#endif //_HANDLE_H_