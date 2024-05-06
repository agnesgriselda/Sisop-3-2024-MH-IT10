#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX_LENGTH 8192

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[MAX_LENGTH];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error connecting to server\n");
        return -1;
    }

    while (1) {
    printf("You: ");
    memset(buffer, 0, MAX_LENGTH);
    fgets(buffer, MAX_LENGTH - 1, stdin);
    
    // Send the message to the server
    send(sockfd, buffer, strlen(buffer), 0);

    // Check if the user wants to exit
    if (strncmp(buffer, "exit", 4) == 0)
        break;

    memset(buffer, 0, MAX_LENGTH);

    // Read the response from the server
    n = read(sockfd, buffer, MAX_LENGTH - 1);
    if (n < 0)
        error("ERROR reading from socket");

    printf("Server: %s\n", buffer);
}
    printf("Exiting the client\n");
    close(sockfd);
    return 0;
}
