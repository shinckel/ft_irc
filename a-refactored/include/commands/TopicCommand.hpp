#pragma once
#include "Command.hpp"
#include "../core/Server.hpp"
#include "../core/Channel.hpp"

class TopicCommand : public Command {
public:
    TopicCommand(Server* server);
    virtual ~TopicCommand();

    void execute(Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
};
