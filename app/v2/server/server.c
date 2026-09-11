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

// 帧大小的最小值
#define FRAME_MIN_SIZE (4)
void *handle_connection(void *client_sock)
{                                   // 线程处理客户端的连接
    int sock = *(int *)client_sock; // 获取客户端的socket

    unsigned char buffer[BUFFER_SIZE];
    unsigned char send_buffer[BUFFER_SIZE];
    unsigned char total_buffer[1024];
    unsigned char check_sum = 0;
    int total_len = 0;
    int par_index = 0;
    unsigned char data_len = 0;
    unsigned char frame_len = 0;
    int i = 0;

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
        memcpy(total_buffer + total_len, buffer, bytes_received);

        // 更新total_buffer的索引
        total_len += bytes_received;

        i = 0;
        unsigned char *pstr = NULL;
        int parse_pos = 0;
        int invalid_pos = 0;
        i = 0;
        while(i < total_len)
        {
            if(total_buffer[i] == 0xaa)
            {
                parse_pos = i;
            }
            else
            {
                invalid_pos = i;
                i++;
                continue;
            }

            if(i+1 >= total_len)
            {
                break;
            }

            frame_len = total_buffer[i+1];
            if(frame_len < FRAME_MIN_SIZE)
            {
                // 帧长度不正确，跳过
                i++;
                continue;

            }
            if(frame_len+i > total_len)
            {
                break;
            }

            pstr = &total_buffer[i];
            check_sum = 0;
            for(int j = 0; j < frame_len; j++)
            {
                send_buffer[j] = pstr[j];
                if((2 <= j) && (j < (frame_len-1)))
                {
                    check_sum += send_buffer[j];
                }
            }

            if(check_sum == send_buffer[frame_len-1])
            {
                // for(int j = 0; j < frame_len; j++)
                // {
                //     printf("%02x ", send_buffer[j]);
                // }
                // printf("\n");
                boardcast(sock, send_buffer, frame_len);
            }

            i = i + frame_len;
        }

        if((invalid_pos > parse_pos) || (i == total_len))
        {
            memset(total_buffer, 0, sizeof(total_buffer));
            total_len = 0;
        }

        if((invalid_pos < parse_pos) && (i < total_len))
        {
            for(int j = 0; j < (total_len-i); j++)
            {
                total_buffer[j] = total_buffer[j+i];
            }
            total_len = total_len - i;
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

