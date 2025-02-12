#include "PostgresDB.h"

void create_table(PostgresDB* db)
{
    std::string query1 = "CREATE TABLE data (id BIGSERIAL PRIMARY KEY, resourse_name VARCHAR(64) NOT NULL, password VARCHAR(1024) NOT NULL, user_id BIGINT NOT NULL, CONSTRAINT fk_user FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE)";
    std::string query2 = "CREATE TABLE transaction (id BIGSERIAL PRIMARY KEY, name_resourse VARCHAR(64) NOT NULL, name_action VARCHAR(16) NOT NULL, time_appeal TIMESTAMP NOT NULL, ip_user VARCHAR(32) NOT NULL, user_id BIGINT NOT NULL, CONSTRAINT fk_user FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE)";
    std::string query3 = "CREATE TABLE users (id BIGSERIAL PRIMARY KEY, name_user VARCHAR(64) UNIQUE NOT NULL, password VARCHAR(128) NOT NULL, ip_registration VARCHAR(32) NOT NULL)";

    db->execute(query3, {});
    db->execute(query2, {});
    db->execute(query1, {});
}

int main(void)
{
    PostgresDB* db = new PostgresDB("dbname=loganalyzer user=matsuess password=731177889232 host=localhost port=5432");
    db->connect();
    create_table(db);
    db->disconnect();
    delete db;
    return 0;
}