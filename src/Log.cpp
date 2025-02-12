#include "Log.h"

#include <chrono>
#include <fstream>
#include <sstream>

int Log::que_size = 0;
std::mutex Log::mtx_que; //block queue
std::mutex Log::mtx_file; //block file
std::queue<std::string> Log::que;
std::string Log::log_file;

void Log::flush_file()
{
    std::queue<std::string> local_queue;

    {
        std::lock_guard<std::mutex> lg(mtx_que);
        local_queue = std::move(que);
    }

    std::lock_guard<std::mutex> lg(mtx_file);
    std::ofstream file(log_file, std::ios_base::app | std::ios_base::binary);
    while(!local_queue.empty()){
        file.write(local_queue.front().c_str(), local_queue.front().size());
        local_queue.pop();
    }
    file.write("\0", 1);
    file.flush();
    file.close();
}

void Log::make_note(const std::string& str)
{
    std::unique_lock<std::mutex> ul(mtx_que);
    if(que_size > MAX_BUFFER_QUEUE){
        que_size = 0; //Lose thread racing
        ul.unlock();
        flush_file();
        ul.lock();
    }
    que_size++;
    que.push(std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
    + ' ' + str + '\n');
}

std::pair<Code_value, Info_value> to_dataline(const std::string& str)
{
    std::istringstream ss(str);
    std::string time, code, addr;
    ss >> time >> code >> addr;
    return addr.empty() ? std::make_pair(Code_value(std::stoi(code)), Info_value(time)) :
    std::make_pair(Code_value(std::stoi(code)), Info_value(time, addr));
}

void Log::read_all_note(std::unique_ptr<Compressor>& ptr)
{
    //In this funcs maximum can be worked - 1 thread
    char ch;
    if(!que.empty())
        flush_file();
    std::lock_guard<std::mutex> lg(mtx_file);
    std::ifstream file(log_file, std::ios_base::in | std::ios_base::binary);
    while(!file.eof()){
        std::string str;
        while(file.get(ch) && ch != '\n' && ch != '\0'){
            str.push_back(ch);
        }
        if(!str.empty())
            ptr->insert(to_dataline(str));
        if(ptr->size() == MAX_LOG_READ)
            ptr->make_compress();
    }
    file.close();
    new_log_filename();
}

void Log::new_log_filename()
{
    std::time_t t = std::time(nullptr);
    std::tm* time_now = std::localtime(&t);
    log_file = "../res/log/" + std::to_string(time_now->tm_mday) + "-" +
    std::to_string(time_now->tm_mon+1) + "_" + std::to_string(time_now->tm_hour) +
    ":" + std::to_string(time_now->tm_min) + ".bin";
}

// void Log::remainders_log(const std::unique_ptr<Compressor>& obj)
// {
//     std::lock_guard<std::mutex> lg(mtx_file);
//     std::ofstream file(log_file, std::ios_base::app | std::ios_base::binary);
//     while(obj->size()){
//         auto t = obj->lose_bad_log();
//         file.write(t.second.get_time_t().c_str(), t.second.get_time_t().size());
//         file.write(" ", 1);
        
//         std::string code = std::to_string(t.first.get_code_value());
//         file.write(code.c_str(), code.size());
//         file.write(" ", 1);

//         file.write(t.second.get_addr().c_str(), t.second.get_addr().size());
//         file.write("\n", 1);
//     }
//     file.write("\0", 1);
//     file.flush();
//     file.close();
// }
