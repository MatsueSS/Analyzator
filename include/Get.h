#ifndef _GET_H_
#define _GET_H_

#include "Command_checker.h"

class Get : public Command_checker{
public:
    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;
};

#endif //_GET_H_