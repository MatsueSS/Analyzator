#ifndef _POSTGRES_DB_H_
#define _POSTGRES_DB_H_

#include <vector>
#include <string>
#include <postgresql/libpq-fe.h>

class PostgresDB {
public:
    PostgresDB(const std::string& str);

    bool connect();
    void disconnect();
    std::vector<std::vector<std::string>> fetch(const std::string&, std::vector<std::string>&);
    bool execute(const std::string&, const std::vector<std::string>&);

    ~PostgresDB();

private:
    std::string connStr;
    PGconn *conn;
}

#endif //_POSTGRES_DB_H_