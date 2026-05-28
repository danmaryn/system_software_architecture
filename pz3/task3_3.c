#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

volatile sig_atomic_t limit_reached = 0;

void handle_sigxfsz(int sig) {
    limit_reached = 1;
}

int main() {
    signal(SIGXFSZ, handle_sigxfsz);
    srand(time(NULL));

    FILE *file = fopen("dice_rolls.txt", "w");
    if (!file) {
        return 1;
    }

    int roll_count = 0;
    while (!limit_reached) {
        int roll = (rand() % 6) + 1;
        int ret = fprintf(file, "Roll %d: %d\n", ++roll_count, roll);
        
        if (ret < 0) {
            if (errno == EFBIG) {
                limit_reached = 1;
            } else {
                break;
            }
        }
        fflush(file);
    }

    printf("File size limit exceeded!\n");
    printf("Total rolls: %d\n", roll_count - 1);
    
    fclose(file);
    return 0;
}