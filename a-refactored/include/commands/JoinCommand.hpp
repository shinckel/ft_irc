#pragma once
#include "Command.hpp"
#include "../core/Channel.hpp"
#include "../core/Server.hpp"

class JoinCommand : public Command {
public:
    JoinCommand(Server* server);
    virtual ~JoinCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};