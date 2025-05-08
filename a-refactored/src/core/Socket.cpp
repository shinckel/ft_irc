#include "core/Socket.hpp"

Socket::Socket(const std::string& port, const std::string& password) : _port(port), _password(password) {
    try {
        parsePortPass(port, password);
        Manager::setPassword(password);

        struct addrinfo* serverInfo = setupSocketAddress();
        Error::printServerInfo(serverInfo); // debugging

        bindAndListen(serverInfo);
        freeaddrinfo(serverInfo);

        std::cout << "Socket initialized on port " << _port << " with password: " << _password << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Socket initialization error: " << e.what() << std::endl;
        throw;
    }
}

Socket::~Socket() {
    close(_socketFd);
    std::cout << "Socket closed" << std::endl;
}

void Socket::parsePortPass(const std::string& port, const std::string& password) {
    int portNum = atoi(port.c_str());
    if (port.length() <= 0 || portNum <= MIN_PORT || portNum > MAX_PORT) {
        throw std::runtime_error("Invalid port number. Must be between 1024 and 65535.");
    }

    if (password.length() == 0 || password.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::runtime_error("Invalid password: Password cannot be empty or whitespace only");
    }
}

struct addrinfo* Socket::setupSocketAddress() {
    struct addrinfo hints, *serverInfo;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, _port.c_str(), &hints, &serverInfo) != 0) {
        throw std::runtime_error("Failed to get address info.");
    }
    return serverInfo;
}

void Socket::bindAndListen(struct addrinfo* serverInfo) {
    struct addrinfo* p;
    int yes = 1;

    for (p = serverInfo; p != NULL; p = p->ai_next) {
        _socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_socketFd < 0) {
            continue;
        }

        setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(_socketFd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }

        close(_socketFd);
    }

    if (p == NULL) {
        throw std::runtime_error("Failed to bind socket.");
    }

    if (listen(_socketFd, BACKLOG) < 0) {
        throw std::runtime_error("Failed to listen on socket.");
    }
}

int Socket::getSocketFd() const {
    return _socketFd;
}

const std::string& Socket::getPort() const {
    return _port;
}

const std::string& Socket::getPassword() const {
    return _password;
}

int Socket::acceptConnection() const {
    struct sockaddr_storage clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientFd = accept(_socketFd, (struct sockaddr*)&clientAddr, &addrLen);

    if (clientFd < 0) {
        throw std::runtime_error("Failed to accept connection.");
    }

    return clientFd;
}
