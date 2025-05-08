#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"

class PassCommand : public Command {
public:
    PassCommand(Server* server);
    virtual ~PassCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
