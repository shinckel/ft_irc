#include "core/Server.hpp"
#include <iostream>
#include <cstdlib> // For atoi

void printUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " <port> <password>" << std::endl;
    std::cout << "  <port>     : Port number to run the IRC server on (e.g., 6667)" << std::endl;
    std::cout << "  <password> : Password required for clients to connect" << std::endl;
}

int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    // Parse port and password
    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Error: Invalid port number. Must be between 1 and 65535." << std::endl;
        return 1;
    }

    std::string password = argv[2];
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty." << std::endl;
        return 1;
    }

    try {
        // Initialize and start the server
        Server server(port, password);
        std::cout << "IRC server is running on port " << port << "..." << std::endl;
        server.run(); // Start the server loop
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
