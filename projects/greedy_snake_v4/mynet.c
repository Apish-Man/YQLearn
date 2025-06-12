/*************************************************************************
  > File Name:    net.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月10日 星期二 10时59分47秒
 ************************************************************************/

#include <sys/types.h>
// #include <winsock2.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "mynet.h"

static int _set_reuseaddr(int fd)
{
    int opt = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

/* 
* 服务端初始化 
* 创建服务端监听套接字，绑定端口，等待客户端连接。
*/
int net_init_server(int *listen_fd, int *client_fd)
{
    if (!listen_fd || !client_fd)
    {
        errno = EINVAL;
        return -1;
    }

    /* 1. 创建套接字 */
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0)
        return -1;

    /* 2. 端口复用，避免 TIME‑WAIT 占坑 */
    _set_reuseaddr(lfd);

    /* 3. 绑定本地地址 */
    // 创建socket
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* 监听全部网卡 */
    serv_addr.sin_port = htons(MP_PORT);

    // 绑定端口
    if (bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        close(lfd);
        return -1;
    }

    /* 4. 监听（backlog=1 已足够）*/
    if (listen(lfd, 1) < 0)
    {
        close(lfd);
        return -1;
    }

    /* 5. 阻塞等待首位客户端进入房间 */
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);
    int cfd = accept(lfd, (struct sockaddr *)&cli_addr, &len);
    if (cfd < 0)
    {
        close(lfd);
        return -1;
    }
    // 连接成功以后立刻设为非阻塞
    net_set_nonblock(cfd);  

    /* 6. 输出结果 */
    *listen_fd = lfd;
    *client_fd = cfd;
    return 0; /* OK */
}
/* 
* 客户端初始化
* 创建客户端套接字并连接到服务器。
*/
int net_init_client(int *sock_fd, const char *ip)
{
    if (!sock_fd || !ip)
    {
        errno = EINVAL;
        return -1;
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MP_PORT);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        close(fd);
        errno = EINVAL;
        return -1;
    }

    /* 阻塞 connect（Demo 用，简单可靠）*/
    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        close(fd);
        return -1;
    }

    // 连接成功以后立刻设为非阻塞
    net_set_nonblock(fd);  

    *sock_fd = fd;
    return 0;
}
/* 
* 非阻塞模式设置 
* 将套接字设置为非阻塞模式。
*/
int net_set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return -1;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        return -1;
    return 0;
}
/* 
* 在循环中处理数据的 完整发送或接收，应对信号中断和部分读写。
*/
static int _loop_io(int fd, const void *buf, size_t len, int sending)
{
    const char *p = buf;
    size_t left = len;

    while (left > 0) {
        ssize_t n = sending ? send(fd, p, left, MSG_NOSIGNAL)
                            : recv(fd, (void*)p, left, 0);

        if (n > 0) {               /* 已经收/发了一部分 */
            left -= n;  p += n;
            continue;
        }
        if (n == 0) return -1;     /* 对端关闭 */

        if (errno == EINTR) continue;

        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            /* 等待就绪 */
            struct timeval tv = {0, MP_TICK_MS * 1000};
            fd_set set; 
            FD_ZERO(&set); 
            FD_SET(fd, &set);
            
            int r = sending ?
                    select(fd+1, NULL, &set, NULL, &tv) :  /* 等待写就绪 */
                    select(fd+1, &set, NULL, NULL, &tv);   /* 等待读就绪 */
                    
            if (r <= 0) continue;   /* 超时/被信号打断，重进循环 */
            
            /* 就绪后立即重试发送/接收 */
            n = sending ? send(fd, p, left, MSG_NOSIGNAL)
                       : recv(fd, (void*)p, left, 0);
                       
            if (n > 0) {           /* 发送/接收成功 */
                left -= n;  p += n;
                continue;
            }
            if (n == 0) return -1; /* 对端关闭 */
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            return -1;             /* 其他错误 */
        }
        return -1;                 /* 其他错误 */
    }
    return (int)len;               /* 成功 */
}

int net_send(int fd, const void *buf, size_t len)
{
    return _loop_io(fd, buf, len, 1);
}

int net_recv(int fd, void *buf, size_t len)
{
    return _loop_io(fd, buf, len, 0);
}

/* 可选：帧同步小工具，让主循环稳定在 MP_TICK_MS */
void net_sleep_frame(void)
{
    struct timespec ts = {0};
    ts.tv_nsec = MP_TICK_MS * 1000000L;
    nanosleep(&ts, NULL);
}
