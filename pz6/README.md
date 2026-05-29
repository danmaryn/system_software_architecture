# Practical Work 6

## Task 6.v12
### Explanation
This task involves the implementation of a custom thread pool in C using POSIX threads, mutexes, and condition variables. The architecture allows a fixed number of worker threads to continuously poll a shared task queue, wait for available work, and execute tasks dynamically. 

The program was compiled and analyzed using ThreadSanitizer (`-fsanitize=thread`). The absence of any warnings or errors confirms that the task submission and execution mechanisms are thread-safe and free of data races. The execution output demonstrates 10 distinct tasks being efficiently distributed and processed by exactly 4 reused worker threads, validating the core performance benefit of a thread pool.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz6$ ./task6_v12
Task 0 is running on thread 279715422136768
Task 1 is running on thread 279715409553856
Task 2 is running on thread 279715401099712
Task 3 is running on thread 279715392645568
Task 4 is running on thread 279715422136768
Task 5 is running on thread 279715409553856
Task 7 is running on thread 279715392645568
Task 6 is running on thread 279715401099712
Task 8 is running on thread 279715409553856
Task 9 is running on thread 279715392645568
Thread pool destroyed. All tasks completed.
```