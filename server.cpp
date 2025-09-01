#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in6 address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *welcome_message = "Welcome to the IPv6 chat server!";


    if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any; 
    address.sin6_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << " (IPv6)..." << std::endl;
    std::cout << "Connecting to hte server...[ 67 ] " << std::endl;


    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }


    send(new_socket, welcome_message, strlen(welcome_message), 0);
    std::cout << "Welcome message sent to client." << std::endl;

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