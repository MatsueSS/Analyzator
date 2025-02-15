#ifndef _PASSWORD_H_
#define _PASSWORD_H_

#include "Handle.h"
#include "PostgresDB.h"

class Password : public Handle{
public:
    Password(PostgresDB*, int);

    using ActionFunc = std::string (Password::*)(const std::string&);

    virtual int handle(const Client&) override;

private:
    PostgresDB* conn;
    int id;
    std::string addr;
    enum action { get = 0, add = 1, del = 2, edit = 3, bad_act = -1 };

    action check_action() const;
    void next_step(int, action) const;
    std::string get_prompt_for_action(action);
    void make_transaction(action, const std::string&);
    std::string func_get(const std::string&);
    std::string func_add(const std::string&);
    std::string func_del(const std::string&);
    std::string func_edit(const std::string&);

};

#endif //_PASSWORD_H_