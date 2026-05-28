#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    FILE *files[2048];
    int count = 0;

    for (int i = 0; i < 2048; i++) {
        files[i] = fopen("/etc/hosts", "r");
        if (!files[i]) {
            printf("Failed to open file at index %d\n", count);
            perror("Error");
            break;
        }
        count++;
    }

    printf("Total files opened successfully: %d\n", count);

    for (int i = 0; i < count; i++) {
        fclose(files[i]);
    }

    return 0;
}