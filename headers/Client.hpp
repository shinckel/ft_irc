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

#include "Socket.hpp"
#include <sstream>
#include <vector>
#include <string>

// Represents a connected client
// Each connected client is represented by an instance of the Client class
// When a client connects to the server, the accept() call in the Socket class returns a new file descriptor (client_fd) for the client
// This file descriptor is used to communicate with the client
class Client {
private:
    std::stringstream           _buffer;            // buffer for client communication
    int                         _id;                // unique client ID
    std::string					_nickname;          // client's nickname
    std::string					_lastTriedNickname; // last attempted nickname
    std::string                 _username;          // client's username
    std::vector<std::string>    _commands;          // parsed commands
    std::string                 _channel;           // current channel
    std::string                 _hostname;          // client's hostname

public:
    Client() = default;
    explicit Client(int id);
    Client(const Client &src);
    Client &operator=(const Client &src);
    ~Client();

    std::string                 &getChannel();
    std::string                 &getNickname();
    std::string                 &getUsername();
    int                         getId() const;
    std::stringstream           &getBuffer();
    std::string                 &getLastTriedNickname();
    std::string                 getClientPrefix() const;
    std::vector<std::string>    getCommands() const;

    void    setChannel(const std::string &channel);
    void    setNickname(const std::string &nickname);
    void    setLastTriedNickname(const std::string &lastNickname);
    void    setUsername(const std::string &username);
    void    setHostname(const std::string &hostname);
    void    setCommand(const std::string &command);
    void    setRegularCommand(const std::string &command);
};

#endif
