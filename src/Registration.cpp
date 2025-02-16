#include "Registration.h"
#include "Hasher.h"
#include "Log.h"
#include "new_TCP_server.h"

#include <sstream>

int Registration::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    std::istringstream ss(buf);
    std::string name, pass;
    ss >> name >> pass;

    if(name.empty() || pass.empty())
        return empty_values;

    std::string query = "INSERT INTO users (name_user, password, ip_registration) VALUES ($1, $2, $3) RETURNING id;";
    std::vector<std::vector<std::string>> result = db->fetch(query, {name, Hasher::make_hash(pass), sock_ntop((sockaddr*)&obj.cliaddr)});
    if(result.empty()){
        Log::make_note("200101");
        return existing;
    }

    return std::stoi(result[0][0]);
}