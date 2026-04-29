#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <errno.h>
#include "Task.h"
#include "nonblock.h"
#include "event_loop.h"

#define MAX_EVENTS 1024

void event_loop(int epoll_fd, int server_fd)
{
        struct epoll_event events[MAX_EVENTS];
        struct epoll_event ev;
        
        while (1)
        {
                int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
                
                for (int i = 0; i < n; i++)
                {
                        int fd = events[i].data.fd;

                        if (fd == server_fd)
                        {
                                while (1)
                                {
                                        int client_fd = accept(server_fd, NULL, NULL);
                                        if (client_fd < 0)
                                        {
                                                if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                                                perror("accept");
                                                break;
                                        }
                                        set_nonblock(client_fd);

                                        ev.events = EPOLLIN | EPOLLET;
                                        ev.data.fd = client_fd;
                                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                                }
                        }
                        else
                        {
                                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                                pushTask(fd);
                        }
                }
        }
}
