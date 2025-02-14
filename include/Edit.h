#ifndef _EDIT_H_
#define _EDIT_H_

#include "Command_checker.h"

class Edit : public Command_checker{
public:
    virtual int handle(const Client&, PostgresDB*) override;
};

#endif //_EDIT_H_