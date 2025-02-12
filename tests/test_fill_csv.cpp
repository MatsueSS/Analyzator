#include <fstream>
#include <iostream>

std::ofstream generate_csv(const std::string& filename)
{
    return std::ofstream(filename);
}

void fill_file(std::ofstream& file)
{
    file << "Code" << "-" << "Value";
    file.close();
}

int main(void)
{
    std::string filepath = "../res/codes.csv";
    std::ofstream file = generate_csv(filepath);
    fill_file(file);

    return 1;
}