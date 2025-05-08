#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

#include "core/Client.hpp"
#include "utils/Error.hpp"

class Channel {
private:
    std::string _name;                          // Channel name
    std::string _topic;                         // Channel topic
    std::vector<Client *> _clients;             // List of clients in the channel
    std::map<std::string, Client *> _operators; // Map of operator nicknames to Client pointers

public:
    // Constructor and Destructor
    Channel() : _name(""), _topic("") {}
    Channel(const std::string &name);
    ~Channel() {}

    // Getters
    const std::string &getName() const;
    const std::string &getTopic() const;
    const std::vector<Client *> &getClients() const;

    // Setters
    void setTopic(const std::string &topic);

    // Client management
    void addClient(Client *client);
    void removeClient(Client *client);
    bool isClientInChannel(Client *client) const;

    // Operator management
    void addOperator(Client *client);
    void removeOperator(Client *client);
    bool isOperator(Client *client) const;

    // Error handling
    std::string getErrorMessage(int errorCode) const;
    std::vector<std::string> getNamesList() const;
};

#endif