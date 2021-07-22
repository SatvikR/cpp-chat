#include "Server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <arpa/inet.h>

bool Server::start(int port)
{
    m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_fd == 0)
        return false;
    int opt = 1;
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) < 0)
    {
        return false;
    }

    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(m_server_fd, (sockaddr *)&addr, sizeof(addr)))
        return false;

    if (listen(m_server_fd, backlog) < 0)
        return false;

    int addrlen = sizeof(addr);
    std::cout << "listening for connections" << std::endl;
    while (true)
    {
        int new_socket = accept(m_server_fd, (sockaddr *)&addr, (socklen_t *)&addrlen);
        if (new_socket < 0)
            return false;
        std::thread thr(&Server::client_pump, this, new_socket);
        thr.detach();
        m_clients.insert(new_socket);
    }
    return true;
}

static std::string build_message(std::string ip, std::string message)
{
    std::stringstream msg;
    msg << "<" << ip << "> " << message << std::endl;

    return msg.str();
}

void Server::client_pump(int client)
{
    sockaddr_in c_addr;
    socklen_t len = sizeof(c_addr);

    bzero(&c_addr, sizeof(c_addr));
    getsockname(client, (sockaddr *)&c_addr, &len);

    char ip_buffer[16] = {0};
    inet_ntop(AF_INET, &c_addr.sin_addr, ip_buffer, sizeof(ip_buffer));

    std::string ip(ip_buffer);

    while (true)
    {
        char buffer[buffer_size] = {0};
        read(client, buffer, buffer_size);

        std::string message(buffer);

        if (!message.length())
        {
            send_all(build_message(ip, "disconnected"));
            return;
        }
        send_all(build_message(ip, message));
    }
}

void Server::send_all(std::string message)
{
    for (auto &client_fd : m_clients)
        send(client_fd, message.c_str(), message.length(), 0);
}