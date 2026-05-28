#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t cpu_limit_reached = 0;

void handle_sigxcpu(int sig) {
    cpu_limit_reached = 1;
}

void generate_lottery() {
    int i, j;
    int nums49[7];
    int nums36[6];

    for (i = 0; i < 7; i++) {
        int unique;
        do {
            unique = 1;
            nums49[i] = (rand() % 49) + 1;
            for (j = 0; j < i; j++) {
                if (nums49[i] == nums49[j]) unique = 0;
            }
        } while (!unique);
    }

    for (i = 0; i < 6; i++) {
        int unique;
        do {
            unique = 1;
            nums36[i] = (rand() % 36) + 1;
            for (j = 0; j < i; j++) {
                if (nums36[i] == nums36[j]) unique = 0;
            }
        } while (!unique);
    }
}

int main() {
    signal(SIGXCPU, handle_sigxcpu);
    srand(time(NULL));

    unsigned long long iterations = 0;

    while (!cpu_limit_reached) {
        generate_lottery();
        iterations++;
    }

    printf("CPU time limit exceeded!\n");
    printf("Total combinations generated: %llu\n", iterations);

    return 0;
}