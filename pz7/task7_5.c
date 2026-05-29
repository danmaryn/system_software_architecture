#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list_files(const char *base_path) {
    DIR *dir = opendir(base_path);
    if (!dir) {
        return;
    }

    struct dirent *dp;
    char path[2048];

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", base_path, dp->d_name);
            printf("%s\n", path);

            struct stat statbuf;
            if (lstat(path, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    list_files(path);
                }
            }
        }
    }
    closedir(dir);
}

int main() {
    list_files(".");
    return 0;
}