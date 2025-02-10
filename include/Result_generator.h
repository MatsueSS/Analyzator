#ifndef _RESULT_GENERATOR_H_
#define _RESULT_GENERATOR_H_

#include "Compressor.h"
#include "CSV_reader.h"

#include <memory>

class Result_generator{
public:
    Result_generator(std::shared_ptr<Compressor>, std::shared_ptr<CSV_reader>);

    void generate_report() const;

private:
    std::shared_ptr<Compressor> compressor;
    std::shared_ptr<CSV_reader> translator;

};

#endif //_RESULTER_H_