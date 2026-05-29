#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    double n = 10.0;
    
    if (argc > 1) {
        n = atof(argv[1]);
    }

    srand((unsigned int)time(NULL));

    printf("Sequence (0.0 to 1.0):\n");
    for (int i = 0; i < 5; i++) {
        double val = (double)rand() / RAND_MAX;
        printf("%f\n", val);
    }

    printf("\nSequence (0.0 to %f):\n", n);
    for (int i = 0; i < 5; i++) {
        double val = ((double)rand() / RAND_MAX) * n;
        printf("%f\n", val);
    }

    return 0;
}