#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <sstream>

#define PORT 8341
#define MAXLINE 1024

using namespace std;

int my_connect(const string& addr)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, addr.c_str(), &cliaddr.sin_addr);

    connect(sockfd, (sockaddr *)&cliaddr, sizeof(cliaddr));

    return sockfd;
}

void work(int sockfd, const vector<string>& arr, ofstream& file)
{
    char buf[1024];
    ssize_t n;
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(sockfd, &readset);
    for(int i = 0; i < arr.size()+1; i++){
        fd_set master = readset;
        if(select(sockfd+1, &master, nullptr, nullptr, nullptr) > 0){
            if(FD_ISSET(sockfd, &master)){
                if((n = read(sockfd, buf, MAXLINE)) > 0){
                    string str = string(buf, 0, n);
                    std::cout << str << '\n';
                    file << str << '\n';
                }
                else{
                    file << "server was closed\n";
                    return;
                }
            }
            if(i == arr.size())
                continue;
            write(sockfd, arr[i].c_str(), arr[i].size());
            std::cout << arr[i];
        }
    }
}

int main()
{
    ofstream file("client_result.txt");
    ifstream infile("input_data.txt");
    string str;
    int i = 0;
    while(getline(infile, str)){
        int sockfd = my_connect("127.0.0.1");
        string temp, action, name, pass;
        istringstream ss(str);
        ss >> temp >> action >> name >> pass;
        vector<string> arr = {action+'\n', name + ' ' + pass+'\n'};
        file << "---------- TEST " << ++i << " ----------\n";
        string command;
        ss >> command;
        if(command == ""){
            std::cout << arr[0] << arr[1] << '\n';
            work(sockfd, arr, file);
            close(sockfd);
            continue;
        }
        string res_name, res_pass;
        ss >> res_name >> res_pass;
        arr.push_back(command+'\n');
        if(!res_pass.empty() && !res_name.empty())
            arr.push_back(res_name+' '+res_pass+'\n');
        else if(res_pass == "")
            arr.push_back(res_name+'\n');
        else if(res_name == "")
            arr.push_back(res_pass+'\n');
        work(sockfd, arr, file);
        close(sockfd);
    }
    

    file.close();
    infile.close();

}