#pragma once
#include "Client.hpp"
#include <string>
#include <vector>

class Channel {
public:
    // Constructor
    explicit Channel(const std::string& name);

    // Getters
    const std::string& getName() const;
    const std::string& getKey() const;
    const std::string& getModes() const;
    const std::vector<Client*>& getClients() const;
    std::vector<int> getClientIDs() const; // Get the list of client IDs in the channel

    // Mode-related functions
    void setModeI(bool enabled); // Invite-only mode
    void setModeT(bool enabled); // Topic restriction mode
    void setKey(const std::string& key); // Set or remove channel key
    void addClientToOp(int clientFd); // Add a client to operators
    void removeOp(int clientFd); // Remove a client from operators
    void setModeL(int limit); // Set or remove user limit
    bool isOperator(int clientFd) const; // Check if a client is an operator

    // Client management
    void addClient(Client* client);
    void removeClient(Client* client);
    bool checkClient(int clientFd) const;

    // Topic-related functions
    const std::string& getTopic() const;
    void setTopic(const std::string& topic);

    // Messaging
    void clientMessage(int senderFd, const std::string& message) const;
    void channelMessage(const std::string& message) const;

private:
    std::string _name; // Channel name
    std::string _key; // Channel key
    std::string _topic; // Channel topic
    std::string _modes; // Current modes as a string
    bool _modeI; // Invite-only mode
    bool _modeT; // Topic restriction mode
    int _limit; // User limit
    std::vector<int> _operators; // List of operator client FDs
    std::vector<Client*> _clients; // List of clients in the channel
};
