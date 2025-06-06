/*************************************************************************
  > File Name:    food.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月05日 星期四 09时19分55秒
 ************************************************************************/

#include "food.h"

/*
 * 初始化食物
 * @param container 网格的当前状态
 * @param int wid  边界宽度
 * @param int len  边界长度
 * @param int num  食物个数
 * @return NODE*,返回初始化食物，return NULL表示失败
 */
NODE *init_food(Block (*newcontainer)[WIDTH_BOUNDARY], Block (*oldcontainer)[WIDTH_BOUNDARY], int len, int wid, int num)
{
  srand(time(NULL));
  // 创建第一个节点
  NODE *food = createFoodNode(newcontainer, len, wid);
  if (!food)
    return NULL;
  // 更新food
  food->next=food;
  food->prev=food;
  // 更新对应container
  int tmpx = food->data.i, tmpy = food->data.j;
  oldcontainer[tmpx][tmpy] = newcontainer[tmpx][tmpy];
  newcontainer[tmpx][tmpy].type = 2; // food
  // 随机选取一种食物
  newcontainer[tmpx][tmpy].type_index = rand() % NUM_FOOD;
  // dir是方向，食物不设置方向，设置为0
  newcontainer[tmpx][tmpy].dir = 0;
  for (int i = 1; i < num; i++)
  {
    NODE *p = createFoodNode(newcontainer, len, wid);
    if (!p)
      return NULL;
    dclist_tail_add(&food, p);
    // 更新对应container
    int tmpx = p->data.i, tmpy = p->data.j;
    oldcontainer[tmpx][tmpy] = newcontainer[tmpx][tmpy];
    newcontainer[tmpx][tmpy].type = 2; // food
    // 随机选取一种食物
    newcontainer[tmpx][tmpy].type_index = rand() % NUM_FOOD;
    // dir是方向，食物不设置方向，设置为0
    newcontainer[tmpx][tmpy].dir = 0;
  }
  return food;
}

// 创建节点
NODE *createFoodNode(Block (*newcontainer)[WIDTH_BOUNDARY], int len, int wid)
{
  srand(time(NULL));
  // 创建头结点
  NODE *food = (NODE *)malloc(sizeof(NODE));
  if (!food)
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
  food->data.i = tmp_y;
  food->data.j = tmp_x;
  food->next = NULL;
  food->prev = NULL;
  return food;
}