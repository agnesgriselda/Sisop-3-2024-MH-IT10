// driver.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h> // Include time.h for time-related functions


#define SERVER_IP "127.0.0.1"
#define PORT 8000

void log_message(char* command, char* info) {
    FILE *fptr;
    fptr = fopen("race.log", "a");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    time_t now;
    time(&now);
    char timestamp[20];
    strftime(timestamp, 20, "%d/%m/%Y %H:%M:%S", localtime(&now));
    fprintf(fptr, "[Driver] [%s]: [%s] [%s]\n", timestamp, command, info);
    fclose(fptr);
}

int main(int argc, char *argv[]) {
    int sockfd;
    char buffer[100];
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    char command[20], info[20];
    strcpy(command, argv[2]);
    strcpy(info, argv[4]);

    sprintf(buffer, "%s %s", command, info);
    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    log_message(command, info); // log command and info

    printf("[Driver]: [%s] [%s]\n", command, info);

    int n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 0, NULL, NULL);
    buffer[n] = '\0';
    printf("[Paddock]: [%s]\n", buffer);

    close(sockfd);
    return 0;
}
