#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <netinet/in.h> // For sockaddr_in
#include <sys/socket.h> // For socket functions
#include <unistd.h>     // For close()

class Socket
{
private:
    int _fd;                  // File descriptor for the socket
    struct sockaddr_in _addr; // Address structure

public:
    Socket();
    ~Socket();

    void bind(int port);
    void listen();
    int accept();
    int getFd() const;
    void close();
};

#endif