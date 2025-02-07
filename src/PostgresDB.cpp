#include "PostgresDB.h"

PostgresDB::PostgresDB(const std::string& connStr) : connStr(connStr), conn(nullptr) {}

bool PostgresDB::connect() 
{
    conn = PQconnectdb(connStr.c_str());
    if(PQstatus(conn) != CONNECTION_OK){
        return 0;
    }
    return 1;
}

void PostgresDB::disconnect()
{
    PQfinish(conn);
    conn = nullptr;
}

PostgresDB::~PostgresDB()
{
    disconnect();
}

bool PostgresDB::execute(const std::string& query, const std::vector<std::string>& vec)
{
    const char *param_values[vec.size()];
    for(size_t i = 0; i < vec.size(); i++)
        param_values[i] = vec[i].c_str();
    PGresult *res = PQexecParams(conn, query.c_str(), vec.size(), nullptr, 
                                 param_values, nullptr, nullptr, 0 );
    if(PQresultStatus(res) != PGRES_COMMAND_OK){
        return 0;
    }
    PQclear(res);
    return 1;
}

std::vector<std::vector<std::string>> PostgresDB::fetch(const std::string& query, const std::vector<std::string>& vec)
{
    const char *param_values[vec.size()];
    for(size_t i = 0; i < vec.size(); i++)
        param_values[i] = vec[i].c_str();

    std::vector<std::vector<std::string>> result;
    PGresult *res = PQexecParams(conn, query.c_str(), vec.size(), nullptr,
                                 param_values, nullptr, nullptr, 0);
    if(PQresultStatus(res) != PGRES_COMMAND_OK){
        PQclear(res);
        return result;
    }

    int rows = PQntuples(res);
    int columns = PQnfields(res);
    for(int i = 0 ; i < rows; i++){
        std::vector<std::string> temp;
        for(int j = 0; j < columns; j++){
            temp.push_back(PQgetvalue(res, i, j));
        }
        result.push_back(temp);
    }

    PQclear(res);
    return result;
}