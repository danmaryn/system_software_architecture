#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int cmp_float(const void *a, const void *b) {
    float diff = *(float *)a - *(float *)b;
    return (diff > 0) - (diff < 0);
}

int cmp_char(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

void rand_int(void *elem) {
    *(int *)elem = rand() % 100;
}

void rand_float(void *elem) {
    *(float *)elem = (float)(rand() % 1000) / 10.0f;
}

void rand_char(void *elem) {
    *(char *)elem = 'A' + (rand() % 26);
}

void print_int(const void *elem) { printf("%d ", *(int *)elem); }
void print_float(const void *elem) { printf("%.1f ", *(float *)elem); }
void print_char(const void *elem) { printf("%c ", *(char *)elem); }

void fill_array(void *arr, size_t n, size_t elem_size, void (*rand_func)(void *)) {
    char *ptr = (char *)arr;
    for (size_t i = 0; i < n; i++) {
        rand_func(ptr + i * elem_size);
    }
}

void print_array(void *arr, size_t n, size_t elem_size, void (*print_func)(const void *)) {
    char *ptr = (char *)arr;
    for (size_t i = 0; i < n; i++) {
        print_func(ptr + i * elem_size);
    }
    printf("\n");
}

int recursive_search(void *arr, size_t n, size_t elem_size, void *target, int (*cmp)(const void *, const void *), size_t index) {
    if (index >= n) return -1;
    char *ptr = (char *)arr;
    if (cmp(ptr + index * elem_size, target) == 0) return (int)index;
    return recursive_search(arr, n, elem_size, target, cmp, index + 1);
}

int binary_search(void *arr, size_t n, size_t elem_size, void *target, int (*cmp)(const void *, const void *)) {
    int left = 0, right = n - 1;
    char *ptr = (char *)arr;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int res = cmp(ptr + mid * elem_size, target);
        if (res == 0) return mid;
        if (res < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <int|float|char>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));
    size_t n = 10;
    void *arr = NULL;
    size_t elem_size = 0;

    if (strcmp(argv[1], "int") == 0) {
        elem_size = sizeof(int);
        arr = malloc(n * elem_size);
        fill_array(arr, n, elem_size, rand_int);

        printf("Generated array (int): ");
        print_array(arr, n, elem_size, print_int);

        int target;
        printf("Enter int to search: ");
        if (scanf("%d", &target) != 1) return 1;

        int idx = recursive_search(arr, n, elem_size, &target, cmp_int, 0);
        printf("Recursive search index: %d\n\n", idx);

        qsort(arr, n, elem_size, cmp_int);
        printf("Sorted array: ");
        print_array(arr, n, elem_size, print_int);

        idx = binary_search(arr, n, elem_size, &target, cmp_int);
        printf("Binary search index: %d\n", idx);
    }
    else if (strcmp(argv[1], "float") == 0) {
        elem_size = sizeof(float);
        arr = malloc(n * elem_size);
        fill_array(arr, n, elem_size, rand_float);

        printf("Generated array (float): ");
        print_array(arr, n, elem_size, print_float);

        float target;
        printf("Enter float to search: ");
        if (scanf("%f", &target) != 1) return 1;

        int idx = recursive_search(arr, n, elem_size, &target, cmp_float, 0);
        printf("Recursive search index: %d\n\n", idx);

        qsort(arr, n, elem_size, cmp_float);
        printf("Sorted array: ");
        print_array(arr, n, elem_size, print_float);

        idx = binary_search(arr, n, elem_size, &target, cmp_float);
        printf("Binary search index: %d\n", idx);
    }
    else if (strcmp(argv[1], "char") == 0) {
        elem_size = sizeof(char);
        arr = malloc(n * elem_size);
        fill_array(arr, n, elem_size, rand_char);

        printf("Generated array (char): ");
        print_array(arr, n, elem_size, print_char);

        char target;
        printf("Enter char to search: ");
        if (scanf(" %c", &target) != 1) return 1;

        int idx = recursive_search(arr, n, elem_size, &target, cmp_char, 0);
        printf("Recursive search index: %d\n\n", idx);

        qsort(arr, n, elem_size, cmp_char);
        printf("Sorted array: ");
        print_array(arr, n, elem_size, print_char);

        idx = binary_search(arr, n, elem_size, &target, cmp_char);
        printf("Binary search index: %d\n", idx);
    }
    else {
        printf("Unknown type. Use int, float or char.\n");
        return 1;
    }

    free(arr);
    return 0;
}