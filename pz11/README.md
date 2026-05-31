# Practical Work 11

## Task 11.v12 (Individual Task)
### Explanation
This task implements a publisher-subscriber system using POSIX real-time signals (`SIGRTMIN + N`). The parent process acts as the publisher, spawning three child processes that act as subscribers. Each subscriber uses `sigprocmask()` to block its uniquely assigned real-time signal and then synchronously waits for it using `sigwaitinfo()`. The publisher uses `sigqueue()` to send signals along with an integer payload (`sigval`) to each subscriber, demonstrating a lightweight, priority-based IPC mechanism.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz11$ ./task11_v12
Subscriber 1 (PID 110008) listening on signal 35
Subscriber 2 (PID 110009) listening on signal 36
Subscriber 3 (PID 110010) listening on signal 37
Subscriber 2 (Priority 2) received message: 101
Subscriber 2 (Priority 2) received message: 201
Subscriber 3 (Priority 3) received message: 102
Subscriber 3 (Priority 3) received message: 202
Subscriber 1 (Priority 1) received message: 100
Subscriber 1 (Priority 1) received message: 200
```