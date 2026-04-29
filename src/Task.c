#include <pthread.h>
#include <stdlib.h>
#include "Task.h"

typedef struct Task
{
        int client_fd;
        struct Task *next;
} Task;

Task *task_head = NULL;
Task *task_tail = NULL;
pthread_mutex_t task_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t task_cond = PTHREAD_COND_INITIALIZER;

void pushTask(int client_fd)
{
        Task *t = malloc(sizeof(Task));
        t->client_fd = client_fd;
        t->next = NULL;

        pthread_mutex_lock(&task_lock);
        if (task_tail)
                task_tail->next = t;
        else
                task_head = t;
        task_tail = t;
        pthread_cond_signal(&task_cond);
        pthread_mutex_unlock(&task_lock);
}

int popTask(void)
{
        pthread_mutex_lock(&task_lock);
        while (!task_head)
                pthread_cond_wait(&task_cond, &task_lock);

        Task *t = task_head;
        task_head = t->next;
        if (!task_head)
                task_tail = NULL;

        int fd = t->client_fd;
        free(t);
        pthread_mutex_unlock(&task_lock);
        return fd;
}
