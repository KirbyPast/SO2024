#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

void print_usage() {
    printf("Usage: tac [-b] [-s STRING] file1 file2 ...\n");
}

void tac_file(const char *filename, int ignore_blank, const char *separator) {
	int fd = open(filename, O_RDWR);
	char* map;
	map = mmap(NULL,
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (!ignore_blank || strlen(line) > 1) {
            if (line_count < MAX_LINES) {
                strcpy(lines[line_count], line);
                line_count++;
            } else {
                fprintf(stderr, "Warning: Maximum line count reached, skipping remaining lines in file: %s\n", filename);
                break;
            }
        }
    }

    fclose(file);

    for (int i = line_count - 1; i >= 0; i--) {
        if (separator != NULL && strstr(lines[i], separator) == NULL) {
            continue;
        }
        fputs(lines[i], stdout);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int ignore_blank = 0;
    const char *separator = NULL;

    while ((opt = getopt(argc, argv, "bs:")) != -1) {
        switch (opt) {
            case 'b':
                ignore_blank = 1;
                break;
            case 's':
                separator = optarg;
                break;
            default:
                print_usage();
                return EXIT_FAILURE;
        }
    }

    if (optind == argc) {
        print_usage();
        return EXIT_FAILURE;
    }

    for (int i = optind; i < argc; i++) {
        tac_file(argv[i], ignore_blank, separator);
    }

    return EXIT_SUCCESS;
}
