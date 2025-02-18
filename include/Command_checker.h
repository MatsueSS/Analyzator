#ifndef _COMMAND_CHECKER_H_
#define _COMMAND_CHECKER_H_

#include "Handle.h"
#include "new_TCP_server.h"

#define NOONE 0
#define SUCCESS 1

class Command_checker : public Handle{
public:
    virtual ~Command_checker() = default;

    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;

protected:
    //entered in transaction's table users actions
    void make_transaction(Action, int, const std::string&, const std::string&, std::unique_ptr<PostgresDB>&);
};

#endif //_COMMAND_CHECKER_H_