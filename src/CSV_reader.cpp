#include "CSV_reader.h"
#include "Compressor.h"
#include "Log.h"

#include <fstream>
#include <sstream>

CSV_reader::CSV_reader(const std::string& filepath)
{
    definition_codes.reserve(MAX_SIZE_CODE);
    std::ifstream file(filepath);
    if(!file.is_open()){
        Log::make_note("5001");
        //generate file by default
    }
    std::string line;
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string definition, code;
        std::getline(ss, code, ':');
        std::getline(ss, definition);
        definition_codes.insert(std::make_pair(std::stoi(code), definition));
    }
    file.close();
}

CSV_reader::CSV_reader(CSV_reader&& obj) noexcept 
    : definition_codes(std::move(obj.definition_codes)) {}

CSV_reader& CSV_reader::operator=(CSV_reader&& obj) noexcept
{
    if(this == &obj)
        return *this;

    definition_codes = std::move(definition_codes);
    return *this;
}

std::string CSV_reader::get_definition(const Code_value& obj) const
{
    return definition_codes.find(obj)->second;
}