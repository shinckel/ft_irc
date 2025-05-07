/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:52:49 by shinckel          #+#    #+#             */
/*   Updated: 2025/05/06 18:03:32 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server(std::string port, std::string password): _port(port), _pass(password) {
  LOG("Socket initialized");
  startServer(port, password);
}

Server::~Server() {
  LOG("Socket destroyed");
}

void Server::createSocket(int &server_fd, struct addrinfo *&server_info, const std::string &port) {
  struct addrinfo hints;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;      // support both IPv4 and IPv6
  hints.ai_socktype = SOCK_STREAM;  // use TCP stream sockets
  hints.ai_flags = AI_PASSIVE;      // bind to wildcard address

  int status = getaddrinfo(NULL, port.c_str(), &hints, &server_info); // use getaddrinfo to resolve address
  if (status != 0) {
    throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(status)));
  }
  
  // server_fd = socket(-1, SOCK_STREAM, 0); // force an error for checking ERROR log
  server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
  if (server_fd == -1) {
    freeaddrinfo(server_info); // don't forget to free addrinfo on error
    throw std::runtime_error("Socket creation failed");
  }
  LOG("Socket created successfully");
}

void Server::bindSocket(int server_fd, struct addrinfo *server_info) {
  // pass port 80 as first argument when running the executable for testing ERROR log
  if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
    freeaddrinfo(server_info);
    throw std::runtime_error("Bind failed on port");
  }
  LOG("Socket bound to port");
}

void Server::startListening(int server_fd) {
  (void)server_fd;
  // if (listen(-1, MAX_CLIENTS) == -1) { // invalid file descriptor, force error for checking ERROR log
  if (listen(server_fd, MAX_CLIENTS) == -1) {
    throw std::runtime_error("Listen failed");
  }
  LOG("Server is listening for incoming connections");
}

void Server::startServer(std::string port, std::string password) {
  int server_fd = -1;
  struct addrinfo *server_info = NULL;
  (void)password; // TODO: how to authenticate password?

  try {
    createSocket(server_fd, server_info, port); // step 1: create a socket
    bindSocket(server_fd, server_info);         // step 2: bind the socket to a port
    freeaddrinfo(server_info);
    startListening(server_fd);                  // step 3: listen for incoming connections
    // delegate client handling to SocketData...
    acceptClientConnections(server_fd);         // step 4: handle incoming client connections
  } catch (const std::runtime_error &e) {
    ERROR(e.what());
    if (server_fd != -1) {
      close(server_fd); // ensure the socket is closed on error
    }
    return;
  }

  // close server socket after exiting the loop
  close(server_fd);
}
