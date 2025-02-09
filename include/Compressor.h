#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_

#include "Code_value.h"
#include "Info_value.h"

#include <unordered_map>

#define MAX_LOG_READ 250 //Value for max size transfer. Be careful in edit this value
#define MAX_SIZE_CODE 32 //Maximum count code-value in .csv with definition code.

class Compressor{
public:
    Compressor();

    void insert(const std::pair<Code_value, Info_value>&);
    void insert(const Code_value&, const Info_value&);

    void make_compress();

    int size() const;

private:
    std::unordered_multimap<Code_value, Info_value> transfer; //accumulate log data
    std::unordered_map<Code_value, Info_value> compressed_info; //compressed data
    int sz; //transfer size

    void erase();
};

#endif //_COMPRESSOR_H_