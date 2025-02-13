#include "Command_checker.h"

#include <unordered_map>

int Command_checker::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    static const std::unordered_map<std::string, Action> commands = {
        {"get", get}, {"delete", del}, {"del", del}, {"add", add}, {"edit", edit}
    };

    auto it = commands.find(buf);
    return it != commands.end() ? it->second : NOONE;
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