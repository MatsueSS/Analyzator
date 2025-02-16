#include "CSV_reader.h"
#include "Compressor.h"
#include "Log.h"

#include <fstream>
#include <sstream>

CSV_reader::CSV_reader(const std::string& filepath)
{
    definition_codes.reserve(MAX_SIZE_CODE);
    std::ifstream file(filepath);
    if(!file){
        Log::make_note("5001");
        file.close();
        create_table_and_fill();
        file.open(filepath);
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

void create_table_and_fill()
{
    std::ofstream file("../res/codes.csv");
    
    file << "101:system error create socket\n";
    file << "102:system error bind\n";
    file << "103:system error listen\n";
    file << "106:memory error accept\n";
    file << "107:limit open descriptor for thread/system\n";
    file << "108:memory error select\n";
    file << "109:system error select\n";
    file << "110:impossible create a file\n";
    file << "402:error lose connection with data base\n";
    file << "1001:logic/signal error in select\n";
    file << "1002:firewall rules forbid connection\n";
    file << "1003:connection with client was lost\n";
    file << "1004:error create hash-object\n";
    file << "2001:error connect with Postgres Data Base\n";
    file << "4001:logic error in design with password-action\n";
    file << "5001:file with code-error designations is missing\n";
    file << "10001:error update with new funcs and lose new realizing\n";
    file << "100001:new connection\n";
    file << "100002:close connection\n";
    file << "200001:bad quiry-SQL\n";
    file << "400001:maximum communication time\n";
    file << "400002:average time\n";
    file << "400003:banned client\n";
    file << "400004:count connect-clients\n";

    file.close();
}