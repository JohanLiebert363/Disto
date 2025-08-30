#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address, serv_addr;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *welcome_message = "Welcome to the chat server!";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << "..." << std::endl;

    // Accept a client connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Send a welcome message to the client
    send(new_socket, welcome_message, strlen(welcome_message), 0);
    std::cout << "Welcome message sent to client." << std::endl;

    // Replace "127.0.0.1" with the server's IP address
    if (inet_pton(AF_INET, "192.168.1.45", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Receive and echo messages
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            std::cout << "Client: " << buffer << std::endl;
            send(new_socket, buffer, strlen(buffer), 0); // Echo the message back
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
