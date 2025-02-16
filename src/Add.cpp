#include "Add.h"
#include "Log.h"

#include <sstream>

int Add::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    std::istringstream ss(buf);
    std::string name, pass;
    ss >> name >> pass;
    if(name.empty() || pass.empty())
        return empty_values;

    if(!is_not_exist(name, obj.id, db))
        return existing;

    if(!db->is_connect()){
        Log::make_note("2001");
        if(!db->connect()){
            Log::make_note("202");
            return disconnect;
        }
    }

    std::string query = "INSERT INTO data (resourse_name, password, user_id) VALUES ($1, $2, $3);";
    db->execute(query, {name, pass, std::to_string(obj.id)});
    make_transaction(add, obj.id, name, sock_ntop((sockaddr *)&obj.cliaddr), db);
    return SUCCESS;
}

bool Add::is_not_exist(const std::string& name, int id, std::unique_ptr<PostgresDB>& db)
{
    std::string query = "SELECT * FROM data WHERE resourse_name = $1 AND user_id = $2;";
    std::vector<std::vector<std::string>> result = db->fetch(query, {name, std::to_string(id)});
    return result.empty();
}