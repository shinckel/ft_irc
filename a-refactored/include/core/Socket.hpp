#pragma once
#include <string>
#include <netinet/in.h>

class Socket {
public:
    Socket();
    ~Socket();

    void bind(int port);
    void listen(int backlog = 5);
    int accept();
    void close();

    int getSocketFd() const;

private:
    int _socketFd;
    struct sockaddr_in _address;

    void createSocket();
};