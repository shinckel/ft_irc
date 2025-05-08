#include "core/Manager.hpp"

void Manager::joinAction(Client &client) {
  const std::vector<std::string> &command = client.getCommand();

  // Check if the command has enough parameters
  if (command.size() < 2) {
      sendIrcMessage(client.getId(), formatMessage(client, "461") + " JOIN :Not enough parameters");
      return;
  }

  std::string channelName = command[1];

  // Check if the channel exists; if not, create it
  Channel &channel = (_channels.find(channelName) == _channels.end())
                         ? _channels.insert(std::make_pair(channelName, Channel(channelName))).first->second
                         : _channels[channelName];

  // Add the client to the channel
  channel.addClient(&client);

  // Send confirmation to the client
  sendIrcMessage(client.getId(), formatMessage(client) + " JOIN :" + channelName);

  // Send the list of users in the channel
  sendNamesList(channelName, client);
}

void Manager::sendNamesList(const std::string &channelName, Client &client) {
  if (_channels.find(channelName) == _channels.end()) {
      return;
  }

  Channel &channel = _channels[channelName];
  std::vector<std::string> namesList = channel.getNamesList();

  std::string namesMessage = formatMessage(client, "353") + " = " + channelName + " :";
  for (size_t i = 0; i < namesList.size(); ++i) {
      namesMessage += namesList[i];
      if (i != namesList.size() - 1) {
          namesMessage += " ";
      }
  }

  sendIrcMessage(client.getId(), namesMessage);
  sendIrcMessage(client.getId(), formatMessage(client, "366") + " " + channelName + " :End of /NAMES list");
}