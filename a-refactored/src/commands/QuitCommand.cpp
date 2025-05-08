// #include "commands/QuitCommand.hpp"
// #include <iostream>

// QuitCommand::QuitCommand(Server* server) : _server(server) {}

// QuitCommand::~QuitCommand() {}

// void QuitCommand::execute(Client* client, const std::vector<std::string>& args) {
//     // Notify all channels that the client is quitting
//     removeClientFromChannels(client);

//     // Send a quit message to the client
//     client->sendMessage("QUIT :Bye!\n");

//     // Remove the client from the server
//     std::vector<Client*>& clients = _server->getClients();
//     for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
//         if (*it == client) {
//             clients.erase(it);
//             delete client; // Free the memory
//             break;
//         }
//     }
// }

// void QuitCommand::removeClientFromChannels(Client* client) {
//     std::vector<Channel*>& channels = _server->getChannels();
//     for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         Channel* channel = *it;
//         if (channel->checkClient(client->getFd())) {
//             channel->removeClient(client);
//             channel->channelMessage("QUIT: " + client->getNick() + " has quit\r\n");
//         }
//     }
// }
