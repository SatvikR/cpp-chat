#include "Client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>

bool Client::join(std::string host, int port)
{
    m_client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_client_fd < 0)
        return false;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
        return false;

    if (connect(m_client_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
        return false;

    std::thread thr(&Client::read_pump, this);
    thr.detach();

    write_pump();
    return true;
}

void Client::write_pump()
{
    while (true)
    {
        std::string message;
        std::getline(std::cin, message);

        send(m_client_fd, message.c_str(), message.length(), 0);
    }
}

void Client::read_pump()
{
    while (true)
    {
        char buffer[buffer_size] = {0};
        read(m_client_fd, buffer, buffer_size);

        std::string message(buffer);
        if (!message.length())
            return;

        std::cout << '\r' << message << std::endl
                  << std::flush;
    }
}