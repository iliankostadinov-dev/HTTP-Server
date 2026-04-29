#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#include "nonblock.h"
#include "client_thread.h"
#include "event_loop.h"

#define PORT 8080
#define THREADS 8

int main(void)
{
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0)
        { 
                perror("socket"); 
                exit(EXIT_FAILURE); 
        }

        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        { 
                perror("bind"); 
                exit(EXIT_FAILURE); 
        }
        
        if (listen(server_fd, 512) == -1)
        { 
                perror("listen"); 
                exit(EXIT_FAILURE); 
        }

        set_nonblock(server_fd);

        int ep = epoll_create1(0);
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = server_fd;
        epoll_ctl(ep, EPOLL_CTL_ADD, server_fd, &ev);

        start_client_threads(THREADS);

        printf("Server running on http://localhost:%d/\n", PORT);
        
        event_loop(ep, server_fd);

        close(server_fd);
        return 0;
}
