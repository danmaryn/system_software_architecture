#include <stdio.h>

#ifdef GLOBAL_UNINIT
int global_array[1000];
#endif

#ifdef GLOBAL_INIT
int global_array[1000] = {1}; 
#endif

int main() {
    printf("Hello World\n");

#ifdef LOCAL_ARRAYS
    int local_uninit[1000];
    int local_init[1000] = {1};
    
    local_uninit[0] = 5;
    printf("Local check: %d, %d\n", local_uninit[0], local_init[0]);
#endif

    return 0;
}