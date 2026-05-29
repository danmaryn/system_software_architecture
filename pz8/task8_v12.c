#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    const char *filename = "shared_file.txt";
    
    int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    close(fd);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        int write_fd = open(filename, O_WRONLY | O_APPEND);
        if (write_fd == -1) {
            perror("open write");
            exit(1);
        }
        
        const char *msg1 = "Part 1: Hello...\n";
        write(write_fd, msg1, strlen(msg1));
        
        sleep(2);
        
        const char *msg2 = "Part 2: World!\n";
        write(write_fd, msg2, strlen(msg2));
        
        close(write_fd);
        exit(0);
    } else {
        int read_fd = open(filename, O_RDONLY);
        if (read_fd == -1) {
            perror("open read");
            return 1;
        }

        char buffer[64];
        
        for (int i = 0; i < 3; i++) {
            ssize_t bytes_read = read(read_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("[Reader] Read %zd bytes: %s", bytes_read, buffer);
            } else if (bytes_read == 0) {
                printf("[Reader] EOF reached. Waiting for more data...\n");
            } else {
                perror("read");
            }
            sleep(1);
        }
        
        close(read_fd);
        wait(NULL);
        unlink(filename);
    }

    return 0;
}