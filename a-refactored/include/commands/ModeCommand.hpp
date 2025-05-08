#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"
#include "../core/Channel.hpp"

class ModeCommand : public Command {
public:
    ModeCommand(Server* server);
    virtual ~ModeCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;

    void handleChannelMode(Client* client, const std::string& channelName, const std::vector<std::string>& modeArgs);
    void handleUserMode(Client* client, const std::string& targetNick, const std::vector<std::string>& modeArgs);
};
