#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <word> <file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[2], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    char buffer[2048];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, argv[1])) {
            printf("%s", buffer);
        }
    }

    fclose(file);
    return 0;
}