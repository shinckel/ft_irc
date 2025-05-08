#include "core/Client.hpp"
#include "utils/Parser.hpp"
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

Client::Client(int fd) : _fd(fd) {}

Client::~Client() {
    if (_fd >= 0) {
        close(_fd);
    }
}

int Client::getFd() const {
    return _fd;
}

std::string Client::receiveMessage() {
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = read(_fd, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        return "";
    }
    return std::string(buffer);
}

void Client::sendMessage(const std::string& message) {
    if (write(_fd, message.c_str(), message.size()) < 0) {
        throw std::runtime_error("Failed to send message");
    }
}

void Client::setRawCommand(const std::string& rawCommand) {
    _rawCommand = rawCommand;
    _commandTokens = Parser::split(rawCommand, ' '); // Use the Parser to split tokens
}

const std::string& Client::getRawCommand() const {
    return _rawCommand;
}

const std::vector<std::string>& Client::getCommandTokens() const {
    return _commandTokens;
}