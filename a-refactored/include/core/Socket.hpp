#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "Manager.hpp"
#include "utils/Error.hpp"

#define BACKLOG 10
#define MAX_PORT 65535
#define MIN_PORT 1023

class Socket {
private:
    std::string _port;
    std::string _password;
    int _socketFd;

    void parsePortPass(const std::string& port, const std::string& password);
    struct addrinfo* setupSocketAddress();
    void bindAndListen(struct addrinfo* serverInfo);

public:
    Socket(const std::string& port, const std::string& password);
    ~Socket();

    int getSocketFd() const;
    const std::string& getPort() const;
    const std::string& getPassword() const;
    int acceptConnection() const;
};

#endif
