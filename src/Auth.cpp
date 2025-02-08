#include "Auth.h"
#include "Hasher.h"
#include "Log.h"

#include <sstream>

Auth::Auth(const std::unique_ptr<PostgresDB>& ptr) : conn(ptr) {}

int Auth::handle(const Client& client)
{
    hello(client.sockfd);
    bool v;
    int id = -1;
    for(int i = 1, v = read_fd(client.sockfd); i < AUTH_TRY && v && id <= 0; i++, v = read_fd(client.sockfd)){
        std::string name, pass;
        action act = reg_or_auth(buf, name, pass);
        switch(act){
        case 0:
            id = make_auth(name, pass);
            break;
        case 1:
            id = make_reg(name, pass, sock_ntop((sockaddr *)&client.cliaddr));
            break;
        default:
            error(client.sockfd);
        }
        if(id == 0){
            bad_reg(client.sockfd);
            i--;
        }
    }
    if(id == -1)
        bad_client(client.sockfd);

    return id;
}

int Auth::make_auth(const std::string& name, const std::string& pass)
{
    std::string query = "SELECT * FROM users WHERE name_user = $1;";
    std::vector<std::vector<std::string>> result = conn->fetch(query, {name});
    if(result.empty()){
        Log::make_note("200003");
        return -1;
    }
    return Hasher::verify_password(result[0][2], pass) ? std::stoi(result[0][0]) : -1;
}

int Auth::make_reg(const std::string& name, const std::string& pass, const std::string& addr)
{
    std::string hash_pass = Hasher::make_hash(pass);
    if(hash_pass.empty()){
        Log::make_note("200004");
        return -1;
    }
    std::string query = "INSERT INTO users (name_user, password, ip_registration) VALUES ($1, $2, $3);";
    if(!conn->execute(query, {name, hash_pass, addr})){
        Log::make_note("200101");
        return 0;
    }
    query = "SELECT * FROM users WHERE name_user = $1;";
    std::vector<std::vector<std::string>> result = conn->fetch(query, {name});
    if(result.empty()){
        Log::make_note("200005");
        return -1;
    }
    return std::stoi(result[0][0]);
}

void Auth::hello(int sockfd)
{
    write_str("hello, Do you want registration or authentification?\nJust write need action and your name and password with separators - spaces\n", sockfd);
}

void Auth::bad_client(int sockfd)
{
    write_str("You lose all tries\nIn the next try you will banned\n", sockfd);
}

void Auth::error(int sockfd)
{
    write_str("You entered false action\n", sockfd);
}

void Auth::bad_reg(int sockfd)
{
    write_str("User with this name is exist.\n Please, enter new inquiry\n", sockfd);
}

Auth::action Auth::reg_or_auth(const std::string& str, std::string& name, std::string& pass)
{
    std::istringstream ss(str);
    std::string act;
    ss >> act >> name >> pass;
    if(act == "authentification" || act == "auth")
        return auth;
    else if(act == "registration" || act == "reg")
        return reg;
    else
        return noone;
}