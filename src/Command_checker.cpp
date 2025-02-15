#include "Command_checker.h"

#include <unordered_map>
#include <unistd.h>

int Command_checker::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    static const std::unordered_map<std::string, Action> commands = {
        {"get", get}, {"delete", del}, {"del", del}, {"add", add}, {"edit", edit}
    };

    static const std::unordered_map<Action, std::string> descriptions = {
        {get, "Please enter the name of the desired resourse\n"},
        {add, "Please enter add name and password for this resourse\n"},
        {del, "Please enter the name of the desired resourse\n"},
        {edit, "Please enter the new password of your auth\n"}
    };

    auto it = commands.find(buf);

    if(it == commands.end()){
        std::string bad = "You entered the bad actions. Try again.\n";
        write(obj.sockfd, bad.c_str(), bad.size());
        return NOONE;
    }

    write(obj.sockfd, descriptions.find(it->second)->second.c_str(), descriptions.find(it->second)->second.size());

    return it->second;
}

void Command_checker::make_transaction(Action act, int id, const std::string& name, 
    const std::string& addr, std::unique_ptr<PostgresDB>& db)
{
    static const std::unordered_map<Action, std::string> commands = {
        {get, "get"}, {add, "add"}, {del, "delete"}, {edit, "edit"}
    };
    std::string query = "INSERT INTO transaction (name_resourse, name_action, time_appeal, ip_user, user_id) VALUES ($1, $2, NOW(), $3, $4);";
    db->execute(query, {name, commands.at(act), addr, std::to_string(id)});
}