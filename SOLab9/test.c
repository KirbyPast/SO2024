#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char to_base_64(int x) {
    if (x >= 0 && x < 26) {
        return 'A' + x;
    }
    else if (x >= 26 && x < 52) {
        return 'a' + (x - 26);
    }
    else if (x >= 52 && x < 62) {
        return '0' + (x - 52);
    }
    else if(x == 62) {
        return '+';
    }
    else if(x == 63) {
        return '/';
    }
    else {
        return -1;
    }
}
 
int main(int argc, char* argv[]) {
    int input_fd, output_fd;
    long size_in, size_out;
    ssize_t bytes_in, bytes_out; 

    if (argc != 3) {
        printf("Usage: src_file dest_file");
        return 1;
    }

    input_fd = open(argv[1] , O_RDONLY);
    if (input_fd == -1) {
        perror(argv[1]);
        return 2;
    }

    if (access(argv[2], F_OK) == 0) {
        // fisierul deja exista
        printf("Fisierul '%s' deja exista. Doriti sa suprascrieti? y/n: ", argv[2]);
        char answer = 0;
        scanf("%c", &answer);
        if (answer == 'y') {
            output_fd = open(argv[2] , O_RDWR | O_TRUNC , 0600);
            if (output_fd == -1) {
                perror(argv[2]);
                return 3;
            }
        }
        else if (answer == 'n') {
            printf("Exiting\n");
            return 0;
        }
        else {
            printf("Invalid answer.\n");
            return 4;
        }
    }
    else {
        output_fd = open(argv[2] , O_RDWR | O_CREAT | O_TRUNC , 0600);
        if (output_fd == -1) {
            perror(argv[2]);
            return 3;
        }
    }

    struct stat sb;
    if (fstat(input_fd, &sb) == -1) {
        perror("Error at fstat");
        return 4;
    }
    size_in = sb.st_size;
    char* input_map = mmap(NULL, size_in, PROT_READ, MAP_PRIVATE, input_fd, 0);
    if (input_map == MAP_FAILED) {
        perror("Error at mmap");
        return 4;
    }

    // TODO: nu i bine
    size_out = size_in * 2;
    char* output_map = mmap(NULL, size_out, PROT_WRITE | PROT_EXEC, MAP_PRIVATE, output_fd, 0);
    if (output_map == MAP_FAILED) {
        perror("Error at mmap");
        return 5;
    }

    close(input_fd);
    close(output_fd);

    int it_input = 0, it_output = 0;
    while (it_input < size_in) {
        printf("%d\n", it_input);

        int tr = 0;
        char transform[4] = {};


        tr = tr << 8;
        tr += input_map[it_input++];
        if (it_input < size_in) {
            tr = tr << 8;
            tr += input_map[it_input++];
        }
        if (it_input < size_in) {
            tr = tr << 8;
            tr += input_map[it_input++];
        }

        transform[0] = to_base_64(tr >> 18);
        transform[1] = to_base_64(tr >> 12 & 63);
        transform[2] = to_base_64(tr >> 6 & 63);
        transform[3] = to_base_64(tr & 63);

        for (int i = 0; i < 4; i++) {
            printf("%c", transform[i]);
        }
        printf("\n");

        memcpy(output_map + it_output, transform, 4);
        printf("aici\n");
        it_output += 4;
    }

    printf("%s", output_map);

    if (-1 == munmap(input_map, size_in)) {
        perror("Error at munmap");
        return 6;
    }
    if (-1 == munmap(output_map, size_out)) {
        perror("Error at munmap");
        return 6;
    }
    return 0;
}
