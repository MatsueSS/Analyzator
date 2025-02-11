#include "Result_generator.h"

#include <chrono>
#include <fstream>

Result_generator::Result_generator(std::unique_ptr<Compressor> ptr_com, std::unique_ptr<CSV_reader> ptr_csv)
    : compressor(std::move(ptr_com)), translator(std::move(ptr_csv)) {}

void Result_generator::generate_report() const
{
    time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time = ctime(&t);
    time = std::string(time, 0, time.size()-1);
    std::ofstream file("../res/" + time);
    file << "Daily result on " << time << '\n';
    const std::unordered_map<Code_value, int>& preresult = compressor->get_result();
    for(auto it = preresult.begin(); it != preresult.end(); it++){
        file << translator->get_definition(it->first) << ": " << it->second << '\n';
    }
    file.close();
}

Result_generator::Result_generator(Result_generator&& obj)
{
    compressor = std::move(obj.compressor);
    translator = std::move(obj.translator);
}

Result_generator& Result_generator::operator=(Result_generator&& obj)
{
    if(this == &obj)
        return *this;

    compressor = std::move(obj.compressor);
    translator = std::move(obj.translator);

    return *this;
}