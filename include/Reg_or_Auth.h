#ifndef _REG_OR_AUTH_H_
#define _REG_OR_AUTH_H_

#include "Handle.h"

#define NOONE 0
#define CLIENT_DISCONNECTED -1

class Reg_or_Auth : public Handle {
public:
    virtual int handle(const Client&) override;
};

#endif //_REG_OR_AUTH_H_