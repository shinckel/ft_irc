#include "Manager.hpp"

std::string partSyntax(Client &client, std::string channelName, std::string exitMsg) {
	if(exitMsg[exitMsg.size()-1] == '\n')
		exitMsg.erase(exitMsg.size() - 1, 1);
	return (":" + client.getNickName() + "!" + client.getUserName() + "@localhost PART " + channelName + " :" + exitMsg);
}

std::string channelDeletedSyntax(const std::string &channelName) {
  return (":server NOTICE * :Channel " + channelName + " has been deleted.");
}

void Manager::partAction(Client &client) {
  std::vector<std::string> command = client.getCommand();

  // Ensure the command has at least two elements (PART and channel name)
  if (command.size() < 2 || command[1].empty()) {
      sendIrcMessage(client.getId(), formatMessage(client, "461 PART") + " :Not enough parameters");
      return;
  }

  // ignore the parting strings and only take the channel name
  std::string channelName = command[1];
  size_t spacePos = channelName.find(' ');
  if (spacePos != std::string::npos) {
      channelName = channelName.substr(0, spacePos);
  }

  if (_channels.find(channelName) == _channels.end()) {
      sendIrcMessage(client.getId(), formatMessage(client, "403") + " " + channelName + " :No such channel");
      return;
  }

  Channel &channel = _channels[channelName];
  if (!channel.checkClient(client.getId())) {
      sendIrcMessage(client.getId(), formatMessage(client, "442") + " " + channelName + " :You're not on that channel");
      return;
  }

  std::string exitMsg = (command.size() > 2) ? command[2] : "";
  std::string partMessage = partSyntax(client, channelName, exitMsg);
  channel.channelMessage(partMessage);
  sendIrcMessage(client.getId(), partMessage);

  channel.removeClient(client.getId());

  // If the channel is empty, delete it
  if (!channel.howManyClients()) {
    sendIrcMessage(client.getId(), channelDeletedSyntax(channelName));
    _channels.erase(channelName);
  }
}
