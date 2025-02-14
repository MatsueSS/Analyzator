#ifndef _ADD_H_
#define _ADD_H_

#include "Command_checker.h"

class Add : public Command_checker{
public:
    virtual int handle(const Client&, PostgresDB*) override;

private:
    bool is_not_exist(const std::string&, int, PostgresDB*);
};

#endif //_ADD_H_