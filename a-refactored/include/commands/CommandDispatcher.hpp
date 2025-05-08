#pragma once
#include "Command.hpp"
#include "JoinCommand.hpp"
#include "KickCommand.hpp"
#include "InviteCommand.hpp"
#include "QuitCommand.hpp"
#include "TopicCommand.hpp"
#include "WhoCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "ModeCommand.hpp"
#include "../core/Server.hpp"
#include <map>
#include <string>

class CommandDispatcher {
public:
    CommandDispatcher(Server* server);
    ~CommandDispatcher();

    void dispatch(const std::string& commandName, Client* client, const std::vector<std::string>& args);

private:
    Server* _server;
    std::map<std::string, Command*> _commands;

    void registerCommand(const std::string& name, Command* command);
};
