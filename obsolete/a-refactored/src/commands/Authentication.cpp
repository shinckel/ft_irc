#include "core/Manager.hpp"

void Manager::nickAction(Client &client) {
  const std::vector<std::string> &command = client.getCommand();

  if (command.size() < 2) {
      sendIrcMessage(client.getId(), formatMessage(client, "431") + " :No nickname given");
      return;
  }

  std::string nickname = command[1];
  client.setNickName(nickname);
  std::cout << "Client " << client.getId() << " nickname set to: " << nickname << std::endl;

  sendIrcMessage(client.getId(), ":" + nickname + " NICK :" + nickname);
}

void Manager::userAction(Client &client) {
  const std::vector<std::string> &command = client.getCommand();

  if (command.size() < 5) {
      sendIrcMessage(client.getId(), formatMessage(client, "461") + " USER :Not enough parameters");
      return;
  }

  std::string username = command[1];
  std::string realname = command[4];
  client.setUserName(username);
  std::cout << "Client " << client.getId() << " username set to: " << username << std::endl;

  if (!client.getNickName().empty()) {
      // Send the full registration sequence
      sendIrcMessage(client.getId(), ":localhost 001 " + client.getNickName() + " :Welcome to the IRC server!");
      sendIrcMessage(client.getId(), ":localhost 002 " + client.getNickName() + " :Your host is localhost, running version 1.0");
      sendIrcMessage(client.getId(), ":localhost 003 " + client.getNickName() + " :This server was created today");
      sendIrcMessage(client.getId(), ":localhost 004 " + client.getNickName() + " localhost 1.0 o o");
  } else {
      std::cerr << "Error: Nickname not set before USER command" << std::endl;
  }
}
// void Manager::userAction(Client &client) {
//   const std::vector<std::string> &command = client.getCommand();

//   if (command.size() < 5) {
//       sendIrcMessage(client.getId(), formatMessage(client, "461") + " USER :Not enough parameters");
//       return;
//   }

//   std::string username = command[1];
//   std::string realname = command[4];
//   client.setUserName(username);
//   std::cout << "Client " << client.getId() << " username set to: " << username << std::endl;

//   if (!client.getNickName().empty()) {
//       sendIrcMessage(client.getId(), ":localhost 001 " + client.getNickName() + " :Welcome to the IRC server!");
//   } else {
//       std::cerr << "Error: Nickname not set before USER command" << std::endl;
//   }
// }

// void Manager::userAction(Client &client) {
//   const std::vector<std::string> &command = client.getCommand();

//   if (command.size() < 5) {
//       sendIrcMessage(client.getId(), formatMessage(client, "461") + " USER :Not enough parameters");
//       return;
//   }

//   std::string username = command[1];
//   std::string realname = command[4];
//   client.setUserName(username);
//   std::cout << "Client " << client.getId() << " username set to: " << username << std::endl;

//   if (!client.getNickName().empty()) {
//       sendIrcMessage(client.getId(), ":localhost 001 " + client.getNickName() + " :Welcome to the IRC server!");
//   } else {
//       std::cerr << "Error: Nickname not set before USER command" << std::endl;
//   }
// }
