#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
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
    
    printf("Data telah disimpan di shared memory.\n");

    // Skenario seolah-olah data baru saja ditulis, kita akan membaca dan mengolahnya
    // Membaca data dari shared memory
    printf("Data dari shared memory: %s\n", (char *)shm_ptr);

    // Disini anda bisa menambahkan fungsi untuk mengolah data, contohnya:
    // Misalnya mengolah data CSV atau parsing untuk keperluan lainnya.
    // Contoh: Hanya mencetak kembali untuk memastikan data terbaca dengan benar.
    printf("Mengolah data yang diterima: %s\n", (char *)shm_ptr);

    // Detach dari shared memory
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Hapus segment shared memory
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    printf("Shared memory telah dihapus.\n");

    return 0;
}


