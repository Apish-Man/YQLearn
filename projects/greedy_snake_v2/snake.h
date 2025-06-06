/*************************************************************************
  > File Name:    snake.h
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月03日 星期二 20时21分52秒
 ************************************************************************/

#ifndef _SNAKE_H
#define _SNAKE_H

#include "block.h"
#include <time.h>
#include "dclist.h"

// 使用双向循环链表
// //定义蛇节点结构体：
// typedef struct SnakeNode{
//     int x;  //在container中的第一个下标
//     int y;  //在container中的第二个下标
//     struct SnakeNode *next,*prev;//下一个蛇节点,上一个蛇节点
// }SnakeNode,*Snake;

/*
 * 初始化蛇函数，创建一个蛇头一个蛇尾一个蛇中间，同时会随机一个位置
 * @param container 网格的当前状态
 * @param int wid  边界宽度
 * @param int len  边界长度
 * @return NODE*,返回初始化蛇，return NULL表示失败
 */
extern NODE *init_snake(Block (*newcontainer)[WIDTH_BOUNDARY], Block (*oldcontainer)[WIDTH_BOUNDARY], int len, int wid);

/*
 * 创建节点
 */
extern NODE *createSnakeNode(Block (*newcontainer)[WIDTH_BOUNDARY], int len, int wid);

// 根据链表更新container
// newNode  oldHead  node node ...  tail  NULL
// p        q                       p     q
// 先保存p的block状态，再每次把q的block状态复制到p,直到q为NULL，然后把q的block设置为空
// 不需要修改Snake，故使用NODE*
// 成功返回0，失败返回-1
extern int updateContainerOnSnake(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE *Snake);

// 根据链表和输入方向更新container
// newNode  oldHead  node node ...  tail  NULL
// p        q                       p     q
// 先保存p的block状态，再每次把q的block状态复制到p,直到q为NULL，然后把q的block设置为空
// 头节点对应的container方向改变
// 不需要修改Snake，故使用NODE*
// 成功返回0，失败返回-1
extern int updateContainerOnSnakeDir(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake,int dir,int next_i,int next_j);

/* 辅助：根据整条蛇重新刷 container */
static void paint_snake(Block (*newC)[WIDTH_BOUNDARY],Block (*oldC)[WIDTH_BOUNDARY],NODE  *Snake, int len, int wid);
#endif //_SNAKE_H
