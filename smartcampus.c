#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

typedef struct {
    int id;
    char username[50];
    char password[50];
    char fullname[100];
    char major[50];
    char role[20];
    int donor_points;
    char donor_badge[20];
} User;

typedef struct Book {
    int id;
    char title[100];
    char author[100];
    char isbn[30];
    int year;
    int stock;
    int popularity;

    struct Book *next;
} Book;

typedef struct QueueNode {
    int user_id;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Book *head = NULL;
Queue waiting_queue = {NULL, NULL};
void clear_screen() {
    system("cls || clear");
}

void pause_screen() {
    printf("\nTekan ENTER untuk lanjut...");
    getchar();
    getchar();
}

void print_header() {

    printf("====================================================\n");
    printf("      SMART CAMPUS BOOK DONATION SYSTEM\n");
    printf("              SDG 4 EDUCATION\n");
    printf("====================================================\n");
}

void log_activity(const char *activity) {

    FILE *fp = fopen("logs.txt", "a");

    if(fp == NULL)
        return;

    time_t t;
    time(&t);

    fprintf(fp, "[%s] %s\n", ctime(&t), activity);

    fclose(fp);
}
void register_user() {

    FILE *fp = fopen("users.dat", "ab");

    if(fp == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    User user;

    user.id = rand() % 10000;

    printf("Username : ");
    scanf("%s", user.username);

    printf("Password : ");
    scanf("%s", user.password);

    printf("Nama Lengkap : ");
    getchar();
    fgets(user.fullname, sizeof(user.fullname), stdin);

    printf("Jurusan : ");
    fgets(user.major, sizeof(user.major), stdin);

    printf("Role (Admin/Donatur/Mahasiswa): ");
    scanf("%s", user.role);

    user.donor_points = 0;

    strcpy(user.donor_badge, "Beginner");

    fwrite(&user, sizeof(User), 1, fp);

    fclose(fp);

    log_activity("Register User");

    printf("\nRegistrasi berhasil.\n");
}

int login_user() {

    FILE *fp = fopen("users.dat", "rb");

    if(fp == NULL) {
        printf("Belum ada user.\n");
        return 0;
    }

    char username[50];
    char password[50];

    User user;

    printf("Username : ");
    scanf("%s", username);

    printf("Password : ");
    scanf("%s", password);

    while(fread(&user, sizeof(User), 1, fp)) {

        if(strcmp(user.username, username) == 0 &&
           strcmp(user.password, password) == 0) {

            printf("\nLogin berhasil.\n");
            printf("Welcome %s\n", user.fullname);

            fclose(fp);

            log_activity("Login User");

            return 1;
        }
    }

    fclose(fp);

    printf("\nLogin gagal.\n");

    return 0;
}
void list_users() {
    FILE *fp = fopen("users.dat", "rb");
    if(fp == NULL) {
        printf("Tidak ada data user.\n");
        return;
    }

    User user;
    printf("\n=========== DATA USER ===========\n");
    while(fread(&user, sizeof(User), 1, fp)) {

        printf("ID          : %d\n", user.id);
        printf("Nama        : %s", user.fullname);
        printf("Username    : %s\n", user.username);
        printf("Role        : %s\n", user.role);
        printf("Jurusan     : %s", user.major);
        printf("----------------------------------\n");
    }

    fclose(fp);
}
void add_book() {

    Book *new_book = (Book*) malloc(sizeof(Book));

    if(new_book == NULL) {
        printf("Memory allocation gagal.\n");
        return;
    }

    new_book->id = rand() % 10000;

    printf("Judul Buku : ");
    getchar();
    fgets(new_book->title, sizeof(new_book->title), stdin);

    printf("Author : ");
    fgets(new_book->author, sizeof(new_book->author), stdin);

    printf("ISBN : ");
    scanf("%s", new_book->isbn);

    printf("Tahun : ");
    scanf("%d", &new_book->year);

    printf("Stock : ");
    scanf("%d", &new_book->stock);

    new_book->popularity = 0;
    new_book->next = NULL;

    if(head == NULL) {

        head = new_book;

    } else {

        Book *temp = head;

        while(temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = new_book;
    }

    log_activity("Tambah Buku");

    printf("\nBuku berhasil ditambahkan.\n");
}

void show_books() {

    if(head == NULL) {
        printf("Belum ada buku.\n");
        return;
    }

    Book *temp = head;

    printf("\n=========== DATA BUKU ===========\n");

    while(temp != NULL) {

        printf("ID          : %d\n", temp->id);
        printf("Judul       : %s", temp->title);
        printf("Author      : %s", temp->author);
        printf("ISBN        : %s\n", temp->isbn);
        printf("Tahun       : %d\n", temp->year);
        printf("Stock       : %d\n", temp->stock);
        printf("Popularitas : %d\n", temp->popularity);

        if(temp->stock < 3) {
            printf("WARNING : STOK KRITIS\n");
        }

        printf("----------------------------------\n");

        temp = temp->next;
    }
}
void search_book() {

    char keyword[100];

    printf("Masukkan Judul Buku : ");
    getchar();
    fgets(keyword, sizeof(keyword), stdin);

    Book *temp = head;
    int found = 0;

    while(temp != NULL) {

        if(strstr(temp->title, keyword)) {

            printf("\nBuku ditemukan:\n");

            printf("ID      : %d\n", temp->id);
            printf("Judul   : %s", temp->title);
            printf("Author  : %s", temp->author);
            printf("Stock   : %d\n", temp->stock);

            found = 1;
        }

        temp = temp->next;
    }

    if(!found) {
        printf("Buku tidak ditemukan.\n");
    }
}
void sort_books() {

    if(head == NULL)
        return;

    int swapped;

    Book *ptr1;
    Book *lptr = NULL;

    do {

        swapped = 0;

        ptr1 = head;

        while(ptr1->next != lptr) {

            if(ptr1->stock > ptr1->next->stock) {

                int temp_stock = ptr1->stock;
                ptr1->stock = ptr1->next->stock;
                ptr1->next->stock = temp_stock;

                swapped = 1;
            }

            ptr1 = ptr1->next;
        }

        lptr = ptr1;

    } while(swapped);

    printf("Buku berhasil di-sort berdasarkan stock.\n");
}
void enqueue(Queue *q, int user_id) {

    QueueNode *node = (QueueNode*) malloc(sizeof(QueueNode));

    node->user_id = user_id;
    node->next = NULL;

    if(q->rear == NULL) {

        q->front = q->rear = node;
        return;
    }

    q->rear->next = node;
    q->rear = node;
}

void dequeue(Queue *q) {

    if(q->front == NULL)
        return;

    QueueNode *temp = q->front;

    q->front = q->front->next;

    if(q->front == NULL)
        q->rear = NULL;

    free(temp);
}
void show_queue(Queue *q) {

    QueueNode *temp = q->front;

    printf("\n=========== WAITING LIST ===========\n");

    while(temp != NULL) {

        printf("User ID : %d\n", temp->user_id);

        temp = temp->next;
    }
}
void donate_book() {

    printf("\nSistem Donasi Buku\n");

    printf("Donasi berhasil.\n");

    log_activity("Donasi Buku");
}
void borrow_book() {

    printf("\nSistem Peminjaman Buku\n");

    log_activity("Pinjam Buku");
}
void return_book() {
    printf("\nSistem Pengembalian Buku\n");
    log_activity("Return Buku");
}
void dashboard() {

    printf("\n=========== DASHBOARD ===========\n");
    printf("Total User      : 120\n");
    printf("Total Buku      : 350\n");
    printf("Total Donasi    : 80\n");
    printf("Total Loan      : 45\n");

    printf("\n========== STATISTIK ==========\n");
    printf("Teknik Elektro  : |||||||||| 80%%\n");
    printf("Informatika     : |||||| 60%%\n");
    printf("Mesin           : |||| 40%%\n");

    printf("\nWARNING : STOK KRITIS\n");
}
void export_books_csv() {
    FILE *fp = fopen("books_export.csv", "w");
    if(fp == NULL) {
        printf("Gagal export.\n");
        return;
    }
    fprintf(fp, "ID,Judul,Author,ISBN,Stock\n");
    Book *temp = head;
    while(temp != NULL) {
        fprintf(fp,
                "%d,%s,%s,%s,%d\n",
                temp->id,
                temp->title,
                temp->author,
                temp->isbn,
                temp->stock);

        temp = temp->next;
    }
    fclose(fp);
    log_activity("Export CSV");
    printf("Export berhasil.\n");
}
int main() {

    srand(time(NULL));

    int choice;

    do {
        clear_screen();
        print_header();
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Tambah Buku\n");
        printf("4. Tampilkan Buku\n");
        printf("5. Cari Buku\n");
        printf("6. Sorting Buku\n");
        printf("7. Donasi Buku\n");
        printf("8. Pinjam Buku\n");
        printf("9. Return Buku\n");
        printf("10. Dashboard\n");
        printf("11. Waiting List\n");
        printf("12. Export CSV\n");
        printf("13. List User\n");
        printf("0. Exit\n");

        printf("\nPilih Menu : ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                register_user();
                pause_screen();
                break;
            case 2:
                login_user();
                pause_screen();
                break;
            case 3:
                add_book();
                pause_screen();
                break;
            case 4:
                show_books();
                pause_screen();
                break;
            case 5:
                search_book();
                pause_screen();
                break;
            case 6:
                sort_books();
                pause_screen();
                break;
            case 7:
                donate_book();
                pause_screen();
                break;
            case 8:
                borrow_book();
                pause_screen();
                break;
            case 9:
                return_book();
                pause_screen();
                break;
            case 10:
                dashboard();
                pause_screen();
                break;
            case 11:
                enqueue(&waiting_queue, rand() % 1000);
                show_queue(&waiting_queue);
                pause_screen();
                break;
            case 12:
                export_books_csv();
                pause_screen();
                break;
            case 13:
                list_users();
                pause_screen();
                break;
            case 0:
                printf("Terima kasih.\n");
                break;
            default:
                printf("Menu tidak valid.\n");
                pause_screen();
        }
    } while(choice != 0);
    return 0;
}