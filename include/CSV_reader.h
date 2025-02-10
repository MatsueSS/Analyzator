#ifndef _CSV_READER_H_
#define _CSV_READER_H_

#include "Code_value.h"

#include <string>
#include <unordered_map>

void create_table_and_fill();

class CSV_reader{
public:
    CSV_reader(const std::string&);
    CSV_reader(const CSV_reader&) = delete;
    CSV_reader(CSV_reader&&) noexcept;

    CSV_reader& operator=(const CSV_reader&) = delete;
    CSV_reader& operator=(CSV_reader&&) noexcept;

    std::string get_definition(const Code_value&) const;

private:
    std::unordered_map<Code_value, std::string> definition_codes;
};

#endif //_CSV_READER_H_