#include <iostream>
#include "Server.h"

constexpr int PORT = 5005;

int main()
{
    Server server;
    server.start(PORT);
}
