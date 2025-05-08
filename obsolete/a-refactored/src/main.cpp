#include "core/Server.hpp"

void printUsage() {
    std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
    std::cout << "  <port>     : Port number to run the IRC server on (e.g., 6667)" << std::endl;
    std::cout << "  <password> : Password required for clients to connect" << std::endl;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        printUsage();
        return EXIT_FAILURE;
    }

    try {
        Manager::createMap();

        // Initialize and start the server
        Server server(argv[1], argv[2]);
        server.start();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
