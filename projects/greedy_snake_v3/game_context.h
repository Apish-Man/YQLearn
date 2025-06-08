/*************************************************************************
  > File Name:    game_context.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月07日 星期六 23时14分34秒
 ************************************************************************/

#ifndef _GAME_CONTEXT_H
#define _GAME_CONTEXT_H


#include "block.h"
#include "dclist.h"
#include <ncurses.h>
#include "snake.h"
#include "food.h"
#include "obstacle.h"
#include "score.h"

// 定义游戏状态
enum GAME_STATE { READY, RUNNING, PAUSED, GAME_OVER,INIT };

//定义初始化时食物个数
#define NUM_INIT_FOOD 10

//定义初始化时障碍物个数
#define NUM_INIT_ONSTACLE 10

// 定义玩家昵称最大长度
#define NAME_MAXLEN 16

// 定义win初始坐标
#define INIT_X 0
#define INIT_Y 0

typedef struct {
    /* --- 核心可变数据 --- */
    Block   newC[LENTH_BOUNDARY][WIDTH_BOUNDARY];
    Block   oldC[LENTH_BOUNDARY][WIDTH_BOUNDARY];

    NODE   *Snake;
    NODE   *Food;
    NODE   *Obstacle;

    enum GAME_STATE gstate;  // 当前游戏状态
    int     status;          // updateGame 返回值
    int     press_dir;       // 最近一次方向键
    long    score;           // 当前分数
    long last_saved_score;   //记录最新一次存储的分数，维护动态刷新表单，初始化-1
    char  player_name[NAME_MAXLEN];   //玩家昵称


    /* --- 资源 / 只读配置 --- */
    int     len, wid;        // 网格长宽（行列）
    int     origin_x, origin_y; // 左上角在屏幕中的坐标
    WINDOW *win_game;
    WINDOW *win_status;
    WINDOW *win_debug;
    WINDOW *win_board;//排行榜窗口
} GameContext;

/*
 * 初始化游戏上下文
 * 初始化new old container,Snake Food Obstacle链表
 * 初始化游戏状态,当前玩家状态，最后一次方向键，当前分数
 * 若目前状态是RUNNING，处理游戏状态更新
 */
extern int  gc_init  (GameContext *ctx, int origin_x, int origin_y);

/*
 * 重置游戏状态
 * 重置new old container,Snake Food Obstacle链表
 * 初始化游戏状态,当前玩家状态，最后一次方向键，当前分数
 */
extern int gc_reset(GameContext *ctx);

/*
 * 销毁上下文
 * 释放链表
 * 释放窗口
 * 初始化失败返回0，成功返回1
 */
extern int gc_destroy(GameContext *ctx);

/*
 * 输入处理
 * 根据输入字符，更新游戏状态，此处主要是更新游戏对应的结构体
 * 初始化失败返回0，成功返回1
 */
extern int gc_handle_input(GameContext *ctx, int ch);

/*
 * 游戏逻辑推进
 * 根据输入字符，更新游戏状态，此处主要是更新游戏对应的结构体
 * 初始化失败返回0，成功返回1
 */
extern int gc_tick_logic(GameContext *ctx);

/*
 * 沿给定方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身，如果未结束，返回1，如果运行出错，返回0
 */
extern int go_next_one(GameContext *ctx);

/*
 * 渲染逻辑
 * 根据更新后的ctx，渲染数据
 * 渲染失败返回0，成功返回1
 */
extern int gc_tick_render(GameContext *ctx);

/*
 * 绘制排行榜
 * 绘制失败返回0，成功返回1
 */
extern int draw_board(GameContext *ctx);

/* 
* 居中打印函数
* win为窗口
* y为在第几行
* text为要打印的文本内容
*/
extern void center_print(WINDOW *win, int y, const char *text);

#endif //_GAME_CONTEXT_H
