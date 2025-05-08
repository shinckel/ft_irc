#include "core/Client.hpp"
#include <iostream>

Client::Client(int id) : _clientID(id), _nickName(""), _userName(""), _channel(""), _lastTriedNick("") {
    std::cout << "Client created with id: " << id << std::endl;
}

Client::~Client() {}

Client::Client(const Client &src) {
    *this = src;
}

Client &Client::operator=(const Client &src) {
    if (this != &src) {
        _clientID = src._clientID;
        _nickName = src._nickName;
        _userName = src._userName;
        _channel = src._channel;
        _lastTriedNick = src._lastTriedNick;
        _commands = src._commands;
    }
    return *this;
}

// Getters
int Client::getId() const {
    return _clientID;
}

std::string &Client::getNickName() {
    return _nickName;
}

std::string &Client::getLastNick() {
    return _lastTriedNick;
}

std::string &Client::getUserName() {
    return _userName;
}

std::string &Client::getChannel() {
    return _channel;
}

std::string Client::getClientPrefix() const {
    return _nickName + "!" + _userName + "@" + _hostname;
}

std::stringstream &Client::getBuffer() {
    return _buffer;
}

std::vector<std::string> &Client::getCommand() {
    return _commands;
}

// Setters
void Client::setNickName(const std::string &nickName) {
    _nickName = nickName;
}

void Client::setLastNick(const std::string &lastNick) {
    _lastTriedNick = lastNick;
}

void Client::setUserName(const std::string &userName) {
    _userName = userName;
}

void Client::setChannel(const std::string &channel) {
    _channel = channel;
}

void Client::setHostName(const std::string &hostname) {
    _hostname = hostname;
}
