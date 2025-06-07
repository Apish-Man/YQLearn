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

/*
* 根据container的状态，生成一个随机且合适的位置放置food,并且更新container
* @param Food,新的食物链表
* @param newContainer,当前状态
* @param oldContainer,旧状态
* @return int 1为成功，0为失败
*/
int add_one_food_and_update_container(NODE** Food,Block (*newcontainer)[WIDTH_BOUNDARY],Block (*oldcontainer)[WIDTH_BOUNDARY],int len,int wid)
{
  // 已经删除旧节点，现在只需添加新节点，并且更新对应container
  NODE* newNode=createFoodNode(newcontainer,len,wid);
  if(newNode==NULL) return 0;
  int new_i=newNode->data.i,new_j=newNode->data.j;
  // 新节点插入
  dclist_head_add(Food,newNode);
  // 更新container
  oldcontainer[new_i][new_j]=newcontainer[new_i][new_j];
  newcontainer[new_i][new_j]=(Block){
        .type = food,
        .type_index = rand() % NUM_FOOD,
        .dir = none,
        .color = 0,
        .top_left_x = newcontainer[new_i][new_j].top_left_x,
        .top_left_y = newcontainer[new_i][new_j].top_left_y};
  
  return 1;
}
