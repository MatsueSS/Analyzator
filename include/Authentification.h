#ifndef _AUTHENTIFICATION_H_
#define _AUTHENTIFICATION_H_

#include "Reg_or_Auth.h"

class Authentification : public Reg_or_Auth{
public:
    virtual int handle(const Client&, PostgresDB*) override;
};

#endif //_AUTHNTIFICATION_H_