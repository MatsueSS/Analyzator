#include "Log.h"

void test(std::unique_ptr<Compressor>& ptr)
{
    Log::read_all_note(ptr);
}

int main(void)
{
    std::unique_ptr<Compressor> compressor = std::make_unique<Compressor>();
    test(compressor);

    return 0;
}