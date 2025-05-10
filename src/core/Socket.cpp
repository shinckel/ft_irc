#include "Server.hpp"

// step 0: check arguments validity
// step 1: create a socket
// step 2: bind the socket to a port
// step 3: listen for incoming connections
// step 4: handle incoming client connections
// step 5: handle client data
// step 6: handle client disconnections
// step 7: handle client messages
// step 8: handle client actions
// step 9: close server socket after exiting the loop

// constructor: initializes the socket with the given port and password
Server::Server(std::string port, std::string password) : _port(port) {
    std::cout << "Socket Constructor called" << std::endl;
    Manager::setPassword(password); // set the server password
    parsePortPass(port, password);  // validate port and password
    initSocket();                   // initialize the server socket
    startMainLoop();                // start the main event loop
}

Server::~Server() {
    std::cout << "Socket destroyed" << std::endl;
}

void Server::parsePortPass(std::string port, std::string password) {
    // Verifique se o valor do `port` é válido antes de usá-lo
    if (port.empty()) {
        std::cerr << "Port is not valid" << std::endl;
        exit(1); // ou outro erro apropriado
    }

    // Supondo que você tenha uma validação para a senha:
    if (password.empty()) {
        std::cerr << "Password is not set" << std::endl;
        exit(1); // ou outro erro apropriado
    }
}

// void Server::parsePortPass(std::string port, std::string password) {
//     if (port.length() <= 0 || atoi(port.c_str()) <= MIN_PORT || atoi(port.c_str()) > MAX_PORT) {
//         std::cout << "Invalid port" << std::endl;
//         exit(1);
//     }
//     if (password.length() == 0) {
//         std::cout << "Invalid Password" << std::endl;
//         exit(1);
//     }
// }

// creates and returns the socket address structure
struct addrinfo *Server::socketAddress() {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof(hints)); // clear the hints structure
    hints.ai_family = AF_UNSPEC;     // allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // use TCP
    hints.ai_flags = AI_PASSIVE;     // use the local IP address

    // get address info for the server
    if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        exit(1);
    }
    return servinfo;
}

// initializes the server socket, binds it, and starts listening
void Server::initSocket() {
    struct addrinfo *server_socket, *p;
    int check_returns = 0;
    int yes = 1;

    server_socket = socketAddress(); // get the server address info
    if (server_socket == NULL) {
        std::cerr << "Error getting server address information" << std::endl;
        exit(1); // Exit se não conseguir obter as informações de endereço
    }
    for (p = server_socket; p != NULL; p = p->ai_next) {
        _socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_socketFd < 0) {
            std::cerr << "Could not open socket" << std::endl;
            continue; // Tenta o próximo endereço se falhar
        }
        setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)); // allow address reuse
        check_returns = bind(_socketFd, p->ai_addr, p->ai_addrlen);
        if (check_returns < 0) {
            std::cout << "couldnt bind the socket" << std::endl;
            close(_socketFd);
            continue;
        }
        break;
    }
    if (p == NULL) {
        std::cout << "Unable to bind" << std::endl;
        exit(1);
    }
    freeaddrinfo(server_socket); // free the address info structure
    check_returns = listen(_socketFd, BACKLOG); // start listening for connections
    if (check_returns < 0) {
        std::cout << "couldnt listen" << std::endl;
        exit(1);
    }
    std::cout << _socketFd << std::endl; // log the socket file descriptor
}

// main event loop: handles incoming connections and client data using poll()
// void Server::startMainLoop() {
//     Manager::createMap(); // initialize the manager

//     // vector to store pollfd structures for monitoring file descriptors
//     std::vector<struct pollfd> pollfds;

//     // add the listening socket to the pollfds vector
//     struct pollfd listenFd;
//     listenFd.fd = _socketFd;
//     listenFd.events = POLLIN; // monitor for incoming connections
//     listenFd.revents = 0; //IMPORTANTE
//     pollfds.push_back(listenFd);

//     while (1) {
//         // wait for events on the monitored file descriptors
//         int pollCount = poll(pollfds.data(), pollfds.size(), -1);
//         if (pollCount == -1) {
//             std::cerr << "Error polling()" << std::endl;
//             exit(4);
//         }

//         // iterate through the pollfds vector to handle events
//         std::vector<int> toRemove;

//         for (size_t i = 0; i < pollfds.size(); ++i) {
//             if (pollfds[i].revents & POLLIN) {
//                 if (pollfds[i].fd == _socketFd) {
//                     // Accept a new connection
//                     struct sockaddr_storage address;
//                     socklen_t addrlen = sizeof(address);
//                     int newFd = accept(_socketFd, (struct sockaddr *)&address, &addrlen);
//                     if (newFd == -1) {
//                         std::cerr << "Error accepting connection" << std::endl;
//                     } else {
//                         // Add new connection to pollfds
//                         struct pollfd clientFd;
//                         clientFd.fd = newFd;
//                         clientFd.events = POLLIN;
//                         clientFd.revents = 0;
//                         pollfds.push_back(clientFd);
//                         acceptedConnection(newFd); // handle new connection
//                     }
//                 } else {
//                     // Handle client data
//                     handleData(pollfds[i].fd);

//                     // If the client disconnected, mark for removal
//                     if (!Manager::isClient(pollfds[i].fd)) {
//                         toRemove.push_back(i); // Mark for removal after iteration
//                         continue; // Skip to the next iteration
//                     }
                    
//                 }
//             }
//         }

//         // Now, remove the disconnected clients from pollfds
//         for (size_t i = 0; i < toRemove.size(); ++i) {
//             close(pollfds[toRemove[i]].fd);
//             pollfds.erase(pollfds.begin() + toRemove[i]);
//         }
//     }
// }

void Server::startMainLoop() {
    Manager::createMap(); // initialize the manager

    // vector to store pollfd structures for monitoring file descriptors
    std::vector<struct pollfd> pollfds;

    // add the listening socket to the pollfds vector
    struct pollfd listenFd;
    listenFd.fd = _socketFd;
    listenFd.events = POLLIN; // monitor for incoming connections
    listenFd.revents = 0;     // explicitly initialize revents
    pollfds.push_back(listenFd);

    while (1) {
        // wait for events on the monitored file descriptors
        int pollCount = poll(pollfds.data(), pollfds.size(), -1);
        if (pollCount == -1) {
            std::cerr << "Error polling()" << std::endl;
            exit(4);
        }

        // iterate through the pollfds vector to handle events
        std::vector<int> toRemove;

        for (size_t i = 0; i < pollfds.size(); ++i) {
            if (pollfds[i].revents & POLLIN) {
                if (pollfds[i].fd == _socketFd) {
                    // Accept a new connection
                    struct sockaddr_storage address;
                    socklen_t addrlen = sizeof(address);
                    int newFd = accept(_socketFd, (struct sockaddr *)&address, &addrlen);
                    if (newFd == -1) {
                        std::cerr << "Error accepting connection" << std::endl;
                    } else {
                        // Add new connection to pollfds
                        struct pollfd clientFd;
                        clientFd.fd = newFd;
                        clientFd.events = POLLIN;
                        clientFd.revents = 0;
                        pollfds.push_back(clientFd);
                        acceptedConnection(newFd); // handle new connection
                    }
                } else {
                    // Check if the client still exists
                    if (!Manager::isClient(pollfds[i].fd)) {
                        toRemove.push_back(i); // Mark for removal
                        continue;             // Skip further processing for this fd
                    }

                    // Handle client data
                    handleData(pollfds[i].fd);

                    // If the client disconnected, mark for removal
                    if (!Manager::isClient(pollfds[i].fd)) {
                        toRemove.push_back(i); // Mark for removal after iteration
                    }
                }
            }
        }

        // Now, remove the disconnected clients from pollfds
        for (size_t i = 0; i < toRemove.size(); ++i) {
            close(pollfds[toRemove[i]].fd);
            pollfds.erase(pollfds.begin() + toRemove[i]);
        }
    }
}
