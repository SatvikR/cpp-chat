#include "Client.h"

constexpr auto HOST = "127.0.0.1";
constexpr int PORT = 5005;

int main()
{
    Client client;
    client.join(HOST, PORT);
}