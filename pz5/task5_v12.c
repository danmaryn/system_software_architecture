#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct AlignedData {
    char c;
    int i;
};

int main() {
    struct AlignedData dest;
    
    memset(&dest, 0xFF, sizeof(dest));

    uint8_t packed_buffer[5] = { 'A', 42, 0, 0, 0 };

    memcpy(&dest, packed_buffer, 5);

    printf("--- Padding Overwrite Demonstration ---\n");
    printf("Expected: c = 'A', i = 42\n");
    printf("Actual:   c = '%c', i = %d (0x%X)\n", dest.c, dest.i, dest.i);
    
    printf("\nRaw memory dump of dest (%zu bytes):\n", sizeof(dest));
    uint8_t *raw = (uint8_t*)&dest;
    for (size_t k = 0; k < sizeof(dest); k++) {
        printf("[%zu]: %02X ", k, raw[k]);
        if (k == 0) printf(" <- char 'c'\n");
        else if (k >= 1 && k <= 3) printf(" <- padding (overwritten!)\n");
        else if (k == 4) printf(" <- int 'i' (start)\n");
        else printf(" <- int 'i'\n");
    }

    return 0;
}