#include "commands/JoinCommand.hpp"
#include <iostream>

JoinCommand::JoinCommand(Server* server) : _server(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client* client, const std::vector<std::string>& args) {
    if (args.empty()) {
        client->sendMessage("ERROR: JOIN command requires a channel name.\n");
        return;
    }

    const std::string& channelName = args[0];
    Channel* channel = NULL;

    // Check if the channel already exists
    std::vector<Channel*>& channels = _server->getChannels();
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if ((*it)->getName() == channelName) {
            channel = *it;
            break;
        }
    }

    // If the channel doesn't exist, create it
    if (channel == NULL) {
        channel = new Channel(channelName);
        channels.push_back(channel);
    }

    // Add the client to the channel
    channel->addClient(client);
    client->sendMessage("You have joined the channel: " + channelName + "\n");
}