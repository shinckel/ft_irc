#include "commands/CapCommand.hpp"

CapCommand::CapCommand(Server* server) : _server(server) {}

CapCommand::~CapCommand() {}

void CapCommand::execute(Client* client, const std::vector<std::string>& args) {
    if (args.empty()) {
        client->sendMessage("ERROR: CAP command requires arguments.\n");
        return;
    }

    std::string cap = args[0];
    if (cap == "LS") {
        client->sendMessage("CAP * LS :multi-prefix sasl\n");
    } else if (cap == "REQ") {
        client->sendMessage("CAP * ACK :multi-prefix sasl\n");
    } else if (cap == "END") {
        client->sendMessage("CAP * ACK :multi-prefix sasl\n");
    } else {
        client->sendMessage("ERROR: Unknown CAP subcommand.\n");
    }
}
