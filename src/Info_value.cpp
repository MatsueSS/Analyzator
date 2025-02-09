#include "Info_value.h"

#include <chrono>

Info_value::Info_value(const std::string& ctime_str, const std::string& addr = "") :
    ctime_str(ctime_str), addr(addr) {}

std::string Info_value::get_addr() const
{
    return addr;
}

std::string Info_value::get_time_t() const
{
    return ctime_str;
}

std::string Info_value::from_time_t() const
{
    time_t t = stoll(ctime_str);
    return ctime(&t);
}