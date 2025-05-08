#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"

class NickCommand : public Command {
public:
    NickCommand(Server* server);
    virtual ~NickCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
