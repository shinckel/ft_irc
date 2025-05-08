// #include "commands/ModeCommand.hpp"
// #include <iostream>

// ModeCommand::ModeCommand(Server* server) : _server(server) {}

// ModeCommand::~ModeCommand() {}

// void ModeCommand::execute(Client* client, const std::vector<std::string>& args) {
//     if (args.empty()) {
//         client->sendMessage("ERROR: MODE command requires a target.\n");
//         return;
//     }

//     const std::string& target = args[0];
//     std::vector<std::string> modeArgs(args.begin() + 1, args.end());

//     if (target[0] == '#') {
//         // Handle channel mode
//         handleChannelMode(client, target, modeArgs);
//     } else {
//         // Handle user mode
//         handleUserMode(client, target, modeArgs);
//     }
// }

// void ModeCommand::handleChannelMode(Client* client, const std::string& channelName, const std::vector<std::string>& modeArgs) {
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

//     // Check if the client is an operator in the channel
//     if (!channel->isOperator(client->getFd())) {
//         client->sendMessage("ERROR: You are not a channel operator in: " + channelName + "\n");
//         return;
//     }

//     // Handle mode changes
//     if (modeArgs.empty()) {
//         // If no mode arguments are provided, return the current channel modes
//         client->sendMessage("Current modes for channel " + channelName + ": " + channel->getModes() + "\n");
//     } else {
//         // Apply the mode changes
//         std::string modeChangeResult = channel->applyModes(modeArgs);
//         channel->channelMessage("MODE " + channelName + " " + modeChangeResult + "\r\n");
//     }
// }

// void ModeCommand::handleUserMode(Client* client, const std::string& targetNick, const std::vector<std::string>& modeArgs) {
//     // Find the target user
//     Client* targetClient = NULL;
//     std::vector<Client*>& clients = _server->getClients();
//     for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
//         if ((*it)->getNick() == targetNick) {
//             targetClient = *it;
//             break;
//         }
//     }

//     if (targetClient == NULL) {
//         client->sendMessage("ERROR: No such user: " + targetNick + "\n");
//         return;
//     }

//     // Handle user mode changes
//     if (targetClient != client) {
//         client->sendMessage("ERROR: You can only change your own user modes.\n");
//         return;
//     }

//     if (modeArgs.empty()) {
//         // If no mode arguments are provided, return the current user modes
//         client->sendMessage("Your current modes: " + client->getModes() + "\n");
//     } else {
//         // Apply the mode changes
//         std::string modeChangeResult = client->applyModes(modeArgs);
//         client->sendMessage("MODE " + targetNick + " " + modeChangeResult + "\n");
//     }
// }
