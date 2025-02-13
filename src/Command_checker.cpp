#include "Command_checker.h"

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