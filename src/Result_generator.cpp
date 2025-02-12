#include "Result_generator.h"
#include "Log.h"

#include <chrono>
#include <fstream>

Result_generator::Result_generator(std::unique_ptr<Compressor> ptr_com, std::unique_ptr<CSV_reader> ptr_csv)
    : compressor(std::move(ptr_com)), translator(std::move(ptr_csv)) {}

void Result_generator::generate_report() const
{
    std::time_t t = std::time(nullptr);
    std::tm* time = std::localtime(&t);
    std::ofstream file("../res/results/" + std::to_string(time->tm_mday) + "-"
    + std::to_string(time->tm_mon+1) + "_" + std::to_string(time->tm_hour) 
    + ":" + std::to_string(time->tm_min) + ".txt");

    file << "Daily result on " << time->tm_mday << '-' << time->tm_mon+1 << '-' 
    << time->tm_year+1900 << ' ' << time->tm_hour << ':' << time->tm_min << '\n';

    const std::unordered_map<Code_value, int>& preresult = compressor->get_result();
    for(auto it = preresult.begin(); it != preresult.end(); it++){
        file << translator->get_definition(it->first) << ": " << it->second << '\n';
    }
    file.close();

    // Log::remainders_log(compressor);
}

Result_generator::Result_generator(Result_generator&& obj) noexcept
{
    compressor = std::move(obj.compressor);
    translator = std::move(obj.translator);
}

Result_generator& Result_generator::operator=(Result_generator&& obj) noexcept
{
    if(this == &obj)
        return *this;

    compressor = std::move(obj.compressor);
    translator = std::move(obj.translator);

    return *this;
}