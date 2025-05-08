// #include "commands/JoinCommand.hpp"
// #include "core/Server.hpp"
// #include "core/Channel.hpp"

// void JoinCommand::execute(Client *client, const std::vector<std::string> &args) {
//     if (args.empty()) {
//         client->sendMessage("ERROR: No channel specified.\n");
//         return;
//     }

//     const std::string &channelName = args[0];
//     std::vector<Channel *> &channels = const_cast<std::vector<Channel *> &>(_server->getChannels());

//     // Check if the channel already exists
//     Channel *channel = NULL;
//     for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         if ((*it)->getName() == channelName) {
//             channel = *it;
//             break;
//         }
//     }

//     // If the channel doesn't exist, create it
//     if (!channel) {
//         channel = new Channel(channelName);
//         channels.push_back(channel);
//     }

//     // Add the client to the channel
//     channel->addClient(client);
//     client->sendMessage("You have joined the channel: " + channelName + "\n");
// }
