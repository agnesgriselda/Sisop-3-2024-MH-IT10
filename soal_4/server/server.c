#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <curl/curl.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 8192
#define MAX_RESPONSE_SIZE 8192
char CSV_FILE_NAME[MAX_BUFFER_SIZE] = "../myanimelist.csv";
char LOG_FILE_NAME[MAX_BUFFER_SIZE] = "../change.log";

char cwd[MAX_BUFFER_SIZE];

void downloadCSV()
{
    system("rm -f ../myanimelist.csv");
    system("wget --content-disposition --no-check-certificate \"https://drive.google.com/uc?export=download&id=10p_kzuOgaFY3WT6FVPJIXFbkej2s9f50\" -P ../");
}


void show(char *token, char *response) {
    // Open the CSV file for reading
    FILE *fp = fopen("../myanimelist.csv", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening CSV file\n");
        strcpy(response, "Error opening CSV file\n");
        return;
    }

    // Allocate memory for buffer to hold each line read from the file
    char *line = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        strcpy(response, "Memory allocation failed\n");
        fclose(fp);
        return;
    }

    // Buffer to store the response
    response[0] = '\0';

    // Read each line from the file
    while (fgets(line, MAX_BUFFER_SIZE, fp) != NULL) {
        // Check if the line contains the token
        if (strstr(line, token) != NULL) {
            // Append the line to the response
            strcat(response, line);
        }
    }

    // Close the file
    fclose(fp);

    // Free the allocated memory
    free(line);    
}

void showAll(char *response) {
    FILE *fp = fopen("../myanimelist.csv", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening CSV file\n");
        strcpy(response, "Error opening CSV file\n");
        return;
    }

    // Initialize response buffer
    response[0] = '\0';

    // Allocate memory for buffer to hold each line read from the file
    char line[MAX_BUFFER_SIZE];

    // Read each line from the file
    while (fgets(line, MAX_BUFFER_SIZE, fp) != NULL) {
        // Append the line to the response
        strcat(response, line);
    }

    // Close the file
    fclose(fp);
}


void add(char *token, char *response) {
    // Open the CSV file for appending
    FILE *csv = fopen("../myanimelist.csv", "a");
    if (csv == NULL) {
        fprintf(stderr, "Error opening CSV file\n");
        strcpy(response, "Error opening CSV file\n");
        return;
    }

    // Append the new entry to the file
    fprintf(csv, "\n%s", token);

    // Close the file
    fclose(csv);

    // Set success response
    strcpy(response, "Anime berhasil ditambahkan");

    // Log the addition to change.log
    FILE *log = fopen("../change.log", "a");
    if (log != NULL) {
        time_t currentTime;
        struct tm *localTime;
        // Get the current time
        time(&currentTime);
        localTime = localtime(&currentTime);
        // Write the log entry
        fprintf(log, "[%02d/%02d/%02d][ADD] %s ditambahkan.\n", 
                localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900, token);
        fclose(log);
    } else {
        fprintf(stderr, "Error opening change.log\n");
    }
}


void edit(char *token, char *response) {
    // Extract the previous and new values from the token
    char prev[MAX_BUFFER_SIZE];
    char new[MAX_BUFFER_SIZE];
    sscanf(token, "%[^,],%[^\n]", prev, new);

    // Open the CSV file for reading
    FILE *csv = fopen("../myanimelist.csv", "r");
    if (csv == NULL) {
        fprintf(stderr, "Error opening CSV file\n");
        strcpy(response, "Error opening CSV file\n");
        return;
    }

    // Open a temporary file for writing
    FILE *tmpFile = fopen("tmpfile", "w");
    if (tmpFile == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        strcpy(response, "Error creating temporary file\n");
        fclose(csv);
        return;
    }

    // Allocate memory for buffer to hold each line read from the file
    char *line = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        strcpy(response, "Memory allocation failed\n");
        fclose(csv);
        fclose(tmpFile);
        return;
    }

    // Flag to indicate if the entry was found and edited
    int edited = 0;

    // Read each line from the file
    while (fgets(line, MAX_BUFFER_SIZE, csv) != NULL) {
        char *found = strstr(line, prev);
        // Check if the line contains the previous value
        if (found != NULL) {
            // Move the file pointer back to the beginning of the line
            fseek(csv, -(strlen(line) - strlen(found)), SEEK_CUR);
            // Write the new content to the temporary file
            fprintf(tmpFile, "%s", new);
            edited = 1;
        } else {
            // Write the unchanged line to the temporary file
            fputs(line, tmpFile);
        }
    }

    // Close the files
    fclose(csv);
    fclose(tmpFile);

    // Replace the original file with the temporary file
    if (rename("tmpfile", "../myanimelist.csv") != 0) {
        fprintf(stderr, "Error renaming temporary file\n");
        strcpy(response, "Error renaming temporary file\n");
        free(line);
        return;
    }

    // Set response based on whether the entry was edited
    if (edited) {
        strcpy(response, "Anime berhasil diedit");
    } else {
        strcpy(response, "Anime tidak ditemukan untuk diedit");
    }

    // Log the edit to change.log
    FILE *log = fopen("../change.log", "a");
    if (log != NULL) {
        time_t currentTime;
        struct tm *localTime;
        // Get the current time
        time(&currentTime);
        localTime = localtime(&currentTime);
        // Write the log entry
        fprintf(log, "[%02d/%02d/%02d][EDIT] %s diubah menjadi %s.\n",
                localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900, prev, new);
        fclose(log);
    } else {
        fprintf(stderr, "Error opening change.log\n");
    }

    // Free the allocated memory
    free(line);
}

void delete(char *token, char *response) {
    // Open the CSV file for reading
    FILE *csv = fopen("../myanimelist.csv", "r");
    if (csv == NULL) {
        fprintf(stderr, "Error opening CSV file\n");
        strcpy(response, "Error opening CSV file\n");
        return;
    }

    // Open a temporary file for writing
    FILE *tmpFile = fopen("tmpfile", "w");
    if (tmpFile == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        strcpy(response, "Error creating temporary file\n");
        fclose(csv);
        return;
    }

    // Allocate memory for buffer to hold each line read from the file
    char *line = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        strcpy(response, "Memory allocation failed\n");
        fclose(csv);
        fclose(tmpFile);
        return;
    }

    // Flag to indicate if the entry was found and deleted
    int deleted = 0;

    // Read each line from the file
    while (fgets(line, MAX_BUFFER_SIZE, csv) != NULL) {
        char *found = strstr(line, token);
        // Check if the line contains the entry to be deleted
        if (found != NULL) {
            deleted = 1;
        } else {
            // Write the line to the temporary file if it doesn't contain the entry to be deleted
            fputs(line, tmpFile);
        }
    }

    // Close the files
    fclose(csv);
    fclose(tmpFile);

    // Replace the original file with the temporary file
    if (rename("tmpfile", "../myanimelist.csv") != 0) {
        fprintf(stderr, "Error renaming temporary file\n");
        strcpy(response, "Error renaming temporary file\n");
        free(line);
        return;
    }

    // Set response based on whether the entry was deleted
    if (deleted) {
        strcpy(response, "Anime berhasil dihapus");
    } else {
        strcpy(response, "Anime tidak ditemukan untuk dihapus");
    }

    // Log the deletion to change.log
    FILE *log = fopen("../change.log", "a");
    if (log != NULL) {
        time_t currentTime;
        struct tm *localTime;
        // Get the current time
        time(&currentTime);
        localTime = localtime(&currentTime);
        // Write the log entry
        fprintf(log, "[%02d/%02d/%02d][DEL] %s berhasil dihapus.\n",
                localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900, token);
        fclose(log);
    } else {
        fprintf(stderr, "Error opening change.log\n");
    }

    // Free the allocated memory
    free(line);
}

void handleRequest(char *buffer, char *response) {
    char command[MAX_BUFFER_SIZE];
    char token[MAX_BUFFER_SIZE];

    // Extract the command and token from the buffer
    sscanf(buffer, "%s %[^\n]", command, token);

    // Clear buffer and token to avoid residual data
    memset(buffer, 0, sizeof(buffer));

    // Process the command
    if (strcmp(command, "show") == 0) {
        if (strlen(token) == 0) {
            // If token is empty, show all entries
            printf("Showing all entries\n");
            showAll(response);
        } else {
            // Show entries based on the provided token
            show(token, response);
        }
    } else if (strcmp(command, "add") == 0) {
        // Add a new entry
        add(token, response);
    } else if (strcmp(command, "edit") == 0) {
        // Edit an existing entry
        edit(token, response);
    } else if (strcmp(command, "delete") == 0) {
        // Delete an entry
        delete(token, response);
    } else {
        // Invalid command
        strcpy(response, "Invalid command");
    }

    // Clear command and token to avoid residual data
    memset(buffer, 0, sizeof(buffer));
    memset(token, 0, sizeof(token));
}


int main() {
    // Download the CSV file
    downloadCSV();
    
    // Prepare paths for CSV and log files
    char csvPath[MAX_BUFFER_SIZE], logPath[MAX_BUFFER_SIZE];
    strcpy(csvPath, cwd);
    strcat(csvPath, CSV_FILE_NAME);
    strcpy(LOG_FILE_NAME, logPath);
    strcpy(logPath, cwd);
    strcat(logPath, LOG_FILE_NAME);

    // Socket variables
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    char buffer[MAX_BUFFER_SIZE];
    char response[MAX_RESPONSE_SIZE];

    // Check if memory allocation succeeded
    if (response == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Socket binding failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        perror("Socket listening failed");
        return -1;
    }

    // Accept connections and handle requests
    while (1) {
        addrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientSocket < 0) {
            perror("Error accepting connection");
            continue;
        }

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            memset(response, 0, sizeof(response));
            read(clientSocket, buffer, sizeof(buffer));

            printf("Received: %s\n", buffer);

            if (strcmp(buffer, "exit") == 0) {
                printf("Closing connection\n");
                close(clientSocket);
                break;
            }

            handleRequest(buffer, response);
            send(clientSocket, response, strlen(response), 0);
        }
    }

    // Free allocated memory and close socket
    close(serverSocket);
    return 0;
}
