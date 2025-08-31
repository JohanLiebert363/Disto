#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in6 serv_addr;
    char buffer[1024] = {0};

    
    if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(PORT);
    
    if (inet_pton(AF_INET6, "2a06:63c4:e00:2d00:ea7e:abb3:71f2:b918", &serv_addr.sin6_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }


    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to the server!" << std::endl;
    while (true) {
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        send(sock, message.c_str(), message.length(), 0);

        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 1024);
        if (valread > 0) {
            std::cout << "Server: " << buffer << std::endl;
        }
    }

    close(sock);
    return 0;
}
