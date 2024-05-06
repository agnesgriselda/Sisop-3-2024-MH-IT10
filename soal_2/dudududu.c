#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void convertToNumber(char* string, int* number) {
    if (strcmp(string, "nol") == 0) {
        *number = 0;
    } else if (strcmp(string, "satu") == 0) {
        *number = 1;
    } else if (strcmp(string, "dua") == 0) {
        *number = 2;
    } else if (strcmp(string, "tiga") == 0) {
        *number = 3;
    } else if (strcmp(string, "empat") == 0) {
        *number = 4;
    } else if (strcmp(string, "lima") == 0) {
        *number = 5;
    } else if (strcmp(string, "enam") == 0) {
        *number = 6;
    } else if (strcmp(string, "tujuh") == 0) {
        *number = 7;
    } else if (strcmp(string, "delapan") == 0) {
        *number = 8;
    } else if (strcmp(string, "sembilan") == 0) {
        *number = 9;
    } else {
        // Jika string tidak cocok dengan angka yang dikenali, maka atur *number menjadi -1
        *number = -1;
    }
}
void convertToWords(int number, char* words) {
    char* ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};
    char* tens[] = {"", "sepuluh", "dua puluh", "tiga puluh", "empat puluh", "lima puluh", "enam puluh", "tujuh puluh", "delapan puluh", "sembilan puluh"};

    if (number < 10) {
        strcpy(words, ones[number]);
    } else if (number < 20) {
        switch (number) {
            case 10: strcpy(words, "sepuluh"); break;
            case 11: strcpy(words, "sebelas"); break;
            default: sprintf(words, "%s belas", ones[number - 10]); break;
        }
    } else {
        sprintf(words, "%s %s", tens[number / 10], ones[number % 10]);
    }
}
void writeToLog(char* type, int operand1, int operand2, int result) {
    FILE* logFile = fopen("histori.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S", timeinfo);

    char operand1Str[20], operand2Str[20], resultStr[20];
    convertToWords(operand1, operand1Str);
    convertToWords(operand2, operand2Str);
    convertToWords(result, resultStr);

    if (strcmp(type, "KURANG") == 0) {
        if (result == 0) {
            fprintf(logFile, "[%s] [%s] %s %s %s sama dengan nol.\n", timestamp, type, operand1Str, type + 1, operand2Str);
        } else if (operand1 < operand2) {
            fprintf(logFile, "[%s] [%s] ERROR pada pengurangan.\n", timestamp, type);
        } else {
            fprintf(logFile, "[%s] [%s] %s %s %s sama dengan %s.\n", timestamp, type, operand1Str, type + 1, operand2Str, resultStr);
        }
    } else {
        fprintf(logFile, "[%s] [%s] %s %s %s sama dengan %s.\n", timestamp, type, operand1Str, type + 1, operand2Str, resultStr);
    }
    fclose(logFile);
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [operasi]\n", argv[0]);
        return 1;
    }
    // Memeriksa apakah operasi yang diberikan adalah operasi yang valid
    if (!(strcmp(argv[1], "-tambah") == 0 || strcmp(argv[1], "-kurang") == 0 ||
          strcmp(argv[1], "-bagi") == 0 || strcmp(argv[1], "-kali") == 0)) {
        printf("Usage: %s [operasi]\n", argv[0]);
        return 1;
    }
    int fd1[2]; // pipe 1
    int fd2[2]; // pipe 2

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe failed");
        return 1;
    }
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    int num1, num2;
    if (pid > 0) { // Parent process
        close(fd1[0]); // Close unused write end of pipe 1

        char input[20];

        printf("Enter operand 1: ");
        scanf("%s", input);
        convertToNumber(input, &num1);

        printf("Enter operand 2: ");
        scanf("%s", input);
        convertToNumber(input, &num2);

        if (num1 == -1 || num2 == -1) {
            printf("Invalid input. Please enter numbers from zero to nine.\n");
            return 1;
        }
        int result;
        if (strcmp(argv[1], "-kali") == 0) {
            result = num1 * num2;
            writeToLog("KALI", num1, num2, result);
        } else if (strcmp(argv[1], "-tambah") == 0) {
            result = num1 + num2;
            writeToLog("TAMBAH", num1, num2, result);
        } else if (strcmp(argv[1], "-kurang") == 0) {
            result = num1 - num2;
            writeToLog("KURANG", num1, num2, result);
        } else if (strcmp(argv[1], "-bagi") == 0) {
            if (num2 == 0) {
                printf("ERROR: Division by zero is not allowed.\n");
                return 1;
            }
            result = num1 / num2;
            writeToLog("BAGI", num1, num2, result);
        }
        write(fd1[1], &result, sizeof(result));
        close(fd1[1]);

        wait(NULL);
    } else { // Child process
        close(fd1[1]); // Close unused write end of pipe 1

        int result;
        read(fd1[0], &result, sizeof(result));
        close(fd1[0]);

        writeToLog(argv[1] + 1, num1, num2, result);
    }
    return 0;
}
