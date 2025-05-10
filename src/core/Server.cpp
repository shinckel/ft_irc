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

/* handle data from a client by filling the buffer */
void Server::handleData(int fd) {
    // check if the client has quit
    if (!Manager::isClient(fd)) {
        std::cerr << "Error: Attempted to handle data for a non-existent client (fd: " << fd << ")" << std::endl;
        close(fd);           // close the socket
        return;
    }

    int nbrBytes;          // number of bytes received
    char buffer[BUFFER_SIZE]; // buffer to store received data

    bzero(buffer, sizeof(buffer)); // clear the buffer

    // check for errors or client disconnection
    nbrBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (nbrBytes <= 0) {
        if (nbrBytes == 0) {
            // client disconnected
            std::cout << "Client with socket " << fd << " disconnected" << std::endl;
            Manager::removeClientFromChannels(*Manager::getClientByID(fd)); // remove client from channels
            Manager::removeClient(fd); // remove client from the manager
        } else {
            std::cout << "Random receive error" << std::endl;
        }
        close(fd); // close the socket
    } else {
        // Obtenha a referência para o buffer do cliente
        std::stringstream &clientBuffer = Manager::getClientBuffer(fd);

        // Concatene os dados recebidos no buffer do cliente
        clientBuffer.str(clientBuffer.str() + buffer); // corrige a concatenação com o buffer

        // Encontre a posição do primeiro '\n' (quebra de linha)
        int newLine = clientBuffer.str().find('\n');
        while (newLine >= 0) {
            // Processa a mensagem até o '\n'
            std::string temp = clientBuffer.str().substr(newLine + 1); // pega o restante dos dados
            clientBuffer.str(clientBuffer.str().substr(0, newLine + 1)); // corta o buffer até a linha

            std::cout << "[" << fd << "]" << clientBuffer.str(); // exibe o buffer processado

            handleMessage(fd); // chama a função para tratar o comando/mensagem

            // Atualiza o buffer com os dados restantes
            clientBuffer.str(temp); // restabelece o buffer com o restante dos dados
            newLine = clientBuffer.str().find('\n'); // procura por outro '\n' no restante
        }
    }
}

/* handle a message from a client */
void Server::handleMessage(int fd) {
    // sanitize the buffer to remove unwanted characters
    std::string temp = Manager::getClientBuffer(fd).str();
    Manager::getClientBuffer(fd).str(Parser::sanitizeBuffer(temp));

    // get the client object
    std::vector<Client>::iterator iter = Manager::getClientByID(fd);
    Client &temporary = *Manager::getClientByID(fd);

    // set the client's command
    temporary.setCommand(Manager::getClientBuffer(fd).str());

    // check if the command is an action or a normal message
    if (Parser::isAction(temporary.getCommand()[0], fd) || (Manager::getClientByID(fd)->getChannel().size() && Manager::normalMsg(temporary))) {
        Manager::runActions(*iter); // execute the action
    } else {
        // if the client is not in a channel, send a message
        std::cout << "sending regular you're not in channel msg" << std::endl;
        send(fd, "You are not in a channel, please join a channel!\n", 49, 0);
    }
}
