#include "Log.h"

#include <iostream>

void test(std::unique_ptr<Compressor>& ptr)
{
    Log::read_all_note(ptr);
}

int main(void)
{
    std::unique_ptr<Compressor> compressor = std::make_unique<Compressor>();
    test(compressor);

    const std::unordered_map<Code_value, int>& result = compressor->get_result();

    std::cout << compressor->get_result().size() << '\n';
    std::cout << result.size() << '\n';

    return 0;
}