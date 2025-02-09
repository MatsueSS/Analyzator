#include "Password.h"
#include "Log.h"
#include "Hasher.h"

#include <unordered_map>
#include <sstream>

Password::Password(PostgresDB* ptr, int id) : conn(ptr), id(id) {}

int Password::handle(const Client& obj)
{
    addr = sock_ntop((sockaddr *)&obj.cliaddr);
    write_str("For next just enter command: (get) password, (add) password, (del) password, (edit) auth password\nJust enter command, which write in the brackets\n", obj.sockfd);
    bool v;
    while((v = read_fd(obj.sockfd))){
        action act = check_action();
        next_step(obj.sockfd, act);
        if(act == bad_act)
            continue;
        
        if(!(v = read_fd(obj.sockfd)))
            return -1;
        
        std::string result = get_prompt_for_action(act);
        if(result == "Failure")
            return -1;
        
        write_str(result, obj.sockfd);
    }
    return 0;
}

Password::action Password::check_action() const
{
    static const std::unordered_map<std::string, action> commands = {
        {"get", get},
        {"add", add},
        {"delete", del},
        {"edit", edit}
    };

    auto it = commands.find(buf);
    return it != commands.end() ? it->second : bad_act;
}

void Password::next_step(int sockfd, action act) const
{
    static const std::unordered_map<action, std::string> step = {
        {get, "Please enter the name of the desired resourse\n"},
        {add, "Please enter add name and password for this resourse\n"},
        {del, "Please enter the name of the desired resourse\n"},
        {edit, "Please enter the new password of your auth\n"},
        {bad_act, "You entered the bad actions. Try again\n"}
    };

    auto it = step.find(act);
    if(it == step.end()){
        //critical error
        Log::make_note("401");
        return;
    }
    write_str(it->second, sockfd);
}
 
std::string Password::get_prompt_for_action(action act)
{
    static const std::unordered_map<action, ActionFunc> map_action = {
        {get, &Password::func_get},
        {add, &Password::func_add},
        {del, &Password::func_del},
        {edit, &Password::func_edit}
    };

    auto it = map_action.find(act);
    if(it == map_action.end()){
        //error with new-funcs
        Log::make_note("10001");
        return "Failure";
    }

    return (this->*it->second)(buf);
}

std::string Password::func_get(const std::string& str)
{
    std::string query = "SELECT * FROM data WHERE resourse_name = $1 AND user_id = $2;";
    std::vector<std::vector<std::string>> result = conn->fetch(query, {str, std::to_string(id)});
    if(result.empty())
        return "Resourse with this name doesn't exist\n";

    make_transaction(get, str);
    return result[0][2] + '\n';
}

std::string Password::func_add(const std::string& str)
{
    std::string name = "", pass = "";
    std::istringstream ss(str);
    ss >> name >> pass;
    if(name.empty() || pass.empty())
        return "Bad inquiry\n";

    std::string query = "INSERT INTO data (resourse_name, password, user_id) VALUES ($1, $2, $3);";
    if(!conn->execute(query, {name, pass, std::to_string(id)}))
        return "Bad inquiry\n";

    make_transaction(add, name);
    return "All good. Your inquiry was completed\n";
}

std::string Password::func_del(const std::string& str)
{
    std::string query = "DELETE FROM data WHERE resourse_name = $1 AND user_id = $2;";
    if(!conn->execute(query, {str, std::to_string(id)}))
        return "Bad inquiry\n";

    make_transaction(del, str);
    return "All good. Your inquiry was completed\n";
}

std::string Password::func_edit(const std::string& str)
{
    std::string query = "UPDATE users SET password = $1 WHERE id = $2;";
    if(!conn->execute(query, {Hasher::make_hash(str), std::to_string(id)}))
        return "Bad inquiry\n";

    make_transaction(edit, str);
    return "All good. Your inquiry was completed\n";
}

void Password::make_transaction(action act, const std::string& str)
{
    static const std::unordered_map<action, std::string> commands = {
        {get, "get"},
        {add, "add"},
        {del, "delete"},
        {edit, "edit"}
    };

    auto it = commands.find(act);
    if(it == commands.end()){
        //error new-func
        Log::make_note("10002");
        return;
    }

    std::string query = "INSERT INTO transaction (name_resourse, name_action, time_appeal, ip_user, user_id) VALUES ($1, $2, NOW(), $3, $4);";
    if(!conn->execute(query, {str, it->second, addr, std::to_string(id)})){
        //critical error
        Log::make_note("402");
    }
}