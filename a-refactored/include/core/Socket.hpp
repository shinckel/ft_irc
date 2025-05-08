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

#define BACKLOG 10
#define MAX_PORT 65535
#define MIN_PORT 1023

class Socket {
private:
    std::string _port;
    std::string _password; // Add password as a private member
    int _socketFd;

    void parsePort(const std::string& port);
    struct addrinfo* setupSocketAddress();
    void bindAndListen(struct addrinfo* serverInfo);

public:
    Socket(const std::string& port, const std::string& password); // Update constructor
    ~Socket();

    int getSocketFd() const;
    const std::string& getPassword() const; // Add a getter for the password
    int acceptConnection() const;
};

#endif
