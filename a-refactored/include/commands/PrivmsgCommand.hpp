#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"
#include "../core/Channel.hpp"

class PrivmsgCommand : public Command {
public:
    PrivmsgCommand(Server* server);
    virtual ~PrivmsgCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;

    int getIDByNick(const std::string& nick);
};
