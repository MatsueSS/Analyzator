#ifndef _AUTH_H_
#define _AUTH_H_

#include "Handle.h"
#include "PostgresDB.h"

#include <memory>

#define AUTH_TRY 3 //Count tries for authentification

class Auth : public Handle {
public:
    Auth(std::unique_ptr<PostgresDB>);

    virtual int handle(const Client&) override;

private:
    std::unique_ptr<PostgresDB> conn;
    enum action { auth = 0, reg = 1, noone = -1};

    action reg_or_auth(const std::string&, std::string&, std::string&);
    bool make_auth(const std::string&, const std::string&);
    void make_reg(const std::string&, const std::string&, const std::string&);
    void error(int);
    void bad_client(int);

};

#endif //_AUTH_H_