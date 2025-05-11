#include "Server.hpp"

/* iterate through successful connections to get data to read */
void Server::getData() {
    struct sockaddr_storage address; // struct to store socket address (supports IPv4, IPv6, etc.)
    socklen_t addrlen;               // size of the socket address structure
    int newFd;

    // iterate through the pollfds vector to check for events
    for (size_t i = 0; i < pollfds.size(); ++i) {
        if (pollfds[i].revents & POLLIN) { // check if there's data to read
            if (pollfds[i].fd == _socketFd) {
                // new connection detected
                addrlen = sizeof(address);
                newFd = accept(_socketFd, (struct sockaddr *)&address, &addrlen);
                if (newFd == -1) {
                    std::cout << "Error accepting connection" << std::endl;
                } else {
                    // add the new connection to the pollfds vector
                    struct pollfd clientFd;
                    clientFd.fd = newFd;
                    clientFd.events = POLLIN; // monitor for incoming data
                    pollfds.push_back(clientFd);
                    acceptedConnection(newFd); // handle the new connection
                }
            } else {
                // handle data from an existing client
                handleData(pollfds[i].fd);

                // if the client disconnected, remove it from the pollfds vector
                if (!Manager::isClient(pollfds[i].fd)) {
                    close(pollfds[i].fd); // close the client socket
                    pollfds.erase(pollfds.begin() + i); // remove from the vector
                    --i; // adjust the index after removal
                }
            }
        }
    }
}

/* get the hostname for each client */
void Server::acceptedConnection(int newFd) {
    struct sockaddr_storage peerAddress; // struct to store the client's address
    socklen_t peerAddrlen = sizeof(peerAddress);

    // get the client's address
    getpeername(newFd, (struct sockaddr *)&peerAddress, &peerAddrlen);

    char host[NI_MAXHOST]; // buffer to store the hostname

    // convert the address to a string
    std::string ipAddress = inet_ntoa(((struct sockaddr_in *)&peerAddress)->sin_addr);

    // get the hostname using the IP address
    struct hostent *hostEntry = gethostbyname(ipAddress.c_str());
    if (hostEntry) {
        strncpy(host, hostEntry->h_name, NI_MAXHOST);
        host[NI_MAXHOST - 1] = '\0'; // ensure null termination
        std::cout << "Connection accepted from " << host << std::endl;
        Manager::addClient(newFd); // add the client to the manager
    } else {
        std::cerr << "gethostbyname failed" << std::endl;
    }
}

bool Server::validateClient(int fd) {
    if (!Manager::isClient(fd)) {
        std::cerr << "Error: Client with fd " << fd << " does not exist. Cleaning up..." << std::endl;
        close(fd); // Close the socket
        return false;
    }
    return true;
}

void Server::handleData(int fd) {
    if (!validateClient(fd)) {
        return; // Exit early if the client is invalid
    }

    int nbrBytes;          // Number of bytes received
    char buffer[BUFFER_SIZE]; // Buffer to store received data

    bzero(buffer, sizeof(buffer)); // Clear the buffer

    // Check for errors or client disconnection
    nbrBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (nbrBytes <= 0) {
        if (nbrBytes == 0) {
            // Client disconnected
            std::cout << "Client with socket " << fd << " disconnected" << std::endl;
            Manager::removeClientFromChannels(*Manager::getClientByID(fd)); // Remove client from channels
            Manager::removeClient(fd); // Remove client from the manager
        }
        close(fd); // Close the socket
        return; // Exit after handling disconnection
    } else {
        buffer[nbrBytes] = '\0';

        std::stringstream &clientBuffer = Manager::getClientBuffer(fd);
        clientBuffer.str(clientBuffer.str() + buffer);

        int newLine = clientBuffer.str().find('\n');
        while (newLine >= 0) {
            std::vector<Client>::iterator it = Manager::getClientByID(fd);
            Client &client = *it;

            std::string temp = clientBuffer.str().substr(newLine + 1);
            clientBuffer.str(clientBuffer.str().substr(0, newLine + 1));

            if (!client.isAuthenticated()) {
                std::string sanitizedBuffer = clientBuffer.str();
                size_t lastNotWhitespace = sanitizedBuffer.find_last_not_of(" \t\r\n");
                if (lastNotWhitespace != std::string::npos) {
                    sanitizedBuffer.erase(lastNotWhitespace + 1);
                } else {
                    sanitizedBuffer.clear();
                }

                if (sanitizedBuffer.substr(0, 4) == "PASS") {
                    client.setCommand(sanitizedBuffer);
                    Manager::passAction(client);
                } else {
                    Manager::sendIrcMessage(fd, ":localhost 451 * :You must authenticate using PASS first");
                }
            } else {
                client.setCommand(clientBuffer.str());
                handleMessage(fd);
            }

            // Check if the client still exists before continuing to read the buffer
            // ...otherwise, leaks!!!
            if (!validateClient(fd)) {
                return; // Exit if the client was removed
            }

            clientBuffer.str(temp);
            newLine = clientBuffer.str().find('\n');
        }
    }
}

void Server::handleMessage(int fd) {
    Client &client = *Manager::getClientByID(fd);

    std::vector<std::string> command = client.getCommand();

    if (command.empty()) {
        std::cerr << "Error: Empty command received from client " << client.getId() << std::endl;
        return;
    }

    const std::string &action = command[0];
    if (Parser::isAction(action, fd) || (client.getChannel().size() && Manager::normalMsg(client))) {
        Manager::runActions(client); // Execute the action for the client
    } else {
        std::cout << "sending regular you're not in channel msg" << std::endl;
        send(fd, "You are not in a channel, please join a channel!\n", 49, 0);
    }
}
