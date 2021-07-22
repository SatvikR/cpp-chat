#include <string>

class Client
{
public:
    Client() = default;

    bool join(std::string host, int port);

private:
    void read_pump();
    void write_pump();

private:
    int m_client_fd;

    static constexpr int buffer_size = 2048;
};