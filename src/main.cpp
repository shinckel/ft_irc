#include "Server.hpp"
#include <signal.h>

#define MAX_FDS 1024


void handleSig(int sig)
{
	(void) sig;
	for(int i = 0; i < MAX_FDS; ++i)
		close(i);
}


void printUsage() {
    std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
    std::cout << "  <port>     : Port number to run the IRC server on (e.g., 6667)" << std::endl;
    std::cout << "  <password> : Password required for clients to connect" << std::endl;
}

int main(int argc, char **argv) {
    signal(SIGINT, handleSig);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT,handleSig);
    if (argc != 3) {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        printUsage();
        return EXIT_FAILURE;
    }

    Server socket_serv(argv[1], argv[2]);
    
    return EXIT_SUCCESS;
}
