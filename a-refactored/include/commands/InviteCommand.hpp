#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"
#include "../core/Channel.hpp"

class InviteCommand : public Command {
public:
    InviteCommand(Server* server);
    virtual ~InviteCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;

    int getIDByNick(const std::string& nick);
};
