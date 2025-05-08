#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <stdexcept>
# include <unistd.h>
# include <algorithm>
# include <poll.h>
# include <cerrno>
# include <cstring>

# include "Socket.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Server
{
public:
    Server(int port, const std::string &password);
    ~Server();

    void stop();
    void run();

    const std::vector<Channel *> &getChannels() const;

private:
    Socket _socket;
    bool _running;
    std::string _password;
    std::vector<Client *> _clients;
    std::vector<Channel *> _channels;
    std::vector<struct pollfd> _pollfds;

    void handleNewConnection();
    void handleClientMessage(Client *client);
    void removeClient(Client *client);

    std::vector<int> getAllClientIDs() const;
    Client *getClientByID(int clientID) const;
    Channel *getChannelByName(const std::string &channelName) const;
    bool isClientOpInChannel(int clientID, const std::string &channelName) const;

    void updatePollfds();
};

#endif
