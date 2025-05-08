#include "core/Manager.hpp"

// Channel& Manager::getOrCreateChannel(const std::string& channelName) {
//     // Check if the channel already exists
//     if (_channels.find(channelName) == _channels.end()) {
//         // Create a new channel and add it to the map
//         _channels[channelName] = Channel(channelName);
//         std::cout << "Channel created: " << channelName << std::endl;
//     }
//     return _channels[channelName];
// }

// void Manager::joinAction(Client &client) {
//   const std::vector<std::string> &command = client.getCommand();

//   if (command.size() < 2) {
//       sendIrcMessage(client.getId(), formatMessage(client, "461") + " JOIN :Not enough parameters");
//       return;
//   }

//   std::string channelName = command[1];
//   Channel &channel = Manager::getOrCreateChannel(channelName);

//   // Add the client to the channel
//   channel.addClient(&client);
//   std::cout << "Client " << client.getNickName() << " added to channel: " << channelName << std::endl;

//   // Send JOIN confirmation
//   sendIrcMessage(client.getId(), ":" + client.getNickName() + "!@" + "localhost JOIN :" + channelName);

//   // Send NAMES list
//   std::string userList = channel.getUserList(); // Implement this to return a space-separated list of nicknames
//   sendIrcMessage(client.getId(), ":localhost 353 " + client.getNickName() + " = " + channelName + " :" + userList);

//   // Send End of NAMES list
//   sendIrcMessage(client.getId(), ":localhost 366 " + client.getNickName() + " " + channelName + " :End of /NAMES list");
// }

// // void Manager::joinAction(Client &client) {
// //   const std::vector<std::string> &command = client.getCommand();

// //   if (command.size() < 2) {
// //       sendIrcMessage(client.getId(), formatMessage(client, "461") + " JOIN :Not enough parameters");
// //       return;
// //   }

// //   std::string channelName = command[1];
// //   Channel &channel = Manager::getOrCreateChannel(channelName);

// //   // Add the client to the channel (pass the address of the client)
// //   channel.addClient(&client);
// //   std::cout << "Client " << client.getNickName() << " added to channel: " << channelName << std::endl;

// //   // Send JOIN confirmation
// //   sendIrcMessage(client.getId(), ":" + client.getNickName() + "!@" + "localhost JOIN :" + channelName);

// //   // Send NAMES list
// //   std::string userList = channel.getUserList(); // Implement this to return a space-separated list of nicknames
// //   sendIrcMessage(client.getId(), ":localhost 353 " + client.getNickName() + " = " + channelName + " :" + userList);

// //   // Send End of NAMES list
// //   sendIrcMessage(client.getId(), ":localhost 366 " + client.getNickName() + " " + channelName + " :End of /NAMES list");
// // }

// void Manager::sendNamesList(const std::string &channelName, Client &client) {
//   if (_channels.find(channelName) == _channels.end()) {
//       return;
//   }

//   Channel &channel = _channels[channelName];
//   std::vector<std::string> namesList = channel.getNamesList();

//   std::string namesMessage = formatMessage(client, "353") + " = " + channelName + " :";
//   for (size_t i = 0; i < namesList.size(); ++i) {
//       namesMessage += namesList[i];
//       if (i != namesList.size() - 1) {
//           namesMessage += " ";
//       }
//   }

//   sendIrcMessage(client.getId(), namesMessage);
//   sendIrcMessage(client.getId(), formatMessage(client, "366") + " " + channelName + " :End of /NAMES list");
// }

void Manager::joinAction(Client &client){
  std::vector<std::string> command = client.getCommand();
  std::string channelName = command[1];
  if (Parser::joinParse(client)) {
      if ((int)command[1].find(" ") >= 0)
          channelName = command[1].substr(0, command[1].find(" "));
      else
          channelName = command[1];
      //Check if the channel exists, create if not
      if (_channels.find(channelName) == _channels.end()) {
          _channels[channelName] = Channel(channelName);
      }
      sendIrcMessage(client.getId(), formatMessage(client) + " JOIN :" + channelName);
      //if there is topic send topic
      if (_channels.find(channelName)->second.getTopic() != "") {
          sendIrcMessage(client.getId(), formatMessage(client, TOPIC_CHANNEL) + " " + channelName + " :" + _channels.find(channelName)->second.getTopic());
      }
      _channels[channelName].addClient(&client);
  }
}

void Manager::sendNamesList(const std::string &channelName, Client &client) {
  std::string _serverName = "my_server";
  std::vector<std::string> namesList = _channels[channelName].getNamesList();
  std::string namesMessage = Manager::formatMessage(client, NAMREPLY) + " = " + channelName + " :";
  for (unsigned long i = 0; i < namesList.size(); i++) {
      namesMessage += namesList[i];
      if (i != namesList.size() - 1)
          namesMessage += " ";
  }
  sendIrcMessage(client.getId(), namesMessage);
  // Send end of NAMES list
  sendIrcMessage(client.getId(), formatMessage(client, ENDOFNAMES) + " " + channelName + " :End of /NAMES list");
}