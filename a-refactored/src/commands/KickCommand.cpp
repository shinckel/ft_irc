// #include "commands/KickCommand.hpp"
// #include <iostream>

// KickCommand::KickCommand(Server* server) : _server(server) {}

// KickCommand::~KickCommand() {}

// void KickCommand::execute(Client* client, const std::vector<std::string>& args) {
//     if (args.size() < 2) {
//         client->sendMessage("ERROR: KICK command requires a channel name and a client name.\n");
//         return;
//     }

//     const std::string& channelName = args[0];
//     const std::string& targetClientName = args[1];

//     // Find the channel
//     Channel* channel = NULL;
//     std::vector<Channel*>& channels = _server->getChannels();
//     for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         if ((*it)->getName() == channelName) {
//             channel = *it;
//             break;
//         }
//     }

//     if (channel == NULL) {
//         client->sendMessage("ERROR: No such channel: " + channelName + "\n");
//         return;
//     }

//     // Find the target client in the channel
//     std::vector<Client*>& clients = channel->getClients();
//     Client* targetClient = NULL;
//     for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
//         if ((*it)->getFd() == atoi(targetClientName.c_str())) { // Assuming client name is their FD
//             targetClient = *it;
//             break;
//         }
//     }

//     if (targetClient == NULL) {
//         client->sendMessage("ERROR: No such client in channel: " + targetClientName + "\n");
//         return;
//     }

//     // Remove the client from the channel
//     channel->removeClient(targetClient);
//     targetClient->sendMessage("You have been kicked from the channel: " + channelName + "\n");
//     client->sendMessage("Client " + targetClientName + " has been kicked from the channel: " + channelName + "\n");
// }