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

void Socket::acceptClientConnections(int server_fd) {
  while (true) {
    struct sockaddr_storage client_addr; // supports both IPv4 and IPv6
    socklen_t client_len = sizeof(client_addr);

    // accept a new client connection
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
      // ERROR("Accept failed: " + std::string(strerror(errno)));
      continue; // skip to the next iteration if accept() fails
    }

    // determine the client's IP address and port
    char client_ip[INET6_ADDRSTRLEN];
    std::ostringstream oss;
    if (client_addr.ss_family == AF_INET) { // IPv4
      struct sockaddr_in *addr_in = (struct sockaddr_in*)&client_addr;
      inet_ntop(AF_INET, &(addr_in->sin_addr), client_ip, sizeof(client_ip));
      oss << ntohs(addr_in->sin_port);
      LOG("New connection from " + std::string(client_ip) + ":" + oss.str());
    } else if (client_addr.ss_family == AF_INET6) { // IPv6
      struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6*)&client_addr;
      inet_ntop(AF_INET6, &(addr_in6->sin6_addr), client_ip, sizeof(client_ip));
      oss << ntohs(addr_in6->sin6_port);
      LOG("New connection from " + std::string(client_ip)  + ":" + oss.str());
    }

    // TODO: Delegate client handling to another function or thread
    close(client_fd);
  }
}