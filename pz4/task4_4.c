#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_buggy() {
    printf("--- Running BUGGY version ---\n");
    void *ptr = NULL;
    int n = 100;
    int iterations = 2;

    while (iterations > 0) {
        if (!ptr) {
            ptr = malloc(n);
            printf("Allocated memory at: %p\n", ptr);
        } else {
            printf("ptr is NOT NULL (holds %p), skipping malloc!\n", ptr);
        }
        
        printf("Using ptr at: %p\n", ptr);
        
        printf("Calling free(%p)...\n", ptr);
        free(ptr);
        
        iterations--;
    }
}

void run_fixed() {
    printf("--- Running FIXED version ---\n");
    void *ptr = NULL;
    int n = 100;
    int iterations = 2;

    while (iterations > 0) {
        if (!ptr) {
            ptr = malloc(n);
            printf("Allocated memory at: %p\n", ptr);
        }
        
        printf("Using ptr at: %p\n", ptr);
        
        printf("Calling free(%p)...\n", ptr);
        free(ptr);
        
        // THE FIX: Reset pointer to NULL after freeing
        ptr = NULL; 
        
        iterations--;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "bug") == 0) {
        run_buggy();
    } else if (argc > 1 && strcmp(argv[1], "fix") == 0) {
        run_fixed();
    } else {
        printf("Usage: %s [bug|fix]\n", argv[0]);
    }
    return 0;
}