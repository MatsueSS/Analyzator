#ifndef _LOG_H_
#define _LOG_H_

#include <queue>
#include <mutex>
#include <string>

#define MAX_BUFFER_QUEUE 2 //Maximum size in queue

class Log{
public:
    static void make_note(const std::string&);
    //static void read_all_note();

private:
    static std::mutex mtx;
    static std::queue<std::string> que;
    static int que_size;
};

#endif //_LOG_H_