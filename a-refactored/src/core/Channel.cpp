#include "core/Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string& name) : _name(name) {}

const std::string& Channel::getName() const {
    return _name;
}

void Channel::addClient(Client* client) {
    _clients.push_back(client);
}

void Channel::removeClient(Client* client) {
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
}

const std::vector<Client*>& Channel::getClients() const {
    return _clients;
}

std::vector<int> Channel::getClientIDs() const {
    std::vector<int> clientIDs;
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        clientIDs.push_back((*it)->getFd());
    }
    return clientIDs;
}

// TOPIC COMMAND
const std::string& Channel::getTopic() const {
    return _topic;
}

void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

// WHO COMMAND
bool Channel::checkClient(int clientFd) const {
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() == clientFd) {
            return true;
        }
    }
    return false;
}

// PRIVMSG COMMAND
void Channel::clientMessage(int senderFd, const std::string& message) const {
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() != senderFd) {
            (*it)->sendMessage(message);
        }
    }
}

// FUNCTIONS RELATED TO MODE COMMAND IN CHANNEL
bool Channel::isOperator(int clientFd) const {
    for (std::vector<int>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (*it == clientFd) {
            return true;
        }
    }
    return false;
}

const std::string& Channel::getModes() const {
    static std::string modes;
    modes.clear();
    if (_modeI) modes += "i";
    if (_modeT) modes += "t";
    if (!_key.empty()) modes += "k";
    if (_limit > 0) modes += "l";
    return modes;
}

void Channel::setModeI(bool enabled) {
    _modeI = enabled;
}

void Channel::setModeT(bool enabled) {
    _modeT = enabled;
}

void Channel::setModeL(int limit) {
    _limit = limit;
}

void Channel::setKey(const std::string& key) {
    _key = key;
}

void Channel::addClientToOp(int clientFd) {
    if (std::find(_operators.begin(), _operators.end(), clientFd) == _operators.end()) {
        _operators.push_back(clientFd);
    }
}

void Channel::removeOp(int clientFd) {
    _operators.erase(std::remove(_operators.begin(), _operators.end(), clientFd), _operators.end());
}

// TODO: improve it, make it smaller
std::string Channel::applyModes(const std::vector<std::string>& modeArgs) {
    std::string result;

    for (std::vector<std::string>::const_iterator it = modeArgs.begin(); it != modeArgs.end(); ++it) {
        if ((*it)[0] == '+') {
            // Handle adding modes
            if ((*it)[1] == 'i') {
                setModeI(true);
                result += "+i ";
            } else if ((*it)[1] == 't') {
                setModeT(true);
                result += "+t ";
            } else if ((*it)[1] == 'k') {
                // Set the channel key
                std::string key = (it + 1 != modeArgs.end()) ? *(it + 1) : ""; // Get the next argument as the key
                setKey(key);
                result += "+k " + key + " ";
                ++it; // Skip the key argument
            } else if ((*it)[1] == 'o') {
                // Add a client to operators
                std::string nick = (it + 1 != modeArgs.end()) ? *(it + 1) : "";
                int target = Manager::getIDbyNick(nick); // Assuming Manager::getIDbyNick is accessible
                addClientToOp(target);
                result += "+o " + nick + " ";
                ++it; // Skip the nick argument
            } else if ((*it)[1] == 'l') {
                // Set the user limit
                std::string limitStr = (it + 1 != modeArgs.end()) ? *(it + 1) : "0";
                int limit = atoi(limitStr.c_str());
                setModeL(limit);
                result += "+l " + limitStr + " ";
                ++it; // Skip the limit argument
            }
        } else if ((*it)[0] == '-') {
            // Handle removing modes
            if ((*it)[1] == 'i') {
                setModeI(false);
                result += "-i ";
            } else if ((*it)[1] == 't') {
                setModeT(false);
                result += "-t ";
            } else if ((*it)[1] == 'k') {
                // Remove the channel key
                setKey("");
                result += "-k ";
            } else if ((*it)[1] == 'o') {
                // Remove a client from operators
                std::string nick = (it + 1 != modeArgs.end()) ? *(it + 1) : "";
                int target = Manager::getIDbyNick(nick); // Assuming Manager::getIDbyNick is accessible
                removeOp(target);
                result += "-o " + nick + " ";
                ++it; // Skip the nick argument
            } else if ((*it)[1] == 'l') {
                // Remove the user limit
                setModeL(0);
                result += "-l ";
            }
        }
    }

    return result;
}
