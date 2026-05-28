#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--- Test 1: realloc(NULL, size) ---\n");
    size_t size = 64;
    void *ptr1 = realloc(NULL, size);
    
    if (ptr1) {
        printf("realloc(NULL, %zu) successfully allocated memory at: %p\n", size, ptr1);
        free(ptr1);
    } else {
        printf("realloc(NULL, %zu) failed.\n", size);
    }

    printf("\n--- Test 2: realloc(ptr, 0) ---\n");
    void *ptr2 = malloc(128);
    printf("Initial allocation at: %p\n", ptr2);
    
    void *ptr3 = realloc(ptr2, 0);
    
    if (ptr3 == NULL) {
        printf("realloc(ptr, 0) returned NULL. Memory at %p was freed.\n", ptr2);
    } else {
        printf("realloc(ptr, 0) returned non-NULL: %p\n", ptr3);
        free(ptr3); 
    }

    return 0;
}