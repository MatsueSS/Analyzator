#include "Reg_or_Auth.h"

#include <unordered_map>

int Reg_or_Auth::handle(const Client& obj, std::unique_ptr<PostgresDB>&)
{
    static const std::unordered_map<std::string, Action> actions = {
        {"auth", authentification}, {"authentification", authentification},
        {"reg", registration}, {"registration", registration}
    };

    if(!read_fd)
        return CLIENT_DISCONNECTED;
    
    auto it = actions.find(buf);
    return it != actions.end() ? it->second : NOONE;
}