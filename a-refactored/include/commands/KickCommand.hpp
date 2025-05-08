#pragma once
#include "Command.hpp"
#include "../core/Channel.hpp"
#include "../core/Server.hpp"

class KickCommand : public Command {
public:
    KickCommand(Server* server);
    virtual ~KickCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
