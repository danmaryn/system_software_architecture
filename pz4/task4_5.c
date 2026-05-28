#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    size_t initial_size = 128;
    void *ptr = malloc(initial_size);
    
    if (!ptr) {
        printf("Initial malloc failed.\n");
        return 1;
    }
    printf("Initial memory allocated at: %p\n", ptr);

    size_t huge_size = SIZE_MAX;
    printf("Attempting to realloc to %zu bytes...\n", huge_size);
    
    void *new_ptr = realloc(ptr, huge_size);

    if (new_ptr == NULL) {
        printf("realloc failed and returned NULL.\n");
        printf("Original pointer (ptr) is still valid: %p\n", ptr);
        
        free(ptr);
        printf("Original memory freed successfully to prevent memory leak.\n");
    } else {
        printf("realloc succeeded (unexpected!).\n");
        free(new_ptr);
    }

    return 0;
}