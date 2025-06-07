/*************************************************************************
  > File Name:    game.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月05日 星期四 09时22分25秒
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H

#include "block.h"
#include "dclist.h"
#include "snake.h"
#include "food.h"
#include "obstacle.h"
#include <unistd.h>

//定义初始化时食物个数
#define NUM_INIT_FOOD 10

//定义初始化时障碍物个数
#define NUM_INIT_ONSTACLE 10

// 定义win初始坐标
#define INIT_X 0
#define INIT_Y 0

// 定义游戏状态
enum GAME_STATE { READY, RUNNING, PAUSED, GAME_OVER };

/*
 * 结束游戏
 * 
 */
extern int end_game();

/*
 * 重新开始游戏
 * 重置new,old container，重置Snake、food
 * 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
 */
extern int restart_game();

/*
* 初始化函数,传入win，oldcontainer,newcontainer,len,wid
* 边界在最开始已经初始化，不再在此处初始化
* 初始化小蛇，食物，障碍物，分别传入三个双向循环链表
*/
extern int init_game(WINDOW *win,Block (*newContainer)[WIDTH_BOUNDARY],Block (*oldContainer)[WIDTH_BOUNDARY],int len,int wid,NODE** Snake,NODE** Food,NODE** Obstacle);

/*
 * 处理按键输入,根据传入字符处理游戏状态
 * q,退出，s，开始，p，暂停，r，重新开始
 * 若目前状态是RUNNING，处理游戏状态更新
 */
extern void handle_input(int ch, enum GAME_STATE *s,int *press_dir);

/*
 * 显示游戏信息
 * s，游戏状态，status，玩家状态，score，得分
 */
extern void show_ui(WINDOW *w, enum GAME_STATE s, int status, long score);

/*
* 更新下一步游戏状态，同步更新容器内容,传入oldcontainer,newcontainer,len,wid,Snake,Food,Obstacle,press_dir
* 根据传入的press_dir更新container和三个链表的状态，press_dir，0-4：enum DIRECTIONS {none,up,down,left,right}; //定义方向
* 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
*/
extern int updateGame(Block (*newContainer)[WIDTH_BOUNDARY],Block (*oldContainer)[WIDTH_BOUNDARY], int len,int wid,NODE** Snake,NODE** Food,NODE** Obstacle,int press_dir);

/*
 * 沿给定方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身，如果未结束，返回1，如果运行出错，返回0
 */
extern int go_next_one(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle,int press_dir);

/*
* 销毁游戏,free堆空间，保存游戏状态，比如最高分数，排名等
* 成功返回1，失败返回0
*/
extern int destory_game(NODE** Snake,NODE** Food,NODE** Obstacle);
#endif //_GAME_H
