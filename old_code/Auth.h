#ifndef _AUTH_H_
#define _AUTH_H_

#include "Handle.h"
#include "PostgresDB.h"

#include <memory>

#define AUTH_TRY 3 //Count tries for authentification
#define ALL_GOOD 1
#define FALSE_DATA -1 //client entered bad data
#define ERROR_CREDENTIALS -2 //client entered empry value
#define ERROR_EXISTING -3 //client entered existing data in for registration
#define LOSE_TRIES -4 //client lost all tries auth or reg
#define ERROR_NOT_USER -5
#define CRITICAL_SITUATION -6

class Auth : public Handle {
public:
    Auth(PostgresDB*);

    virtual int handle(const Client&) override;

private:
    PostgresDB* conn;
    enum action { auth = 0, reg = 1, noone = -1};

    action reg_or_auth(const std::string&, std::string&, std::string&);
    int make_auth(const std::string&, const std::string&);
    int make_reg(const std::string&, const std::string&, const std::string&);
    void error(int);
    void hello(int);

};

#endif //_AUTH_H_