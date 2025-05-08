#include "utils/Error.hpp"

void Error::printServerInfo(struct addrinfo* serverInfo) {
    char ipStr[INET_ADDRSTRLEN]; // Buffer to store the IP address as a string

    for (struct addrinfo* p = serverInfo; p != NULL; p = p->ai_next) {
        // Address family
        std::cout << "Address Family: ";
        if (p->ai_family == AF_INET) {
            std::cout << "IPv4" << std::endl;
        } else if (p->ai_family == AF_INET6) {
            std::cout << "IPv6" << std::endl;
        } else {
            std::cout << "Unknown" << std::endl;
        }

        // Socket type
        std::cout << "Socket Type: ";
        if (p->ai_socktype == SOCK_STREAM) {
            std::cout << "Stream (TCP)" << std::endl;
        } else if (p->ai_socktype == SOCK_DGRAM) {
            std::cout << "Datagram (UDP)" << std::endl;
        } else {
            std::cout << "Unknown" << std::endl;
        }

        // Protocol
        std::cout << "Protocol: ";
        if (p->ai_protocol == IPPROTO_TCP) {
            std::cout << "TCP" << std::endl;
        } else if (p->ai_protocol == IPPROTO_UDP) {
            std::cout << "UDP" << std::endl;
        } else {
            std::cout << "Unknown" << std::endl;
        }

        // Convert the IP address to a human-readable string
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, sizeof(ipStr));
            std::cout << "IP Address: " << ipStr << std::endl;
            std::cout << "Port: " << ntohs(ipv4->sin_port) << std::endl;
        } else if (p->ai_family == AF_INET6) { // IPv6
            char ipStr6[INET6_ADDRSTRLEN];
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipStr6, sizeof(ipStr6));
            std::cout << "IP Address: " << ipStr6 << std::endl;
            std::cout << "Port: " << ntohs(ipv6->sin6_port) << std::endl;
        }

        std::cout << "----------------------------------------" << std::endl;
    }
}
