#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 33306

#define BUFFER_SIZE 128
#define MAX_CLIENTS 10
int client_sockets[MAX_CLIENTS] = {0};
const char *return_str = "The connection has reached its maximum capacity and will be disconnected\n";

int boardcast(int sock, unsigned char *buffer, int len)
{
    int i = 0;
    printf("boardcast %d bytes from client %d:\n", len, sock);
    for(i = 0; i < len; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    for (i = 0; i < sizeof(client_sockets); i++)
    {
        if (client_sockets[i] != 0 && client_sockets[i] != sock)
        {
            printf("send %d bytes to client %d\n", len, client_sockets[i]);
            send(client_sockets[i], buffer, len, MSG_NOSIGNAL);
        }
    }
    return 0;
}
void *handle_connection(void *client_sock)
{                                   // 线程处理客户端的连接
    int sock = *(int *)client_sock; // 获取客户端的socket

    unsigned char buffer[BUFFER_SIZE];
    unsigned char send_buffer[BUFFER_SIZE];
    unsigned char total_buffer[1024];
    unsigned char check_sum = 0;
    int w_index = 0;
    int par_index = 0;

    memset(total_buffer, 0, sizeof(total_buffer));

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0); // 接收客户端发送的数据,并存入buffer,阻塞等待

        if (bytes_received < 0)
        {
            perror("recv");
            close(sock);
            break;
        }
        else if (bytes_received == 0)
        {
            for(int i = 0; i < sizeof(client_sockets); i++)
            {
                if (client_sockets[i] == sock)
                {
                    client_sockets[i] = 0;
                    break;
                }
            }
            printf("client %d disconnected\n", sock);
            close(sock);
            break;
        }
        // printf("rev data=%d\n", bytes_received);

        // 将收到的数据存入total_buffer
        memcpy(total_buffer + w_index, buffer, bytes_received);

        // 更新total_buffer的索引
        w_index += bytes_received;

        // printf("total_buffer: ");
        // for(int i = 0; i < w_index; i++)
        // {
        //     printf("%02x ", total_buffer[i]);
        // }
        // printf("\n");
        par_index = 0;

        // 解析total_buffer中的数据
        // 循环里面会用到i+2，所以i的取值范围是0到w_index-2和sizeof(total_buffer)-2中的较小值
        for(int i = 0; (i < w_index-2) && (i < (sizeof(total_buffer)-2)); i++)
        {
            // 先查找tou 0xaa
            if(total_buffer[i] != 0xaa)
            {
                // 将不符合的数据舍弃掉 par_index表示为已经解析过的数据的索引
                par_index = i+1;
                continue;
            }

            // 头后面的第二个字节存储的是帧长度 确保剩余的数据长度满足一帧
            if(total_buffer[i+2] <= w_index - i)
            {
                // 初始化 check_sum 和 send_buffer
                check_sum = 0;
                memset(send_buffer, 0, sizeof(send_buffer));

                // 计算checksum并将数据同步放入 send_buffer
                for(int j = 0; j < total_buffer[i+2]-1 && j < sizeof(send_buffer); j++)
                {
                    check_sum += total_buffer[i+j];
                    send_buffer[j] = total_buffer[i+j];
                }

                // printf("checksum = %02x\n", check_sum);
                // printf("total_buffer = %02x\n", total_buffer[i+total_buffer[i+2]-1]);

                // 比较checksum
                if(check_sum == total_buffer[i+total_buffer[i+2]-1])
                {
                    send_buffer[total_buffer[i+2]-1] = check_sum;
                    boardcast(sock, send_buffer, total_buffer[i+2]);
                    i = i + total_buffer[i+2]-1;    // 解析成功，直接更新i的index。-1是因为for循环里面会+1
                    par_index = i+1;                // 解析成功，将已经解析过的数据舍弃掉
                    // printf("parse success.\n");
                }
                else
                {
                    printf("data checksum error");
                    par_index = i+1;                // 解析失败，将已经解析过的数据舍弃掉
                }
            }
            else
            {
                // 数据长度不满足一帧，跳出循环
                break;
            }
        }

        if(par_index > 0)
        {
            // 如果total_buffer中还有剩余的数据，将其移动到total_buffer的开头
            for(int i = 0; i < w_index - par_index; i++)
            {
                total_buffer[i] = total_buffer[i + par_index];
            }
            // 将其他数据置为0
            memset(total_buffer+w_index - par_index, 0, sizeof(total_buffer) - w_index + par_index);
            w_index = w_index - par_index;
        }

    }
    printf("client disconnect.\n");
    return NULL;
}

int main()
{
    int i = 0;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    memset(client_sockets, 0, sizeof(client_sockets));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有地址
    address.sin_port = htons(PORT);

    // 设置 SO_REUSEADDR 选项
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    { // 设置最大连接数为10
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen); // 接收客户端的连接请求,并返回socket,阻塞等待
        if (new_socket < 0)
        {
            perror("Accept failed");
            // exit(EXIT_FAILURE);
        }

        printf("New connection established.%d\n", new_socket); // 打印客户端的socket
        for(i = 0; i < sizeof(client_sockets); i++)
        {
            if(client_sockets[i] == 0)
            {
                client_sockets[i] = new_socket;
                break;
            }
        }
        if(i == sizeof(client_sockets))
        {
            // 达到最大连接数，通知app客户端连接失败，然后断开连接
            send(new_socket, return_str, strlen(return_str), 0);
            close(new_socket);
            continue;
        }

        pthread_t thread_id;

        pthread_create(&thread_id, NULL, handle_connection, &client_sockets[i]); // 创建线程处理客户端的连接
        pthread_detach(thread_id);                                        // 线程分离，主线程不等待子线程结束
    }

    close(server_fd);
    return 0;
}

