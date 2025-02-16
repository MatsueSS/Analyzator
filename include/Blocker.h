#ifndef _BLOCKER_H_
#define _BLOCKER_H_

#include <string>
#include <map>
#include <mutex>

#define TIME_BLOCK 10 //block bad users on minutes

class Blocker{
public:
    //Added new prison with mutex
    static void add_prison(const std::string&);
    //Check prison on prisons
    static bool has_prison(const std::string&);

private:
    static std::map<std::string, std::string> prisons;
    static std::mutex mtx_add_prison;
};

#endif //_BLOCKER_H_