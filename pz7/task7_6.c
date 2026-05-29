#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int filter_dirs(const struct dirent *entry) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        return 0;
    }

    struct stat statbuf;
    if (lstat(entry->d_name, &statbuf) == 0) {
        return S_ISDIR(statbuf.st_mode);
    }
    return 0;
}

int main() {
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, filter_dirs, alphasort);
    if (n < 0) {
        perror("scandir");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("%s\n", namelist[i]->d_name);
        free(namelist[i]);
    }
    free(namelist);

    return 0;
}