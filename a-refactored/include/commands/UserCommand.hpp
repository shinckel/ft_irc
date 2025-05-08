#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"

class UserCommand : public Command {
public:
    UserCommand(Server* server);
    virtual ~UserCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
