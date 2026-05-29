# Practical Work 8

## Task 8.1
### Explanation
This task investigates whether the `write()` system call can return a byte count less than the requested `nbytes`. The program demonstrates this by attempting to write 1MB of data into a non-blocking POSIX pipe. Since the default pipe buffer capacity in Linux is exactly 64KB (65536 bytes), the `write()` call successfully writes only what fits into the buffer and returns that partial amount, proving that partial writes occur when buffers are full.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz8$ ./task8_1
Attempting to write 1048576 bytes to a non-blocking pipe...
Requested nbytes: 1048576
Actual count returned: 65536
Conclusion: Yes, count can be less than nbytes! (Pipe buffer filled up)
```

---

## Task 8.2
### Explanation
This program demonstrates the behavior of the `lseek()` and `read()` system calls. A file is populated with the byte sequence `4, 5, 2, 2, 3, 3, 7, 9, 1, 5`. The `lseek(fd, 3, SEEK_SET)` call moves the file offset to index 3 (the fourth byte). A subsequent `read(fd, &buffer, 4)` fetches the next 4 bytes starting from that offset, correctly returning `2, 3, 3, 7`.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz8$ ./task8_2
Buffer contains: 2 3 3 7 
```

---

## Task 8.3
### Explanation
This task evaluates the performance of the standard library `qsort()` function against various dataset arrangements to identify its worst-case scenario. It generates arrays of 5,000,000 integers structured as random, strictly sorted, reverse sorted, identical elements, and an "organ pipe" distribution. The timing results show that modern `glibc` implementations of `qsort` are highly optimized against traditional worst-case scenarios (like sorted arrays), making fully random data the most time-consuming to sort.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz8$ ./task8_3
Correctness test passed.

Testing with array size: 5000000
Random data:      0.626010 seconds
Sorted data:      0.089056 seconds
Reverse sorted:   0.117529 seconds
Identical data:   0.111936 seconds
Organ pipe data:  0.103392 seconds
```

---

## Task 8.4
### Explanation
This task explores the core mechanics of process creation using `fork()`. When `fork()` is executed successfully, it duplicates the current process. It returns `0` to the newly created child process and returns the actual Process ID (PID) of the child to the parent process. The output confirms this parallel execution, printing both the child's PID and `0`.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz8$ ./task8_4
101706
0
```

---

## Task 8.v12
### Explanation
This task simulates a partial file reading scenario with parallel writing from another process. Using `fork()`, the program creates a writer process that writes to a file in two chunks with a `sleep()` delay between them. Simultaneously, a reader process continuously polls the file. The output illustrates the reader encountering an EOF initially, then reading the first chunk ("Part 1"), and finally reading the remaining chunk ("Part 2") once the writer completes its delay and flushes the rest of the data.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz8$ ./task8_v12
[Reader] EOF reached. Waiting for more data...
[Reader] Read 17 bytes: Part 1: Hello...
[Reader] Read 15 bytes: Part 2: World!
```