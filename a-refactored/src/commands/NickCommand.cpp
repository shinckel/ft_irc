// #include "commands/NickCommand.hpp"

// NickCommand::NickCommand(Server* server) : _server(server) {}

// NickCommand::~NickCommand() {}

// void NickCommand::execute(Client* client, const std::vector<std::string>& args) {
//     if (args.size() < 1) {
//         client->sendMessage("ERROR: NICK command requires a nickname.\n");
//         return;
//     }

//     std::string newNick = args[0];
//     if (!client->getNickName().empty()) {
//         client->sendMessage(":" + client->getNickName() + " NICK :" + newNick + "\n");
//     } else {
//         client->sendMessage(":" + client->getLastNick() + " NICK :" + newNick + "\n");
//     }
//     client->setNickName(newNick);
// }
