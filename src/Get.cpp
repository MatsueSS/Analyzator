#include "Get.h"

#include <unistd.h>

int Get::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;
    
    std::string query = "SELECT * FROM data WHERE resourse_name = $1 AND id = $2";
    std::vector<std::vector<std::string>> result = db->fetch(query, {buf, std::to_string(id)});
    if(result.empty())
        return not_exist;

    write(obj.sockfd, result[0][2].c_str(), result[0][2].size());

    return SUCCESS;
}