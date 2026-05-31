# Practical Work 10

## Example 1: Crash Diagnostics (crash_diag)
### Explanation
This example demonstrates how to handle fatal signals like `SIGSEGV` instead of letting the program crash silently. [cite_start]It uses `sigaction()` with the `SA_SIGINFO` flag to extract diagnostic information, such as the signal number and the exact memory fault address[cite: 169]. This allows for a safe and informative exit process.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz10$ ./crash_diag
About to crash. PID=109538

=== crash captured ===
signal: 11
si_code: 1
fault address: 0x0
Register dump is implemented here only for x86-64.
```

---

## Example 2: Correct Sleeping (sleep_correct)
### Explanation
[cite_start]This program illustrates the proper way to handle sleep functions that are interrupted by signals, which return `EINTR`[cite: 209]. [cite_start]For relative sleep, it implements a loop using `nanosleep()` to resume sleeping for the remaining time[cite: 211]. [cite_start]For periodic tasks, it prevents time drift by using `clock_nanosleep()` with an absolute deadline (`TIMER_ABSTIME`)[cite: 215].

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz10$ ./sleep_correct
PID=109667. In another terminal: kill -USR1 109667
Relative sleep for 5 seconds using nanosleep restart loop...
Relative sleep finished. got_usr1=1
Now 5 periodic ticks with absolute clock_nanosleep deadlines...
tick 1
tick 2
tick 3
tick 4
tick 5
```

---

## Example 3: Publisher-Subscriber (rt_pubsub)
### Explanation
This code demonstrates a lightweight IPC publisher-subscriber mechanism using real-time signals[cite: 247]. The publisher utilizes `sigqueue()` to send a signal along with an integer payload[cite: 250]. The subscriber synchronously waits for these signals using `sigwaitinfo()` to safely receive and read the `si_value`[cite: 250].

### Result
**Terminal 1 (Subscriber):**
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz10$ ./rt_pubsub sub
subscriber PID=109764, waiting for signal 34 (SIGRTMIN)
received signal=34 value=10 from pid=109774 uid=1000
received signal=34 value=20 from pid=109774 uid=1000
received signal=34 value=-1 from pid=109774 uid=1000
negative value received: shutting down subscriber
```

**Terminal 2 (Publisher):**
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz10$ ./rt_pubsub pub 109764 10 20 -1
sent value=10 to pid=109764 via signal=34
sent value=20 to pid=109764 via signal=34
sent value=-1 to pid=109764 via signal=34
```

---

## Task 10.v12 (Individual Task)
### Explanation
The task requires a child process to execute the command `echo Hello from child` by passing parameters through `execlp()`[cite: 137]. The parent process safely uses `fork()` to create the child and waits for its completion, while the child successfully replaces its execution image with the `echo` utility.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz10$ ./task10_v12
Hello from child
```