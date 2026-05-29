# Practical Work 7

## Task 7.1
### Explanation
This task demonstrates Inter-Process Communication (IPC) using pipes. The `popen()` function is used to execute the `rwho` command, read its output, and pipe it directly into the `more` command. It avoids temporary files by dynamically passing the data stream between processes.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_1
ltsvtbcv ltsvtbcvtmcastr:pts/1 May 29 14:44
```

---

## Task 7.2
### Explanation
This program simulates the behavior of the `ls -l` command without invoking it. It uses system calls like `opendir()`, `readdir()`, and `lstat()` to iterate through the current directory and extract metadata (permissions, links, ownership, size, and modification time). Functions like `getpwuid()` and `getgrgid()` translate numeric UIDs/GIDs into human-readable names.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_2
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr  1797 May 29 14:48 task7_2.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker 70896 May 29 14:48 task7_2
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   489 May 29 14:43 task7_1.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker 70592 May 29 14:43 task7_1
```

---

## Task 7.3
### Explanation
A simplified implementation of the `grep` utility. It reads a specified file line by line using `fgets()` and checks for the presence of a target substring using the `strstr()` function, printing only the matching lines.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_3 include task7_1.c
#include <stdio.h>
#include <stdlib.h>
```

---

## Task 7.4
### Explanation
This is a basic clone of the `more` utility. It reads a file and pauses execution every 20 lines. To ensure it correctly captures the user's keystroke (Enter) even when `stdin` might be redirected, it directly opens and reads from `/dev/tty`.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_4 task7_2.c
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
```

---

## Task 7.5
### Explanation
This program recursively traverses and lists all files in the current directory and its subdirectories. It uses `opendir()` and `readdir()`, ignoring the `.` and `..` references to prevent infinite loops, and recursively calls itself whenever a directory is encountered.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_5
./task7_4.c
./task7_3.c
./task7_2.c
./task7_5
./task7_4
./task7_5.c
./task7_2
./task7_1.c
./task7_1
./task7_3
```

---

## Task 7.6
### Explanation
This tool lists only subdirectories within the current folder, sorted alphabetically. It utilizes the POSIX `scandir()` function combined with a custom filter function (to exclude non-directories) and the built-in `alphasort` for ordering.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_6
A_folder
M_folder
Z_folder
```

---

## Task 7.7
### Explanation
An interactive permission modifier. It scans the current directory for `.c` files owned by the executing user. For each matching file, it prompts the user, and if confirmed, uses the `chmod()` system call to append the `S_IROTH` (read permission for others) flag.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_7
File: task7_4.c. Grant read permission to others? (y/n): y
Permission granted for task7_4.c
File: task7_3.c. Grant read permission to others? (y/n): y
Permission granted for task7_3.c
File: task7_2.c. Grant read permission to others? (y/n): n
File: task7_5.c. Grant read permission to others? (y/n): n
File: task7_6.c. Grant read permission to others? (y/n): y
Permission granted for task7_6.c
File: task7_1.c. Grant read permission to others? (y/n): n
File: task7_7.c. Grant read permission to others? (y/n): y
Permission granted for task7_7.c
```

---

## Task 7.8
### Explanation
An interactive file deletion tool. It iterates through all regular files in the directory. For each file, it requests confirmation. If the user inputs 'y', the file is permanently removed using the `unlink()` system call.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_8
Delete file 'task7_7'? (y/n): n
Delete file 'task7_4.c'? (y/n): n
Delete file 'task7_3.c'? (y/n): n
Delete file 'delete_me_3.txt'? (y/n): y
Deleted 'delete_me_3.txt'
Delete file 'task7_2.c'? (y/n): n
Delete file 'task7_5'? (y/n): n
Delete file 'task7_8.c'? (y/n): n
Delete file 'task7_4'? (y/n): n
Delete file 'task7_8'? (y/n): n
Delete file 'task7_5.c'? (y/n): n
Delete file 'task7_2'? (y/n): n
Delete file 'task7_6.c'? (y/n): n
Delete file 'task7_6'? (y/n): n
Delete file 'delete_me_2.txt'? (y/n): y
Deleted 'delete_me_2.txt'
Delete file 'task7_1.c'? (y/n): n
Delete file 'task7_1'? (y/n): n
Delete file 'task7_7.c'? (y/n): n
Delete file 'delete_me_1.txt'? (y/n): y
Deleted 'delete_me_1.txt'
Delete file 'task7_3'? (y/n): n
```

---

## Task 7.9
### Explanation
This program accurately measures execution time in milliseconds. It utilizes `clock_gettime()` with the `CLOCK_MONOTONIC` flag, which guarantees a continuously incrementing timer immune to system clock adjustments (NTP syncs), making it ideal for performance benchmarking.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_9
Execution time: 73 ms
```

---

## Task 7.10
### Explanation
A demonstration of pseudo-random floating-point number generation. The generator is seeded uniquely using `srand(time(NULL))`. It scales the standard `rand()` integer output (which ranges up to `RAND_MAX`) into floating-point decimals, both mapped to a `[0.0, 1.0]` range and a dynamically scaled `[0.0, n]` range.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_10 5.5
Sequence (0.0 to 1.0):
0.796099
0.134324
0.585511
0.307951
0.756252

Sequence (0.0 to 5.500000):
4.609529
2.460296
0.396665
4.060105
4.112430
```

---

## Task 7.v12
### Explanation
This task implements a wrapper program that logs its execution history entirely in RAM, without creating any log files on the disk. It achieves this by using POSIX Shared Memory (`shm_open`, `ftruncate`, `mmap`). When a command is passed, it formats a timestamped log entry, appends it to the shared memory segment, and executes the command via `fork()` and `execvp()`. The logs persist in memory and can be retrieved using the `--read` flag.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_v12 ls -l
total 228
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70592 \u0442\u0440\u0430 29 14:43 task7_1
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70504 \u0442\u0440\u0430 29 22:12 task7_10
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   552 \u0442\u0440\u0430 29 22:12 task7_10.c
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   489 \u0442\u0440\u0430 29 14:43 task7_1.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70896 \u0442\u0440\u0430 29 14:48 task7_2
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr  1797 \u0442\u0440\u0430 29 14:48 task7_2.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70696 \u0442\u0440\u0430 29 14:51 task7_3
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   505 \u0442\u0440\u0430 29 14:51 task7_3.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70776 \u0442\u0440\u0430 29 14:53 task7_4
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   894 \u0442\u0440\u0430 29 14:53 task7_4.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70712 \u0442\u0440\u0430 29 14:56 task7_5
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   774 \u0442\u0440\u0430 29 14:56 task7_5.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70712 \u0442\u0440\u0430 29 15:01 task7_6
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   741 \u0442\u0440\u0430 29 15:03 task7_6.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70856 \u0442\u0440\u0430 29 15:04 task7_7
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr  1358 \u0442\u0440\u0430 29 15:03 task7_7.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70768 \u0442\u0440\u0430 29 22:06 task7_8
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr  1087 \u0442\u0440\u0430 29 21:49 task7_8.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          70472 \u0442\u0440\u0430 29 22:10 task7_9
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr   430 \u0442\u0440\u0430 29 22:10 task7_9.c
-rwxrwxr-x 1 ltsvtbcvtmcastr docker          71272 \u0442\u0440\u0430 29 22:14 task7_v12
-rw-rw-r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr  1923 \u0442\u0440\u0430 29 22:14 task7_v12.c
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_v12 echo "No log files were created!"
No log files were created!
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_v12 date
\u043f'\u044f\u0442\u043d\u0438\u0446\u044f, 29 \u0442\u0440\u0430\u0432\u043d\u044f 2026 22:14:50 +0000
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz7$ ./task7_v12 --read
--- In-Memory Execution Log ---
[Fri May 29 22:14:27 2026] User ID: 1000, Command: ls -l 
[Fri May 29 22:14:42 2026] User ID: 1000, Command: echo No log files were created! 
[Fri May 29 22:14:50 2026] User ID: 1000, Command: date 
```