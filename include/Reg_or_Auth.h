#ifndef _REG_OR_AUTH_H_
#define _REG_OR_AUTH_H_

#include "Handle.h"

#define NOONE 0
#define CLIENT_TRIES 3

class Reg_or_Auth : public Handle {
public:
    virtual ~Reg_or_Auth() = default;

    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;
};

#endif //_REG_OR_AUTH_H_