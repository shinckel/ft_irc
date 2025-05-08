#include "core/Server.hpp"

Server::Server(int port, const std::string &password) : _running(false), _password(password), _channels()
{
    _socket.bind(port);
    _socket.listen();
}

Server::~Server() {
    stop();
}

void Server::stop() {
    _running = false;
    _socket.close();
    std::cout << "Server stopped." << std::endl;
}

void Server::run()
{
    _running = true;
    std::cout << "Server is running..." << std::endl;

    // Add the server socket to the pollfd vector
    struct pollfd serverPollfd;
    serverPollfd.fd = _socket.getFd();
    serverPollfd.events = POLLIN; // Monitor for incoming connections
    _pollfds.push_back(serverPollfd);

    while (_running)
    {
        // Use poll() to monitor file descriptors
        int pollCount = poll(_pollfds.data(), _pollfds.size(), -1); // -1 means no timeout
        if (pollCount == -1)
        {
            std::cerr << "poll() error: " << strerror(errno) << std::endl;
            continue;
        }

        for (size_t i = 0; i < _pollfds.size(); ++i)
        {
            if (_pollfds[i].revents & POLLIN) // Check if there's data to read
            {
                if (_pollfds[i].fd == _socket.getFd())
                {
                    // New connection
                    handleNewConnection();
                }
                else
                {
                    // Existing client sent a message
                    Client *client = getClientByID(_pollfds[i].fd);
                    if (client)
                    {
                        handleClientMessage(client);
                    }
                }
            }
            else if (_pollfds[i].revents & (POLLHUP | POLLERR))
            {
                // Client disconnected or error occurred
                Client *client = getClientByID(_pollfds[i].fd);
                if (client)
                {
                    removeClient(client);
                }
            }
        }

        // Update the pollfd vector after handling events
        updatePollfds();
    }
}

void Server::updatePollfds()
{
    _pollfds.clear();

    // Add the server socket
    struct pollfd serverPollfd;
    serverPollfd.fd = _socket.getFd();
    serverPollfd.events = POLLIN;
    _pollfds.push_back(serverPollfd);

    // Add client sockets
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        struct pollfd clientPollfd;
        clientPollfd.fd = (*it)->getFd();
        clientPollfd.events = POLLIN;
        _pollfds.push_back(clientPollfd);
    }
}

void Server::handleNewConnection()
{
    int clientFd = _socket.accept();
    Client *newClient = new Client(clientFd);

    // Prompt for password
    newClient->sendMessage("Enter server password:");
    std::string clientPassword = newClient->receiveMessage();

    if (clientPassword != _password)
    {
        newClient->sendMessage("Invalid password. Connection closed.");
        close(clientFd);
        delete newClient;
        return;
    }

    newClient->sendMessage("Welcome to the IRC server!");
    _clients.push_back(newClient);
    std::cout << "New client connected: " << clientFd << std::endl;
}

void Server::handleClientMessage(Client* client) {
    std::string message = client->receiveMessage();
    if (message.empty()) {
        removeClient(client);
        return;
    }
    std::cout << "Message from client " << client->getFd() << ": " << message << std::endl;
    // Process the message (e.g., dispatch commands)
}

void Server::removeClient(Client* client) {
    std::cout << "Client disconnected: " << client->getFd() << std::endl;
    close(client->getFd());
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
    delete client;
}

std::vector<int> Server::getAllClientIDs() const {
    std::vector<int> clientIDs;
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        clientIDs.push_back((*it)->getFd());
    }
    return clientIDs;
}

Client* Server::getClientByID(int clientID) const {
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() == clientID) {
            return *it;
        }
    }
    return NULL;
}

Channel* Server::getChannelByName(const std::string& channelName) const {
    for (std::vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if ((*it)->getName() == channelName) {
            return *it;
        }
    }
    return NULL;
}

bool Server::isClientOpInChannel(int clientID, const std::string& channelName) const {
    Channel* channel = getChannelByName(channelName);
    if (channel == NULL) {
        return false;
    }
    return channel->isOperator(clientID);
}

const std::vector<Channel *> &Server::getChannels() const {
    return _channels;
}
