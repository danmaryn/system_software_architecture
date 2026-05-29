#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int compare_ints(const void *a, const void *b) {
    int int_a = *((int *)a);
    int int_b = *((int *)b);
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

void verify_sort() {
    int test_arr[] = {9, 4, 7, 2, 8, 1, 5, 3, 6};
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t n = sizeof(test_arr) / sizeof(test_arr[0]);

    qsort(test_arr, n, sizeof(int), compare_ints);

    for (size_t i = 0; i < n; i++) {
        assert(test_arr[i] == expected[i]);
    }
    printf("Correctness test passed.\n\n");
}

double measure_qsort_time(int *arr, size_t n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(arr, n, sizeof(int), compare_ints);
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void generate_random(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) arr[i] = rand() % n;
}

void generate_sorted(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) arr[i] = i;
}

void generate_reverse(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) arr[i] = n - i;
}

void generate_identical(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) arr[i] = 42;
}

void generate_organ_pipe(int *arr, size_t n) {
    size_t mid = n / 2;
    for (size_t i = 0; i < mid; i++) arr[i] = i;
    for (size_t i = mid; i < n; i++) arr[i] = n - i;
}

int main() {
    verify_sort();

    size_t n = 5000000;
    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    srand(time(NULL));

    printf("Testing with array size: %zu\n", n);

    generate_random(arr, n);
    printf("Random data:      %f seconds\n", measure_qsort_time(arr, n));

    generate_sorted(arr, n);
    printf("Sorted data:      %f seconds\n", measure_qsort_time(arr, n));

    generate_reverse(arr, n);
    printf("Reverse sorted:   %f seconds\n", measure_qsort_time(arr, n));

    generate_identical(arr, n);
    printf("Identical data:   %f seconds\n", measure_qsort_time(arr, n));

    generate_organ_pipe(arr, n);
    printf("Organ pipe data:  %f seconds\n", measure_qsort_time(arr, n));

    free(arr);
    return 0;
}