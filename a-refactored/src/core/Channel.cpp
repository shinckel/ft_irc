#include "core/Channel.hpp"
#include <algorithm>
#include <iostream>

Channel::Channel(const std::string &name) : _name(name), _topic("") {
    std::cout << "Channel created: " << _name << std::endl;
}

Channel::~Channel() {
    std::cout << "Channel destroyed: " << _name << std::endl;
}

// Getters
const std::string &Channel::getName() const {
    return _name;
}

const std::string &Channel::getTopic() const {
    return _topic;
}

const std::vector<Client *> &Channel::getClients() const {
    return _clients;
}

// Setters
void Channel::setTopic(const std::string &topic) {
    _topic = topic;
    std::cout << "Topic for channel " << _name << " set to: " << _topic << std::endl;
}

// Client management
void Channel::addClient(Client *client) {
    if (isClientInChannel(client)) {
        std::cerr << getErrorMessage(Error::ERR_ALREADYREGISTRED) << std::endl;
        return;
    }
    _clients.push_back(client);
    std::cout << "Client " << client->getNickName() << " added to channel " << _name << std::endl;
}

void Channel::removeClient(Client *client) {
    std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        _clients.erase(it);
        std::cout << "Client " << client->getNickName() << " removed from channel " << _name << std::endl;
    } else {
        std::cerr << getErrorMessage(Error::ERR_NOTONCHANNEL) << std::endl;
    }
}

bool Channel::isClientInChannel(Client *client) const {
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

// Operator management
void Channel::addOperator(Client *client) {
    if (!isClientInChannel(client)) {
        std::cerr << getErrorMessage(Error::ERR_NOTONCHANNEL) << std::endl;
        return;
    }
    _operators[client->getNickName()] = client;
    std::cout << "Client " << client->getNickName() << " added as operator in channel " << _name << std::endl;
}

void Channel::removeOperator(Client *client) {
    std::map<std::string, Client *>::iterator it = _operators.find(client->getNickName());
    if (it != _operators.end()) {
        _operators.erase(it);
        std::cout << "Client " << client->getNickName() << " removed as operator in channel " << _name << std::endl;
    } else {
        std::cerr << getErrorMessage(Error::ERR_NOTONCHANNEL) << std::endl;
    }
}

bool Channel::isOperator(Client *client) const {
    return _operators.find(client->getNickName()) != _operators.end();
}

// Error handling
std::string Channel::getErrorMessage(int errorCode) const {
    return Error::getErrorMessage(errorCode);
}
