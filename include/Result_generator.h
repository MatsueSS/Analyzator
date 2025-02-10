#ifndef _RESULT_GENERATOR_H_
#define _RESULT_GENERATOR_H_

#include "Compressor.h"
#include "CSV_reader.h"

#include <memory>

class Result_generator{
public:
    Result_generator(std::unique_ptr<Compressor>, std::unique_ptr<CSV_reader>);

    void generate_report() const;

private:
    std::unique_ptr<Compressor> compressor;
    std::unique_ptr<CSV_reader> translator;

};

#endif //_RESULT_GENERATOR_H_