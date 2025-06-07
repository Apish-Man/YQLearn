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
 * 处理按键输入,根据传入字符处理游戏状态
 * q,退出，s，开始，p，暂停，r，重新开始
 * 若目前状态是RUNNING，处理游戏状态更新
 */
void handle_input(int ch, enum GAME_STATE *s,int *press_dir) {
    switch (ch) {
    case 'q': case 'Q': end_game(); break;
    case 'b': case 'B': if (*s==READY)  *s = RUNNING;  break;
    case 'p': case 'P': if (*s==RUNNING) *s = PAUSED;
                        else if (*s==PAUSED)  *s = RUNNING; break;
    case 'r': case 'R': if (*s==GAME_OVER) restart_game();  break;
    default:
        if (*s==RUNNING) {
          // 修改转向
          switch (ch){
              case 'W':case 'w':*press_dir = up;break;
              case 'S':case 's':*press_dir = down;break;
              case 'A':case 'a':*press_dir = left;break;
              case 'D':case 'd':*press_dir = right;break;
              case ERR:*press_dir = none;break;
              default:*press_dir = none;break;
          }
        }
    }
}

/*
 * 结束游戏
 * 
 */
int end_game()
{
  return 1;
}

/*
 * 重新开始游戏
 * 重置new,old container，重置Snake、food
 * 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
 */
int restart_game()
{
  // 1.先销毁游戏
  // destory_game(&Snake,&Food,&Obstacle);
  // // 2.清空container，但是保持边界
  // initContainer(container, LENTH_BOUNDARY, WIDTH_BOUNDARY, INIT_X, INIT_Y);
  // // 3.重置三个链表
  // Snake=init_snake(container,oldcontainer,LENTH_BOUNDARY,WIDTH_BOUNDARY);
  // Food=init_food(container,oldcontainer,LENTH_BOUNDARY,WIDTH_BOUNDARY);
  // Obstacle=init_obstacle(container,oldcontainer,LENTH_BOUNDARY,WIDTH_BOUNDARY,NUM_INIT_ONSTACLE);
  // 4.分数重置，状态重置
  return 1;
}

/*
 * 显示游戏信息
 * s，游戏状态，status，玩家状态，score，得分
 */
void show_ui(WINDOW *w, enum GAME_STATE s, int status, long score) {
    werase(w);
    box(w, 0, 0);
    switch (s) {
    case READY:
        mvwprintw(w,1,2,"W/S/A/D Control the direction, B to start the game");
        break;
    case RUNNING:
        mvwprintw(w,1,2,"Scores: %ld   P:Pause  Q:Exit", score);
        break;
    case PAUSED:
        mvwprintw(w,1,2,"Paused  P:Continue  Q:Exit");
        break;
    case GAME_OVER:
        mvwprintw(w,1,2,"GAME OVER!  %s  Scores:%ld  R:Restart  Q:Exit",status==-1?"Hit the obstacle":status==-2?"crash into the wall":"Bite yourself", score);
        break;
    }
    wrefresh(w);
}


/*
 * 更新下一步游戏状态，同步更新容器内容,传入oldcontainer,newcontainer,len,wid,Snake,Food,Obstacle,press_dir
 * 根据传入的press_dir更新container和三个链表的状态，press_dir，0-4：enum DIRECTIONS {none,up,down,left,right}; //定义方向
 * 如果游戏结束，返回-1，如果未结束，返回1，如果运行出错，返回0
 */
int updateGame(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle, int press_dir)
{
  int status = 1; // 保存玩家状态，也存储运行信息
  
  status = go_next_one(newContainer, oldContainer, len, wid, Snake, Food, Obstacle,press_dir);
  
  return status;
}

/*
 * 沿给定方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身，如果未结束，返回1，如果运行出错，返回0
 */
int go_next_one(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, NODE **Food, NODE **Obstacle,int press_dir)
{
  if (*Snake == NULL)
  {
    return 0;
  }
  // 0.获取头元素坐标
  int head_i = (*Snake)->data.i, head_j = (*Snake)->data.j;
  // 1.获取下一位置坐标
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3,dir对应1-4，对应于此的0-3
  int dir;
  if(press_dir==0)
  {
    // 直行
    dir = newContainer[head_i][head_j].dir;
  }else{
    // 带方向
    dir=press_dir;
  }
  int next_i = head_i + directions[dir - 1][0];
  int next_j = head_j + directions[dir - 1][1];
  if (next_i >= LENTH_BOUNDARY || next_j >= WIDTH_BOUNDARY || next_i < 0 || next_j < 0)
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
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int  snapshotCnt = 0;
    snapshot_old_snake(*Snake,snapshotPos,&snapshotCnt);
    // b.更新Snake链表
    NODE *tail = (*Snake)->prev, *head = *Snake; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // b.1创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(Snake, newNode);

    // c.更新Food链表
    // c.1 删除该位置对应的food
    dclist_delete(Food,(Coordinate){next_i,next_j});
    // c.2.生成新的食物节点到Food并且更新container（因为Snake和Food不一样，Food不用考虑状态什么）
    add_one_food_and_update_container(Food,newContainer,oldContainer,len,wid);

    // d.清空旧container
    clear_old_Snake(snapshotPos,snapshotCnt,newContainer,oldContainer);

    // e.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(*Snake,newContainer,dir);
    return 1;
  }
  else
  {
    // 2.5 遇到空格，安全，向前走
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int  snapshotCnt = 0;
    snapshot_old_snake(*Snake,snapshotPos,&snapshotCnt);
    // b.更新链表
    NODE *tail = (*Snake)->prev, *head = *Snake; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // b.1创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(Snake, newNode);
    // b.2 更新链表中的下标关系（只删除尾节点即可）
    dclist_tail_delete(Snake);

    // c.清空旧container
    clear_old_Snake(snapshotPos,snapshotCnt,newContainer,oldContainer);

    // d.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(*Snake,newContainer,dir);
    return 1;
  }
}

/*
* 销毁游戏,free堆空间，保存游戏状态，比如最高分数，排名等
* 成功返回1，失败返回0
*/
int destory_game(NODE** Snake,NODE** Food,NODE** Obstacle)
{
  dclist_destroy(Snake);
  dclist_destroy(Food);
  dclist_destroy(Obstacle);
  return 1;
}
