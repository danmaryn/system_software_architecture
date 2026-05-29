#include <stdio.h>
#include <stdlib.h>

void print_file(const char *filename, int *line_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
        (*line_count)++;

        if (*line_count >= 20) {
            FILE *tty = fopen("/dev/tty", "r");
            if (tty) {
                fgetc(tty);
                fclose(tty);
            } else {
                getchar();
            }
            *line_count = 0;
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    int line_count = 0;
    for (int i = 1; i < argc; i++) {
        print_file(argv[i], &line_count);
    }

    return 0;
}