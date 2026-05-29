#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>

int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (volatile int i = 0; i < 100000000; i++) {}

    clock_gettime(CLOCK_MONOTONIC, &end);

    long milliseconds = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

    printf("Execution time: %ld ms\n", milliseconds);

    return 0;
}