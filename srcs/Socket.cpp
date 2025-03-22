/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:52:49 by shinckel          #+#    #+#             */
/*   Updated: 2025/03/22 20:52:49 by shinckel         ###   ########.fr       */
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

// port 6667
// The server creates a socket and binds it to a specific port (e.g., 6667).
// It listens for incoming connections from clients.
void  Socket::startServer(std::string port, std::string password) {
  int server_fd;
  struct sockaddr_in  server_addr;
  (void)password;
  
  // step 1: create a socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    std::cerr << "Socket creation failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  // step 2: bind the socket to a port
  std::memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; // bind to all available interfaces
  server_addr.sin_port = htons(atoi(port.c_str())); // convert port to network byte order
  LOG(server_addr.sin_port);

  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    std::cerr << "Bind failed" << std::endl;
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // step 3: listen for incoming connections
  if (listen(server_fd, BACKLOG) == -1) {
    std::cerr << "Listen failed" << std::endl;
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  LOG("Server is listening on port " + port);

  // step 4: accept incoming connections (in a loop)
  while(true) {
    struct sockaddr_in  client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
      std::cerr << "Accept failed" << std::endl;
      continue;
    }

    std::cout << "New connection from "
              << inet_ntoa(client_addr.sin_addr) << ":"
              << ntohs(client_addr.sin_port) << std::endl;
    
    close(client_fd);
  }

  // close server socket
  close(server_fd);
}

