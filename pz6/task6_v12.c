#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Task {
    void (*function)(void *);
    void *argument;
    struct Task *next;
} Task;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    Task *task_queue_head;
    Task *task_queue_tail;
    int thread_count;
    int shutdown;
} ThreadPool;

void *thread_worker(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    while (1) {
        pthread_mutex_lock(&(pool->lock));

        while (pool->task_queue_head == NULL && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        if (pool->shutdown && pool->task_queue_head == NULL) {
            pthread_mutex_unlock(&(pool->lock));
            break;
        }

        Task *task = pool->task_queue_head;
        if (task != NULL) {
            pool->task_queue_head = task->next;
            if (pool->task_queue_head == NULL) {
                pool->task_queue_tail = NULL;
            }
        }

        pthread_mutex_unlock(&(pool->lock));

        if (task != NULL) {
            (*(task->function))(task->argument);
            free(task);
        }
    }
    pthread_exit(NULL);
    return NULL;
}

ThreadPool *thread_pool_create(int num_threads) {
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    pool->thread_count = num_threads;
    pool->shutdown = 0;
    pool->task_queue_head = NULL;
    pool->task_queue_tail = NULL;

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&(pool->threads[i]), NULL, thread_worker, (void *)pool);
    }
    return pool;
}

void thread_pool_submit(ThreadPool *pool, void (*function)(void *), void *argument) {
    Task *new_task = (Task *)malloc(sizeof(Task));
    new_task->function = function;
    new_task->argument = argument;
    new_task->next = NULL;

    pthread_mutex_lock(&(pool->lock));

    if (pool->task_queue_tail == NULL) {
        pool->task_queue_head = new_task;
        pool->task_queue_tail = new_task;
    } else {
        pool->task_queue_tail->next = new_task;
        pool->task_queue_tail = new_task;
    }

    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
}

void thread_pool_destroy(ThreadPool *pool) {
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = 1;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool->threads);
    free(pool);
}

void dummy_task(void *arg) {
    int id = *((int *)arg);
    printf("Task %d is running on thread %lu\n", id, pthread_self());
    usleep(100000); 
}

int main() {
    ThreadPool *pool = thread_pool_create(4);

    int task_ids[10];
    for (int i = 0; i < 10; i++) {
        task_ids[i] = i;
        thread_pool_submit(pool, dummy_task, &task_ids[i]);
    }

    sleep(2);

    thread_pool_destroy(pool);
    printf("Thread pool destroyed. All tasks completed.\n");

    return 0;
}