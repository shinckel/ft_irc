// #include "commands/TopicCommand.hpp"
// #include <iostream>

// TopicCommand::TopicCommand(Server* server) : _server(server) {}

// TopicCommand::~TopicCommand() {}

// void TopicCommand::execute(Client* client, const std::vector<std::string>& args) {
//     if (args.empty()) {
//         client->sendMessage("ERROR: TOPIC command requires a channel name.\n");
//         return;
//     }

//     const std::string& channelName = args[0];
//     std::string topic = args.size() > 1 ? args[1] : "";

//     // Find the channel
//     Channel* channel = NULL;
//     std::vector<Channel*>& channels = _server->getChannels();
//     for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         if ((*it)->getName() == channelName) {
//             channel = *it;
//             break;
//         }
//     }

//     if (channel == NULL) {
//         client->sendMessage("ERROR: No such channel: " + channelName + "\n");
//         return;
//     }

//     // Check if the client is a member of the channel
//     if (!channel->checkClient(client->getFd())) {
//         client->sendMessage("ERROR: You are not a member of the channel: " + channelName + "\n");
//         return;
//     }

//     // If no topic is provided, return the current topic
//     if (topic.empty()) {
//         const std::string& currentTopic = channel->getTopic();
//         if (currentTopic.empty()) {
//             client->sendMessage("The channel " + channelName + " has no topic set.\n");
//         } else {
//             client->sendMessage("The topic for channel " + channelName + " is: " + currentTopic + "\n");
//         }
//         return;
//     }

//     // Set the new topic
//     channel->setTopic(topic);
//     channel->channelMessage("TOPIC: " + client->getNick() + " has set the topic to: " + topic + "\r\n");
// }
