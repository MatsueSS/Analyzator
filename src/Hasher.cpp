#include "Hasher.h"

std::string Hasher::make_hash(const std::string &psw)
{
    std::string hash(crypto_pwhash_STRBYTES, '\0');
    if(crypto_pwhash_str(
        hash.data(),
        psw.c_str(),
        psw.length(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    ) != 0)
        return "";
    return hash;
}

bool Hasher::verify_password(const std::string &hashed, const std::string &psw)
{
    return crypto_pwhash_str_verify(hashed.c_str(), psw.c_str(), psw.length()) == 0;
}