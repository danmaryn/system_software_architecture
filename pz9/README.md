# Practical Work 9

## Task 9.1
### Explanation
This script reads the `/etc/passwd` file using the `getent passwd` command to list all regular users defined on the system. It filters the output by checking if the User ID (UID) is greater than or equal to 1000, less than 60000 (to exclude `nobody`), and ensures it does not match the UID of the currently executing user.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_1.sh
My UID: 1000
Other regular users (UID >= 1000):
----------------------------------
No other regular users found on this system.
```

---

## Task 9.2
### Explanation
This script demonstrates privilege escalation using `sudo`. It attempts to read the `/etc/shadow` file, which contains user password hashes and is strictly accessible only to the root user. By prefixing the command with `sudo`, the regular user temporarily gains administrative rights to read the restricted file.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_2.sh
[sudo] password for ltsvtbcvtmcastr: 
root:*:20306:0:99999:7:::
daemon:*:20306:0:99999:7:::
bin:*:20306:0:99999:7:::
...
ltsvtbcvtmcastr:$6$<REDACTED_PASSWORD_HASH>:20487:0:99999:7:::
...
rwhod:!:20602::::::
```

---

## Task 9.3
### Explanation
This script explores how file and directory permissions interact. A file created by a regular user is copied by `root` into the user's home directory. The script then shows that the regular user cannot modify the copied file because it is owned by `root`. However, the regular user can successfully delete the file because file deletion is governed by the write permissions of the parent directory (the user's home directory), not the file itself.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_3.sh
Attempting to modify the file as a regular user...
./task9_3.sh: line 8: /home/ltsvtbcvtmcastr/root_copied_file.txt: Permission denied
Attempting to delete the file as a regular user...
The file was successfully deleted.
```

---

## Task 9.4
### Explanation
This script uses the `whoami` and `id` commands to display the effective identity and group memberships of the executing user. The `id` command provides a comprehensive view, including the primary User ID (UID), primary Group ID (GID), and a list of all supplementary groups the user belongs to (such as `sudo`, `docker`, etc.).

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_4.sh
Command: whoami
ltsvtbcvtmcastr

Command: id
uid=1000(ltsvtbcvtmcastr) gid=125(docker) groups=125(docker),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),101(lxd),1000(ltsvtbcvtmcastr)
```

---

## Task 9.5
### Explanation
This script tests read and write access under varying permission sets. A temporary file is created and its ownership is changed to `root`. The script then tests access when the file permissions are `644` (read for others), `600` (no access for others), and `666` (read and write for others). It successfully demonstrates that since the executing user is neither the owner (`root`) nor in the owning group, access is strictly determined by the "others" permission bits.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_5.sh
Permissions: 664 | Owner: ltsvtbcvtmcastr:docker
Read: YES
Write: YES

Permissions: 644 | Owner: root:root
Read: YES
./task9_5.sh: line 16: temp_file.txt: Permission denied
Write: NO

Permissions: 600 | Owner: root:root
Read: NO
./task9_5.sh: line 16: temp_file.txt: Permission denied
Write: NO

Permissions: 666 | Owner: root:root
Read: YES
Write: YES
```

---

## Task 9.6
### Explanation
This script audits access rights for three distinct files: a user-owned configuration file (`~/.bashrc`), a strictly protected system file (`/etc/shadow`), and a system executable (`/usr/bin/ls`). It attempts to read, write, and execute each file, proving that the operating system correctly enforces file boundaries—allowing full access to user files, denying access to shadow files, and permitting only reading/execution of system binaries.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_6.sh
--- File: /home/ltsvtbcvtmcastr/.bashrc ---
-rw-r--r-- 1 ltsvtbcvtmcastr ltsvtbcvtmcastr 3771 бер 31  2024 /home/ltsvtbcvtmcastr/.bashrc
Attempting to read...
Read: SUCCESS
Attempting to write...
Write: SUCCESS
Attempting to execute...
Execute: DENIED / FAILED

--- File: /etc/shadow ---
-rw-r----- 1 root shadow 1435 тра 29 14:44 /etc/shadow
Attempting to read...
Read: DENIED
Attempting to write...
./task9_6.sh: line 17: /etc/shadow: Permission denied
Write: DENIED
Attempting to execute...
Execute: DENIED / FAILED

--- File: /usr/bin/ls ---
-rwxr-xr-x 1 root root 199464 чер 22  2025 /usr/bin/ls
Attempting to read...
Read: SUCCESS
Attempting to write...
./task9_6.sh: line 17: /usr/bin/ls: Permission denied
Write: DENIED
Attempting to execute...
Execute: SUCCESS
```

---

## Task 9.v12
### Explanation
This task investigates the behavior of the `fork()` mechanism when system resources are artificially restricted. The script determines the current number of processes owned by the user and sets a hard limit (`ulimit -u`) to just slightly above that number. It then runs a loop attempting to spawn multiple background processes. The output demonstrates that once the resource threshold is hit, the shell fails to `fork` new child processes, throwing a "Resource temporarily unavailable" (EAGAIN) error.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz9$ ./task9_v12.sh
Current processes: 124
Setting max processes limit to: 129
./task9_v12.sh: fork: retry: Resource temporarily unavailable
./task9_v12.sh: fork: retry: Resource temporarily unavailable
./task9_v12.sh: fork: retry: Resource temporarily unavailable
./task9_v12.sh: fork: retry: Resource temporarily unavailable
./task9_v12.sh: fork: Resource temporarily unavailable
```