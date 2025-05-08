#include "core/Server.hpp"
#include "utils/Error.hpp"
#include <stdexcept>

#include "core/Server.hpp"
#include <iostream>

Server::Server(const std::string& port, const std::string& password)
    : _socket(port, password), _password(password) {}

void Server::start() {
    std::cout << "Server started on port " << _socket.getPort() << std::endl;

    while (true) {
        try {
            handleNewConnection();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void Server::handleNewConnection() {
    try {
        int clientFd = _socket.acceptConnection();
        Manager::addClient(clientFd);
        std::cout << "New client connected: " << clientFd << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error accepting new connection: " << e.what() << std::endl;
    }
}

void Server::handleClientData(int clientFd) {
    try {
        char buffer[1024];
        int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cout << "Client disconnected: " << clientFd << std::endl;
            } else {
                std::cerr << "Error reading from client: " << clientFd << std::endl;
            }
            close(clientFd);
            Manager::removeClient(clientFd);
        } else {
            buffer[bytesRead] = '\0';
            Client &client = *Manager::getClientByID(clientFd);
            Parser::processClientMessage(client, buffer); // Use Parser to process the message
            Manager::runActions(client); // Execute actions based on the parsed message
        }
    } catch (const std::exception &e) {
        std::cerr << "Error handling client data: " << e.what() << std::endl;
    }
}
