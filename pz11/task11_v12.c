#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void subscriber(int id, int sig) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, sig);
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("Subscriber %d (PID %d) listening on signal %d\n", id, getpid(), sig);

    for (int i = 0; i < 2; i++) {
        siginfo_t si;
        if (sigwaitinfo(&set, &si) != -1) {
            printf("Subscriber %d (Priority %d) received message: %d\n", id, sig - SIGRTMIN, si.si_value.sival_int);
        }
    }
    exit(0);
}

int main() {
    pid_t pids[3];
    int sigs[3] = { SIGRTMIN + 1, SIGRTMIN + 2, SIGRTMIN + 3 };

    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            subscriber(i + 1, sigs[i]);
        }
    }

    sleep(1);

    for (int j = 1; j <= 2; j++) {
        for (int i = 0; i < 3; i++) {
            union sigval val;
            val.sival_int = j * 100 + i;
            sigqueue(pids[i], sigs[i], val);
        }
    }

    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}