#include "Del.h"

int Del::handle(const Client& obj, PostgresDB* db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    std::string query = "DELETE FROM data WHERE resourse_name = $1 AND id = $2;";
    make_transaction(del, id, buf, sock_ntop((sockaddr *)&obj.cliaddr), db);
    return db->execute(query, {buf, std::to_string(id)}) ? SUCCESS : not_exist;
}