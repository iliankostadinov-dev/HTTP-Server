#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include "client_handler.h"

#define BUFF_SIZE 8192

const char* getFileType(const char* path)
{
        if (strstr(path, ".html")) return "text/html";
        if (strstr(path, ".css")) return "text/css";
        if (strstr(path, ".js")) return "application/javascript";
        if (strstr(path, ".png")) return "image/png";
        if (strstr(path, ".jpg")) return "image/jpeg";
        if (strstr(path, ".jpeg")) return "image/jpeg";
        if (strstr(path, ".gif")) return "image/gif";
        if (strstr(path, ".txt")) return "text/plain";
        return "application/octet-stream";
}

ssize_t send_all(int client_fd, const void *data, size_t len)
{
        size_t sent = 0;
        const char *ptr = (const char*)data;
        while (sent < len)
        {
                ssize_t n = send(client_fd, ptr + sent, len - sent, 0);
                if (n <= 0)
                {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
                        else return -1; 
                }
                sent += n;
        }
        return sent;
}

void handle_client(int client_fd)
{
        char buff[BUFF_SIZE];
        ssize_t n;
        int keep_alive = 1;

        while (keep_alive)
        {
                n = recv(client_fd, buff, sizeof(buff) - 1, 0);
                if (n == -1)
                {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                                continue;
                        else
                        {
                                perror("recv");
                                break;
                        }
                }
                else if (n == 0)
                    break;
                
                buff[n] = '\0';

                char method[16], path[256];
                if (sscanf(buff, "%s %s", method, path) != 2)
                        break;

                if (strcmp(path, "/") == 0)
                        strcpy(path, "/index.html");

                char filepath[512];
                snprintf(filepath, sizeof(filepath), "html%s", path);

                printf("Client fd = %d: %s %s\n", client_fd, method, path);

                if (strcmp(method, "GET") == 0)
                {
                        FILE *pf = fopen(filepath, "rb");
                        char hdr[BUFF_SIZE];
                        int hlen;
                
                        if (!pf)
                        {
                                const char *msg = "<h1>404 Not Found</h1>";
                                hlen = snprintf(hdr, sizeof(hdr),
                                    "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Content-Length: %zu\r\n"
                                    "Connection: close\r\n\r\n",
                                    strlen(msg));
                                
                                printf(" HTTP 404 \n%s\n", hdr);
                                
                                send_all(client_fd, hdr, hlen);
                                send_all(client_fd, msg, strlen(msg));
                                break;
                        }

                        fseek(pf, 0, SEEK_END);
                        long size = ftell(pf);
                        fseek(pf, 0, SEEK_SET);

                        hlen = snprintf(hdr, sizeof(hdr),
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: %s\r\n"
                            "Content-Length: %ld\r\n"
                            "Connection: keep-alive\r\n\r\n",
                            getFileType(filepath), size);
                        
                        printf(" HTTP SENT \n%s\n\n", hdr);

                        send_all(client_fd, hdr, hlen);
                        while ((n = fread(buff, 1, sizeof(buff), pf)) > 0)
                                send_all(client_fd, buff, n);

                        fclose(pf);
                }

                if (strstr(buff, "Connection: close"))
                    keep_alive = 0;
        }

        close(client_fd);
}
