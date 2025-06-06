/*************************************************************************
  > File Name:    game.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月05日 星期四 09时22分30秒
 ************************************************************************/

#include "game.h"

/*
 * 初始化函数,传入win，oldcontainer,newcontainer,len,wid
 * 边界在最开始已经初始化，不再在此处初始化
 * 初始化小蛇，食物，障碍物，分别传入三个双向循环链表
 */
int init_game(WINDOW *win, Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle)
{
  // 初始化小蛇
  NODE *p = init_snake(newContainer, oldContainer, len, wid);
  if (!p)
    return -1;
  *Snake = p;
  // 初始化食物
  p = init_food(newContainer, oldContainer, len, wid, NUM_INIT_FOOD);
  if (!p)
    return -1;
  *Food = p;
  // 初始化障碍物
  p = init_obstacle(newContainer, oldContainer, len, wid, NUM_INIT_ONSTACLE);
  if (!p)
    return -1;
  *Obstacle = p;
  return 1;
}

/*
 * 更新container状态函数,传入oldcontainer,newcontainer,len,wid,Snake,Food,Obstacle,press_dir
 * 根据传入的press_dir更新container和三个链表的状态，press_dir，0-4：enum DIRECTIONS {none,up,down,left,right}; //定义方向
 * 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
 */
int updateContainer(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle, int press_dir)
{
  int status = 1; // 保存玩家状态，也存储运行信息
  // 1.press_dir=0，按当前状态向前一格
  if (press_dir == 0)
  {
    status = go_straight_none(newContainer, oldContainer, len, wid, Snake, Food, Obstacle);
  }
  else
  {
    // 根据press_dir的方向移动
    status = go_base_press(newContainer, oldContainer, len, wid, Snake, Food, Obstacle, press_dir);
  }
  return status;
}

/*
 * 沿当前方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身，如果未结束，返回1，如果运行出错，返回0
 */
int go_straight_none(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle)
{
  if (*Snake == NULL)
  {
    return 0;
  }
  // 0.获取头元素坐标
  int head_i = (*Snake)->data.i, head_j = (*Snake)->data.j;
  // 1.获取下一位置坐标
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3,dir对应1-4，对应于此的0-3
  int dir = newContainer[head_i][head_j].dir;
  int next_i = head_i + directions[dir - 1][0];
  int next_j = head_j + directions[dir - 1][1];
  if (next_i > LENTH_BOUNDARY || next_j > WIDTH_BOUNDARY || next_i < 0 || next_j < 0)
    return 0;
  // 2.判断下一位置状态
  if (newContainer[next_i][next_j].type == obstacle)
  {
    // 2.1障碍物，结束
    return -1;
  }
  else if (newContainer[next_i][next_j].type == boundary)
  {
    // 2.2边界，结束
    return -2;
  }
  else if (newContainer[next_i][next_j].type == snake)
  {
    // 2.3咬到自身，结束
    return -3;
  }
  else if (newContainer[next_i][next_j].type == food)
  {
    // 2.4食物，向前走，并且添加新节点
    // 食物位置作为新的头节点，使用头插法插入
  }
  else
  {
    // 2.5 遇到空格，安全，向前走
    // 遍历Snake链表，更新其位置，并且把最后的位置设置为空
    NODE *tail = (*Snake)->prev, *head = *Snake; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // 1.创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(Snake, newNode);
    // 4.更新容器状态
    int flag = updateContainerOnSnake(newContainer, oldContainer, len, wid, *Snake);
    if (flag == -1)
      return 0;
    // 5.清除旧蛇尾位置
    int tail_i = tail->data.i, tail_j = tail->data.j;
    oldContainer[tail_i][tail_j] = newContainer[tail_i][tail_j];
    newContainer[tail_i][tail_j] = (Block){
        .type = empty,
        .type_index = -1,
        .dir = none,
        .color = 0,
        .top_left_x = newContainer[tail_i][tail_j].top_left_x,
        .top_left_y = newContainer[tail_i][tail_j].top_left_y};
    // 删除这个元素
    dclist_tail_delete(Snake);
    // 6.释放尾节点,delete函数会释放
    // free(tail);
    return 1;
  }
}

/*
 * 沿输入方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
 */
int go_base_press(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle, int press_dir)
{

  if (*Snake == NULL)
  {
    return 0;
  }
  // 0.获取头元素坐标
  int head_i = (*Snake)->data.i, head_j = (*Snake)->data.j;
  // 1.获取下一位置坐标
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3,dir对应1-4，对应于此的0-3
  int dir = press_dir;
  int next_i = head_i + directions[dir - 1][0];
  int next_j = head_j + directions[dir - 1][1];
  if (next_i > LENTH_BOUNDARY || next_j > WIDTH_BOUNDARY || next_i < 0 || next_j < 0)
    return 0;
  // 2.判断下一位置状态
  if (newContainer[next_i][next_j].type == obstacle)
  {
    // 2.1障碍物，结束
    return -1;
  }
  else if (newContainer[next_i][next_j].type == boundary)
  {
    // 2.2边界，结束
    return -2;
  }
  else if (newContainer[next_i][next_j].type == snake)
  {
    // 2.3咬到自身，结束
    return -3;
  }
  else if (newContainer[next_i][next_j].type == food)
  {
    // 2.4食物，向前走，并且添加新节点
    // 食物位置作为新的头节点，使用头插法插入
  }
  else
  {
    // 2.5 遇到空格，安全，向前走
    // 4.直接修改链表，更改链表的下标，个数不变
    int flag = updateContainerOnSnakeDir(newContainer, oldContainer, len, wid, Snake, press_dir,next_i,next_j);
    if (flag == -1)
      return 0;
    // newContainer[tail_i][tail_j] = (Block){
    //     .type = empty,
    //     .type_index = -1,
    //     .dir = none,
    //     .color = 0,
    //     .top_left_x = newContainer[tail_i][tail_j].top_left_x,
    //     .top_left_y = newContainer[tail_i][tail_j].top_left_y};
    return 1;
  }
}

