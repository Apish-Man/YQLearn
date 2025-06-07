/*************************************************************************
  > File Name:    obstacle.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月05日 星期四 16时31分57秒
 ************************************************************************/

#include "obstacle.h"

/*
 * 初始化障碍物
 * @param container 网格的当前状态
 * @param int wid  边界宽度
 * @param int len  边界长度
 * @param int num  障碍物个数个数
 * @return NODE*,返回初始化障碍物，return NULL表示失败
 */
NODE *init_obstacle(Block (*newcontainer)[WIDTH_BOUNDARY], Block (*oldcontainer)[WIDTH_BOUNDARY], int len, int wid, int num)
{
  // 创建第一个节点
  NODE *obstacle = createObstacleNode(newcontainer, len, wid);
  if (!obstacle)
    return NULL;
  // 更新obstacle
  obstacle->next=obstacle;
  obstacle->prev=obstacle;
  // 更新对应container
  int tmpx = obstacle->data.i, tmpy = obstacle->data.j;
  oldcontainer[tmpx][tmpy] = newcontainer[tmpx][tmpy];
  newcontainer[tmpx][tmpy].type = 3; // obstacle
  // 随机选取一种障碍物
  newcontainer[tmpx][tmpy].type_index = rand() % NUM_OBSTACLE;
  // dir是方向，障碍物不设置方向，设置为0
  newcontainer[tmpx][tmpy].dir = 0;
  for (int i = 1; i < num; i++)
  {
    NODE *p = createObstacleNode(newcontainer, len, wid);
    if (!p)
      return NULL;
    dclist_tail_add(&obstacle, p);
    // 更新对应container
    int tmpx = p->data.i, tmpy = p->data.j;
    oldcontainer[tmpx][tmpy] = newcontainer[tmpx][tmpy];
    newcontainer[tmpx][tmpy].type = 3; // obstacle
    // 随机选取一种障碍物
    newcontainer[tmpx][tmpy].type_index = rand() % NUM_OBSTACLE;
    // dir是方向，障碍物不设置方向，设置为0
    newcontainer[tmpx][tmpy].dir = 0;
  }
  return obstacle;
}

// 创建节点
NODE *createObstacleNode(Block (*newcontainer)[WIDTH_BOUNDARY], int len, int wid)
{
  // 创建头结点
  NODE *obstacle = (NODE *)malloc(sizeof(NODE));
  if (!obstacle)
    return NULL;
  // 头节点，x,y赋值
  int tmp_x, tmp_y;
  // 找到合适位置---此时不考虑碰撞，只考虑生成多个无关数据
  do
  {
    // x是横坐标，范围为1-（WIDTH_BOUNDARY-2）1-48
    tmp_x = rand() % (wid - 2) + 1;
    // y是横坐标，范围为1-LENTH_BOUNDARY-2）1-48
    tmp_y = rand() % (len - 2) + 1;
  } while (newcontainer[tmp_y][tmp_x].type != 0);
  obstacle->data.i = tmp_y;
  obstacle->data.j = tmp_x;
  obstacle->next = NULL;
  obstacle->prev = NULL;
  return obstacle;
}
