#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <pthread.h>

#define PORT 33306
#define BUFFER_SIZE 1024

void *handle_connection(void *client_sock)
{
    int i = 0;
    unsigned char buffer[BUFFER_SIZE];
    int sock = *(int *)client_sock; // 获取客户端的socket
    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int len = recv(sock, buffer, BUFFER_SIZE, 0);
        if(len <= 0)
        {
            printf("%s %s %d: recv error\n", __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        for(i = 0; i < len; i++)
        {
            printf("%02x ", buffer[i]);
        }
        printf("\n");
        fflush(stdout);
    }
    return NULL;
}
int send_socket_data(int sock, unsigned char *data, int len)
{
    if(sock <= 0 || data == NULL || len !=  69)
    {
        printf("%s %s %d: param error\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    srandom(time(NULL));
    memset(data, 0, len);
    data[0] = 0xaa;
    data[1] = 0x02;
    data[2] = 69;

    // 分机1
    data[3] = 0x00;
    data[4] = random()%240;
    data[5] = 0x00;
    data[6] = random()%240;

    data[7] = 0x00;
    data[8] = random()%240;
    data[9] = 0x00;
    data[10] = random()%240;

    data[11] = 0x00;
    data[12] = random()%240;
    data[13] = 0x00;
    data[14] = random()%240;

    // 分机2
    data[15] = 0x00;
    data[16] = random()%240;
    data[17] = 0x00;
    data[18] = random()%240;

    data[19] = 0x00;
    data[20] = random()%240;
    data[21] = 0x00;
    data[22] = random()%240;

    data[23] = 0x00;
    data[24] = random()%240;
    data[25] = 0x00;
    data[26] = random()%240;

    // 分机3
    data[27] = 0x00;
    data[28] = random()%240;
    data[29] = 0x00;
    data[30] = random()%240;

    data[31] = 0x00;
    data[32] = random()%240;
    data[33] = 0x00;
    data[34] = random()%240;

    data[35] = 0x00;
    data[36] = random()%240;
    data[37] = 0x00;
    data[38] = random()%240;

    // 分机4
    data[39] = 0x00;
    data[40] = random()%240;
    data[41] = 0x00;
    data[42] = random()%240;

    data[43] = 0x00;
    data[44] = random()%240;
    data[45] = 0x00;
    data[46] = random()%240;

    data[47] = 0x00;
    data[48] = random()%240;
    data[49] = 0x00;
    data[50] = random()%240;

    // 分机5
    data[51] = 0x00;
    data[52] = random()%240;
    data[53] = 0x00;
    data[54] = random()%240;

    data[55] = 0x00;
    data[56] = random()%240;
    data[57] = 0x00;
    data[58] = random()%240;

    data[59] = 0x00;
    data[60] = random()%240;
    data[61] = 0x00;
    data[62] = random()%240;

    // CTL
    data[63] = 0x80 | (random()%64);
    data[64] = 0x80 | (random()%64);
    data[65] = 0x80 | (random()%64);
    data[66] = 0x80 | (random()%64);
    data[67] = 0x80 | (random()%64);

    data[68] = 0x00;
    for(int i = 0; i < len-1; i++)
    {
        data[68] += data[i];
    }
    for(int i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
    }
    send(sock, data, 69, 0);
    return 0;
}


int send_socket_data2(int sock, unsigned char *data, int len)
{
    unsigned char total_buffer[1024];
    if(sock <= 0 || data == NULL || len !=  69)
    {
        printf("%s %s %d: param error\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    srandom(time(NULL));
    memset(data, 0, len);
    data[0] = 0xaa;
    data[1] = 0x02;
    data[2] = 69;

    // 分机1
    data[3] = 0x00;
    data[4] = random()%240;
    data[5] = 0x00;
    data[6] = random()%240;

    data[7] = 0x00;
    data[8] = random()%240;
    data[9] = 0x00;
    data[10] = random()%240;

    data[11] = 0x00;
    data[12] = random()%240;
    data[13] = 0x00;
    data[14] = random()%240;

    // 分机2
    data[15] = 0x00;
    data[16] = random()%240;
    data[17] = 0x00;
    data[18] = random()%240;

    data[19] = 0x00;
    data[20] = random()%240;
    data[21] = 0x00;
    data[22] = random()%240;

    data[23] = 0x00;
    data[24] = random()%240;
    data[25] = 0x00;
    data[26] = random()%240;

    // 分机3
    data[27] = 0x00;
    data[28] = random()%240;
    data[29] = 0x00;
    data[30] = random()%240;

    data[31] = 0x00;
    data[32] = random()%240;
    data[33] = 0x00;
    data[34] = random()%240;

    data[35] = 0x00;
    data[36] = random()%240;
    data[37] = 0x00;
    data[38] = random()%240;

    // 分机4
    data[39] = 0x00;
    data[40] = random()%240;
    data[41] = 0x00;
    data[42] = random()%240;

    data[43] = 0x00;
    data[44] = random()%240;
    data[45] = 0x00;
    data[46] = random()%240;

    data[47] = 0x00;
    data[48] = random()%240;
    data[49] = 0x00;
    data[50] = random()%240;

    // 分机5
    data[51] = 0x00;
    data[52] = random()%240;
    data[53] = 0x00;
    data[54] = random()%240;

    data[55] = 0x00;
    data[56] = random()%240;
    data[57] = 0x00;
    data[58] = random()%240;

    data[59] = 0x00;
    data[60] = random()%240;
    data[61] = 0x00;
    data[62] = random()%240;

    // CTL
    data[63] = 0x80 | (random()%64);
    data[64] = 0x80 | (random()%64);
    data[65] = 0x80 | (random()%64);
    data[66] = 0x80 | (random()%64);
    data[67] = 0x80 | (random()%64);

    data[68] = 0x00;
    for(int i = 0; i < len-1; i++)
    {
        data[68] += data[i];
    }

    memset(total_buffer, 0, sizeof(total_buffer));
    for(int i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
    }

    memcpy(total_buffer, data, len);
    memcpy(total_buffer+len, data, len);
    memcpy(total_buffer+len+len, data, len);
    send(sock, total_buffer, len-1, 0);
    sleep(1);
    send(sock, total_buffer+len-1, len-1, 0);
    sleep(3);
    send(sock, total_buffer+len+len-2, len+2, 0);
    return 0;
}


int send_socket_data3(int sock, unsigned char *data, int len)
{
    unsigned char total_buffer[1024];
    if(sock <= 0 || data == NULL || len !=  69)
    {
        printf("%s %s %d: param error\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    srandom(time(NULL));
    memset(data, 0, len);
    data[0] = 0xaa;
    data[1] = 0x02;
    data[2] = 69;

    // 分机1
    data[3] = 0x00;
    data[4] = random()%240;
    data[5] = 0x00;
    data[6] = random()%240;

    data[7] = 0x00;
    data[8] = random()%240;
    data[9] = 0x00;
    data[10] = random()%240;

    data[11] = 0x00;
    data[12] = random()%240;
    data[13] = 0x00;
    data[14] = random()%240;

    // 分机2
    data[15] = 0x00;
    data[16] = random()%240;
    data[17] = 0x00;
    data[18] = random()%240;

    data[19] = 0x00;
    data[20] = random()%240;
    data[21] = 0x00;
    data[22] = random()%240;

    data[23] = 0x00;
    data[24] = random()%240;
    data[25] = 0x00;
    data[26] = random()%240;

    // 分机3
    data[27] = 0x00;
    data[28] = random()%240;
    data[29] = 0x00;
    data[30] = random()%240;

    data[31] = 0x00;
    data[32] = random()%240;
    data[33] = 0x00;
    data[34] = random()%240;

    data[35] = 0x00;
    data[36] = random()%240;
    data[37] = 0x00;
    data[38] = random()%240;

    // 分机4
    data[39] = 0x00;
    data[40] = random()%240;
    data[41] = 0x00;
    data[42] = random()%240;

    data[43] = 0x00;
    data[44] = random()%240;
    data[45] = 0x00;
    data[46] = random()%240;

    data[47] = 0x00;
    data[48] = random()%240;
    data[49] = 0x00;
    data[50] = random()%240;

    // 分机5
    data[51] = 0x00;
    data[52] = random()%240;
    data[53] = 0x00;
    data[54] = random()%240;

    data[55] = 0x00;
    data[56] = random()%240;
    data[57] = 0x00;
    data[58] = random()%240;

    data[59] = 0x00;
    data[60] = random()%240;
    data[61] = 0x00;
    data[62] = random()%240;

    // CTL
    data[63] = 0x80 | (random()%64);
    data[64] = 0x80 | (random()%64);
    data[65] = 0x80 | (random()%64);
    data[66] = 0x80 | (random()%64);
    data[67] = 0x80 | (random()%64);

    data[68] = 0x00;
    for(int i = 0; i < len-1; i++)
    {
        data[68] += data[i];
    }

    memset(total_buffer, 0, sizeof(total_buffer));
    for(int i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
    }

    memcpy(total_buffer, data, len);
    memcpy(total_buffer+len, data, len);
    memcpy(total_buffer+len+len, data, len);
    memcpy(total_buffer+len+len+len, data, len);
    memcpy(total_buffer+len+len+len+len, data, len);
    send(sock, total_buffer, len*5, 0);
    return 0;
}
int send_socket_data4(int sock, unsigned char *data, int len)
{
    unsigned char total_buffer[1024];
    if(sock <= 0 || data == NULL || len !=  69)
    {
        printf("%s %s %d: param error\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    srandom(time(NULL));
    memset(data, 0, len);
    data[0] = 0xaa;
    data[1] = 0x02;
    data[2] = 69;

    // 分机1
    data[3] = 0x00;
    data[4] = random()%240;
    data[5] = 0x00;
    data[6] = random()%240;

    data[7] = 0x00;
    data[8] = random()%240;
    data[9] = 0x00;
    data[10] = random()%240;

    data[11] = 0x00;
    data[12] = random()%240;
    data[13] = 0x00;
    data[14] = random()%240;

    // 分机2
    data[15] = 0x00;
    data[16] = random()%240;
    data[17] = 0x00;
    data[18] = random()%240;

    data[19] = 0x00;
    data[20] = random()%240;
    data[21] = 0x00;
    data[22] = random()%240;

    data[23] = 0x00;
    data[24] = random()%240;
    data[25] = 0x00;
    data[26] = random()%240;

    // 分机3
    data[27] = 0x00;
    data[28] = random()%240;
    data[29] = 0x00;
    data[30] = random()%240;

    data[31] = 0x00;
    data[32] = random()%240;
    data[33] = 0x00;
    data[34] = random()%240;

    data[35] = 0x00;
    data[36] = random()%240;
    data[37] = 0x00;
    data[38] = random()%240;

    // 分机4
    data[39] = 0x00;
    data[40] = random()%240;
    data[41] = 0x00;
    data[42] = random()%240;

    data[43] = 0x00;
    data[44] = random()%240;
    data[45] = 0x00;
    data[46] = random()%240;

    data[47] = 0x00;
    data[48] = random()%240;
    data[49] = 0x00;
    data[50] = random()%240;

    // 分机5
    data[51] = 0x00;
    data[52] = random()%240;
    data[53] = 0x00;
    data[54] = random()%240;

    data[55] = 0x00;
    data[56] = random()%240;
    data[57] = 0x00;
    data[58] = random()%240;

    data[59] = 0x00;
    data[60] = random()%240;
    data[61] = 0x00;
    data[62] = random()%240;

    // CTL
    data[63] = 0x80 | (random()%64);
    data[64] = 0x80 | (random()%64);
    data[65] = 0x80 | (random()%64);
    data[66] = 0x80 | (random()%64);
    data[67] = 0x80 | (random()%64);

    data[68] = 0x00;
    // for(int i = 0; i < len-1; i++)
    // {
    //     data[68] += data[i];
    // }

    memset(total_buffer, 0, sizeof(total_buffer));
    for(int i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
    }

    memcpy(total_buffer, data, len);
    send(sock, total_buffer, len, 0);
    return 0;
}
int main() 
{
    int cmd = 0;
    int sock = 0;
    int count = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client!";
    char buffer[BUFFER_SIZE] = {0};
    unsigned char send_buff[69];

    // 1. 创建 socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. 将 IP 地址从字符串转换为二进制格式
    if (inet_pton(AF_INET, "47.109.24.25", &serv_addr.sin_addr) <= 0) {
//    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    // 3. 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_id;

    pthread_create(&thread_id, NULL, handle_connection, &sock); // 创建线程处理客户端的连接

    // 4. 发送消息
    // send(sock, message, strlen(message), 0);
    // printf("Message sent to server.\n");

    while(1)
    {
        printf("Enter command: \n");
        printf("    0-exit.\n");
        printf("    1-Send single frame data.\n");
        printf("    2-Send 3 identical frames of data with frame intervals of 1S and 3S.\n");
        printf("    3-Send 3 frames of the same data in 3 batches, but each time not a complete frame is sent.\n");
        printf("    4-Send a frame of uncalculated checksum data.\n");
        scanf("%d", &cmd);
        if(cmd == 1)
        {
            printf("send data to server:\n");
            send_socket_data(sock, send_buff, 69);
            continue;
        }
        else if(cmd == 2)
        {
            send_socket_data2(sock, send_buff, 69);
        }
        else if(cmd == 3)
        {
            send_socket_data3(sock, send_buff, 69);
        }
        else if(cmd == 4)
        {
            send_socket_data4(sock, send_buff, 69);
        }
        else if(cmd == 0)
        {
            break;
        }
    }
    // 5. 接收响应
    read(sock, buffer, BUFFER_SIZE);
    printf("Server response: %s\n", buffer);

    // 6. 关闭连接
    close(sock);

    return 0;
}
