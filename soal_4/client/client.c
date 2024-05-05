#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX_LENGTH 1024

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
        printf("Please enter the command: ");
        memset(buffer, 0, MAX_LENGTH);
        fgets(buffer, MAX_LENGTH - 1, stdin);
         if (strncmp(buffer, "exit", 4) == 0)
            break;

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");

        memset(buffer, 0, MAX_LENGTH);
        n = read(sockfd, buffer, MAX_LENGTH - 1);
        if (n < 0)
            error("ERROR reading from socket");

        printf("Server response: %s\n", buffer);
       
    }

    close(sockfd);
    return 0;
}

