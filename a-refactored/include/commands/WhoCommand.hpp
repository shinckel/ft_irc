#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"
#include "../core/Channel.hpp"

class WhoCommand : public Command {
public:
    WhoCommand(Server* server);
    virtual ~WhoCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
