#ifndef _HASHER_H_
#define _HASHER_H_

//Need for hash password, which clients give for server for authentification

#include <sodium.h>
#include <string>

class Hasher{
public:
    static std::string make_hash(const std::string&);

    static bool verify_password(const std::string&, const std::string&);
};

#endif //_HASHER_H_