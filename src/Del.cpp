#include "Del.h"
#include "Log.h"

int Del::handle(const Client& obj, std::unique_ptr<PostgresDB>& db)
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

    std::string query = "DELETE FROM data WHERE resourse_name = $1 AND id = $2;";
    make_transaction(del, obj.id, buf, sock_ntop((sockaddr *)&obj.cliaddr), db);
    return db->execute(query, {buf, std::to_string(obj.id)}) ? SUCCESS : not_exist;
}