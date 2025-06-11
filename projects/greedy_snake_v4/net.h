/*************************************************************************
  > File Name:    net.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月10日 星期二 10时59分41秒
 ************************************************************************/

#ifndef _NET_H
#define _NET_H
#include <stdint.h>
#include <stdbool.h>
#include "game_context.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ------------------ 可调宏 ------------------ */
#define MP_PORT        12345          /* 默认端口 */
#define MP_MAX_BODY    120            /* 蛇最大长度（定死便于序列化） */
#define MP_TICK_MS     50             /* 网络帧间隔 = 渲染帧间隔 */

/* 便于网络传输的轻量包 —— 结构严格对齐  */
#pragma pack(push,1)
typedef struct { int8_t x,y; } MPPos;
typedef struct {
    uint8_t len;               /* 身体实际长度              */
    MPPos   body[MP_MAX_BODY]; /* (x,y) 数组，最多占用 240B  */
} MPSnake;

/*
* 整个游戏状态的传输包，包含两条蛇、食物、障碍物等位置和得分
*/
typedef struct {                /* 每帧广播包 */
    MPSnake s1,s2;              /* 两条蛇     */
    MPPos   food[NUM_INIT_FOOD];               /* 食物数组   */
    MPPos   obstacle[NUM_OBSTACLE];               /* 障碍物数组   */
    uint16_t score1, score2;    /* 双方得分   */
    uint8_t  gstate;            /* READY/RUNNING/… */
} MPStatePkt;
#pragma pack(pop)

/* 
* 本地按键包：一个字节即可 
* 单个按键的网络传输包
*/
typedef struct { char key; } MPKeyPkt;

/* ------------------ API ------------------ */
// 服务器端初始化
int  net_init_server(int *listen_fd, int *client_fd);
// 客户端初始化
int  net_init_client(int *sock_fd, const char *ip);
// 设置非阻塞
int  net_set_nonblock(int fd);
// 发送数据
int  net_send(int fd, const void *buf, size_t len);
// 接收数据
int  net_recv(int fd, void *buf, size_t len);

#endif //_NET_H
