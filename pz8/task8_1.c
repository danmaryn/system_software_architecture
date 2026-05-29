#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    int flags = fcntl(pipefd[1], F_GETFL, 0);
    fcntl(pipefd[1], F_SETFL, flags | O_NONBLOCK);

    size_t nbytes = 1024 * 1024;
    char *buffer = malloc(nbytes);
    memset(buffer, 'A', nbytes);

    printf("Attempting to write %zu bytes to a non-blocking pipe...\n", nbytes);

    ssize_t count = write(pipefd[1], buffer, nbytes);

    if (count == -1) {
        perror("write");
    } else {
        printf("Requested nbytes: %zu\n", nbytes);
        printf("Actual count returned: %zd\n", count);
        
        if (count < nbytes) {
            printf("Conclusion: Yes, count can be less than nbytes! (Pipe buffer filled up)\n");
        }
    }

    free(buffer);
    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}