#include "TCP_server.h"

int main(void)
{
    TCP_server tcp;
    tcp.socket();
    tcp.bind();
    tcp.listen();
    tcp.start();

    return 0;
}