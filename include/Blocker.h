#ifndef _BLOCKER_H_
#define _BLOCKER_H_

#include <string>
#include <map>
#include <mutex>

class Blocker{
public:
    //Added new prison with mutex
    static void add_prison(std::string addr);
    //Check prison on prisons
    static bool has_prison(std::string addr);

private:
    static std::map<std::string, std::string> prisons;
    static std::mutex mtx_add_prison;
};

#endif //_BLOCKER_H_