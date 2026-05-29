#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void print_permissions(mode_t mode) {
    printf( (S_ISDIR(mode)) ? "d" : "-");
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
}

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
        if (ent->d_name[0] == '.') {
            continue;
        }

        if (lstat(ent->d_name, &file_stat) == -1) {
            perror("lstat");
            continue;
        }

        print_permissions(file_stat.st_mode);
        printf(" %lu ", (unsigned long)file_stat.st_nlink);

        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group *gr = getgrgid(file_stat.st_gid);

        if (pw != NULL) printf("%s ", pw->pw_name);
        else printf("%d ", file_stat.st_uid);

        if (gr != NULL) printf("%s ", gr->gr_name);
        else printf("%d ", file_stat.st_gid);

        printf("%5lld ", (long long)file_stat.st_size);

        char time_buf[256];
        struct tm *tm_info = localtime(&file_stat.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
        printf("%s ", time_buf);

        printf("%s\n", ent->d_name);
    }

    closedir(dir);
    return 0;
}