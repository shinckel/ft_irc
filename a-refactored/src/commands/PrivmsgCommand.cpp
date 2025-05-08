#include "commands/PrivmsgCommand.hpp"
#include "../utils/Parser.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server* server) : _server(server) {}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 2 || args[1].find(":") == std::string::npos) {
        client->sendMessage("ERROR: PRIVMSG command requires a target and a message.\n");
        return;
    }

    std::string targetName = args[0];
    std::string msg = args[1].substr(args[1].find(":") + 1);

    // Check if the target is a channel
    if (targetName[0] == '#') {
        std::string channelName = targetName;

        // Find the channel
        Channel* channel = NULL;
        std::vector<Channel*>& channels = _server->getChannels();
        for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
            if ((*it)->getName() == channelName) {
                channel = *it;
                break;
            }
        }

        if (channel == NULL) {
            client->sendMessage("ERROR: No such channel: " + channelName + "\n");
            return;
        }

        // Check if the client is a member of the channel
        if (!channel->checkClient(client->getFd())) {
            client->sendMessage("ERROR: You are not a member of the channel: " + channelName + "\n");
            return;
        }

        // Broadcast the message to all members of the channel
        std::string formatMsg = client->getNick() + " PRIVMSG " + channelName + " :" + msg;
        channel->clientMessage(client->getFd(), formatMsg.c_str());
    } else {
        // Target is a user, send the message directly to the user
        int targetId = getIDByNick(targetName);
        if (targetId != -1) {
            std::string ircMessage = client->getNick() + " PRIVMSG " + targetName + " :" + msg;
            Client* targetClient = NULL;
            std::vector<Client*>& clients = _server->getClients();
            for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
                if ((*it)->getFd() == targetId) {
                    targetClient = *it;
                    break;
                }
            }

            if (targetClient != NULL) {
                targetClient->sendMessage(ircMessage);
            }
        } else {
            // Handle the case where the target user does not exist
            client->sendMessage("ERROR: No such nick: " + targetName + "\n");
        }
    }
}

int PrivmsgCommand::getIDByNick(const std::string& nick) {
    std::vector<Client*>& clients = _server->getClients();
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNick() == nick) {
            return (*it)->getFd();
        }
    }
    return -1; // Return -1 if no user with the given nickname is found
}
