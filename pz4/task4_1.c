#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    size_t max_size = SIZE_MAX;
    
    printf("Platform: %d-bit\n", (int)(sizeof(void*) * 8));
    printf("Max size_t: %zu\n", max_size);
    printf("Max ptrdiff_t: %td\n", PTRDIFF_MAX);
    
    void *ptr = malloc(max_size);
    if (!ptr) {
        printf("malloc(SIZE_MAX) failed.\n");
    } else {
        free(ptr);
    }

    return 0;
}