#include "Edit.h"
#include "Log.h"
#include "Hasher.h"

int Edit::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
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

    std::string query = "UPDATE users SET password = $1 WHERE id = $2;";
    db->execute(query, {Hasher::make_hash(buf), std::to_string(obj.id)});
    make_transaction(edit, obj.id, buf, sock_ntop((sockaddr*)&obj.cliaddr), db);
    return SUCCESS;
}