#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

volatile sig_atomic_t limit_reached = 0;

void handle_sigxfsz(int sig) {
    limit_reached = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Program need two arguments\n");
        return 1;
    }

    signal(SIGXFSZ, handle_sigxfsz);

    FILE *src = fopen(argv[1], "rb");
    if (!src) {
        printf("Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (!dest) {
        printf("Cannot open file %s for writing\n", argv[2]);
        fclose(src);
        return 1;
    }

    char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (limit_reached) {
            break;
        }
        
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dest);
        
        if (bytes_written < bytes_read) {
            if (errno == EFBIG || limit_reached) {
                limit_reached = 1;
                break;
            } else {
                break;
            }
        }
    }

    if (limit_reached) {
        printf("File size limit exceeded!\n");
    } else {
        printf("File copied successfully.\n");
    }

    fclose(src);
    fclose(dest);
    return 0;
}