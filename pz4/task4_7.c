#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sbar {
    int data1;
    double data2;
};

void run_old() {
    struct sbar *ptr, *newptr;
    
    ptr = calloc(1000, sizeof(struct sbar));
    newptr = realloc(ptr, 500 * sizeof(struct sbar));
    
    if (newptr) {
        free(newptr);
    } else {
        free(ptr);
    }
}

void run_new() {
    struct sbar *ptr, *newptr;
    
    ptr = calloc(1000, sizeof(struct sbar));
    newptr = reallocarray(ptr, 500, sizeof(struct sbar));
    
    if (newptr) {
        free(newptr);
    } else {
        free(ptr);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "old") == 0) {
        run_old();
    } else if (argc > 1 && strcmp(argv[1], "new") == 0) {
        run_new();
    } else {
        printf("Usage: %s [old|new]\n", argv[0]);
    }
    return 0;
}