#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    pid_t pid = getpid();
    printf("Current Process ID: %d\n", pid);

    int server_fd, new_socket, epoll_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct epoll_event event, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE] = {0};
    int opt = 1;

    // 创建服务器套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定套接字到指定端口
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
         perror("bind failed");
         close(server_fd);
         exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 创建 epoll 实例
    if ((epoll_fd = epoll_create1(0)) == -1) {
        perror("epoll_create1");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    event.events = EPOLLIN;
    event.data.fd = server_fd;

    // 将服务器套接字添加到 epoll 监视列表中
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) { 
        perror("epoll_ctl");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int n_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n_ready == -1) {
            perror("epoll_wait");
            close(server_fd);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < n_ready; i++) {
             if (events[i].data.fd == server_fd) {
                 // 接受新连接
                 if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                     perror("accept");
                     close(server_fd);
                     close(epoll_fd);
                     exit(EXIT_FAILURE);
                }

                event.events = EPOLLIN;
                event.data.fd = new_socket;

                // 将新连接套接字添加到 epoll 监视列表中
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("epoll_ctl");
                    close(new_socket);
                    close(server_fd);
                    close(epoll_fd);
                    exit(EXIT_FAILURE);
                }

                printf("新连接：套接字 %d\n", new_socket);

            } else {
                // 处理来自客户端的消息
                int client_fd = events[i].data.fd;
                int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
                if (bytes_read <= 0) {
                    if (bytes_read == 0) {
                        printf("连接关闭：套接字 %d\n", client_fd);
                    } else { 
                        perror("recv");
                    } 
                    close(client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL); 
                } else {
                    printf("收到消息：%s\n", buffer);
                    if (send(client_fd, buffer, bytes_read, 0) == -1) {
                        perror("send");
                        close(client_fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    }
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}