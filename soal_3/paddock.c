// paddock.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888

#include "actions.c"

void log_message(char* source, char* command, char* info) {
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
    fprintf(fptr, "[%s] [%s]: [%s] [%s]\n", source, timestamp, command, info);
    fclose(fptr);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    int len, n;
    len = sizeof(cliaddr);

    while (1) {
        n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        char command[20], info[20];
        sscanf(buffer, "%s %s", command, info);

        char response[100];

        if (strcmp(command, "Gap") == 0) {
            float distance = atof(info);
            strcpy(response, Gap(distance));
        } else if (strcmp(command, "Fuel") == 0) {
            int fuel = atoi(info);
            strcpy(response, Fuel(fuel));
        } else if (strcmp(command, "Tire") == 0) {
            int tire = atoi(info);
            strcpy(response, Tire(tire));
        } else if (strcmp(command, "TireChange") == 0) {
            strcpy(response, TireChange(info));
        } else {
            strcpy(response, "Unknown command");
        }

        log_message("Paddock", command, response);

        printf("[Paddock]: [%s] [%s]\n", command, response);

        sendto(sockfd, (const char *)response, strlen(response), 0, (const struct sockaddr *)&cliaddr, len);
    }

    return 0;
}

