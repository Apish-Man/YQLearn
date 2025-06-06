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
#define NUM_INIT_FOOD 4

//定义初始化时障碍物个数
#define NUM_INIT_ONSTACLE 4

/*
* 初始化函数,传入win，oldcontainer,newcontainer,len,wid
* 边界在最开始已经初始化，不再在此处初始化
* 初始化小蛇，食物，障碍物，分别传入三个双向循环链表
*/
extern int init_game(WINDOW *win,Block (*newContainer)[WIDTH_BOUNDARY],Block (*oldContainer)[WIDTH_BOUNDARY],int len,int wid,NODE** Snake,NODE** Food,NODE** Obstacle);

/*
* 更新container状态函数,传入oldcontainer,newcontainer,len,wid,Snake,Food,Obstacle,press_dir
* 根据传入的press_dir更新container和三个链表的状态，press_dir，0-4：enum DIRECTIONS {none,up,down,left,right}; //定义方向
* 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
*/
extern int updateContainer(Block (*newContainer)[WIDTH_BOUNDARY],Block (*oldContainer)[WIDTH_BOUNDARY], int len,int wid,NODE** Snake,NODE** Food,NODE** Obstacle,int press_dir);

/*
* 沿当前方向向前走，走的步数为1步
* 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
* 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
*/
extern int go_straight_none(Block (*newContainer)[WIDTH_BOUNDARY],Block (*oldContainer)[WIDTH_BOUNDARY], int len,int wid,NODE** Snake,NODE** Food,NODE** Obstacle);

/*
* 沿输入方向向前走，走的步数为1步
* 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
* 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
*/
extern int go_base_press(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle,int press_dir);



#endif //_GAME_H
