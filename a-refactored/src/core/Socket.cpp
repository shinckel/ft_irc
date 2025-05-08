#include "core/Socket.hpp"
#include <iostream>
#include <cstring> // For memset

Socket::Socket()
{
    // Create the socket
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
    {
        throw std::runtime_error("Failed to create socket");
    }

    // Allow address reuse
    int opt = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        throw std::runtime_error("Failed to set socket options");
    }

    // Initialize the address structure
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
}

Socket::~Socket()
{
    close();
}

void Socket::bind(int port)
{
    _addr.sin_port = htons(port); // Convert port to network byte order
    if (::bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        throw std::runtime_error("Failed to bind socket");
    }
}

void Socket::listen()
{
    if (::listen(_fd, SOMAXCONN) == -1)
    {
        throw std::runtime_error("Failed to listen on socket");
    }
}

int Socket::accept()
{
    int clientFd = ::accept(_fd, NULL, NULL);
    if (clientFd == -1)
    {
        throw std::runtime_error("Failed to accept connection");
    }
    return clientFd;
}

int Socket::getFd() const
{
    return _fd;
}

void Socket::close()
{
    if (_fd != -1)
    {
        ::close(_fd);
        _fd = -1;
    }
}
