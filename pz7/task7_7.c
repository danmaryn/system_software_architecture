#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *ent;
    struct stat file_stat;
    uid_t my_uid = geteuid();

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        size_t len = strlen(ent->d_name);
        if (len > 2 && strcmp(ent->d_name + len - 2, ".c") == 0) {
            if (stat(ent->d_name, &file_stat) == 0) {
                if (S_ISREG(file_stat.st_mode) && file_stat.st_uid == my_uid) {
                    char answer;
                    printf("File: %s. Grant read permission to others? (y/n): ", ent->d_name);
                    if (scanf(" %c", &answer) == 1) {
                        if (answer == 'y' || answer == 'Y') {
                            mode_t new_mode = file_stat.st_mode | S_IROTH;
                            if (chmod(ent->d_name, new_mode) == 0) {
                                printf("Permission granted for %s\n", ent->d_name);
                            } else {
                                perror("chmod");
                            }
                        }
                    }
                }
            }
        }
    }

    closedir(dir);
    return 0;
}