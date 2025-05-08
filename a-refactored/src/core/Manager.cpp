#include "core/Manager.hpp"

// Static member initialization
std::string Manager::_password;
std::vector<Client> Manager::_clients;
std::map<std::string, Channel> Manager::_channels;
std::map<std::string, eventFunction> Manager::_actionMap;
std::string Manager::hostName = "localhost";

// Client management
void Manager::addClient(int id) {
    _clients.push_back(Client(id));
    std::cout << "Client " << id << " added to Clients" << std::endl;
}

void Manager::removeClient(int id) {
    for (size_t i = 0; i < _clients.size(); ++i) {
        if (_clients[i].getId() == id) {
            _clients.erase(_clients.begin() + i);
            std::cout << "Client " << id << " removed from Clients" << std::endl;
            break;
        }
    }
}

std::vector<Client> &Manager::getClients() {
    return _clients;
}

std::vector<Client>::iterator Manager::getClientByID(int id) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getId() == id) {
            return it;
        }
    }
    return _clients.end();
}

std::stringstream &Manager::getClientBuffer(int id) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getId() == id) {
            return it->getBuffer();
        }
    }
    throw std::runtime_error("Client not found");
}

// Channel management
std::map<std::string, Channel> &Manager::getChannels() {
    return _channels;
}

// Actions
void Manager::createMap() {
    // Example: Add actions to the map
    _actionMap["JOIN"] = [](Client &client) { /* Join action */ };
    _actionMap["NICK"] = [](Client &client) { /* Nick action */ };
}

void Manager::runActions(Client &client) {
    std::string cmd = client.getCommand()[0];
    std::map<std::string, eventFunction>::iterator it = _actionMap.find(cmd);
    if (it != _actionMap.end()) {
        it->second(client);
    } else {
        sendIrcMessage(client.getId(), formatMessage(client, "421") + " :Unknown command");
    }
}

// Utilities
std::string Manager::formatMessage(Client &client) {
    return ":" + client.getNickName() + "!" + client.getUserName() + "@" + hostName;
}

std::string Manager::formatMessage(Client &client, const std::string &message) {
    return ":" + hostName + " " + message + " " + client.getNickName();
}

int Manager::sendIrcMessage(int clientId, const std::string &message) {
    std::cout << "Sending message to client " << clientId << ": " << message << std::endl;
    return 0; // Simulate sending a message
}
