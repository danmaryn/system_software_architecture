#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("task8_2_data.bin", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    unsigned char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};
    if (write(fd, data, sizeof(data)) != sizeof(data)) {
        perror("write");
        close(fd);
        return 1;
    }

    lseek(fd, 3, SEEK_SET);

    unsigned char buffer[4] = {0};
    if (read(fd, buffer, 4) == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    printf("Buffer contains: ");
    for (int i = 0; i < 4; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    close(fd);
    unlink("task8_2_data.bin");

    return 0;
}