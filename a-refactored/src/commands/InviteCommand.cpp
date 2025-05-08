#include "commands/InviteCommand.hpp"
#include "../utils/Parser.hpp"
#include <iostream>

InviteCommand::InviteCommand(Server* server) : _server(server) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(Client* client, const std::vector<std::string>& args) {
    if (!Parser::inviteParse(client)) {
        client->sendMessage("ERROR: Invalid INVITE command format.\n");
        return;
    }

    if (args.size() < 2) {
        client->sendMessage("ERROR: INVITE command requires a nickname and a channel name.\n");
        return;
    }

    const std::string& nick = args[0];
    const std::string& channelName = args[1];

    // Get the ID of the user by their nickname
    int id = getIDByNick(nick);
    if (id == -1) {
        client->sendMessage("ERROR: No such user with nickname: " + nick + "\n");
        return;
    }

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

    // Add the invited user to the channel's invited list
    channel->addInvited(id);

    // Notify the inviter
    client->sendMessage("You have invited " + nick + " to join " + channelName + "\n");

    // Notify the invited user
    Client* invitedClient = NULL;
    std::vector<Client*>& clients = _server->getClients();
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getFd() == id) {
            invitedClient = *it;
            break;
        }
    }

    if (invitedClient != NULL) {
        invitedClient->sendMessage("NOTICE: You have been invited to join " + channelName + " by " + client->getFd() + "\n");
    }
}

int InviteCommand::getIDByNick(const std::string& nick) {
    std::vector<Client*>& clients = _server->getClients();
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNick() == nick) {
            return (*it)->getFd();
        }
    }
    return -1; // Return -1 if no user with the given nickname is found
}
