#include "commands/PassCommand.hpp"

PassCommand::PassCommand(Server* server) : _server(server) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 1) {
        client->sendMessage("ERROR: PASS command requires a password.\n");
        return;
    }

    std::string password = args[0];
    _server->setPassword(password);
    client->sendMessage("Password set successfully.\n");
}
