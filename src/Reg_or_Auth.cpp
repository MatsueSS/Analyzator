#include "Reg_or_Auth.h"

#include <unordered_map>

int Reg_or_Auth::handle(const Client& obj)
{
    static const std::unordered_map<std::string, int> actions = {
        {"auth", AUTHENTIFICATION}, {"authentification", AUTHENTIFICATION},
        {"reg", REGISTRATION}, {"registration", REGISTRATION}
    };

    if(!read_fd)
        return CLIENT_DISCONNECTED;
    
    auto it = actions.find(buf);
    return it != actions.end() ? it->second : NOONE;
}