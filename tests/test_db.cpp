#include "PostgresDB.h"

#include <iostream>

template<class Iter>
void print(Iter it1, Iter it2){
    auto it11 = it1->begin();
    while(it1 != it2){
        while(it11 != it1->end())
            std::cout << *it11++ << ' ';
        std::cout << std::endl;
        it1++;
    }
}

int main(void)
{
    std::string connStr = "dbname=loganalyzer user=matsuess password=731177889232 host=localhost port=5432";
    PostgresDB db(connStr);

    std::cout << "Connect with exist db: " << db.connect() << std::endl;

    std::string example_query_true_1 = "UPDATE users SET name_user = $1 WHERE id = $2";
    std::string example_query_false_1 = "UPDATE users SET name = $1 WHERE id = $2";

    std::string example_query_true_2 = "SELECT * FROM users WHERE id = $1;";
    std::string example_query_false_2 = "SELECT * FROM users WHERE id = $1;";

    std::cout << "Result execute with true query: " << db.execute(example_query_true_1, {"matsu", "2"}) << std::endl;
    std::cout << "Result execute with false query: " << db.execute(example_query_false_1, {"ttt", "0"}) << std::endl;

    std::vector<std::vector<std::string>> result;
    
    result = db.fetch(example_query_true_2, {"2"});
    std::cout << "Result fetch with true query: ";
    print(result.begin(), result.end());

    result.clear();
    result = db.fetch(example_query_false_2, {"1"});
    std::cout << "Result fetch with false query: ";
    if(!result.empty())
        print(result.begin(), result.end());

    return 0;
}