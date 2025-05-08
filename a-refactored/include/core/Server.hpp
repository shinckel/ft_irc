#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <stdexcept>

#include "Socket.hpp"
#include "Manager.hpp"
#include "utils/Parser.hpp"
#include "utils/Error.hpp"

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
