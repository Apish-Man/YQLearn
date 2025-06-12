/*************************************************************************
  > File Name:    block.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月04日 星期三 20时12分49秒
 ************************************************************************/

#include "block.h"

int initContainer(Block container[][WIDTH_BOUNDARY], int len, int wid, int x, int y)
{
  if (!container)
  {
    perror("initContainer失败\n");
    return 0;
  }
  // 初始化块
  for (int i = 0; i < len; i++)
  {
    for (int j = 0; j < wid; j++)
    {
      int block_x = x + j * BLOCK_SIZE;
      int block_y = y + i * BLOCK_SIZE;
      initBlock(&container[i][j], block_x, block_y);
    }
  }
  // // 设置上下边界块
  for (int i = 0; i < wid; i++)
  {
    Block *p = &container[0][i];
    p->type = boundary;
    p->type_index = 0; // 0上下边界，1为左右边界
    p =&container[len-1][i];
    p->type = boundary;
    p->type_index = 0;
  }
  // // 设置左右边界块
  for (int i = 0; i < len; i++)
  {
    Block *p = &container[i][0];
    p->type = boundary;
    p->type_index = 1; // 0上下边界，1为左右边界
    p = &container[i][wid-1];
    p->type = boundary;
    p->type_index = 1;
  }
  return 1;
}

int initEmptyContainer(Block (*container)[WIDTH_BOUNDARY], int len, int wid, int x, int y)
{
  if (!container)
  {
    perror("initContainer失败\n");
    return 0;
  }
  // 初始化块
  for (int i = 0; i < len; i++)
  {
    for (int j = 0; j < wid; j++)
    {
      int block_x = x + j * BLOCK_SIZE;
      int block_y = y + i * BLOCK_SIZE;
      initBlock(&container[i][j], block_x, block_y);
    }
  }
  return 1;
}

// 内部初始化block函数
static int initBlock(Block *p, int x, int y)
{
  p->type = empty;
  p->index=0;
  p->type_index = -1;
  p->dir = none;
  p->color = -1;
  p->top_left_x = x;
  p->top_left_y = y;
  return 1;
}

/*
* 清空Container内容渲
* 返回void
*/
void clear_container(Block (*newContainer)[WIDTH_BOUNDARY],int len,int wid)
{
  for (int i = 1; i < len-1; i++)
  {
    for (int j = 1; j < wid-1; j++)
    {
     newContainer[i][j]=(Block){
        .type = empty,
        .index=0,
        .type_index = -1,
        .dir = none,
        .color = 0,
        .top_left_x = newContainer[i][j].top_left_x,
        .top_left_y = newContainer[i][j].top_left_y};
      };
  }
}

int renderContainer(WINDOW *win, Block (*newContainer)[WIDTH_BOUNDARY], Block (*oldContainer)[WIDTH_BOUNDARY], int len, int wid)
{
  // 清理旧画面,动态刷新不需要修改
  // werase(win);

  // 遍历container，渲染
  for (int i =0 ; i < len; i++)
  {
    for (int j = 0; j < wid; j++)
    {
      // // 对每个块进行渲染
      if (memcmp(&newContainer[i][j], &oldContainer[i][j], sizeof(Block)) == 0)
        continue; // 数据相同跳过
      // 渲染新数据,block是一个BLOCK_SIZE*BLOCK_SIZE的区域，已有起始坐标，按照位置和类别渲染
      //  1.渲染新块
      if(!renderBlock(win, &newContainer[i][j]))
      {
        printf("块渲染失败\n");
      }
      // 2.复制新块数据到旧块
      memcpy(&oldContainer[i][j],&newContainer[i][j],sizeof(Block));
    }
  }

  // 刷新页面
  wrefresh(win);
  return 1;
}

// 块渲染函数
int renderBlock(WINDOW *win, Block *bl)
{
    // 获取窗口尺寸
  int max_x, max_y;
  getmaxyx(win, max_y, max_x);

  int block_i = 0, block_j = 0;

  // 设置颜色
  if (bl->type == snake) {
    if (bl->index == 0) {
      wattron(win, COLOR_PAIR(1)); // 第一条蛇使用红色
    } else if (bl->index == 1) {
      wattron(win, COLOR_PAIR(2)); // 第二条蛇使用绿色
    }
  } else if (bl->type == boundary) {
    wattron(win, COLOR_PAIR(3)); // 边界使用黄色
  } else if (bl->type == food) {
    wattron(win, COLOR_PAIR(4)); // 食物使用蓝色
  } else if (bl->type == obstacle) {
    wattron(win, COLOR_PAIR(5)); // 障碍物使用品红色
  }

  for (block_i = 0; block_i < BLOCK_SIZE; block_i++)
  {
    for (block_j = 0; block_j < BLOCK_SIZE; block_j++)
    {
      char c = ' '; // 待打印字符
      if (bl->type == boundary)
      {
        c = boundary_icon[bl->type_index][block_i * BLOCK_SIZE + block_j];
      }
      else if (bl->type == snake)
      {
        if(bl->type_index==0)
          c = snake_head_icon[bl->dir-1][block_i * BLOCK_SIZE + block_j];
        else if(bl->type_index==1)
          c = snake_middle_icon[bl->dir-1][block_i * BLOCK_SIZE + block_j];
        else if(bl->type_index==2)
          c = snake_tail_icon[bl->dir-1][block_i * BLOCK_SIZE + block_j];
      }
      else if(bl->type==food)
      {
        c = food_icon[bl->type_index][block_i * BLOCK_SIZE + block_j];
      }
      else if(bl->type==obstacle)
      {
        c = obstacle_icon[bl->type_index][block_i * BLOCK_SIZE + block_j];
      }
      // char c=snake[block_i][block_j];
      // char c=snake[newp->dir][block_i*BLOCK_SIZE + block_j];
      // c = snake_icon[0][block_i * BLOCK_SIZE + block_j];
      int win_y = bl->top_left_y + block_i;
      int win_x = bl->top_left_x + block_j;
      // 取出字符，放入缓冲区
      if (win_y < max_y && win_x < max_x)
      {
        mvwaddch(win, win_y, win_x, c);
      }
    }
  }

  // 关闭颜色
  wattroff(win, COLOR_PAIR(1));
  wattroff(win, COLOR_PAIR(2));
  wattroff(win, COLOR_PAIR(3));
  wattroff(win, COLOR_PAIR(4));
  wattroff(win, COLOR_PAIR(5));

  return 1;
}

int renderAllContainer(WINDOW *win, Block (*container)[WIDTH_BOUNDARY], int len, int wid)
{
  // 清理旧画面
  // werase(win);

  // 遍历container，渲染
  for (int i = 0; i < len; i++)
  {
    for (int j = 0; j < wid; j++)
    {
      //  对每个块进行渲染
      // 渲染新数据,block是一个BLOCK_SIZE*BLOCK_SIZE的区域，已有起始坐标，按照位置和类别渲染
      //  1.渲染新块
      if(!renderBlock(win, &container[i][j]))
      {
        printf("块渲染失败\n");
      }
    }
  }

  // 刷新页面
  wrefresh(win);
  return 1;
}


