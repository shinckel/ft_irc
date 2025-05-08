#pragma once
#include "Socket.hpp"
#include "Client.hpp"
#include <vector>
#include <string>

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();

private:
    Socket _socket;
    bool _running;
    std::vector<Client*> _clients;

    void handleNewConnection();
    void handleClientMessage(Client* client);
    void removeClient(Client* client);

    std::vector<int> getAllClientIDs() const; // Get all client IDs on the server
    Client* getClientByID(int clientID) const; // Get a client by their ID
    Channel* getChannelByName(const std::string& channelName) const; // Get a channel by its name
    bool isClientOpInChannel(int clientID, const std::string& channelName) const; // Check if a client is an operator in a channel
};
