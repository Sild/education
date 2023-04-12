#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket_name1");

    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    listen(sockfd, 5);

    std::cout << "Listening on socket " << addr.sun_path << std::endl;

    // Accept incoming connections
    int clientfd;
    struct sockaddr_un client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    while ((clientfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_addr_size)) > 0) {
        std::cout << "Received connection from " << client_addr.sun_path << std::endl;

        // Read data from the connection
        char buffer[1024];
        int num_bytes = read(clientfd, buffer, sizeof(buffer));
        if (num_bytes < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            return 1;
        }
        buffer[num_bytes] = '\0';

        // Print the received message
        std::cout << "Received message: " << buffer << std::endl;

        // Close the connection
        close(clientfd);
    }

    if (clientfd < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        return 1;
    }

    return 0;
}
