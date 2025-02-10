#ifndef _RESULT_GENERATOR_H_
#define _RESULT_GENERATOR_H_

#include "Compressor.h"
#include "CSV_reader.h"

class Result_generator{
public:
    Result_generator(const Compressor&, const CSV_reader&);

    void generate_report(const std::string&);

private:
    Compressor compressor;
    CSV_reader translator;

};

#endif //_RESULTER_H_