#include "Authentification.h"
#include "Hasher.h"
#include "Log.h"

#include <sstream>

int Authentification::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(obj.bad_auth_tries == CLIENT_TRIES)
        return blocked;

    if(!read_fd(obj.sockfd))
        return disconnect;
    
    std::istringstream ss(buf);
    std::string name, pass;
    ss >> name >> pass;

    if(name.empty() || pass.empty())
        return empty_values;

    std::string query = "SELECT * FROM users WHERE name_user = $1;";
    std::vector<std::vector<std::string>> result = db->fetch(query, {name});

    if(result.empty()){
        Log::make_note("200001");
        return bad_auth;
    }

    if(!Hasher::verify_password(result[0][2], pass))
        return bad_auth;

    return std::stoi(result[0][0]);
}