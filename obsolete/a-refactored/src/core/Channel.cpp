// #include "core/Channel.hpp"

// Channel::Channel(const std::string &name) : _name(name), _topic("") {
//     std::cout << "Channel created: " << _name << std::endl;
// }

// Channel::~Channel() {
//     std::cout << "Channel destroyed: " << _name << std::endl;
// }

// // Getters
// const std::string &Channel::getName() const {
//     return _name;
// }

// const std::string &Channel::getTopic() const {
//     return _topic;
// }

// const std::vector<Client *> &Channel::getClients() const {
//     return _clients;
// }

// // Setters
// void Channel::setTopic(const std::string &topic) {
//     _topic = topic;
//     std::cout << "Topic for channel " << _name << " set to: " << _topic << std::endl;
// }

// // Client management
// void Channel::addClient(Client *client) {
//     if (isClientInChannel(client)) {
//         std::cerr << "Error: Client " << client->getNickName() << " is already in channel " << _name << std::endl;
//         return;
//     }
//     _clients.push_back(client);
//     std::cout << "Client " << client->getNickName() << " added to channel " << _name << std::endl;
// }

// void Channel::removeClient(Client *client) {
//     std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
//     if (it != _clients.end()) {
//         _clients.erase(it);
//         std::cout << "Client " << client->getNickName() << " removed from channel " << _name << std::endl;
//     } else {
//         std::cerr << "Error: Client " << client->getNickName() << " is not in channel " << _name << std::endl;
//     }
// }

// bool Channel::isClientInChannel(Client *client) const {
//     return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
// }

// // Operator management
// void Channel::addOperator(Client *client) {
//     if (!isClientInChannel(client)) {
//         std::cerr << "Error: Client " << client->getNickName() << " is not in channel " << _name << std::endl;
//         return;
//     }
//     _operators[client->getNickName()] = client;
//     std::cout << "Client " << client->getNickName() << " added as operator in channel " << _name << std::endl;
// }

// void Channel::removeOperator(Client *client) {
//     std::map<std::string, Client *>::iterator it = _operators.find(client->getNickName());
//     if (it != _operators.end()) {
//         _operators.erase(it);
//         std::cout << "Client " << client->getNickName() << " removed as operator in channel " << _name << std::endl;
//     } else {
//         std::cerr << "Error: Client " << client->getNickName() << " is not an operator in channel " << _name << std::endl;
//     }
// }

// bool Channel::isOperator(Client *client) const {
//     return _operators.find(client->getNickName()) != _operators.end();
// }

// // Error handling
// std::string Channel::getErrorMessage(int errorCode) const {
//     return Error::getErrorMessage(errorCode);
// }

#include "core/Channel.hpp"
#include <algorithm>
#include <iostream>

Channel::Channel(const std::string &name) : _name(name), _topic("") {
    std::cout << "Channel created: " << _name << std::endl;
}

const std::string &Channel::getName() const {
    return _name;
}

const std::string &Channel::getTopic() const {
    return _topic;
}

const std::vector<Client *> &Channel::getClients() const {
    return _clients;
}

void Channel::setTopic(const std::string &topic) {
    _topic = topic;
    std::cout << "Topic for channel " << _name << " set to: " << _topic << std::endl;
}

// void Channel::addClient(Client *client) {
//     if (isClientInChannel(client)) {
//         std::cerr << "Error: Client " << client->getNickName() << " is already in channel " << _name << std::endl;
//         return;
//     }
//     _clients.push_back(client);
//     std::cout << "Client " << client->getNickName() << " added to channel " << _name << std::endl;
// }
void Channel::addClient(Client *client) {
    std::cout << "Adding client to channel: " << _name << std::endl;
    std::cout << "Client nickname: " << client->getNickName() << std::endl;

    if (isClientInChannel(client)) {
        std::cerr << "Error: Client " << client->getNickName() << " is already in channel " << _name << std::endl;
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
        std::cerr << "Error: Client " << client->getNickName() << " is not in channel " << _name << std::endl;
    }
}

bool Channel::isClientInChannel(Client *client) const {
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

std::vector<std::string> Channel::getNamesList() const {
    std::vector<std::string> namesList;
    for (size_t i = 0; i < _clients.size(); ++i) {
        namesList.push_back(_clients[i]->getNickName());
    }
    return namesList;
}

std::string Channel::getUserList() const {
    std::ostringstream userListStream;

    for (size_t i = 0; i < _clients.size(); ++i) {
        userListStream << _clients[i]->getNickName();
        if (i != _clients.size() - 1) {
            userListStream << " "; // Add a space between nicknames
        }
    }

    return userListStream.str();
}
