#include "commands/WhoCommand.hpp"
#include <iostream>

WhoCommand::WhoCommand(Server* server) : _server(server) {}

WhoCommand::~WhoCommand() {}

void WhoCommand::execute(Client* client, const std::vector<std::string>& args) {
    // Determine the channel name or use "*" for all users
    std::string channelName = args.empty() ? "*" : args[0];

    // Get the list of clients in the specified channel
    std::vector<int> clientList;
    if (channelName == "*") {
        // If channelName is "*", get all clients on the server
        clientList = _server->getAllClientIDs();
    } else {
        // Otherwise, get the list of clients in the specified channel
        Channel* channel = _server->getChannelByName(channelName);
        if (channel == NULL) {
            client->sendMessage("ERROR: No such channel: " + channelName + "\n");
            return;
        }

        // Check if the client is a member of the channel
        if (!channel->checkClient(client->getFd())) {
            client->sendMessage("ERROR: You are not a member of the channel: " + channelName + "\n");
            return;
        }

        // Get the list of client IDs in the channel
        clientList = channel->getClientIDs();
    }

    // Send WHO replies for each client in the list
    for (std::vector<int>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
        Client* targetClient = _server->getClientByID(*it);
        if (targetClient == NULL) {
            continue; // Skip invalid clients
        }

        // Determine the status of the client in the channel
        std::string status = "";
        if (channelName != "*" && _server->isClientOpInChannel(*it, channelName)) {
            status = "@"; // Operator
        } else {
            status = "+"; // Regular user
        }

        // Format and send the WHO reply
        client->sendMessage("WHO " + channelName + " localhost ft_irc " +
                            targetClient->getNick() + " H" + status + " :1 " +
                            targetClient->getUserName() + "\n");
    }

    // Send the end of WHO list message
    client->sendMessage("End of WHO list.\n");
}
