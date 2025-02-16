#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_

/*This class helps to read a file in portions. This class is designed to allocate 
the amount of necessary information into a separate container, which will be 
output to the resulting file*/

#include "Code_value.h"
#include "Info_value.h"

#include <unordered_map>

#define MAX_LOG_READ 250 //Value for max size transfer. Be careful in edit this value
#define MAX_SIZE_CODE 32 //Maximum count code-value in .csv with definition code.
#define LIMIT_ACCUMULATION_ADDR 250 //Limited this value, because i take limit clients

class Compressor{
public:
    Compressor();
    Compressor(const Compressor&) = delete;
    Compressor(Compressor&&) noexcept;

    Compressor& operator=(const Compressor&) = delete;
    Compressor& operator=(Compressor&&) noexcept;

    void insert(const std::pair<Code_value, Info_value>&);
    void insert(const Code_value&, const Info_value&);

    //Analyze all data in this object and sorted his
    void make_compress();

    const std::unordered_map<Code_value, int>& get_result() const;

    // std::pair<Code_value, Info_value> lose_bad_log();

    int size() const;

private:
    std::unordered_multimap<Code_value, Info_value> transfer; //accumulate log data
    std::unordered_map<Code_value, int> compressed_info; //compressed data
    int sz; //transfer size

    //Help be analyze time code connect and disconnect
    void handle_time();
};

#endif //_COMPRESSOR_H_