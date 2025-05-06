/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:45:42 by shinckel          #+#    #+#             */
/*   Updated: 2025/04/23 17:45:42 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

#include "Socket.hpp"

// client as the connected entity
// each connected client is represented by an instance of the Client class
// when a client connects to the server, the accept() call in the Socket class returns a new file descriptor (client_fd) for the client
// this file descriptor is used to communicate with the client
class Client {
private:
    int _fd;                // file descriptor for the client socket
    std::string _ip;        // IP address of the client
    int _port;              // port number of the client

public:
    Client(int fd, const std::string& ip, int port);
    ~Client();

    // getters
    int getFd() const;
    std::string getIp() const;
    int getPort() const;

    // communication methods
    void sendMessage(const std::string& message) const;
    std::string receiveMessage() const;
};

#endif
