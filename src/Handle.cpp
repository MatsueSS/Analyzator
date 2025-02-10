#include "Handle.h"
#include "Log.h"

#include <unistd.h>

bool Handle::read_fd(int sockfd)
{
    if((n = read(sockfd, buf, MAXLINE-1)) > 0){
        if(buf[n-1] == '\n')
            buf[n-1] = 0;
        else
            buf[n] = 0;
        return 1;
    }
    if(errno == ECONNRESET){
        Log::make_note("1003");
    }
    return 0;
}

void Handle::write_str(const std::string& str, int sockfd) const
{
    write(sockfd, str.c_str(), str.size());
}