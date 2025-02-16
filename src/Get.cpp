#include "Get.h"
#include "Log.h"

#include <unistd.h>

int Get::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    if(!db->is_connect()){
        Log::make_note("2001");
        if(!db->connect()){
            Log::make_note("202");
            return disconnect;
        }
    }
    
    std::string query = "SELECT * FROM data WHERE resourse_name = $1 AND id = $2";
    std::vector<std::vector<std::string>> result = db->fetch(query, {buf, std::to_string(obj.id)});
    if(result.empty())
        return not_exist;

    make_transaction(get, obj.id, buf, sock_ntop((sockaddr *)&obj.cliaddr), db);
    write(obj.sockfd, result[0][2].c_str(), result[0][2].size());

    return SUCCESS;
}