/*************************************************************************
  > File Name:    block.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月04日 星期三 20时12分42秒
 ************************************************************************/

#ifndef _BLOCK_H
#define _BLOCK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#define WIDTH_BOUNDARY 60 // 边界宽度/块
#define LENTH_BOUNDARY 15 // 边界长度/块
#define NUM_FOOD 4    //食物类别个数
#define NUM_OBSTACLE 2    //障碍物类别个数

#define MAX_SNAKE WIDTH_BOUNDARY*LENTH_BOUNDARY  //定义蛇的最大长度

// 定义图标显示
// 蛇头0，对应上0，下1，左2，右3
extern const char snake_head_icon[4][9];
// 蛇中1，对应上0，下1，左2，右3
extern const char snake_middle_icon[8][9];
// 蛇尾2，对应上0，下1，左2，右3
extern const char snake_tail_icon[4][9];
// 食物，不定义方向
extern const char food_icon[NUM_FOOD][9];
// 障碍物，不定义方向
extern const char obstacle_icon[NUM_OBSTACLE][9];
// 边界，对应上下0，左右1
extern const char boundary_icon[2][9];
#define BLOCK_SIZE 3  //定义每个块的大小

enum BLOCK_TYPES {empty,snake,food,obstacle,boundary}; //定义块类型
 /* 拐角：用组合( prevDir , nextDir ) 判断四种转弯 */
        /* 这里把 dir 当成“拐角编号”留给渲染层 */
        /*             up->right  5
           5 6 7 8 →   right->down 6
                       down->left  7
                       left->up    8        */
enum DIRECTIONS {none,up,down,left,right,corner0,corner1,corner2,corner3}; //定义方向和拐角

//定义块结构体
typedef struct
{
  enum BLOCK_TYPES type;
  int type_index;
  enum DIRECTIONS dir;
  int color;
  int top_left_x;//此处是窗口坐标位置
  int top_left_y;
}Block;

/*
* 传入的x，y为起始位置坐标（左上角）
* container为二维数组
* 初始化container里面的block，并且给其设置边界
* 返回int，1为成功，0为失败
*/
extern int initContainer(Block container[][WIDTH_BOUNDARY],int len,int wid,int x,int y);

/*
* 传入的x，y为起始位置坐标（左上角）
* container为二维数组
* 初始化container里面的block，全部设置为空块
* 返回int，1为成功，0为失败
*/
extern int initEmptyContainer(Block (*container)[WIDTH_BOUNDARY],int len,int wid,int x,int y);

//内部初始化block函数
static int initBlock(Block* p,int x,int y);

/*
* 根据Container内容渲染窗口
* oldContainer用来判断数据是否一致
* 返回渲染结果
*/
extern int renderContainer(WINDOW *win,Block (*newContainer)[WIDTH_BOUNDARY],Block (*oldContainer)[WIDTH_BOUNDARY],int len,int wid);

// 块渲染函数
int renderBlock(WINDOW *win,Block *bl);


/*
* 根据Container内容渲染窗口
* 不会判断数据是否一致，进行全部渲染
* 返回渲染结果
*/
extern int renderAllContainer(WINDOW *win,Block (*container)[WIDTH_BOUNDARY],int len,int wid);


#endif //_BLOCK_H
