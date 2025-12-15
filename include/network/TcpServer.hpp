#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>      // For close()
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in

class TcpServer {
private:
    int server_fd;
    int client_fd;
    int port;

public:
    TcpServer(int _port) : port(_port), server_fd(-1), client_fd(-1) {}

    ~TcpServer() {
        if (client_fd >= 0) close(client_fd);
        if (server_fd >= 0) close(server_fd);
    }

    // 1. Start Listening
    void start() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "Error: Could not create socket\n";
            exit(EXIT_FAILURE);
        }

        // Bind to port
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
        address.sin_port = htons(port);       // Convert port to network byte order

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Error: Bind failed (Port " << port << " might be in use)\n";
            exit(EXIT_FAILURE);
        }

        // Listen
        if (listen(server_fd, 1) < 0) { // Backlog of 1 connection
            std::cerr << "Error: Listen failed\n";
            exit(EXIT_FAILURE);
        }

        std::cout << "Server listening on port " << port << "...\n";
    }

    // 2. Accept a Client (Blocking)
    void wait_for_connection() {
        std::cout << "Waiting for a client to connect...\n";
        int addrlen = sizeof(sockaddr_in);
        sockaddr_in address;
        
        client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            std::cerr << "Error: Accept failed\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "Client connected!\n";
    }

    // 3. Read a Message
    // Returns true if we got a message, false if client disconnected
    template<typename T>
    bool read_message(T& msg) {
        // We try to read exactly sizeof(T) bytes
        int bytes_read = read(client_fd, &msg, sizeof(T));
        
        if (bytes_read <= 0) {
            return false; // Connection closed or error
        }
        return true;
    }
};