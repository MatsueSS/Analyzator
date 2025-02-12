#include "Auth.h"
#include "Hasher.h"
#include "Log.h"

#include <sstream>
#include <map>

Auth::Auth(PostgresDB* ptr) : conn(ptr) {}

int Auth::handle(const Client& client)
{
    static const std::map<int, std::string> results = {
        {FALSE_DATA, "You have entered incorrect values.Try again\n"},
        {ERROR_CREDENTIALS, "You entered empty name or empty password.Try again\n"},
        {ERROR_EXISTING, "User with this name is exist.\n Please, enter new inquiry\n"},
        {LOSE_TRIES, "You lose all tries\nIn the next try you will banned\n"},
        {ALL_GOOD, "All good. You can use this app.\n"}
    };
    hello(client.sockfd);
    int id = FALSE_DATA;
    for(int i = 0; i < AUTH_TRY; i++){
        if(!read_fd(client.sockfd))
            break;
        std::string name, pass;
        action act = reg_or_auth(buf, name, pass);

        if(act == auth)
            id = make_auth(name, pass);
        else if(act == reg)
            id = make_reg(name, pass, sock_ntop((sockaddr *)&client.cliaddr));
        else{
            error(client.sockfd);
            continue;
        }
        if(id > 0){
            write_str(results.at(ALL_GOOD), client.sockfd);
            return id;
        }
        
        if(id == ERROR_NOT_USER)
            return CRITICAL_SITUATION;
        
        write_str(results.at(id), client.sockfd);
    }
    write_str(results.at(LOSE_TRIES), client.sockfd);
    return FALSE_DATA;
}

int Auth::make_auth(const std::string& name, const std::string& pass)
{
    std::string query = "SELECT * FROM users WHERE name_user = $1;";
    std::vector<std::vector<std::string>> result = conn->fetch(query, {name});
    if(result.empty()){
        Log::make_note("200001");
        return FALSE_DATA;
    }
    return Hasher::verify_password(result[0][2], pass) ? std::stoi(result[0][0]) : -1;
}

int Auth::make_reg(const std::string& name, const std::string& pass, const std::string& addr)
{
    if(name.empty() || pass.empty())
        return ERROR_CREDENTIALS;

    std::string hash_pass = Hasher::make_hash(pass);
    if(hash_pass.empty()){
        Log::make_note("1004");
        return ERROR_NOT_USER;
    }

    std::string query = "INSERT INTO users (name_user, password, ip_registration) VALUES ($1, $2, $3);";
    if(!conn->execute(query, {name, hash_pass, addr})){
        Log::make_note("200101");
        return ERROR_EXISTING;
    }
    query = "SELECT * FROM users WHERE name_user = $1;";
    std::vector<std::vector<std::string>> result = conn->fetch(query, {name});
    if(result.empty()){
        Log::make_note("201");
        return ERROR_NOT_USER;
    }
    return std::stoi(result[0][0]);
}

void Auth::hello(int sockfd)
{
    write_str("hello, Do you want registration or authentification?\nJust write need action and your name and password with separators - spaces\n", sockfd);
}

void Auth::error(int sockfd)
{
    write_str("You entered false action\n", sockfd);
}

Auth::action Auth::reg_or_auth(const std::string& str, std::string& name, std::string& pass)
{
    static const std::unordered_map<std::string, action> actions = {
        {"auth", auth}, {"authentification", auth},
        {"reg", reg}, {"registration", reg}
    };

    std::istringstream ss(str);
    std::string act;
    ss >> act >> name >> pass;

    auto it = actions.find(act);
    return it != actions.end() ? it->second : noone;
}