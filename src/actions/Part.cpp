#include "Manager.hpp"

// void Manager::partAction(Client &client) {
//     std::vector<std::string> command = client.getCommand();

//     // Ensure the command has at least two elements (PART and channel name)
//     if (command.size() < 2 || command[1].empty()) {
//         sendIrcMessage(client.getId(), formatMessage(client, "461 PART") + " :Not enough parameters");
//         return;
//     }

//     <std::string, Channel> it = _channels.find(command[1]);

//     std::string channelName = command[1];
//     std::cout << "Debug Channel name: " << channelName << std::endl;
//     if (_channels.find(channelName) == _channels.end()) {
//         sendIrcMessage(client.getId(), formatMessage(client, "403") + " " + channelName + " :No such channel");
//         return;
//     }

//     Channel &channel = _channels[channelName];
//     // Check if the client is a member of the channel
//     if (!channel.checkClient(client.getId())) {
//         sendIrcMessage(client.getId(), formatMessage(client, "442") + " " + channelName + " :You're not on that channel");
//         return;
//     }

//     // Notify the channel members about the PART action
//     std::string partMessage = ":" + client.getNickName() + "!" + client.getUserName() + "@" + hostName + " PART " + channelName;
//     if (command.size() > 2) {
//         partMessage += " " + command[2]; // append parting message if provided
//     }
//     channel.channelMessage(partMessage);

//     channel.removeClient(client.getId());

//     // If the channel is empty, delete it
//     // if (!channel) {
//     //     _channels.erase(channelName);
//     // }
// }

void Manager::partAction(Client &client) {
  std::vector<std::string> command = client.getCommand();

  // Ensure the command has at least two elements (PART and channel name)
  if (command.size() < 2 || command[1].empty()) {
      sendIrcMessage(client.getId(), formatMessage(client, "461 PART") + " :Not enough parameters");
      return;
  }

  // Extract the channel name (ignore any parting message)
  std::string channelName = command[1];
  size_t spacePos = channelName.find(' ');
  if (spacePos != std::string::npos) {
      channelName = channelName.substr(0, spacePos); // Take only the part before the space
  }

  // Check if the channel exists
  if (_channels.find(channelName) == _channels.end()) {
      sendIrcMessage(client.getId(), formatMessage(client, "403") + " " + channelName + " :No such channel");
      return;
  }

  Channel &channel = _channels[channelName];

  // Check if the client is a member of the channel
  if (!channel.checkClient(client.getId())) {
      sendIrcMessage(client.getId(), formatMessage(client, "442") + " " + channelName + " :You're not on that channel");
      return;
  }

  // Notify the channel members about the PART action
  std::string partMessage = ":" + client.getNickName() + "!" + client.getUserName() + "@" + hostName + " PART " + channelName;
  if (command.size() > 2) {
      partMessage += " :" + command[2]; // Append the parting message if provided
  }
  channel.channelMessage(partMessage);

  // Send confirmation to the client
  sendIrcMessage(client.getId(), partMessage);

  // Remove the client from the channel
  channel.removeClient(client.getId());

  // If the channel is empty, delete it
  if (!channel.howManyClients()) {
      _channels.erase(channelName);
  }
}
