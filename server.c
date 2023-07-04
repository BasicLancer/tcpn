#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd, newsockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0) {
        perror("Socket listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP server running on port %d\n", PORT);

    while (1) {
        // Accept client connection
        if ((newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
            perror("Socket accept failed");
            exit(EXIT_FAILURE);
        }

        // Receive message from client
        int msg_len = read(newsockfd, buffer, BUFFER_SIZE);
        if (msg_len < 0) {
            perror("Message receive failed");
            exit(EXIT_FAILURE);
        }

        buffer[msg_len] = '\0';
        printf("Received message: %s\n", buffer);

        // Send response to client
        if (write(newsockfd, buffer, msg_len) < 0) {
            perror("Message send failed");
            exit(EXIT_FAILURE);
        }
        printf("Response sent.\n");

        // Close the client socket
        close(newsockfd);
    }

    // Close the server socket
    close(sockfd);
    return 0;
}
