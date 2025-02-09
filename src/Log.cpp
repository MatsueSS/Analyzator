#include "Log.h"

#include <chrono>
#include <fstream>

int Log::que_size = 0;
std::mutex Log::mtx;
std::queue<std::string> Log::que;

void Log::flush_file()
{
    std::lock_guard<std::mutex> lg(mtx);
    std::ofstream file("../res/log.bin", std::ios_base::app | std::ios_base::binary);
    while(!que.empty()){
        file.write(que.front().c_str(), que.front().size());
        que.pop();
    }
    file.write("\0", 1);
    file.flush();
    file.close();
    que_size = 0;
}

void Log::make_note(const std::string& str)
{
    //In this place probably race, but this doesn't matter
    if(que_size > MAX_BUFFER_QUEUE)
        flush_file();
    std::lock_guard<std::mutex> lg(mtx);
    que_size++;
    que.push(std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
    + ' ' + str + '\n');
}

void Log::read_all_note()
{
    
}