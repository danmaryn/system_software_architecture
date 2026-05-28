# Practical Work 3

## Task 3.1
### Explanation
This test demonstrates viewing and modifying soft and hard limits for open file descriptors using the `ulimit` utility. It confirms the core security principle of resource limits: while an unprivileged user can lower their hard limit, they cannot raise it back up during the current session without root privileges.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ docker run -it --rm ubuntu bash
root@fe5a0e7dce55:/# useradd -m testuser
root@fe5a0e7dce55:/# su - testuser
$ ulimit -n
1024
$ulimit -aS | grep "open files"$ ulimit -aH | grep "open files"
$ulimit -n 3000$ ulimit -aS | grep "open files"
$ulimit -aH | grep "open files"$ ulimit -n 3001
-sh: 7: ulimit: error setting limit (Operation not permitted)
$ulimit -n 2000$ ulimit -n
2000
$ulimit -aS | grep "open files"$ ulimit -aH | grep "open files"
$ ulimit -n 3000
-sh: 12: ulimit: error setting limit (Operation not permitted)
$ exit
root@fe5a0e7dce55:/# ulimit -n
1024
root@fe5a0e7dce55:/# ulimit -aS | grep "open files"
open files                          (-n) 1024
root@fe5a0e7dce55:/# ulimit -aH | grep "open files"
open files                          (-n) 524288
root@fe5a0e7dce55:/# ulimit -n 3000
root@fe5a0e7dce55:/# ulimit -aS | grep "open files"
open files                          (-n) 3000
root@fe5a0e7dce55:/# ulimit -aH | grep "open files"
open files                          (-n) 3000
root@fe5a0e7dce55:/# ulimit -n 3001
bash: ulimit: open files: cannot modify limit: Operation not permitted
root@fe5a0e7dce55:/# ulimit -n 2000
root@fe5a0e7dce55:/# ulimit -n
2000
root@fe5a0e7dce55:/# ulimit -aS | grep "open files"
open files                          (-n) 2000
root@fe5a0e7dce55:/# ulimit -aH | grep "open files"
open files                          (-n) 2000
root@fe5a0e7dce55:/# ulimit -n 3000
bash: ulimit: open files: cannot modify limit: Operation not permitted
root@fe5a0e7dce55:/# exit
exit
```

---

## Task 3.2
### Explanation
This experiment demonstrates restricting CPU time for a specific process. By applying a 2-second hard limit via `ulimit -t`, the infinite loop within the application is forcefully terminated (`Killed`) by the OS kernel once the quota is exhausted, which is precisely tracked and verified by the `perf stat` output.

### Result
```text
perf stat ./task3_2
./task3_2: Killed

 Performance counter stats for './task3_2':

                 6      context-switches                 #      3.0 cs/sec  cs_per_second      
                 0      cpu-migrations                   #      0.0 migrations/sec  migrations_per_second
                41      page-faults                      #     20.5 faults/sec  page_faults_per_second
           2001.60 msec task-clock                       #      1.0 CPUs  CPUs_utilized        
   <not supported>      cpu-cycles                       #      nan GHz  cycles_frequency      

       2.001968900 seconds time elapsed

       2.001928000 seconds user
       0.000000000 seconds sys
```

---

## Task 3.3
### Explanation
This test evaluates maximum file size constraints (`ulimit -f`). The process traps the `SIGXFSZ` signal and checks for the `EFBIG` error code during file operations. Once the generated output hits the restricted boundary of 2 blocks (2048 bytes), the program safely stops writing and handles the limit organically.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ bash -c 'ulimit -f 2; ./task3_3'
File size limit exceeded!
Total rolls: 179
```

---

## Task 3.4
### Explanation
This task highlights the architectural difference between soft and hard resource limits. By establishing a soft CPU time limit (`-S -t 1`) and a slightly higher hard limit (`-H -t 2`), the application successfully intercepts the `SIGXCPU` signal at the 1-second mark, safely halting calculations and logging the total operations before a forced termination occurs.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ bash -c 'ulimit -S -t 1; ulimit -H -t 2; ./task3_4'
CPU time limit exceeded!
Total combinations generated: 9917496
```

---

## Task 3.5
### Explanation
This program verifies robust error handling logic during file I/O operations. It dynamically handles multiple failure states: missing input arguments, unreadable source files, and writing procedures that breach OS-level file size constraints, successfully recovering or exiting appropriately in all scenarios.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ head -c 5000 /dev/urandom > source.txt
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ ./task3_5
Program need two arguments
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ ./task3_5 non_existent_file.txt dest.txt
Cannot open file non_existent_file.txt for reading
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ bash -c 'ulimit -f 2; ./task3_5 source.txt dest.txt'
File size limit exceeded!
```

---

## Task 3.6
### Explanation
This execution showcases the effects of stack memory limitations (`ulimit -s`). The program initiates a deep recursive function call loop, actively consuming the stack payload. Once it surpasses the artificially imposed 1024 KB boundary, the operating system intervenes, causing a predictable `Segmentation fault`.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ bash -c 'ulimit -s 1024; ./task3_6'
Starting recursion...
Current stack depth: 100
Current stack depth: 200
Current stack depth: 300
Current stack depth: 400
Current stack depth: 500
Current stack depth: 600
Current stack depth: 700
Current stack depth: 800
Current stack depth: 900
Segmentation fault (core dumped)
```

---

## Task 3.v12
### Explanation
This routine audits the OS restrictions applied to active file descriptors (`ulimit -n`). With the upper limit strictly constrained to 100, the process iterates to open valid system files. It successfully establishes 97 active file streams before exhausting its structural quota (the remaining 3 descriptors being allocated to standard input, output, and error pipelines).

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz3$ bash -c 'ulimit -n 100; ./task3_v12'
Failed to open file at index 97
Error: Too many open files
Total files opened successfully: 97
```