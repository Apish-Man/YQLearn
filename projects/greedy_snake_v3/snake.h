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


/*
* 统计Snake链表的下标，并且存储到snapshotPos，统计其个数，记入snapshotCnt
* @param Snake,蛇链表
* @param snapshotPos,暂存位置数组
* @param snapshotCnt,记录旧链表长度
* @return int 1为成功，0为失败
*/
extern int snapshot_old_snake(NODE *Snake,Coordinate *snapshotPos,int* snapshotCnt);

/* 根据坐标差计算方向 */
// enum DIRECTIONS {none,up,down,left,right};
static enum DIRECTIONS delta2dir(int di, int dj);

/*
* 根据snapshotPos和snapshotCnt清空旧链表（将其对应block设置为空），先保存旧状态，再把新状态设置为空
* @param snapshotPos,旧蛇下标信息
* @param snapshotCnt,旧蛇数量
* @param newContainer,新状态
* @param oldContainer,旧状态
* @return int 1为成功，0为失败
*/
extern int clear_old_Snake(Coordinate *snapshotPos,int snapshotCnt,Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY]);

/*
* 根据Snake来重新绘制蛇，包括蛇头，蛇尾，蛇方向,蛇的方向，这几个方向是使用下标计算出来的
* @param Snake,新蛇链表
* @param newContainer,新状态
* @param oldContainer,旧状态
* @return int 1为成功，0为失败
*/
extern int paint_new_Snake(NODE *Snake,Block (*newContainer)[WIDTH_BOUNDARY],int dir);
#endif //_SNAKE_H
