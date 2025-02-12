#ifndef _LOG_H_
#define _LOG_H_

#include "Compressor.h"

#include <queue>
#include <mutex>
#include <memory>
#include <string>

//This macros must be have value = effective maximum - count using thread in programm
#define MAX_BUFFER_QUEUE 25 //Maximum size in queue

//Can be break string for Code_value and Info_value
std::pair<Code_value, Info_value> to_dataline(const std::string&);

class Log{
public:
    static void make_note(const std::string&);
    static void read_all_note(std::unique_ptr<Compressor>&);

    static void new_log_filename();

    static void remainders_log(const std::unique_ptr<Compressor>&);

private:
    static std::mutex mtx_que;
    static std::mutex mtx_file;
    static std::queue<std::string> que;
    static int que_size;
    static std::string log_file;

    static void flush_file();
};

#endif //_LOG_H_