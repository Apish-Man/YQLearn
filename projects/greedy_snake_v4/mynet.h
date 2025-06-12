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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//定义初始化时食物个数
#define NUM_INIT_FOOD 10

//定义初始化时障碍物个数
#define NUM_INIT_ONSTACLE 3

// 定义玩家昵称最大长度
#define NAME_MAXLEN 16

#include "block.h"

/* ------------------ 可调宏 ------------------ */
#define MP_PORT        12345          /* 默认端口 */
#define MP_MAX_BODY    120            /* 蛇最大长度（定死便于序列化） */
#define MP_TICK_MS     20             /* 网络帧间隔 = 渲染帧间隔 */

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
    MPPos   obstacle[NUM_INIT_ONSTACLE];               /* 障碍物数组   */
    uint8_t  wid,len;                        /* 地图长宽 */
    uint8_t dir,dir2;   /* 初始方向（客户端）   */
    uint16_t score, score2;    /* 双方得分   */
    uint8_t  gstate,gstate2;            /* READY/RUNNING/… */
    uint8_t status;              /*游戏状态，status为1表示未结束，小于0表示结束，-1是玩家1赢，-2玩家2赢,0表示异常结束*/
    uint16_t last_step_us;    /* 存放时间帧，用于刷新 */
    char player_name[NAME_MAXLEN];   /* 玩家1昵称 */
    char player_name2[NAME_MAXLEN];  /* 玩家2昵称 */
} MPStatePkt;
#pragma pack(pop)

/* 
* 本地按键包：一个字节即可 
* 单个按键的网络传输包
*/
typedef struct {
    uint8_t  key;                /* 按键 */
} MPKeyPkt;

/* 
* 初始化昵称：16个字节即可 
* 单个按键的网络传输包
*/
typedef struct {
    char player_name[NAME_MAXLEN]; 
} MPNamePkt;

/* ------------------ API ------------------ */
// 服务器端初始化
extern int  net_init_server(int *listen_fd, int *client_fd);
// 客户端初始化
extern int  net_init_client(int *sock_fd, const char *ip);
// 设置非阻塞
extern int  net_set_nonblock(int fd);
// 发送数据
extern int  net_send(int fd, const void *buf, size_t len);
// 接收数据
extern int  net_recv(int fd, void *buf, size_t len);

/* 可选：帧同步小工具，让主循环稳定在 MP_TICK_MS */
extern void net_sleep_frame(void);


#endif //_NET_H
