# Internet Relay Chat (IRC)
This project was developed for 42 school. For comprehensive information regarding the requirements, please consult the PDF file in the subject folder of the repository. Furthermore, I have provided my notes and a concise summary below.

```diff
+ keywords: early real time chat communication — how do I send data from one thing to another?
+ client server setup - run a client in your pc, that connects to a server. Everybody else connected to that server can communicate in real time.
```

## Group Project Division

This project was completed as a group effort. The tasks were divided as follows:

### Person 1: Networking Layer (@shinckel)
Socket Setup: Creating, binding, and listening on the server socket.
Client Management: Handling connected clients, storing their details, and managing communication.
Connection Handling: Using poll() to manage multiple simultaneous connections.

### Person 2: Protocol and Features (@DaniSotti)
Command Parsing: Implementing the IRC protocol commands (e.g., /join, /msg, /quit).
Channel Management: Managing chat rooms, adding/removing users, and broadcasting messages.

### Person 3: Protocol and Features (@zecalejo)
Authentication: Validating server passwords and implementing user roles (e.g., admin privileges).
Tests: checking for errors and fixing bugs.

## High-level Overview

- Chat server in C++ for real-time communication through direct messages and group channels. Built to the C++98 standard, it uses non-blocking I/O and a single poll() operation to handle multiple connections efficiently and without delays.
- It allows users to join channels, send private messages, and participate in group discussions.

0. You must not develop an IRC client — **HexChat** was the chosen one.
1. You must not implement server-to-server communication.
2. Your executable will be run as follows: `./ircserv <port> <password>`
3. IRC server is a program that manages connection between IRC clients.

### Server Socket
The server creates a socket and binds it to a specific port (e.g., 6667). It listens for incoming connections from clients.

```c++
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
```

### Client Socket
Each client (HexChat) creates its own socket to connect to the server's socket.

### Data Exchange
Once connected, the server and client use their respective sockets to exchange messages (e.g., joining channels, sending private messages).

### Non-Blocking I/O
Using poll(), the server can monitor multiple sockets (one for each client) and handle data as it arrives.

## Concepts

| Task | Prototype | Description |
|:----|:-----:|:--------|
| **HexChat** | `ubuntu VM` `sudo apt update` `sudo apt install hexchat` | HexChat will try to open a graphical user interface (GUI), therefore, VM must be initialized with X11 forwarding enabled — install XQuartz, then enable X11 forwarding in SSH `ssh -X ubuntu@your_vm_ip`. Run `hexchat`. |
| **SSL** | | Secure Sockets Layer. TLS: transport layer security. |
| **socket** | | In the context of the IRC server, a socket is the mechanism that enables communication between the server and the connected clients (e.g., HexChat). They allow the server to: accept connections from multiple clients, exchange real-time messages, bind to a specific port for clients to connect, implement the IRC protocol for communication. |
| **Winsock** | | There is no builtin networking in C++ (python and C# have it). You will have to use the builtin operating system API (e.g. Winsock). |

### References
[Internet Relay Chat: Communicate Outside Of Normal Channels](https://www.youtube.com/watch?v=FxZ2epcJ9l0)<br />
[Networking in C++](https://www.youtube.com/watch?v=jS9rBienEFQ)
[Creating a Simple TCP Server in C++ using Winsock](https://medium.com/@tharunappu2004/creating-a-simple-tcp-server-in-c-using-winsock-b75dde86dd39)
[C++ Network Programming Part 1: Sockets](https://www.youtube.com/watch?v=gntyAFoZp-E)
[C++ Network Programming Part 2: TCP & UDP](https://www.youtube.com/watch?v=sXW_sNGvqcU)
