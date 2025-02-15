#ifndef _DEL_H_
#define _DEL_H_

#include "Command_checker.h"

class Del : public Command_checker{
public:
    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;
};

#endif //_DEL_H_