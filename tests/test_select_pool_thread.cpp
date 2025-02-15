#include "new_TCP_server.h"

int main(void)
{
    new_TCP_server server;
    server.socket();
    server.bind();
    server.listen();
    server.start();
}