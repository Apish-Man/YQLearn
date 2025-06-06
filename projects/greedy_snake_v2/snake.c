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
  srand(time(NULL));
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

// 根据链表更新container
// newNode  oldHead  node node ...  tail  head
// p        q                       p     q
// 先保存p的block状态，再每次把q的block状态复制到p,直到q为NULL，然后把p的block设置为空
// 不需要修改Snake，故使用NODE*
// 成功返回0，失败返回-1
int updateContainerOnSnake(Block (*newC)[WIDTH_BOUNDARY], Block (*oldC)[WIDTH_BOUNDARY], int len, int wid, NODE *Snake)
{
  if (!Snake)
    return -1;
  // 头插法需要在这里设置好头节点方向
  int current_i=Snake->data.i,current_j=Snake->data.j;
  int next_i=Snake->next->data.i,next_j=Snake->next->data.j;
  oldC[current_i][current_j]=newC[current_i][current_j];
  newC[current_i][current_j].dir=newC[next_i][next_j].dir;
  paint_snake(newC, oldC, Snake, len, wid);
  return 0;
}

// int updateContainerOnSnake(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE *Snake)
// {
//   // 空
//   if (Snake == NULL)
//     return 0;
//   // 只有一个节点---这个不可能，因为只有一个节点意味着原来没有节点，此时已经结束了游戏
//   if (Snake->next == Snake)
//   {
//     return -1;
//   }

//   // 不止一个节点
//   // 从尾部开始处理，确保每个节点继承前驱节点的方向
//   NODE *p = Snake, *q = p->next;
//   while (q != Snake)
//   {
//     int p_i = p->data.i, p_j = p->data.j;
//     int q_i = q->data.i, q_j = q->data.j;
//     // 存储旧状态
//     oldContainer[p_i][p_j] = newContainer[p_i][p_j];
//     // q的block状态复制到p
//     newContainer[p_i][p_j] = newContainer[q_i][q_j];
//     // 计算p的top_left_x,top_left_y
//     newContainer[p_i][p_j].top_left_y = p_i * BLOCK_SIZE;
//     newContainer[p_i][p_j].top_left_x = p_j * BLOCK_SIZE;
//     // 更新q，q
//     p = q;
//     q = q->next;
//   }
//   // 最后处理

//   return 0;
// }

// 根据链表和输入方向更新container
//  Head  node node ...  noed  tail
//  p       q              p     q
// 从尾部开始遍历，q指向尾部，p指向q的前驱
// 每次更新时，先把q的块设置为空块，然后把p的i,j复制给p；直到q=head，结束，此时，更新q的坐标
// 不需要修改Snake，故使用NODE*
// 成功返回0，失败返回-1
int updateContainerOnSnakeDir(Block (*newC)[WIDTH_BOUNDARY],
                              Block (*oldC)[WIDTH_BOUNDARY],
                              int len, int wid,
                              NODE **Snake, int dir,
                              int next_i, int next_j)
{
  if (!(*Snake) || (*Snake)->next == *Snake)
    return -1;

  /* 1. 从尾到头依次把坐标前移 */
  NODE *tail = (*Snake)->prev;
  tail->data = (*Snake)->data; // 先让尾巴追到旧头
  for (NODE *cur = tail->prev; cur != tail; cur = cur->prev)
    cur->data = cur->prev->data; // 依次前移

  /* 2. 设置新头坐标 & 方向 */
  (*Snake)->data.i = next_i;
  (*Snake)->data.j = next_j;
  newC[next_i][next_j].dir = dir; // 头朝向，用于绘图

  /* 3. 重新刷整条蛇 */
  paint_snake(newC, oldC, *Snake, len, wid);
  return 0;
}

// int updateContainerOnSnakeDir(Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid, NODE **Snake, int dir,int next_i,int next_j) {
//   // 为空或者只有一个节点
//     if (!(*Snake) || (*Snake)->next == Snake) return -1;

//     // 从尾部开始处理，确保每个节点继承前驱节点的方向
//     NODE *current = (*Snake)->prev; // 从尾部开始
//     while (current != (*Snake)) {   // 遍历到头部停止
//         NODE *prev_node = current->prev;
//         int curr_i = current->data.i, curr_j = current->data.j;
//         int prev_i = prev_node->data.i, prev_j = prev_node->data.j;

//         // 更新链表，current的下标变为pre
//         current->data.i=prev_i;
//         current->data.j=prev_j;

//         // 更新container
//         // 保存旧状态
//         oldContainer[curr_i][curr_j] = newContainer[curr_i][curr_j];
//         if(current==(*Snake)->prev)
//         {
//           // 若当前节点是最后一个节点，需要把container置为空
//           newContainer[curr_i][curr_j] = (Block){
//             .type = empty,
//             .type_index = -1,
//             .dir = none,
//             .color = 0,
//             .top_left_x = newContainer[curr_i][curr_j].top_left_x,
//             .top_left_y = newContainer[curr_i][curr_j].top_left_y};
//         }
//         // 更新curr_i和curr_j
//           // 继承前驱节点的状态和方向
//           newContainer[prev_i][prev_j] = newContainer[prev_i][prev_j];
//           newContainer[curr_i][curr_j].top_left_x = curr_j * BLOCK_SIZE;
//           newContainer[curr_i][curr_j].top_left_y = curr_i * BLOCK_SIZE;

//         current = prev_node;
//     }

//     // 单独处理头节点的新方向
//     current->data.i=next_i,current->data.j=next_j;

//     return 0;
// }

/* 辅助：根据整条蛇重新刷 container */
/* ---------- 刷新整条蛇到 container ---------- */
static void paint_snake(Block (*newC)[WIDTH_BOUNDARY], Block (*oldC)[WIDTH_BOUNDARY], NODE *Snake, int len, int wid)
{
  /* 1. 把上一帧所有蛇块暂时记入 oldC，并清空 newC 中的蛇块 */
  NODE *n = Snake;
  if (!n)
    return;
  do
  {
    int i = n->data.i, j = n->data.j;
    oldC[i][j] = newC[i][j];
    newC[i][j].type = empty; // 先清空，稍后再重画
  } while ((n = n->next) != Snake);

  /* 2. 重新遍历，为新位置写入 block */
  n = Snake;
  int idx = 0;
  do
  {
    int i = n->data.i, j = n->data.j;
    newC[i][j].type = snake;
    newC[i][j].type_index =
        (idx == 0) ? 0 : /* 头  */
            (n->next == Snake) ? 2
                               : 1;              /* 尾 : 中间 */
    newC[i][j].dir = (idx == 0) ? newC[i][j].dir /* 头朝向已在外层设置 */
                                : newC[n->prev->data.i][n->prev->data.j].dir;
    newC[i][j].color = 0; /* 颜色你可以自定 */
    newC[i][j].top_left_x = j * BLOCK_SIZE;
    newC[i][j].top_left_y = i * BLOCK_SIZE;
    idx++;
  } while ((n = n->next) != Snake);
}
