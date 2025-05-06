/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:52:49 by shinckel          #+#    #+#             */
/*   Updated: 2025/04/08 13:06:44 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Socket.hpp"

Socket::Socket(std::string port, std::string password): _port(port), _pass(password) {
  LOG("Socket initialized");
  startServer(port, password);
}

Socket::~Socket() {
  LOG("Socket destroyed");
}

void Socket::createSocket(int &server_fd) {
  // server_fd = socket(-1, SOCK_STREAM, 0); // force an error for checking LOG
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    throw std::runtime_error("Socket creation failed");
  }
  LOG("Socket created successfully");
}

void Socket::bindSocket(int server_fd, const std::string &port) {
  // const std::string port2 = "80"; // Port 80 is often restricted or already in use, force an error for checking LOG
  // OR pass 80 as first argument, much easier
  struct sockaddr_in server_addr;
  std::memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; // bind to all available interfaces
  server_addr.sin_port = htons(atoi(port.c_str())); // convert port to network byte order

  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    throw std::runtime_error("Bind failed on port " + port);
  }
  LOG("Socket bound to port " + port);
}

void Socket::startListening(int server_fd) {
  (void)server_fd;
  // if (listen(-1, BACKLOG) == -1) { // invalid file descriptor, force error for checking LOG
  if (listen(server_fd, BACKLOG) == -1) {
    throw std::runtime_error("Listen failed");
  }
  LOG("Server is listening for incoming connections");
}

void Socket::startServer(std::string port, std::string password) {
  int server_fd;
  (void)password; // TODO: how to authenticate password?

  try {
    createSocket(server_fd);       // step 1: create a socket
    bindSocket(server_fd, port);   // step 2: bind the socket to a port
    startListening(server_fd);     // step 3: listen for incoming connections
    // delegate client handling to SocketData...
    acceptClientConnections(server_fd);  // step 4: handle incoming client connections
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
