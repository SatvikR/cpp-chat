#include <netinet/in.h>
#include <set>
#include <string>

class Server
{
public:
    Server() = default;

    bool start(int port);

private:
    void client_pump(int client);
    void send_all(std::string message);

private:
    int m_server_fd;
    std::set<int> m_clients;

    static constexpr int backlog = 10;
    static constexpr int buffer_size = 2048;
};