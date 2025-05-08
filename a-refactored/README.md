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
Authentication: Validating server passwords and implementing user roles (e.g., admin privileges).

## High-level Overview

- Chat server in C++ for real-time communication through direct messages and group channels. Built to the C++98 standard, it uses non-blocking I/O and a single poll() operation to handle multiple connections efficiently and without delays.
- It allows users to join channels, send private messages, and participate in group discussions.

0. You must not develop an IRC client — **HexChat** was the chosen one.
1. You must not implement server-to-server communication.
2. Your executable will be run as follows: `./ircserv <port> <password>`
3. IRC server is a program that manages connection between IRC clients.
4. The Socket class is responsible for managing the server socket and accepting connections: `Socket.cpp` file focused on socket setup and server lifecycle management, `SocketData.cpp` handle client-specific operations.
5. The Client class represents individual connected clients and provides methods for communication.
6. Together, they form a server-client architecture where the Socket manages the server-side operations, and the Client handles the communication with each connected.

```c++
src/
├── core/               # Core server logic
│   ├── Socket.cpp
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── Channel.cpp
├── commands/           # IRC commands
│   ├── JoinCommand.cpp
│   ├── KickCommand.cpp
│   ├── TopicCommand.cpp
│   ├── ...
├── utils/              # Utility functions
│   ├── Parser.cpp
│   ├── Error.cpp
├── main.cpp
include/
├── core/
│   ├── Socket.hpp
│   ├── Server.hpp
│   ├── Client.hpp
│   ├── Channel.hpp
├── commands/
│   ├── JoinCommand.hpp
│   ├── KickCommand.hpp
│   ├── TopicCommand.hpp
│   ├── ...
├── utils/
│   ├── Parser.hpp
│   ├── Error.hpp
```

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
  if (listen(server_fd, MAX_CLIENTS) == -1) {
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
| **Socket** | `A PIPE BETWEEN TWO COMPUTERS ON A NETWORK THROUGH WHICH DATA FLOWS` | In the context of the IRC server, a socket is the mechanism that enables communication between the server and the connected clients (e.g., HexChat). They allow the server to: accept connections from multiple clients, exchange real-time messages, bind to a specific port for clients to connect, implement the IRC protocol for communication. **Biding**: biding ip address and port number to a dedicated socket. One for the server and the client itself will create a socket too (specifying the details of the server). |
| **Winsock** | `shortcut for windows sockets API - WSA` | Winsock is the networking API provided by Windows for socket programming. Since C++ does not have built-in networking capabilities (unlike Python or C#), developers must rely on the operating system's API to interact with networking resources. On Windows, Winsock requires initialization via `WSAStartup` and uses a DLL file to provide core socket functionality. **IMPORTANT**: On Linux-based systems like Ubuntu, networking is handled using the POSIX socket API, which is part of the standard C library. Unlike Windows, no additional initialization (e.g., `WSAStartup`) or external libraries are required. The socket API is ready to use out of the box, and functions like `socket()`, `bind()`, `listen()`, and `accept()` are used to create and manage sockets. This makes socket programming on Linux simpler and more straightforward compared to Windows. |
| **OSI** | `Open Systems Interconnection` | Send data from one application to another application across a network. **Application layer**: software. **Presentation layer**: break data into packages. **Session layer**: usages of OS API, interact with resources from operating system. Sockets. **Transport layer**: TCP/UDP protocols, transport packages. **Network layer**: IP address, identify each machine on the network. **Data link layer**: Ethernet (it has a MAC address), network interface card (NIC). Goes down to the **physical layer**: hardware, electronics, connection between machine and router. ![Screenshot 2025-05-06 at 10 22 57](https://github.com/user-attachments/assets/b95a5bfe-da84-452f-90dc-745b8ce09180) |
| **Architecture** | `client server architecture` | **Client**: Responsible for sending connection requests to a specific IP address and port number. The client initiates communication with the server. **Server**: Listens for incoming connection requests on a designated port and IP address. It handles client connections and manages communication. The server's primary responsibility is to accept and process client requests. |
| **IP Address** | | The IP address of both server and client is configured during Network setup unless it is allocated dynamically e.g. wireless. Loopback address: current machine `127.0.0.1`, useful for testing in the current machine first. |
| **Ports** | `16 bits address - integer between 1 and 65.535` | Ports 1 to 1023 are well know ports reserved for specific applications (port 21 FTP - file transfer protocol). Choose a number over 1024: usually the server port is specified via code and the client port via OS. **DATA CAN BE SENT FROM A PORT AND THEN RECEIVED BY A PORT. PORTS ARE LOCATED AT A SPECIFIC IP ADDRESS**. |
| **Streaming socket** | `SOCK_STREAM -> TCP` | |
| **Datagram socket** | `SOCK_DGRAM -> UDP` | Less secure, doesn't require connection. Therefore, it is faster. |
| **SSL** | | Secure Sockets Layer. TLS: transport layer security. |
| **Server functions** | x | ![Screenshot 2025-05-06 at 11 37 51](https://github.com/user-attachments/assets/99e6a185-6096-4b21-8202-cb4b03b7bfcf) |
| **Client functions** | x | ![Screenshot 2025-05-06 at 11 39 49](https://github.com/user-attachments/assets/ba49eca6-257a-4322-8805-432c51128a7d) |
| **Create socket** | `socket(server_socket->ai_family, server_socket->ai_socktype, server_socket->ai_protocol)` | `ai_family` specifies the address family (e.g., `AF_INET` for IPv4, `AF_INET6` for IPv6, or `AF_UNSPEC` for either). `ai_socktype`: Specifies the type of socket (e.g., `SOCK_STREAM` for TCP or `SOCK_DGRAM` for UDP). `ai_protocol`: Specifies the protocol to be used (e.g., `IPPROTO_TCP` for TCP). This is often set to `0` to automatically select the appropriate protocol. |

### References
[Internet Relay Chat: Communicate Outside Of Normal Channels](https://www.youtube.com/watch?v=FxZ2epcJ9l0)<br />
[Networking in C++](https://www.youtube.com/watch?v=jS9rBienEFQ)<br />
[Creating a Simple TCP Server in C++ using Winsock](https://medium.com/@tharunappu2004/creating-a-simple-tcp-server-in-c-using-winsock-b75dde86dd39)<br />
[C++ Network Programming Part 1: Sockets](https://www.youtube.com/watch?v=gntyAFoZp-E)<br />
[C++ Network Programming Part 2: TCP & UDP](https://www.youtube.com/watch?v=sXW_sNGvqcU)<br />
[GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets/tree/master/examples)<br />
