#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_NAME "/exec_log_v12"
#define SHM_SIZE 8192

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        fprintf(stderr, "       %s --read\n", argv[0]);
        return 1;
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    char *log_space = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (log_space == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    if (strcmp(argv[1], "--read") == 0) {
        printf("--- In-Memory Execution Log ---\n%s", log_space);
        munmap(log_space, SHM_SIZE);
        close(shm_fd);
        return 0;
    }

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; 

    char entry[512] = {0};
    snprintf(entry, sizeof(entry), "[%s] User ID: %d, Command: ", time_str, getuid());
    
    for (int i = 1; i < argc; i++) {
        strncat(entry, argv[i], sizeof(entry) - strlen(entry) - 1);
        strncat(entry, " ", sizeof(entry) - strlen(entry) - 1);
    }
    strncat(entry, "\n", sizeof(entry) - strlen(entry) - 1);

    size_t current_len = strlen(log_space);
    if (current_len + strlen(entry) < SHM_SIZE) {
        strcat(log_space, entry);
    }

    munmap(log_space, SHM_SIZE);
    close(shm_fd);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    } else {
        wait(NULL);
    }

    return 0;
}