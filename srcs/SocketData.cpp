#include "../headers/Socket.hpp"

void  Socket::acceptClientConnections(int server_fd) {
  while (true) {
    struct sockaddr_in client_addr;
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
}