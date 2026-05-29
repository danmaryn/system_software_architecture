#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *ent;
    struct stat file_stat;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        if (stat(ent->d_name, &file_stat) == 0) {
            if (S_ISREG(file_stat.st_mode)) {
                char answer;
                printf("Delete file '%s'? (y/n): ", ent->d_name);
                if (scanf(" %c", &answer) == 1) {
                    if (answer == 'y' || answer == 'Y') {
                        if (unlink(ent->d_name) == 0) {
                            printf("Deleted '%s'\n", ent->d_name);
                        } else {
                            perror("unlink");
                        }
                    }
                }
            }
        }
    }

    closedir(dir);
    return 0;
}