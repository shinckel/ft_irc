#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

#include "core/Client.hpp"
#include "utils/Error.hpp"

class Channel {
private:
    std::string _name;
    std::string _topic;
    std::vector<Client *> _clients;
    std::map<std::string, Client *> _operators;

public:
    Channel(const std::string &name);
    ~Channel();

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
};

#endif
