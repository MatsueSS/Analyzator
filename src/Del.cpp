#include "Del.h"

int Del::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    std::string query = "DELETE FROM data WHERE resourse_name = $1 AND id = $2;";
    make_transaction(del, obj.id, buf, sock_ntop((sockaddr *)&obj.cliaddr), db);
    return db->execute(query, {buf, std::to_string(obj.id)}) ? SUCCESS : not_exist;
}