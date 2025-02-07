#include "PostgresDB.h"

#include <iostream>

int main(void)
{
    std::string connStr = "dbname=loganalyzer user=matsuess password=731177889232 host=localhost port=5432";
    PostgresDB db(connStr);

    std::cout << db.connect();
    
}