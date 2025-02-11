#include "Result_generator.h"
#include "Log.h"

int main(void)
{
    std::unique_ptr<Compressor> compressor = std::make_unique<Compressor>();
    std::unique_ptr<CSV_reader> csv_reader = std::make_unique<CSV_reader>("../res/codes.csv");

    Log::read_all_note(compressor);

    Result_generator result(std::move(compressor), std::move(csv_reader));

    result.generate_report();

    return 0;
}
