/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:19:01 by shinckel          #+#    #+#             */
/*   Updated: 2025/05/06 20:42:01 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Socket.hpp"

void Socket::handleClient(int client_fd) {
  char buffer[1024];
  std::memset(buffer, 0, sizeof(buffer));

  int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
  if (bytes_received <= 0) {
      std::stringstream ss;
      ss << client_fd;

      if (bytes_received == 0) {
          LOG("Client disconnected: FD " + ss.str());
      } else {
          ERROR("Recv failed: " + std::string(strerror(errno)));
      }
      // remove the client from the map and close the socket
      _clients.erase(client_fd);
      close(client_fd);
      return;
  }

  std::string message(buffer);
  std::stringstream ss;
  ss << client_fd;
  LOG("Received from client FD " + ss.str() + ": " + message);

  Client &client = _clients[client_fd];
  client.setCommand(message);

  // Echo the message back to the client
  send(client_fd, message.c_str(), message.size(), 0);
}

void Socket::acceptClientConnections(int server_fd) {
  struct pollfd fds[MAX_CLIENTS];
  int nfds = 1;

  fds[0].fd = server_fd;
  fds[0].events = POLLIN;

  while (true) {
      int poll_count = poll(fds, nfds, -1);
      if (poll_count == -1) {
          ERROR("Poll failed: " + std::string(strerror(errno)));
          break;
      }

      for (int i = 0; i < nfds; ++i) {
          if (fds[i].revents & POLLIN) {
              if (fds[i].fd == server_fd) {
                  struct sockaddr_storage client_addr;
                  socklen_t client_len = sizeof(client_addr);
                  int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

                  if (client_fd == -1) {
                      ERROR("Accept failed: " + std::string(strerror(errno)));
                      continue;
                  }

                  // add client to poll list
                  fds[nfds].fd = client_fd;
                  fds[nfds].events = POLLIN;
                  nfds++;

                  // create a new Client object and store it in the map
                  _clients[client_fd] = Client(client_fd);

                  std::stringstream ss;
                  ss << client_fd;
                  LOG("New client connected: FD " + ss.str());
              } else {
                  handleClient(fds[i].fd);
              }
          }
      }
  }
}