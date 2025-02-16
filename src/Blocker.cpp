#include "Blocker.h"

#include <chrono>

std::mutex Blocker::mtx_add_prison;
std::map<std::string, std::string> Blocker::prisons;

void Blocker::add_prison(const std::string& addr)
{
    std::lock_guard<std::mutex> lg(mtx_add_prison);
    prisons[addr] = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

bool Blocker::has_prison(const std::string& addr)
{
    auto it = prisons.find(addr);
    if(it == prisons.end())
        return false;
    if(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - std::stoll(it->second) > TIME_BLOCK*60)
        return false;
    return true;
}