# Modul 3
## Thread, IPC, & RPC

- Agnes Zenobia __Griselda__ Petrina (5027231034)
- Muhammad __Nafi__ Firdaus (5027231045)
- __Rafika__ Az Zahra Kusumastuti (5027231050)

# Soal 2
Max Verstappen 🏎️ seorang pembalap F1 dan programer memiliki seorang adik bernama Min Verstappen (masih SD) sedang menghadapi tahap paling kelam dalam kehidupan yaitu perkalian matematika, Min meminta bantuan Max untuk membuat kalkulator perkalian sederhana (satu sampai sembilan). Sembari Max nguli dia menyuruh Min untuk belajar perkalian dari web (referensi) agar tidak bergantung pada kalkulator.
(Wajib menerapkan konsep pipes dan fork seperti yang dijelaskan di modul Sisop. Gunakan 2 pipes dengan diagram seperti di modul 3).

a. Sesuai request dari adiknya Max ingin nama programnya dudududu.c. Sebelum program parent process dan child process, ada input dari user berupa 2 string. Contoh input: tiga tujuh. 

b. Pada parent process, program akan mengubah input menjadi angka dan melakukan perkalian dari angka yang telah diubah. Contoh: tiga tujuh menjadi 21. 

c. Pada child process, program akan mengubah hasil angka yang telah diperoleh dari parent process menjadi kalimat. Contoh: `21` menjadi “dua puluh satu”.

d. Max ingin membuat program kalkulator dapat melakukan penjumlahan, pengurangan, dan pembagian, maka pada program buatlah argumen untuk menjalankan program : 
	i. perkalian	: ./kalkulator -kali
 	ii. penjumlahan	: ./kalkulator -tambah
	iii. pengurangan	: ./kalkulator -kurang
	iv. pembagian	: ./kalkulator -bagi
Beberapa hari kemudian karena Max terpaksa keluar dari Australian Grand Prix 2024 membuat Max tidak bersemangat untuk melanjutkan programnya sehingga kalkulator yang dibuatnya cuma menampilkan hasil positif jika bernilai negatif maka program akan print “ERROR” serta cuma menampilkan bilangan bulat jika ada bilangan desimal maka dibulatkan ke bawah.

e. Setelah diberi semangat, Max pun melanjutkan programnya dia ingin (pada child process) kalimat akan di print dengan contoh format : 
	i. perkalian	: “hasil perkalian tiga dan tujuh adalah dua puluh satu.”
	ii. penjumlahan	: “hasil penjumlahan tiga dan tujuh adalah sepuluh.”
	iii. pengurangan	: “hasil pengurangan tujuh dan tiga adalah empat.”
	iv. pembagian	: “hasil pembagian tujuh dan tiga adalah dua.”

f. Max ingin hasil dari setiap perhitungan dicatat dalam sebuah log yang diberi nama histori.log. Pada parent process, lakukan pembuatan file log berdasarkan data yang dikirim dari child process. 
	- Format: [date] [type] [message]
	- Type: KALI, TAMBAH, KURANG, BAGI
	- Ex:
		1. [10/03/24 00:29:47] [KALI] tujuh kali enam sama dengan empat puluh dua.
		2. [10/03/24 00:30:00] [TAMBAH] sembilan tambah sepuluh sama dengan sembilan belas.
		3. [10/03/24 00:30:12] [KURANG] ERROR pada pengurangan.

# Penyelesaian Soal 2
Penjelasan Kode :

### File dudududu.c

# Soal 3

Shall Leglerg🥶 dan Carloss Signs 😎 adalah seorang pembalap F1 untuk tim Ferrari 🥵. Mobil F1 memiliki banyak pengaturan, seperti penghematan ERS, Fuel, Tire Wear dan lainnya. Pada minggu ini ada race di sirkuit Silverstone. Malangnya, seluruh tim Ferrari diracun oleh Super Max Max pada hari sabtu sehingga seluruh kru tim Ferrari tidak bisa membantu Shall Leglerg🥶 dan Carloss Signs 😎 dalam race. Namun, kru Ferrari telah menyiapkan program yang bisa membantu mereka dalam menyelesaikan race secara optimal. Program yang dibuat bisa mengatur pengaturan - pengaturan dalam mobil F1 yang digunakan dalam balapan. Programnya ber ketentuan sebagai berikut:


- Pada program actions.c, program akan berisi function function yang bisa di call oleh paddock.c
Action berisikan sebagai berikut:

a. Gap [Jarak dengan driver di depan (float)]: Jika Jarak antara Driver dengan Musuh di depan adalah < 3.5s maka return Gogogo, jika jarak > 3.5s dan 10s return Push, dan jika jarak > 10s maka return Stay out of trouble.

b. Fuel [Sisa Bensin% (string/int/float)]: Jika bensin lebih dari 80% maka return Push Push Push, jika bensin di antara 50% dan 80% maka return You can go, dan jika bensin kurang dari 50% return Conserve Fuel.

c. Tire [Sisa Ban (int)]: Jika pemakaian ban lebih dari 80 maka return Go Push Go Push, jika pemakaian ban diantara 50 dan 80 return Good Tire Wear, jika pemakaian di antara 30 dan 50 return Conserve Your Tire, dan jika pemakaian ban kurang dari 30 maka return Box Box Box.

d. Tire Change [Tipe ban saat ini (string)]: Jika tipe ban adalah Soft return Mediums Ready, dan jika tipe ban Medium return Box for Softs.

		Contoh:
		[Driver] : [Fuel] [55%]
		[Paddock]: [You can go]

- Pada paddock.c program berjalan secara daemon di background, bisa terhubung dengan driver.c melalui socket RPC.
- Program paddock.c dapat call function yang berada di dalam actions.c.
- Program paddock.c tidak keluar/terminate saat terjadi error dan akan log semua percakapan antara paddock.c dan driver.c di dalam file race.log
```
Format log:
[Source] [DD/MM/YY hh:mm:ss]: [Command] [Additional-info]
ex :
[Driver] [07/04/2024 08:34:50]: [Fuel] [55%]
[Paddock] [07/04/2024 08:34:51]: [Fuel] [You can go]
```

- Program driver.c bisa terhubung dengan paddock.c dan bisa mengirimkan pesan dan menerima pesan serta menampilan pesan tersebut dari paddock.c sesuai dengan perintah atau function call yang diberikan.
- Jika bisa digunakan antar device/os (non local) akan diberi nilai tambahan.
- untuk mengaktifkan RPC call dari driver.c, bisa digunakan in-program CLI atau Argv (bebas) yang penting bisa send command seperti poin B dan menampilkan balasan dari paddock.c


__Argv:__ 

./driver -c Fuel -i 55% 

__in-program CLI:__

Command: Fuel

Info: 55%

  
```
├── client
│   └── driver.c
└── server
    ├── actions.c
    ├── paddock.c
    └── race.log

```
# Penyelesaian Soal 3
Penjelasan Kode:

### File `actions.c`
- Berisi implementasi fungsi-fungsi yang menangani aksi-aksi yang dapat dilakukan dalam balapan.
- Fungsi `Gap(float distance)`: Menerima jarak sebagai parameter dan memberikan rekomendasi aksi berdasarkan jarak tersebut.
  ```c
  char* Gap(float distance) {
    if (distance < 3.5)
        return "Gogogo";
    else if (distance > 3.5 && distance <= 10)
        return "Push";
    else
        return "Stay out of trouble";
  ```
}
- Fungsi `Fuel(int fuel)`: Menerima jumlah bahan bakar sebagai parameter dan memberikan rekomendasi aksi berdasarkan jumlah bahan bakar tersebut.
```c
char* Fuel(int fuel) {
    if (fuel > 80)
        return "Push Push Push";
    else if (fuel >= 50 && fuel <= 80)
        return "You can go";
    else
        return "Conserve Fuel";
}
```
- Fungsi `Tire(int tire)`: Menerima keausan ban sebagai parameter dan memberikan rekomendasi aksi berdasarkan keausan ban tersebut.
```c
char* Tire(int tire) {
    if (tire > 80)
        return "Go Push Go Push";
    else if (tire >= 50 && tire <= 80)
        return "Good Tire Wear";
    else if (tire >= 30 && tire < 50)
        return "Conserve Your Tire";
    else
        return "Box Box Box";
}
```
- Fungsi `TireChange(char* current_tire)`: Menerima jenis ban saat ini sebagai parameter dan memberikan rekomendasi aksi untuk mengganti ban.
```c
char* TireChange(char* current_tire) {
    if (strcmp(current_tire, "Soft") == 0)
        return "Mediums Ready";
    else if (strcmp(current_tire, "Medium") == 0)
        return "Box for Softs";
    else
        return "Unknown tire type";
}
```

### File `paddock.c`
- Menggunakan fungsi-fungsi dari `actions.c`.
  ```c
  #include "actions.c"
  ```
- Menangani pesan dari driver dan memberikan respons berdasarkan perintah yang diberikan.
- Log pesan yang diterima dan responsnya ke dalam file `race.log`.
```c
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
```
```c
void log_driver_message(char* command, char* info) {
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
```
- Menerima pesan dari driver menggunakan protokol UDP.
```c
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
```
- Memberikan respons kepada driver berdasarkan perintah yang diterima.
```c
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
            log_driver_message(command, response);
        } else if (strcmp(command, "Fuel") == 0) {
            int fuel = atoi(info);
            strcpy(response, Fuel(fuel));
            log_driver_message(command, response);
        } else if (strcmp(command, "Tire") == 0) {
            int tire = atoi(info);
            strcpy(response, Tire(tire));
            log_driver_message(command, response);
        } else if (strcmp(command, "TireChange") == 0) {
            strcpy(response, TireChange(info));
            log_driver_message(command, response);
        } else {
            strcpy(response, "Unknown command");
            log_driver_message("Unknown", response);
        }

        log_message("Paddock", command, response);

        printf("[Paddock]: [%s] [%s]\n", command, response);

        sendto(sockfd, (const char *)response, strlen(response), 0, (const struct sockaddr *)&cliaddr, len);
    }

    return 0;
}
```

### File `driver.c`
- Mengirim pesan kepada paddock (server) untuk mendapatkan rekomendasi aksi berdasarkan perintah yang diberikan.
- Menggunakan protokol UDP untuk mengirim dan menerima pesan.
```c
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
```
- Log perintah yang dikirim ke dalam file `race.log`.
```c
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
```
- Menerima respons dari paddock dan mencetaknya.
```c
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
```
__Dokumentasi Output__


![Screenshot 2024-05-09 183503](https://github.com/agnesgriselda/Sisop-3-2024-MH-IT10/assets/150202762/d8583e0d-0cea-4204-a445-c3679050378d)


![Screenshot 2024-05-09 183517](https://github.com/agnesgriselda/Sisop-3-2024-MH-IT10/assets/150202762/441152b0-7180-43c7-9d47-7d65c1097895)


![Screenshot 2024-05-09 183541](https://github.com/agnesgriselda/Sisop-3-2024-MH-IT10/assets/150202762/6a0e5414-ea2b-4b01-81a0-932e815154c1)

# Soal 4

Lewis Hamilton 🏎 seorang wibu akut dan sering melewatkan beberapa episode yang karena sibuk menjadi asisten. Maka dari itu dia membuat list anime yang sedang ongoing (biar tidak lupa) dan yang completed (anime lama tapi pengen ditonton aja). Tapi setelah Lewis pikir-pikir malah kepikiran untuk membuat list anime. Jadi dia membuat file (harap diunduh) dan ingin menggunakan socket yang baru saja dipelajarinya untuk melakukan CRUD pada list animenya.

- Client dan server terhubung melalui socket. 
- client.c di dalam folder client dan server.c di dalam folder server
- Client berfungsi sebagai pengirim pesan dan dapat menerima pesan dari server.
- Server berfungsi sebagai penerima pesan dari client dan hanya menampilkan pesan perintah client saja.  
- Server digunakan untuk membaca myanimelist.csv. Dimana terjadi pengiriman data antara client ke server dan server ke client.
Menampilkan seluruh judul
Menampilkan judul berdasarkan genre
Menampilkan judul berdasarkan hari
Menampilkan status berdasarkan berdasarkan judul
Menambahkan anime ke dalam file myanimelist.csv
Melakukan edit anime berdasarkan judul
Melakukan delete berdasarkan judul
- Selain command yang diberikan akan menampilkan tulisan “Invalid Command”
- Karena Lewis juga ingin track anime yang ditambah, diubah, dan dihapus. Maka dia membuat server dapat mencatat anime yang dihapus dalam sebuah log yang diberi nama `change.log`.
```
Format: [date] [type] [massage]
Type: ADD, EDIT, DEL
Ex:
[29/03/24] [ADD] Kanokari ditambahkan.
[29/03/24] [EDIT] Kamis,Comedy,Kanokari,completed diubah menjadi Jumat,Action,Naruto,completed.
[29/03/24] [DEL] Naruto berhasil dihapus.
```

- Koneksi antara client dan server tidak akan terputus jika ada kesalahan input dari client, cuma terputus jika user mengirim pesan “exit”. Program exit dilakukan pada sisi client.
```
Hasil akhir:
soal_4/
    ├── change.log
    ├── client/
    │   └── client.c
    ├── myanimelist.csv
    └── server/
        └── server.c
```
# Penyelesaian Soal 4

Kode ini merupakan implementasi dari server-client sederhana menggunakan bahasa pemrograman C untuk mengelola data anime dalam format CSV. Server akan menerima permintaan dari client, memprosesnya, dan memberikan respons kembali kepada client.

## Server (`server.c`)

1. **Fungsi `downloadCSV()`**: Mendownload file CSV dari internet. File akan diunduh dari URL yang disediakan dan disimpan di direktori yang ditentukan.
```c
void downloadCSV()
{
    system("rm -f ../myanimelist.csv");
    system("wget --content-disposition --no-check-certificate \"https://drive.google.com/uc?export=download&id=10p_kzuOgaFY3WT6FVPJIXFbkej2s9f50\" -P ../");
}
```

3. **Fungsi `show(token, response)`**: Membuka file CSV, mencari baris yang mengandung token yang diberikan, kemudian mengirimkan baris-baris tersebut sebagai respons.
```c

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
```
4. **Fungsi `showAll(response)`**: Membuka file CSV dan mengirimkan semua baris sebagai respons.
```c
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
```

6. **Fungsi `add(token, response)`**: Menambahkan entri baru ke file CSV dan mencatatnya ke file log.
```c
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
```

8. **Fungsi `edit(token, response)`**: Mengubah entri dalam file CSV berdasarkan token yang diberikan dan mencatat perubahannya ke file log.
```c
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
```

10. **Fungsi `delete(token, response)`**: Menghapus entri dari file CSV berdasarkan token yang diberikan dan mencatat penghapusan tersebut ke file log.
```c
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
```

12. **Fungsi `handleRequest(buffer, response)`**: Memproses permintaan dari client. Mengekstrak perintah dan token dari buffer, lalu memanggil fungsi yang sesuai untuk menangani permintaan tersebut.
```c
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
```

14. **Fungsi `main()`**: 
    - Mendownload file CSV.
    ```c
    downloadCSV();
    ```
    - Membuat socket server dan mengikatnya ke alamat dan port yang ditentukan.
    ```c
     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    ```
    - Mendengarkan koneksi masuk dari client.
    ```c
     if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Socket binding failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        perror("Socket listening failed");
        return -1;
    }
    ```
    - Menerima permintaan dari client, memprosesnya, dan memberikan respons kembali.
```c
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
```

## Client (`client.c`)

1. **Fungsi `main()`**:
    - Membuat socket dan terhubung ke server.
   ```c
   int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[MAX_LENGTH];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error creating socket\n");
        return -1;
   ```
    - Membaca input dari pengguna dan mengirimkannya ke server.
    - Menerima respons dari server dan mencetaknya.
   ```c
   while (1) {
    printf("You: ");
    memset(buffer, 0, MAX_LENGTH);
    fgets(buffer, MAX_LENGTH - 1, stdin);
    
    // Send the message to the server
    send(sockfd, buffer, strlen(buffer), 0);
   ```
    - Mengulangi proses ini sampai pengguna memasukkan perintah "exit".
```c
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
```

## Catatan Penting:

- Kedua kode ini menggunakan protokol TCP.
- Data anime disimpan dalam file CSV (`../myanimelist.csv`).
- Perubahan pada data dicatat dalam file log (`../change.log`).
- Fungsi `downloadCSV()` digunakan untuk mengunduh file CSV dari Google Drive.
- Setiap perubahan pada data anime dicatat dalam file log dengan format tanggal, jenis perubahan, dan detail perubahan.

# Revisi

# Dokumentasi Output


