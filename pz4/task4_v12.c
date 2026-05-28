#include <stdio.h>
#include <stdlib.h>

int main() {
    void *a = malloc(16);
    printf("Block A allocated at: %p\n", a);

    a = realloc(a, 32);
    printf("Block A expanded to 32 bytes at: %p\n\n", a);

    void *b = malloc(16);
    void *guard = malloc(16);

    printf("Block B allocated at: %p\n", b);
    printf("Guard block allocated at: %p\n", guard);

    b = realloc(b, 1024);
    printf("Block B expanded to 1024 bytes at: %p\n", b);

    free(a);
    free(b);
    free(guard);

    return 0;
}