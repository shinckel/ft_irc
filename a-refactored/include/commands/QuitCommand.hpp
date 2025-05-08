#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"
#include "../core/Channel.hpp"

class QuitCommand : public Command {
public:
    QuitCommand(Server* server);
    virtual ~QuitCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;

    void removeClientFromChannels(Client* client);
};
