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
        std::cout << "Parsed command: " << command << std::endl;
        std::cout << "Parsed arguments: ";
        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << args[i] << " ";
        }
        std::cout << std::endl;
    
        // Add the command and arguments to the vector
        client.getCommand().push_back(command);
        client.getCommand().insert(client.getCommand().end(), args.begin(), args.end());
    
        // Debugging: Render all commands in the vector
        std::cout << "Commands in the vector:" << std::endl;
        for (size_t i = 0; i < client.getCommand().size(); ++i) {
            std::cout << "[" << i << "]: " << client.getCommand()[i] << std::endl;
        }
    }
    std::cout << "Processed message from client " << client.getId() << ": " << message << std::endl;
}
