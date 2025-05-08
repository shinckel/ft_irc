#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "Manager.hpp"
#include "utils/Parser.hpp"
#include <vector>
#include <poll.h> // For poll()

class Server {
private:
    Socket _socket;
    std::string _password;
    std::vector<struct pollfd> _pollfds; // Array of pollfd structures

    void handleNewConnection();
    void handleClientData(int clientFd);
    void updatePollfds();

public:
    Server(const std::string& port, const std::string& password);
    void start();
};

#endif