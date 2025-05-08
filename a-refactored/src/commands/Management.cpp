#include "core/Manager.hpp"

void Manager::joinAction(Client &client) {
  std::vector<std::string> command = client.getCommand();
  if (!Parser::joinParse(client) || command.size() < 2) return;

  std::string channelName = command[1];
  size_t spacePos = channelName.find(" ");
  if (spacePos != std::string::npos)
      channelName = channelName.substr(0, spacePos);

  Channel &channel = _channels[channelName];
  if (!_channels.count(channelName))
      _channels[channelName] = Channel(channelName);

  // Se canal for invite-only e cliente não foi convidado
  // if (channel.getModeI() && !channel.isInvited(client.getId())) {
  //     sendIrcMessage(client.getId(), formatMessage(client, ERR_INVITEONLYCHAN) + " " + channelName + " :Cannot join channel (+i)");
  //     return;
  // }

  channel.addClient(&client);
  sendIrcMessage(&client, formatMessage(client) + " JOIN :" + channelName);

  // if (!channel.getTopic().empty()) {
  //     sendIrcMessage(&client, formatMessage(client, TOPIC_CHANNEL) + " " + channelName + " :" + channel.getTopic());
  // }

  sendNamesList(channelName, client);
}

void Manager::sendNamesList(const std::string &channelName, Client &client) {
  std::string _serverName = "my_server";
  std::vector<std::string> namesList = _channels[channelName].getNamesList();
  std::string namesMessage = Manager::formatMessage(client, "353") + " = " + channelName + " :";
  for (unsigned long i = 0; i < namesList.size(); i++) {
      namesMessage += namesList[i];
      if (i != namesList.size() - 1)
          namesMessage += " ";
  }
  sendIrcMessage(&client, namesMessage);
  // Send end of NAMES list
  sendIrcMessage(&client, formatMessage(client, "366") + " " + channelName + " :End of /NAMES list");
}