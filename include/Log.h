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
    //Write a note in queue-notes
    static void make_note(const std::string&);
    //Read all notes in file
    static void read_all_note(std::unique_ptr<Compressor>&);
    //Need for update log file after result file
    static void new_log_filename();

    // static void remainders_log(const std::unique_ptr<Compressor>&);

private:
    static std::mutex mtx_que; //Blocked access from queue
    static std::mutex mtx_file; //Blocked access from file
    static std::queue<std::string> que; //Queue notes
    static int que_size;
    static std::string log_file; //Name file

    //Can be clear queue and write all data in log-file
    static void flush_file();
};

#endif //_LOG_H_