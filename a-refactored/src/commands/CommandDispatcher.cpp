#include "commands/CommandDispatcher.hpp"

CommandDispatcher::CommandDispatcher(Server* server) : _server(server) {
    // Register commands
    // registerCommand("JOIN", new JoinCommand(server));
    // registerCommand("KICK", new KickCommand(server));
    // registerCommand("INVITE", new InviteCommand(_server));
    // registerCommand("QUIT", new QuitCommand(_server));
    // registerCommand("TOPIC", new TopicCommand(_server));
    // registerCommand("WHO", new WhoCommand(_server));
    // registerCommand("PRIVMSG", new PrivmsgCommand(_server));
    // registerCommand("MODE", new ModeCommand(_server));
}

CommandDispatcher::~CommandDispatcher() {
    // Clean up dynamically allocated commands
    for (std::map<std::string, Command*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
        delete it->second;
    }
}

void CommandDispatcher::registerCommand(const std::string& name, Command* command) {
    _commands[name] = command;
}

void CommandDispatcher::dispatch(const std::string& commandName, Client* client, const std::vector<std::string>& args) {
    std::map<std::string, Command*>::iterator it = _commands.find(commandName);
    if (it != _commands.end()) {
        it->second->execute(client, args);
    } else {
        client->sendMessage("ERROR: Unknown command: " + commandName + "\n");
    }
}
