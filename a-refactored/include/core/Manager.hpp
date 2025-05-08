#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"

typedef void (*eventFunction)(Client &client);

class Manager {
private:
    static std::string _password;
    static std::vector<Client> _clients;
    static std::map<std::string, Channel> _channels;
    static std::map<std::string, eventFunction> _actionMap;

public:
    static std::string hostName;

    // Client management
    static void addClient(int id);
    static void removeClient(int id);
    static std::vector<Client> &getClients();
    static std::vector<Client>::iterator getClientByID(int id);
    static std::stringstream &getClientBuffer(int id);

    // Channel management
    static std::map<std::string, Channel> &getChannels();

    // Actions
    static void createMap();
    static void runActions(Client &client);

    // Utilities
    static std::string formatMessage(Client &client);
    static std::string formatMessage(Client &client, const std::string &message);
    static int sendIrcMessage(int clientId, const std::string &message);
};

#endif
