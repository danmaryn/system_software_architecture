#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        execlp("echo", "echo", "Hello", "from", "child", NULL);
        perror("execlp");
        exit(1);
    } else {
        wait(NULL);
    }

    return 0;
}