#include "utils/Parser.hpp"
#include <iostream>
#include <sstream>
#include <vector>

void Parser::processClientMessage(Client &client, const std::string &message) {
    std::istringstream stream(message);
    std::string command;
    std::vector<std::string> args;

    // Parse the command and arguments
    stream >> command;
    std::string arg;
    while (stream >> arg) {
        args.push_back(arg);
    }

    // Store the parsed command and arguments in the client
    client.getBuffer().str(""); // Clear the buffer
    client.getBuffer() << message;
    client.getCommand().clear(); // Clear the command vector
    if (!command.empty()) {
        client.getCommand().push_back(command); // Add the command
        client.getCommand().insert(client.getCommand().end(), args.begin(), args.end());
    }

    std::cout << "Processed message from client " << client.getId() << ": " << message << std::endl;
}
