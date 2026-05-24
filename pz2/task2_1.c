#include <stdio.h>
#include <time.h>
#include <stdint.h>

int main() {
    printf("Current system time_t size: %zu bytes\n\n", sizeof(time_t));

    int32_t max_32 = 0x7FFFFFFF;
    time_t time_32 = (time_t)max_32;
    
    printf("--- 32-bit time_t simulation ---\n");
    printf("Max value: %d\n", max_32);
    struct tm *t32 = gmtime(&time_32);
    if (t32 != NULL) {
        printf("Expiration date (UTC): %s", asctime(t32));
    } else {
        printf("Expiration date: Cannot be calculated.\n");
    }
    printf("\n");

    int64_t max_64 = 0x7FFFFFFFFFFFFFFF;
    time_t time_64 = (time_t)max_64;
    
    printf("--- 64-bit time_t simulation ---\n");
    printf("Max value: %jd\n", (intmax_t)max_64);
    struct tm *t64 = gmtime(&time_64);
    if (t64 != NULL) {
        printf("Expiration date (UTC): %s", asctime(t64));
    } else {
        printf("Expiration date: Far into the future (approx 292 billion years AD).\n");
        printf("Note: gmtime() cannot process this extreme value on this system.\n");
    }

    return 0;
}