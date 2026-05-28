#include <stdio.h>
#include <stdlib.h>

int main() {
    int xa = 1000000;
    int xb = 3000;
    
    int num = xa * xb;
    
    printf("xa = %d, xb = %d\n", xa, xb);
    printf("num (signed int) = %d\n", num);
    printf("num cast to size_t = %zu\n", (size_t)num);
    
    void *ptr = malloc(num);
    
    if (ptr == NULL) {
        printf("malloc failed: cannot allocate memory.\n");
    } else {
        printf("malloc succeeded.\n");
        free(ptr);
    }
    
    return 0;
}