#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "Manager.hpp"
#include "utils/Parser.hpp"
#include <string>

class Server {
private:
    Socket _socket;
    std::string _password;

    void handleNewConnection();
    void handleClientData(int clientFd);

public:
    Server(const std::string& port, const std::string& password);
    void start();
};

#endif
