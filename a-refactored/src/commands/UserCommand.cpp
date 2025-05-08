// #include "commands/UserCommand.hpp"

// UserCommand::UserCommand(Server* server) : _server(server) {}

// UserCommand::~UserCommand() {}

// void UserCommand::execute(Client* client, const std::vector<std::string>& args) {
//     if (args.size() < 1) {
//         client->sendMessage("ERROR: USER command requires a username.\n");
//         return;
//     }

//     std::string username = args[0];
//     client->setUserName(username);
//     client->sendMessage("Username set to: " + username + "\n");
// }
