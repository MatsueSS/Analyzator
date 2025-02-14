#include "Edit.h"

int Edit::handle(const Client& obj, PostgresDB* db)
{
    if(!read_fd(obj.sockfd))
        return disconnect;

    std::string query = "UPDATE users SET password = $1 WHERE id = $2;";
    db->execute(query, {buf, std::to_string(id)});
    make_transaction(edit, id, buf, sock_ntop((sockaddr*)&obj.cliaddr), db);
    return SUCCESS;
}