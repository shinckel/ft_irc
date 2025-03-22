// run < g++ -Wall -Werror -Wextra -std=c++98 -o ircserv ircserv.cpp >
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <map>
#include <set>
#include <cstdio> // Include this header for perror

#define MAX_CLIENTS 100

enum ClientState {
    CONNECTED,
    AUTHENTICATED
};

struct Client {
    int socket;
    std::string nickname;
    ClientState state;
};

void setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

void broadcastMessage(const std::string& message, const std::set<int>& clients, int sender_socket) {
    for (std::set<int>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        int client_socket = *it;
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.size(), 0);
        }
    }
}

void startServer(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Binding the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set the server socket to non-blocking mode
    setNonBlocking(server_fd);

    std::cout << "Server started on port " << port << std::endl;

    std::vector<struct pollfd> poll_fds;
    std::map<int, Client> clients;
    std::set<int> channel_clients;
    struct pollfd server_poll_fd;
    server_poll_fd.fd = server_fd;
    server_poll_fd.events = POLLIN;
    poll_fds.push_back(server_poll_fd);

    // Accepting incoming connections
    while (true) {
        int poll_count = poll(&poll_fds[0], poll_fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < poll_fds.size(); ++i) {
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == server_fd) {
                    // New incoming connection
                    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                        perror("accept");
                        continue;
                    }
                    std::cout << "New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;

                    // Set the new socket to non-blocking mode
                    setNonBlocking(new_socket);

                    struct pollfd client_poll_fd;
                    client_poll_fd.fd = new_socket;
                    client_poll_fd.events = POLLIN;
                    poll_fds.push_back(client_poll_fd);

                    Client client;
                    client.socket = new_socket;
                    client.state = CONNECTED;
                    clients[new_socket] = client;
                } else {
                    // Handle data from a client
                    int client_socket = poll_fds[i].fd;
                    int valread = read(client_socket, buffer, 1024);
                    if (valread <= 0) {
                        // Client disconnected
                        std::cout << "Client disconnected" << std::endl;
                        close(client_socket);
                        poll_fds.erase(poll_fds.begin() + i);
                        clients.erase(client_socket);
                        channel_clients.erase(client_socket);
                        --i;
                    } else {
                        std::string client_message(buffer, valread);
                        std::cout << "Received message: " << client_message << std::endl;

                        if (clients[client_socket].state == CONNECTED) {
                            if (client_message.substr(0, 5) == "NICK ") {
                                clients[client_socket].nickname = client_message.substr(5);
                                clients[client_socket].state = AUTHENTICATED;
                                std::string welcome_message = "Welcome to the IRC server, " + clients[client_socket].nickname + "!\n";
                                send(client_socket, welcome_message.c_str(), welcome_message.size(), 0);
                                channel_clients.insert(client_socket);
                                std::cout << "Client authenticated successfully with nickname: " << clients[client_socket].nickname << std::endl;
                            } else {
                                std::string error_message = "Please set your nickname using the NICK command.\n";
                                send(client_socket, error_message.c_str(), error_message.size(), 0);
                            }
                        } else if (clients[client_socket].state == AUTHENTICATED) {
                            std::string message = clients[client_socket].nickname + ": " + client_message;
                            broadcastMessage(message, channel_clients, client_socket);
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);

    startServer(port);

    return 0;
}
