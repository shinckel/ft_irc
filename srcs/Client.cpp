/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:00:00 by shinckel          #+#    #+#             */
/*   Updated: 2025/04/08 13:30:00 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"

Client::Client(int id) : _id(id), _nickname(""), _username(""), _channel(""), _lastTriedNickname("") {
    std::cout << "Client created with ID: " << id << std::endl;
}

Client::~Client() {}

Client &Client::operator=(const Client &src) {
    if (this != &src) {
        _id = src._id;
        _nickname = src._nickname;
        _username = src._username;
        _channel = src._channel;
        _lastTriedNickname = src._lastTriedNickname;
        _commands = src._commands;
    }
    return *this;
}

Client::Client(const Client &src) {
    *this = src;
}

// Getters
int Client::getId() const {
    return _id;
}

std::stringstream &Client::getBuffer() {
    return _buffer;
}

std::string &Client::getNickname() {
    return _nickname;
}

std::string &Client::getUsername() {
    return _username;
}

std::vector<std::string> Client::getCommands() const {
    return _commands;
}

std::string &Client::getChannel() {
    return _channel;
}

std::string &Client::getLastTriedNickname() {
    return _lastTriedNickname;
}

std::string Client::getClientPrefix() const {
    return _nickname + "!" + _username + "@" + _hostname;
}

// Setters
void Client::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

void Client::setUsername(const std::string &username) {
    _username = username;
}

void Client::setHostname(const std::string &hostname) {
    _hostname = hostname;
}

void Client::setLastTriedNickname(const std::string &lastNickname) {
    _lastTriedNickname = lastNickname;
}

void Client::setCommand(const std::string &command) {
    _commands.clear();
    std::string cmd = command;

    if (!cmd.empty() && cmd[0] == '/') {
        cmd.erase(0, 1); // remove leading '/'
    }

    size_t spacePos = cmd.find(' ');
    if (spacePos != std::string::npos) {
        std::string commandName = cmd.substr(0, spacePos);
        std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper); // convert to uppercase
        _commands.push_back(commandName);
        _commands.push_back(cmd.substr(spacePos + 1));
    } else {
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper); // convert to uppercase
        _commands.push_back(cmd);
    }
}

void Client::setRegularCommand(const std::string &command) {
    _commands.clear();
    std::string cmd = command;

    if (!cmd.empty() && cmd[0] == '/') {
        cmd.erase(0, 1); // remove leading '/'
    }

    _commands.push_back(cmd);
}

void Client::setChannel(const std::string &channel) {
    _channel = channel;
}
