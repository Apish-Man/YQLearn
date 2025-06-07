/*************************************************************************
  > File Name:    snake.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月03日 星期二 20时22分00秒
 ************************************************************************/

#include "snake.h"

/*
 * 初始化蛇函数，创建一个蛇头一个蛇尾一个蛇中间，同时会随机一个位置
 * @param container 网格的当前状态
 * @param int wid  边界宽度
 * @param int len  边界长度
 * @return Snake,返回初始化蛇，return NULL表示失败
 */
NODE *init_snake(Block (*newcontainer)[WIDTH_BOUNDARY], Block (*oldcontainer)[WIDTH_BOUNDARY], int len, int wid)
{
  // 创建头结点
  NODE *snake = createSnakeNode(newcontainer, len, wid);
  if (!snake)
  {
    printf("头节点创建失败\n");
    return NULL;
  }
  // 更新snake
  snake->next = snake;
  snake->prev = snake;
  // 更新对应container
  int tmpx = snake->data.i, tmpy = snake->data.j;
  oldcontainer[tmpx][tmpy] = newcontainer[tmpx][tmpy];
  newcontainer[tmpx][tmpy].type = 1; // snake
  newcontainer[tmpx][tmpy].type_index = 0;
  // dir是方向，范围为1-4
  int dir = rand() % 4 + 1;
  // int dir=3;
  newcontainer[tmpx][tmpy].dir = dir;

  // 在头节点的另外三个位置随机产生一个位置
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3
  int dir_i = (-1) * directions[dir - 1][0];
  int dir_j = (-1) * directions[dir - 1][1];

  // 创建中间节点
  NODE *middle = createSnakeNode(newcontainer, len, wid);
  // 设置中间节点在头节点后，且和其方向一致
  middle->data.i = snake->data.i + dir_i;
  middle->data.j = snake->data.j + dir_j;
  if (!middle)
    return NULL;
  // 尾插法
  int flag = dclist_tail_add(&snake, middle);
  if (flag == -1)
  {
    printf("尾插失败\n");
    return NULL;
  }
  // 更新对应container
  int midx = middle->data.i, midy = middle->data.j;
  oldcontainer[midx][midy] = newcontainer[midx][midy];
  newcontainer[midx][midy].type = 1; // snake
  newcontainer[midx][midy].type_index = 1;
  // dir是方向，范围为0-3
  newcontainer[midx][midy].dir = dir;

  // 创建尾巴,下一个节点必须在上一个节点的后面，并且他的方向应该和前面节点的上一个方向一致
  NODE *tail = createSnakeNode(newcontainer, len, wid);
  // 设置尾巴节点在头节点后，且和其方向一致
  tail->data.i = middle->data.i + dir_i;
  tail->data.j = middle->data.j + dir_j;
  if (!tail)
    return NULL;
  // 尾插法
  flag = dclist_tail_add(&snake, tail);
  if (flag == -1)
  {
    printf("尾插失败\n");
    return NULL;
  }
  // 更新对应container
  int tailx = tail->data.i, taily = tail->data.j;
  oldcontainer[tailx][taily] = newcontainer[tailx][taily];
  newcontainer[tailx][taily].type = 1; // snake
  newcontainer[tailx][taily].type_index = 2;
  // //dir是方向，范围为0-3
  newcontainer[tailx][taily].dir = dir;
  return snake;
}

// 创建节点
NODE *createSnakeNode(Block (*newcontainer)[WIDTH_BOUNDARY], int rows, int cols) // rows对应数组行数(y轴)，cols对应列数(x轴)
{
  // 创建头结点
  NODE *snake = (NODE *)malloc(sizeof(NODE));
  if (!snake)
    return NULL;
  // 头节点，x,y赋值
  int tmp_x, tmp_y;
  // 找到合适位置---此时不考虑碰撞，只考虑生成多个无关数据
  do
  {
    // x是横坐标，范围为mod（WIDTH_BOUNDARY-5*2）5-9  5为设定的边界距离
    tmp_x = rand() % (cols - 2 * 5) + 5;
    // y是横坐标，范围为mod (LENTH_BOUNDARY-5*2）
    tmp_y = rand() % (rows - 2 * 5) + 5;
  } while (newcontainer[tmp_y][tmp_x].type != 0);
  snake->data.i = tmp_y;
  snake->data.j = tmp_x;
  snake->next = NULL;
  snake->prev = NULL;
  return snake;
}

/*
 * 根据Snake来重新绘制蛇，包括蛇头，蛇尾，蛇方向,蛇的方向，这几个方向是使用下标计算出来的
 * @param Snake,新蛇链表
 * @param newContainer,新状态
 * @param oldContainer,旧状态
 * @return int 1为成功，0为失败
 */
int paint_new_Snake(NODE *Snake, Block (*newContainer)[WIDTH_BOUNDARY], int dir)
{
  if (!Snake)
    return 0;
  // 获取链表长度
  int len = 1;
  for (NODE *n=Snake; ; n=n->next, ++len)
      if (n->next == Snake) break;

  // 第二次遍历，写入block
  int idx = 0;
  for (NODE *n = Snake;; n = n->next, idx++)
  {
    int i = n->data.i, j = n->data.j;
    Block *b = &newContainer[i][j];
    // 公共内容
    b->type = snake;
    b->top_left_x = j * BLOCK_SIZE;
    b->top_left_y = i * BLOCK_SIZE;

    // 判断头/尾/身
    if (idx == 0)
    {
      // 头
      b->type_index = 0;
      b->dir = dir;
    }
    else if (idx == len - 1)
    {
      // 尾
      b->type_index = 2;
      int di = n->prev->data.i - i;
      int dj = n->prev->data.j - j;
      b->dir = delta2dir(di, dj);
    }
    else
    {
      // 身体
      b->type_index = 1;
      // 取前后方向，决定是直线还是拐弯
      int dpi = n->prev->data.i - i;
      int dpj = n->prev->data.j - j;
      int dni = i-n->next->data.i;
      int dnj = j-n->next->data.j;

      // 获取前后节点方向
      enum DIRECTIONS prevDir = delta2dir(dpi, dpj);
      enum DIRECTIONS nextDir = delta2dir(dni, dnj);

      if (prevDir == nextDir)
      {
        /* 直线：dir 决定用横还是竖贴图 */
        b->dir = prevDir; /* up/down=竖，left/right=横 */
      }
      else
      {
        /* 拐角：用组合( prevDir , nextDir ) 判断四种转弯 */
        /* 这里把 dir 当成“拐角编号”留给渲染层 */
        /*             up->right  0
           0 1 2 3 →   right->down 1
                       down->left  2
                       left->up    3          */
        enum DIRECTIONS corner =
            (prevDir == right && nextDir == down) || (prevDir == up && nextDir == left) ? corner0 : 
            (prevDir == down && nextDir == left) || (prevDir == right && nextDir == up) ? corner1 :
            (prevDir == left && nextDir == up) || (prevDir == down && nextDir == right)     ? corner2: corner3;
        b->dir = corner;
      }
    }
    if (n->next == Snake) break;
  }
  return 1;
}

/* 根据坐标差计算方向 */
// enum DIRECTIONS {none,up,down,left,right};
static enum DIRECTIONS delta2dir(int di, int dj)
{
  if (di == -1)
    return up; // 行号减一 ⇒ 向上
  if (di == 1)
    return down; // 行号加一 ⇒ 向下
  if (dj == -1)
    return left; // 列号减一 ⇒ 向左
  return right;  // dj == +1  ⇒ 向右
}

/*
 * 统计Snake链表的下标，并且存储到snapshotPos，统计其个数，记入snapshotCnt
 * @param Snake,蛇链表
 * @param snapshotPos,暂存位置数组
 * @param snapshotCnt,记录旧链表长度
 * @return int 1为成功，0为失败
 */
int snapshot_old_snake(NODE *Snake, Coordinate *snapshotPos, int *snapshotCnt)
{
  NODE *p = Snake;
  int cnt = 0;
  if (p == NULL)
  {
    *snapshotCnt = cnt;
    return 1;
  }
  if (p->next == Snake)
  {
    cnt = 1;
    *snapshotCnt = cnt;
    return 1;
  }

  do{
    int p_i = p->data.i, p_j = p->data.j;
    snapshotPos[cnt] = (Coordinate){p_i, p_j};
    cnt++;
    p=p->next;
  }while(p != Snake);
  *snapshotCnt = cnt;
  return 1;
}

/*
 * 根据snapshotPos和snapshotCnt清空旧链表（将其对应block设置为空），先保存旧状态，再把新状态设置为空
 * @param snapshotPos,旧蛇下标信息
 * @param snapshotCnt,旧蛇数量
 * @param newContainer,新状态
 * @param oldContainer,旧状态
 * @return int 1为成功，0为失败
 */
int clear_old_Snake(Coordinate *snapshotPos, int snapshotCnt, Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY])
{
  if (snapshotCnt == 0)
  {
    return 0;
  }
  for (int i = 0; i < snapshotCnt; i++)
  {
    int snake_i = snapshotPos[i].i, snake_j = snapshotPos[i].j;
    oldContainer[snake_i][snake_j] = newContainer[snake_i][snake_j];
    newContainer[snake_i][snake_j] = (Block){
        .type = empty,
        .type_index = -1,
        .dir = none,
        .color = 0,
        .top_left_x = newContainer[snake_i][snake_j].top_left_x,
        .top_left_y = newContainer[snake_i][snake_j].top_left_y};
  }
  return 1;
}
