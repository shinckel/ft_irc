#include "core/Socket.hpp"
#include <stdexcept>
#include <cstring>
#include <unistd.h>

Socket::Socket() : _socketFd(-1) {
    createSocket();
}

Socket::~Socket() {
    close();
}

void Socket::createSocket() {
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
}

void Socket::bind(int port) {
    std::memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(port);

    if (::bind(_socketFd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }
}

void Socket::listen(int backlog) {
    if (::listen(_socketFd, backlog) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

int Socket::accept() {
    int clientFd = ::accept(_socketFd, NULL, NULL);
    if (clientFd < 0) {
        throw std::runtime_error("Failed to accept connection");
    }
    return clientFd;
}

void Socket::close() {
    if (_socketFd >= 0) {
        ::close(_socketFd);
        _socketFd = -1;
    }
}

int Socket::getSocketFd() const {
    return _socketFd;
}