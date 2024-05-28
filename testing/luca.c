#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int is_alphanumeric(char c) {
int main(int argc, char* argv[]) {
    printf("z");
    if (argc != 2) {
        printf("Numar de argumente gresit\n");
        return 2;
    }
    printf("z");
    if (access(argv[1], F_OK) == -1) {
        // printf("Fisierul %s nu exista\n", argv[1]);
        return 2;
    }
    printf("z");
    if (access(argv[1], R_OK) == -1) {
        // printf("Fisierul %s nu are permisiuni de read\n", argv[1]);
        return 2;
    }
    printf("z");

    int fd;
    if ((fd = open(argv[1], O_RDONLY) == -1)) {
        perror("Eroare la apelul open\n");
        return 3;
    }

    int file_is_empty = 1, is_alnum = 1;
    char ch;
    printf("y");
    while (1) {
        int read_code;
        read_code = read(fd, &ch, 1);
        if (read_code == -1) {
            perror("Eroare la apelul read\n");
            return 3;
        }
        if (read_code == 0) {
            break;
        }

        file_is_empty = 0;

        if (!is_alphanumeric(ch)) {
            is_alnum = 0;
            break;
        }
        printf("x");
    }

    if (file_is_empty || !is_alnum) {
        printf("%s:not alphanumeric\n", argv[1]);
    }
    else {
        printf("%s:alphanumeric\n", argv[1]);
    }

    close(fd);
    return 0;
}
include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int is_alphanumeric(char c);
int main(int argc, char* argv[]) {
    printf("z");
    if (argc != 2) {
        printf("Numar de argumente gresit\n");
        return 2;
    }
    printf("z");
    if (access(argv[1], F_OK) == -1) {
        // printf("Fisierul %s nu exista\n", argv[1]);
        return 2;
    }
    printf("z");
    if (access(argv[1], R_OK) == -1) {
        // printf("Fisierul %s nu are permisiuni de read\n", argv[1]);
        return 2;
    }
    printf("z");

    int fd;
    if ((fd = open(argv[1], O_RDONLY) == -1)) {
        perror("Eroare la apelul open\n");
        return 3;
    }

    int file_is_empty = 1, is_alnum = 1;
    char ch;
    printf("y");
    while (1) {
        int read_code;
        read_code = read(fd, &ch, 1);
        if (read_code == -1) {
            perror("Eroare la apelul read\n");
            return 3;
        }
        if (read_code == 0) {
            break;
        }

        file_is_empty = 0;

        if (!is_alphanumeric(ch)) {
            is_alnum = 0;
            break;
        }
        printf("x");
    }

    if (file_is_empty || !is_alnum) {
        printf("%s:not alphanumeric\n", argv[1]);
    }
    else {
        printf("%s:alphanumeric\n", argv[1]);
    }

    close(fd);
    return 0;
}
int isAlphaNumeric(char c){
    return (
    (c >= '0' && c <= '9') ||
    (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z')
  KirbyPast