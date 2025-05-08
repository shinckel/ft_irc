#include "core/Server.hpp"
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <algorithm>

Server::Server(int port) : _running(false) {
    _socket.bind(port);
    _socket.listen();
}

Server::~Server() {
    stop();
}

void Server::start() {
    _running = true;
    std::cout << "Server started and listening for connections..." << std::endl;

    while (_running) {
        try {
            handleNewConnection();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void Server::stop() {
    _running = false;
    _socket.close();
    std::cout << "Server stopped." << std::endl;
}

void Server::handleNewConnection() {
    int clientFd = _socket.accept();
    Client* newClient = new Client(clientFd);
    _clients.push_back(newClient);
    std::cout << "New client connected: " << clientFd << std::endl;
}

void Server::handleClientMessage(Client* client) {
    std::string message = client->receiveMessage();
    if (message.empty()) {
        removeClient(client);
        return;
    }
    std::cout << "Message from client " << client->getFd() << ": " << message << std::endl;
    // Process the message (e.g., dispatch commands)
}

void Server::removeClient(Client* client) {
    std::cout << "Client disconnected: " << client->getFd() << std::endl;
    close(client->getFd());
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
    delete client;
}

std::vector<int> Server::getAllClientIDs() const {
    std::vector<int> clientIDs;
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        clientIDs.push_back((*it)->getFd());
    }
    return clientIDs;
}

Client* Server::getClientByID(int clientID) const {
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() == clientID) {
            return *it;
        }
    }
    return NULL;
}

Channel* Server::getChannelByName(const std::string& channelName) const {
    for (std::vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if ((*it)->getName() == channelName) {
            return *it;
        }
    }
    return NULL;
}

bool Server::isClientOpInChannel(int clientID, const std::string& channelName) const {
    Channel* channel = getChannelByName(channelName);
    if (channel == NULL) {
        return false;
    }
    return channel->isOperator(clientID);
}
