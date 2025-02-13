#ifndef _COMMAND_CHECKER_H_
#define _COMMAND_CHECKER_H_

#include "Handle.h"

class Command_checker : public Handle{
public:
    virtual ~Command_checker() = default;

    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;
};

#endif //_COMMAND_CHECKER_H_