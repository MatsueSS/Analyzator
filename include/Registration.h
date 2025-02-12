#ifndef _REGISTRATION_H_
#define _REGISTRATION_H_

#include "Reg_or_Auth.h"

class Registration : public Reg_or_Auth{
public:
    virtual int handle(const Client&, std::unique_ptr<PostgresDB>&) override;

};

#endif //_REGISTRATION_H_