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
extern NODE* init_snake(Block (*newcontainer)[WIDTH_BOUNDARY],Block (*oldcontainer)[WIDTH_BOUNDARY],int len,int wid);

/*
* 创建节点
*/
extern NODE* createSnakeNode(Block (*newcontainer)[WIDTH_BOUNDARY],int len,int wid);


#endif //_SNAKE_H
