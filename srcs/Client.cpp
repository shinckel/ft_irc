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

Client::Client(int fd, const std::string& ip, int port)
    : _fd(fd), _ip(ip), _port(port) {
    LOG("New client connected: " + ip + ":" + std::to_string(port));
}

Client::~Client() {
    LOG("Client disconnected: " + _ip + ":" + std::to_string(_port));
    close(_fd);
}

int Client::getFd() const {
    return _fd;
}

std::string Client::getIp() const {
    return _ip;
}

int Client::getPort() const {
    return _port;
}

void Client::sendMessage(const std::string& message) const {
    if (send(_fd, message.c_str(), message.size(), 0) == -1) {
        std::cerr << "Failed to send message to client: " << _ip << std::endl;
    }
}

std::string Client::receiveMessage() const {
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        return "";
    }
    return std::string(buffer, bytesReceived);
}
