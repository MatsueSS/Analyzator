#ifndef _INFO_VALUE_H_
#define _INFO_VALUE_H_

#include <string>

class Info_value{
public:
    Info_value() = default;
    Info_value(const std::string&, const std::string& = "");

    std::string from_time_t() const;

    std::string get_time_t() const;
    std::string get_addr() const;

private:
    std::string ctime_str;
    std::string addr;

};

#endif //_INFO_VALUE_H_