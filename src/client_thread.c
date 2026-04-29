#include <pthread.h>
#include "Task.h"
#include "client_handler.h"
#include "client_thread.h"

void* client_thread(void *arg)
{
        while (1)
        {
                int fd = popTask();
                handle_client(fd);
        }
        return NULL;
}

void start_client_threads(int num_threads)
{
        for (int i = 0; i < num_threads; i++)
        {
                pthread_t tid;
                pthread_create(&tid, NULL, client_thread, NULL);
                pthread_detach(tid);
        }
}
