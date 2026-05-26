#include <stdio.h>
#include <stdlib.h>

int global_init_var = 42;
int global_uninit_var;

void grow_stack() {
    int large_array[10000];
    int j;
    
    large_array[0] = 1; 
    
    printf("The large array is near:           %p\n", (void*)&large_array[0]);
    printf("The new stack top is near:         %p\n", (void*)&j);
}

int main() {
    int i;
    int *heap_var = (int*)malloc(sizeof(int));

    if (heap_var == NULL) {
        return 1;
    }

    printf("The text segment (main) is near:   %p\n", (void*)main);
    printf("The data segment (init) is near:   %p\n", (void*)&global_init_var);
    printf("The BSS segment (uninit) is near:  %p\n", (void*)&global_uninit_var);
    printf("The heap is near:                  %p\n", (void*)heap_var);
    printf("The initial stack top is near:     %p\n\n", (void*)&i);

    grow_stack();

    free(heap_var);
    return 0;
}