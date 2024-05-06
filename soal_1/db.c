#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Ukuran shared memory
#define DIRECTORY_PATH "microservices/database"  // Path direktori tujuan

void ensure_directory_exists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

void log_file_transfer(const char *filename, const char *type) {
    ensure_directory_exists(DIRECTORY_PATH);

    char log_file_path[256];
    snprintf(log_file_path, sizeof(log_file_path), "%s/db.log", DIRECTORY_PATH);

    FILE *log_fp = fopen(log_file_path, "a");
    if (log_fp == NULL) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%d/%m/%Y %H:%M:%S", tm_now);

    fprintf(log_fp, "[%s] [%s] [%s]\n", time_buffer, type, filename);
    fclose(log_fp);
}

int main() {
    int shm_id;
    key_t key = 5678;  // Kunci untuk shared memory yang sama dengan auth.c
    void *shm_ptr;

    // Mengakses shared memory yang sudah ada
    shm_id = shmget(key, SHM_SIZE, 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    // Mengaitkan shared memory
    shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // Membaca data dari shared memory
    char *data = (char *)shm_ptr;
    printf("Data dari shared memory: %s\n", data);

    // Simulasi pemindahan file dan log ke db.log
    char *type = strstr(data, "trashcan") ? "Trash Can" : "Parking Lot";
    log_file_transfer(data, type);

    // Detach dari shared memory
    shmdt(shm_ptr);

    // Hapus shared memory
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}

