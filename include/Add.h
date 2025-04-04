#ifndef _ADD_H_
#define _ADD_H_

#include "Command_checker.h"

class Add : public Command_checker{
public:
    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;

private:
    //check existing user with entered name
    bool is_not_exist(const std::string&, int, std::unique_ptr<PostgresDB>&);
};

#endif //_ADD_H_