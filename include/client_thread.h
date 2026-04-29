#ifndef CLIENT_THREADS_H
#define CLIENT_THREADS_H

void* client_thread(void *arg);
void start_client_threads(int num_threads);

#endif
