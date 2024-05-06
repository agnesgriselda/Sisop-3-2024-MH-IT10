#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Ukuran shared memory

int main() {
    int shm_id;
    key_t key = 5678;  // Kunci untuk shared memory
    char *data = "Data CSV yang lolos autentikasi...";  // Data contoh
    void *shm_ptr;

    // Membuat shared memory
    shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
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

    // Menulis data ke shared memory
    memcpy(shm_ptr, data, strlen(data) + 1);

    // Detach dari shared memory (tetap hidup sampai dihapus)
    shmdt(shm_ptr);

    printf("Data telah disimpan di shared memory.\n");

    return 0;
}

