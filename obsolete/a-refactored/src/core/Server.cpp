#include "core/Server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cerrno>

Server::Server(const std::string& port, const std::string& password)
    : _socket(port, password), _password(password) {}

void Server::start() {
    std::cout << "Server started on port " << _socket.getSocketFd() << std::endl;

    // Add the server socket to the pollfd array
    struct pollfd serverPollfd;
    serverPollfd.fd = _socket.getSocketFd();
    serverPollfd.events = POLLIN; // Monitor for incoming connections
    _pollfds.push_back(serverPollfd);

    while (true) {
        // Use poll() to monitor file descriptors
        int pollCount = poll(_pollfds.data(), _pollfds.size(), -1); // -1 means no timeout
        if (pollCount == -1) {
            std::cerr << "poll() error: " << strerror(errno) << std::endl;
            continue;
        }

        for (size_t i = 0; i < _pollfds.size(); ++i) {
            if (_pollfds[i].revents & POLLIN) { // Check if there's data to read
                if (_pollfds[i].fd == _socket.getSocketFd()) {
                    // New connection
                    handleNewConnection();
                } else {
                    // Existing client sent a message
                    handleClientData(_pollfds[i].fd);
                }
            } else if (_pollfds[i].revents & (POLLHUP | POLLERR)) {
                // Client disconnected or error occurred
                std::cout << "Client disconnected: " << _pollfds[i].fd << std::endl;
                close(_pollfds[i].fd);
                Manager::removeClient(_pollfds[i].fd);
                updatePollfds(); // Update the pollfd array
            }
        }
    }
}

void Server::handleNewConnection() {
    try {
        int clientFd = _socket.acceptConnection();
        Manager::addClient(clientFd);
        std::cout << "New client connected: " << clientFd << std::endl;

        // Add the new client to the pollfd array
        struct pollfd clientPollfd;
        clientPollfd.fd = clientFd;
        clientPollfd.events = POLLIN;
        _pollfds.push_back(clientPollfd);

        // Send a welcome message
        std::stringstream ss;
        ss << clientFd;
        std::string welcomeMessage = ":localhost 001 " + ss.str() + " :Welcome to the IRC server!";
        send(clientFd, welcomeMessage.c_str(), welcomeMessage.size(), 0);
        std::cout << "Sent welcome message to client " << clientFd << std::endl;
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
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cerr << "No data available to read from client " << clientFd << " (EAGAIN/EWOULDBLOCK)." << std::endl;
                return;
            } else {
                std::cerr << "Error reading from client " << clientFd << ": " << strerror(errno) << std::endl;
            }
            close(clientFd);
            Manager::removeClient(clientFd);

            // Remove the client from the pollfd array
            for (size_t i = 0; i < _pollfds.size(); ++i) {
                if (_pollfds[i].fd == clientFd) {
                    _pollfds.erase(_pollfds.begin() + i);
                    break;
                }
            }
        } else {
            buffer[bytesRead] = '\0';
            std::cout << "Received data from client " << clientFd << ": " << buffer << std::endl;

            // Check if the client exists
            Client* client = Manager::getClientByID(clientFd);
            if (client) {
                Parser::processClientMessage(*client, buffer);
                Manager::runActions(*client);
            } else {
                std::cerr << "Error: Client with FD " << clientFd << " does not exist." << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error handling client data: " << e.what() << std::endl;
    }
}

void Server::updatePollfds() {
    _pollfds.clear();

    // Add the server socket
    struct pollfd serverPollfd;
    serverPollfd.fd = _socket.getSocketFd();
    serverPollfd.events = POLLIN;
    _pollfds.push_back(serverPollfd);

    // Add client sockets
    const std::vector<Client> &clients = Manager::getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getId() > 0) { // Ensure valid file descriptor
            struct pollfd clientPollfd;
            clientPollfd.fd = clients[i].getId();
            clientPollfd.events = POLLIN;
            _pollfds.push_back(clientPollfd);
        }
    }

    // Debugging log
    std::cout << "Updated pollfds: ";
    for (size_t i = 0; i < _pollfds.size(); ++i) {
        std::cout << _pollfds[i].fd << " ";
    }
    std::cout << std::endl;
}
