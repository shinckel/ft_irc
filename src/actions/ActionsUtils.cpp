#include "Manager.hpp"

void Manager::passAction(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2) {
        std::cerr << "Error: PASS command missing password for client " << client.getId() << std::endl;
        return;
    }

    std::string password = command[1];
    password.erase(0, password.find_first_not_of(" \t\r\n")); // Trim leading whitespace
    password.erase(password.find_last_not_of(" \t\r\n") + 1); // Trim trailing whitespace

    if (password == _password) {
        client.setAuthenticated(true);
        sendIrcMessage(client.getId(), ":server NOTICE * :Authentication successful");
    } else {
        sendIrcMessage(client.getId(), ":server 464 * :Password incorrect");
    }
}

void Manager::capAction(Client &client) {
    std::vector<std::string> command = client.getCommand();
    std::string cap = command[1];
    if (cap == "LS") {
        sendIrcMessage(client.getId(), formatMessage(client) + " CAP * LS :multi-prefix sasl");
    }
    else if (cap == "REQ") {
        sendIrcMessage(client.getId(), formatMessage(client) + " CAP * ACK :multi-prefix sasl");
    }
    else if (cap == "END") {
        sendIrcMessage(client.getId(), formatMessage(client) + " CAP * ACK :multi-prefix sasl");
    }
}

void Manager::sendWhoMessage(const std::vector<int> &list, Client &client, std::string const &channelName) {
    for (int i = 0; i < (int)list.size(); i++) {
        std::vector<Client>::iterator it = getClientByID(list[i]);
        if (it == _clients.end())
            continue; // client was removed
        Client &temp = *it;
        std::string status;

        if (channelName != "*") {
            std::map<std::string, Channel>::iterator it = _channels.find(channelName);
            if (it == _channels.end())
                continue; // or break?
            status = it->second.IsOp(temp.getId()) ? "@" : "+";
        }

        sendIrcMessage(
            client.getId(),
            formatMessage(client, RPL_WHOREPLY) + " " + channelName + " localhost ft_irc " +
            temp.getNickName() + " H" + status + " :1 " + temp.getUserName()
        );
    }

    sendIrcMessage(client.getId(), formatMessage(client, RPL_ENDOFWHO) + " " + channelName + " :End of WHO list");
}

// void Manager::sendWhoMessage(const std::vector<int> &list, Client &client, std::string const &channelName) {
//     for (int i = 0; i < (int)list.size(); i++) {
//         Client &temp = *Manager::getClientByID(list[i]);
//         std::string status;
//         if (channelName != "*")
//             status = _channels.find(channelName)->second.IsOp(temp.getId()) ? "@" : "+";
//         sendIrcMessage(client.getId(), formatMessage(client, RPL_WHOREPLY) + " " + channelName + " localhost ft_irc " + temp.getNickName() + " H" + status + " :1 " + temp.getUserName());
//     }
//     sendIrcMessage(client.getId(), formatMessage(client, RPL_ENDOFWHO) + " " + channelName + " :End of WHO list");
// }

// OLD - SOLID RESULTS, TRY TO IMPROVE IT
// void Manager::nickAction(Client &client)
// {
//     if (Parser::nickParse(client)) 
//     {
//         if (client.getNickName() != "")
//             sendIrcMessage(client.getId(), ":" + client.getNickName() + " NICK :" + client.getCommand()[1]);
//         else
//             sendIrcMessage(client.getId(), ":" + client.getLastNick() + " NICK :" + client.getCommand()[1]);
//         client.setNickName(client.getCommand()[1]);
//     }
// }

// IMPROVEMENT??
void Manager::nickAction(Client &client) {
    if (Parser::nickParse(client)) {
        std::string oldNick = client.getNickName();
        std::string newNick = client.getCommand()[1];

        client.setNickName(newNick);

        // If the client already has a nickname, notify about the change
        if (!oldNick.empty()) {
            sendIrcMessage(client.getId(), ":" + oldNick + "!" + client.getUserName() + "@localhost NICK :" + client.getNickName()); // Notify all clients in the same channels
        } else {
            // If this is the first time setting a nickname, send a welcome message
            sendIrcMessage(client.getId(), ":localhost 001 " + newNick + " :Welcome to the IRC network, " + newNick);
        }
    } else {
        sendIrcMessage(client.getId(), ":localhost 432 * " + client.getCommand()[1] + " :Erroneous nickname");
    }
}

// void Manager::userAction(Client &client) {
//     if (Parser::userParse(client)) {
//         std::vector<std::string> command = client.getCommand();
//         std::string username = command[1].substr(0, command[1].find(" "));
//         client.setUserName(username);
//         sendIrcMessage(client.getId(), ":" + client.getNickName() + "!" + client.getUserName() + "@localhost USER :User created");
//     }
// }
void Manager::userAction(Client &client) {
    if (Parser::userParse(client)) {
        if (client.getNickName().empty()) {
            sendIrcMessage(client.getId(), ":localhost NOTICE * :You must set a nickname using the NICK command.");
        } else {
            std::vector<std::string> command = client.getCommand();
            std::string username = command[1].substr(0, command[1].find(" "));
            client.setUserName(username);
            sendIrcMessage(client.getId(), ":server NOTICE * :User " + username + " has been created successfully.");
        }
    } else {
        sendIrcMessage(client.getId(), ":localhost 461 USER :Invalid USER command");
    }
}