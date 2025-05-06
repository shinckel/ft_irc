/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shinckel <shinckel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:52:49 by shinckel          #+#    #+#             */
/*   Updated: 2025/04/08 13:05:58 by shinckel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

# include <cstring>
# include <stdlib.h>
# include <unistd.h>
# include <string>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <vector>
# include <sstream>
# include <algorithm>
# include <map>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdexcept> // For std::runtime_error?

# include "Error.hpp"
# include "Manager.hpp"
# include "Parser.hpp"
# include "Client.hpp"

# define LOG(x) std::cout << x << std::endl;
# define ERROR(x) std::cerr << "\033[1;41m " + std::string(x) + " \033[0m" << std::endl;

#define BACKLOG 10 // Number of pending connections queu

/*
Server Socket:
The server creates a socket and binds it to a specific port (e.g. 6667).
It listens for incoming connections from clients.

Client Socket:
Each client (HexChat) creates its own socket to connect to the server's socket.

Data Exchange:
Once connected, the server and client use their respective sockets to exchange messages (e.g., joining channels, sending private messages).

Non-Blocking I/O:
Using poll(), the server can monitor multiple sockets (one for each client) and handle data as it arrives, ensuring efficient communication.
*/

// this is the starting point: create a socket to speak to other programs using standard Unix file descriptors
// a socket is an endpoint for communication between two machines over a network
class Socket {
  public:
    Socket(std::string port, std::string password);
    ~Socket();
    void startServer(std::string port, std::string password);

  private:
    std::string _port; // bind socket to a specific port
    std::string _pass; // authenticate clients when they connect to the server

    void  createSocket(int &server_fd);
    void  bindSocket(int server_fd, const std::string &port);
    void  startListening(int server_fd);
    void  acceptClientConnections(int server_fd);
};

#endif
