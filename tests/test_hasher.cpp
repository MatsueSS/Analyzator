#include "Hasher.h"

#include <iostream>

int main(void)
{
    std::string psw = "111222333";
    std::string hash = Hasher::make_hash(psw);
    std::cout << "For psw: " << Hasher::verify_password(hash, psw) << '\n';

    std::string new_psw = "111222333";
    std::cout << "For new_psw: " << Hasher::verify_password(hash, new_psw) << '\n';

    return 0;
}