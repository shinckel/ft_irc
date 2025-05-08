#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"

class CapCommand : public Command {
public:
    CapCommand(Server* server);
    virtual ~CapCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
