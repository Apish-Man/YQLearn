/*************************************************************************
  > File Name:    game_context.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月07日 星期六 23时14分41秒
 ************************************************************************/

#include "game_context.h"

/*
 * 初始化游戏上下文
 * 初始化new old container,Snake Food Obstacle链表
 * 初始化游戏状态,当前玩家状态，最后一次方向键，当前分数
 * 初始化失败返回0，成功返回1
 */
int gc_init(GameContext *ctx, int origin_x, int origin_y)
{
  // 初始化长宽
  ctx->len = LENTH_BOUNDARY;
  ctx->wid = WIDTH_BOUNDARY;
  ctx->win_game = newwin(ctx->len * BLOCK_SIZE, ctx->wid * BLOCK_SIZE, origin_y, origin_x);
  ctx->win_status = newwin(3, ctx->wid * BLOCK_SIZE, origin_x + ctx->len * BLOCK_SIZE, origin_y);
  ctx->win_debug = newwin(3, ctx->wid * BLOCK_SIZE, origin_x + ctx->len * BLOCK_SIZE + 4, origin_y);
  ctx->origin_x = origin_x;
  ctx->origin_y = origin_y;

  int board_w = 23; /* 排行榜栏固定宽度 */
  ctx->win_board = newwin(ctx->len * BLOCK_SIZE, board_w, origin_y, origin_x + ctx->wid * BLOCK_SIZE + 1);

  ctx->press_dir = none;
  ctx->press_dir2 = none;
  ctx->score = 0;
  ctx->score2 = 0;
  ctx->last_saved_score = -1;
  ctx->player_name[0] = '\0';
  ctx->player_name2[0] = '\0';

  ctx->Snake = NULL;
  ctx->Snake2 = NULL;
  ctx->Food = NULL;
  ctx->Obstacle = NULL;

  // 根据主机状态选择合适的重置函数
  if (ctx->mp == 0)
    gc_reset(ctx);
  else
    gc_reset_double(ctx);
  ctx->status = 1;
  return 1;
}

/*
 * 获得服务器初始状态后，根据服务器状态进行初始化
 * 初始化new old container,Snake Food Obstacle链表
 * 初始化游戏状态,当前玩家状态，最后一次方向键，当前分数
 * 初始化失败返回0，成功返回1
 */
int gc_init_client(GameContext *ctx, int origin_x, int origin_y,MPStatePkt *pkt)
{
  ctx->status=1;
  // 初始化长宽
  ctx->len = LENTH_BOUNDARY;
  ctx->wid = WIDTH_BOUNDARY;
  ctx->win_game = newwin(ctx->len * BLOCK_SIZE, ctx->wid * BLOCK_SIZE, origin_y, origin_x);
  ctx->win_status = newwin(3, ctx->wid * BLOCK_SIZE, origin_x + ctx->len * BLOCK_SIZE, origin_y);
  ctx->win_debug = newwin(3, ctx->wid * BLOCK_SIZE, origin_x + ctx->len * BLOCK_SIZE + 4, origin_y);
  ctx->origin_x = origin_x;
  ctx->origin_y = origin_y;

  int board_w = 23; /* 排行榜栏固定宽度 */
  ctx->win_board = newwin(ctx->len * BLOCK_SIZE, board_w, origin_y, origin_x + ctx->wid * BLOCK_SIZE + 1);

  // 初始化container
  initContainer(ctx->newC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);
  initEmptyContainer(ctx->oldC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);

  ctx->Snake = NULL;
  ctx->Snake2 = NULL;
  ctx->Food = NULL;
  ctx->Obstacle = NULL;

  // 对pkt进行解包，同时把数据更新
  gc_unpack_state(ctx,pkt);
  return 1;
}

int gc_init_client2(GameContext *ctx, int origin_x, int origin_y)
{
  ctx->status=1;
  // 初始化长宽
  ctx->len = LENTH_BOUNDARY;
  ctx->wid = WIDTH_BOUNDARY;
  ctx->win_game = newwin(ctx->len * BLOCK_SIZE, ctx->wid * BLOCK_SIZE, origin_y, origin_x);
  ctx->win_status = newwin(3, ctx->wid * BLOCK_SIZE, origin_x + ctx->len * BLOCK_SIZE, origin_y);
  ctx->win_debug = newwin(3, ctx->wid * BLOCK_SIZE, origin_x + ctx->len * BLOCK_SIZE + 4, origin_y);
  ctx->origin_x = origin_x;
  ctx->origin_y = origin_y;

  int board_w = 23; /* 排行榜栏固定宽度 */
  ctx->win_board = newwin(ctx->len * BLOCK_SIZE, board_w, origin_y, origin_x + ctx->wid * BLOCK_SIZE + 1);

  // 初始化container
  initContainer(ctx->newC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);
  initEmptyContainer(ctx->oldC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);

  ctx->Snake = NULL;
  ctx->Snake2 = NULL;
  ctx->Food = NULL;
  ctx->Obstacle = NULL;

  return 1;
}

/*
 * 重置游戏状态
 * 重置new old container,Snake Food Obstacle链表
 * 初始化游戏状态,当前玩家状态，最后一次方向键，当前分数
 * 初始化失败返回0，成功返回1
 */
int gc_reset(GameContext *ctx)
{
  // 初始化container
  initContainer(ctx->newC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);
  if (ctx->status == INIT)
    initEmptyContainer(ctx->oldC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);

  // 初始化链表
  if (ctx->status != INIT)
  {
    // 释放旧链表
    dclist_destroy(&ctx->Snake);
    dclist_destroy(&ctx->Food);
    dclist_destroy(&ctx->Obstacle);
  }

  ctx->Snake = init_snake(ctx->newC, ctx->oldC, ctx->len, ctx->wid, 0); // 第一条蛇
  ctx->Food = init_food(ctx->newC, ctx->oldC, ctx->len, ctx->wid, NUM_INIT_FOOD);
  ctx->Obstacle = init_obstacle(ctx->newC, ctx->oldC, ctx->len, ctx->wid, NUM_INIT_ONSTACLE);

  if (!ctx->Snake || !ctx->Food || !ctx->Obstacle)
    return 0;

  ctx->gstate = READY;
  ctx->status = 1;
  ctx->score = dclist_len(ctx->Snake);
  ctx->press_dir = head_dir(ctx->Snake);

  return 1;
}

/*
 * 双人模式下重置游戏状态
 * 重置new old container,Snake Food Obstacle链表
 * 初始化游戏状态,当前玩家状态，最后一次方向键，当前分数
 * 初始化失败返回0，成功返回1
 */
int gc_reset_double(GameContext *ctx)
{
  // 初始化container
  initContainer(ctx->newC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);
  if (ctx->status == INIT)
    initEmptyContainer(ctx->oldC, ctx->len, ctx->wid, ctx->origin_x, ctx->origin_y);

  // 初始化链表
  if (ctx->Snake!=NULL||ctx->Snake2!=NULL||ctx->Food!=NULL||ctx->Obstacle!=NULL)
  {
    // 释放旧链表
    dclist_destroy(&ctx->Snake);
    dclist_destroy(&ctx->Food);
    dclist_destroy(&ctx->Obstacle);
    dclist_destroy(&ctx->Snake2);
  }

  ctx->Snake = init_snake(ctx->newC, ctx->oldC, ctx->len, ctx->wid, 0);  // 第1条
  ctx->Snake2 = init_snake(ctx->newC, ctx->oldC, ctx->len, ctx->wid, 1); // 第2条
  ctx->Food = init_food(ctx->newC, ctx->oldC, ctx->len, ctx->wid, NUM_INIT_FOOD);
  ctx->Obstacle = init_obstacle(ctx->newC, ctx->oldC, ctx->len, ctx->wid, NUM_INIT_ONSTACLE);

  if (!ctx->Snake || !ctx->Snake2 || !ctx->Food || !ctx->Obstacle)
    return 0;

  ctx->gstate = READY;
  ctx->gstate2 = READY;
  ctx->status = 1;
  ctx->score = dclist_len(ctx->Snake);
  ctx->score2 = dclist_len(ctx->Snake2);
  ctx->press_dir = head_dir(ctx->Snake);
  ctx->press_dir2 = head_dir(ctx->Snake2);
  // ctx->current_dir = ctx->press_dir = none;
  // ctx->current_dir2 = ctx->press_dir2 = none;

  return 1;
}

/*
 * 销毁上下文
 * 释放链表
 * 释放窗口
 * 初始化失败返回0，成功返回1
 */
int gc_destroy(GameContext *ctx)
{

  // 清空container
  clear_container(ctx->newC,ctx->len,ctx->wid);
  clear_container(ctx->oldC,ctx->len,ctx->wid);
  // 写一个清理游戏页面的，方便下次运行
  werase(ctx->win_game); wrefresh(ctx->win_game);delwin(ctx->win_game);clear(); refresh();
  werase(ctx->win_board); wrefresh(ctx->win_board);delwin(ctx->win_board);clear(); refresh();
  // 退出前写入数据
  score_append(ctx->player_name, ctx->score);
  dclist_destroy(&ctx->Snake);
  dclist_destroy(&ctx->Food);
  dclist_destroy(&ctx->Obstacle);

  delwin(ctx->win_game);
  delwin(ctx->win_status);
  delwin(ctx->win_debug);
  delwin(ctx->win_board);

  return 1;
}

/*
 * 输入处理
 * 根据输入字符，更新游戏状态，此处主要是更新游戏对应的结构体
 * 初始化失败返回0，成功返回1
 */
int gc_handle_input(GameContext *ctx, int ch)
{
  switch (ch)
  {
  // case 'q': case 'Q': endwin(); exit(0);
  case 'p':
  case 'P':
    if (ctx->gstate == RUNNING)
      ctx->gstate = PAUSED;
    else if (ctx->gstate == PAUSED)
      ctx->gstate = RUNNING;
    break;
  case 'r':
  case 'R':
    if (ctx->gstate == GAME_OVER)
    {
      score_append(ctx->player_name, ctx->score);
      if(ctx->mp==0)
        gc_reset(ctx);
      else
        gc_reset_double(ctx);
    }
    break;
  case ' ':
    if (ctx->gstate == READY)
      ctx->gstate = RUNNING;
    break;
  default:
    if (ctx->gstate == RUNNING)
    {
      // 忽略与当前方向相同的按键，不允许180°反向
      switch (ch)
      {
      case 'w':
      case 'W':
        ctx->press_dir = up;
        break;
      case 's':
      case 'S':
        ctx->press_dir = down;
        break;
      case 'a':
      case 'A':
        ctx->press_dir = left;
        break;
      case 'd':
      case 'D':
        ctx->press_dir = right;
        break;
      }
    }
    break;
  }
  return 1;
}

/*
 * 输入处理_远程
 * 根据输入字符，更新游戏状态，此处主要是更新游戏对应的结构体
 * 初始化失败返回0，成功返回1
 */
int gc_handle_remote_input(GameContext *ctx, int ch)
{
  switch (ch)
  {
  // case 'q': case 'Q': endwin(); exit(0);
  case ' ':
    if (ctx->gstate2 == READY)
      ctx->gstate2 = RUNNING;
    break;
  case 'p':
  case 'P':
    if (ctx->gstate2 == RUNNING)
      ctx->gstate2 = PAUSED;
    else if (ctx->gstate2 == PAUSED)
      ctx->gstate2 = RUNNING;
    break;
  case 'r':
  case 'R':
    if (ctx->gstate2 == GAME_OVER)
    {
      score_append(ctx->player_name, ctx->score);
      gc_reset_double(ctx);
    }
    break;
  default:
    if (ctx->gstate2 == RUNNING)
    {
      // 忽略与当前方向相同的按键，不允许180°反向
      switch (ch)
      {
      case 'w':
      case 'W':
        ctx->press_dir2 = up;
        break;
      case 's':
      case 'S':
        ctx->press_dir2 = down;
        break;
      case 'a':
      case 'A':
        ctx->press_dir2 = left;
        break;
      case 'd':
      case 'D':
        ctx->press_dir2 = right;
        break;
      }
    }
    break;
  }
  return 1;
}

/*
 * 游戏逻辑推进
 * 根据输入字符，更新游戏状态，此处主要是更新游戏对应的结构体
 * 初始化失败返回0，成功返回1
 */
int gc_tick_logic(GameContext *ctx)
{
  if (ctx->gstate != RUNNING)
    return 1;

  // 沿给定方向或者默认方向前进，并且更新状态
  // 此处需要屏蔽180°反转
  int i = ctx->Snake->data.i, j = ctx->Snake->data.j;
  int last_dir = ctx->newC[i][j].dir, next_dir = ctx->press_dir;
  if ((last_dir == 1 && next_dir == 2) || (last_dir == 2 && next_dir == 1) || (last_dir == 3 && next_dir == 4) || (last_dir == 4 && next_dir == 3))
    ctx->press_dir = last_dir;

  ctx->status = go_next_one(ctx);

  if (ctx->status != 1)
  {
    ctx->gstate = GAME_OVER;
  }
  else
  {
    ctx->score = dclist_len(ctx->Snake);
    // 更新当前排行榜，实现每吃一次，排行榜都会更新一次
    if (ctx->score != ctx->last_saved_score)
    {
      score_append(ctx->player_name, ctx->score);
      ctx->last_saved_score = ctx->score;
    }
  }
  return 1;
}

/*
 * 游戏逻辑推进_双人
 * 根据输入字符，更新游戏状态，此处主要是更新游戏对应的结构体
 * 初始化失败返回0，成功返回1
 * status为1表示未结束，小于0表示结束，-1是玩家1赢，-2玩家2赢,0表示异常结束
 */
int gc_tick_logic_double(GameContext *ctx)
{
  if (ctx->status!=1)
    return 1;
  if(!(ctx->gstate==RUNNING&&ctx->gstate2==RUNNING))
    return 1;

  // 沿给定方向或者默认方向前进，并且更新状态
  // 此处需要屏蔽180°反转
  int i = ctx->Snake->data.i, j = ctx->Snake->data.j;
  int last_dir = ctx->newC[i][j].dir, next_dir = ctx->press_dir;
  if ((last_dir == 1 && next_dir == 2) || (last_dir == 2 && next_dir == 1) || (last_dir == 3 && next_dir == 4) || (last_dir == 4 && next_dir == 3))
    ctx->press_dir = last_dir;
  // 玩家2状态
  int i2 = ctx->Snake2->data.i, j2 = ctx->Snake2->data.j;
  int last_dir2 = ctx->newC[i2][j2].dir, next_dir2 = ctx->press_dir2;
  if ((last_dir2 == 1 && next_dir2 == 2) || (last_dir2 == 2 && next_dir2 == 1) || (last_dir2 == 3 && next_dir2 == 4) || (last_dir2 == 4 && next_dir2 == 3))
    ctx->press_dir2 = last_dir2;

  // 第一条蛇向前走
  int status = go_next_one(ctx);
  // 第二条蛇向前走
  int status2 = go_next_one_double(ctx);

  // status为1表示未结束，小于0表示结束，-1是玩家1赢，-2玩家2赢,0表示异常结束
  if (status == 1 && status2 == 1)
  {
    ctx->status = 1;
  }
  else
  {
    if (status == 1)
      ctx->status = -1; // 1win
    else if (status2 == 1)
      ctx->status = -2; // 2win
    else
      ctx->status = 0; // 异常
  }

  if (ctx->status != 1)
  {
    ctx->gstate = GAME_OVER;
    ctx->gstate2 =GAME_OVER;
  }
  else
  {
    ctx->score = dclist_len(ctx->Snake);
    ctx->score2 = dclist_len(ctx->Snake2);
    // 更新当前排行榜，实现每吃一次，排行榜都会更新一次
    // render_mp_rank(ctx);
    // if (ctx->score != ctx->last_saved_score)
    // {
    //     score_append(ctx->player_name, ctx->score);
    //     ctx->last_saved_score = ctx->score;
    // }
  }
  return 1;
}

/*
 * 沿给定方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身,-4咬到其他蛇，如果未结束，返回1，如果运行出错，返回0
 */
int go_next_one(GameContext *ctx)
{
  if (ctx->Snake == NULL)
  {
    return 0;
  }
  // 0.获取头元素坐标
  int head_i = (ctx->Snake)->data.i, head_j = (ctx->Snake)->data.j;
  // 1.获取下一位置坐标
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3,dir对应1-4，对应于此的0-3
  int dir;
  if (ctx->press_dir == 0)
  {
    // 直行
    dir = ctx->newC[head_i][head_j].dir;
  }
  else
  {
    // 带方向
    dir = ctx->press_dir;
  }
  int next_i = head_i + directions[dir - 1][0];
  int next_j = head_j + directions[dir - 1][1];
  if (next_i >= LENTH_BOUNDARY || next_j >= WIDTH_BOUNDARY || next_i < 0 || next_j < 0)
    return 0;
  // 2.判断下一位置状态

  if (ctx->newC[next_i][next_j].type == obstacle)
  {
    // 2.1障碍物，结束
    ctx->gstate=GAME_OVER;
    return -1;
  }
  else if (ctx->newC[next_i][next_j].type == boundary)
  {
    // 2.2边界，结束
    ctx->gstate=GAME_OVER;
    return -2;
  }
  else if (ctx->newC[next_i][next_j].type == snake)
  {
    if (ctx->newC[next_i][next_j].index == 0)
    {
      // 2.3咬到自身，结束
      ctx->gstate=GAME_OVER;
      return -3;
    }
    else
    {
      // 咬到其他蛇，结束
      ctx->gstate=GAME_OVER;
      return -4;
    }
  }
  else if (ctx->newC[next_i][next_j].type == food)
  {
    // 2.4食物，向前走，并且添加新节点
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int snapshotCnt = 0;
    snapshot_old_snake(ctx->Snake, snapshotPos, &snapshotCnt);
    // b.更新Snake链表
    NODE *tail = (ctx->Snake)->prev, *head = ctx->Snake; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // b.1创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(&ctx->Snake, newNode);

    // c.更新Food链表
    // c.1 删除该位置对应的food
    dclist_delete(&ctx->Food, (Coordinate){next_i, next_j});
    // c.2.生成新的食物节点到Food并且更新container（因为Snake和Food不一样，Food不用考虑状态什么）
    add_one_food_and_update_container(&ctx->Food, ctx->newC, ctx->oldC, ctx->len, ctx->wid);

    // d.清空旧container
    clear_old_Snake(snapshotPos, snapshotCnt, ctx->newC, ctx->oldC);

    // e.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(ctx->Snake, ctx->newC, dir, 0);
    return 1;
  }
  else
  {
    // 2.5 遇到空格，安全，向前走
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int snapshotCnt = 0;
    snapshot_old_snake(ctx->Snake, snapshotPos, &snapshotCnt);
    // b.更新链表
    NODE *tail = (ctx->Snake)->prev, *head = ctx->Snake; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // b.1创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(&ctx->Snake, newNode);
    // b.2 更新链表中的下标关系（只删除尾节点即可）
    dclist_tail_delete(&ctx->Snake);

    // c.清空旧container
    clear_old_Snake(snapshotPos, snapshotCnt, ctx->newC, ctx->oldC);

    // d.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(ctx->Snake, ctx->newC, dir, 0);
    return 1;
  }
}

/*
 * 沿给定方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身,-4咬到其他蛇，如果未结束，返回1，如果运行出错，返回0
 */
int go_next_one_double(GameContext *ctx)
{
  if ((ctx->Snake2) == NULL)
  {
    return 0;
  }
  // 0.获取头元素坐标
  int head_i = (ctx->Snake2)->data.i, head_j = (ctx->Snake2)->data.j;
  // 1.获取下一位置坐标
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3,dir对应1-4，对应于此的0-3
  int dir;
  if (ctx->press_dir2 == 0)
  {
    // 直行
    dir = ctx->newC[head_i][head_j].dir;
  }
  else
  {
    // 带方向
    dir = ctx->press_dir2;
  }
  int next_i = head_i + directions[dir - 1][0];
  int next_j = head_j + directions[dir - 1][1];
  if (next_i >= LENTH_BOUNDARY || next_j >= WIDTH_BOUNDARY || next_i < 0 || next_j < 0)
    return 0;
  // 2.判断下一位置状态

  if (ctx->newC[next_i][next_j].type == obstacle)
  {
    // 2.1障碍物，结束
    ctx->gstate2=GAME_OVER;
    return -1;
  }
  else if (ctx->newC[next_i][next_j].type == boundary)
  {
    // 2.2边界，结束
    ctx->gstate2=GAME_OVER;
    return -2;
  }
  else if (ctx->newC[next_i][next_j].type == snake)
  {
    if (ctx->newC[next_i][next_j].index == 1)
    {
      // 2.3咬到自身，结束
      ctx->gstate2=GAME_OVER;
      return -3;
    }
    else
    {
      // 2.3咬到其他蛇，结束
      ctx->gstate2=GAME_OVER;
      return -4;
    }
  }
  else if (ctx->newC[next_i][next_j].type == food)
  {
    // 2.4食物，向前走，并且添加新节点
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int snapshotCnt = 0;
    snapshot_old_snake(ctx->Snake2, snapshotPos, &snapshotCnt);
    // b.更新Snake链表
    NODE *tail = (ctx->Snake2)->prev, *head = ctx->Snake2; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // b.1创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(&ctx->Snake2, newNode);

    // c.更新Food链表
    // c.1 删除该位置对应的food
    dclist_delete(&ctx->Food, (Coordinate){next_i, next_j});
    // c.2.生成新的食物节点到Food并且更新container（因为Snake和Food不一样，Food不用考虑状态什么）
    add_one_food_and_update_container(&ctx->Food, ctx->newC, ctx->oldC, ctx->len, ctx->wid);

    // d.清空旧container
    clear_old_Snake(snapshotPos, snapshotCnt, ctx->newC, ctx->oldC);

    // e.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(ctx->Snake2, ctx->newC, dir, 1);
    return 1;
  }
  else
  {
    // 2.5 遇到空格，安全，向前走
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int snapshotCnt = 0;
    snapshot_old_snake(ctx->Snake2, snapshotPos, &snapshotCnt);
    // b.更新链表
    NODE *tail = (ctx->Snake2)->prev, *head = ctx->Snake2; // head头，tail指向尾
    if (head == NULL)
      return 0;
    // b.1创建新节点，存储空格,头插法插入双向循环链表（维护链表一致性）
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    newNode->data.i = next_i;
    newNode->data.j = next_j;
    // 添加到链表头部
    dclist_head_add(&ctx->Snake2, newNode);
    // b.2 更新链表中的下标关系（只删除尾节点即可）
    dclist_tail_delete(&ctx->Snake2);

    // c.清空旧container
    clear_old_Snake(snapshotPos, snapshotCnt, ctx->newC, ctx->oldC);

    // d.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(ctx->Snake2, ctx->newC, dir, 1);
    return 1;
  }
}

/*
 * 渲染逻辑
 * 根据更新后的ctx，渲染数据
 * 渲染失败返回0，成功返回1
 */
int gc_tick_render(GameContext *ctx)
{
  // 渲染游戏区域
  // renderContainer(ctx->win_game, ctx->newC, ctx->oldC, ctx->len, ctx->wid);
  renderAllContainer(ctx->win_game, ctx->newC, ctx->len, ctx->wid);

  /* 渲染状态栏 */
  // 清空状态栏
  werase(ctx->win_status);
  // 绘制边框
  box(ctx->win_status, 0, 0);
  char msg[64] = "";
  switch (ctx->gstate)
  {
  case READY:
    strcpy(msg, "Press SPACE to Start");
    break;
  case RUNNING:
    sprintf(msg, "Score: %ld  P:Pause Q:Quit", ctx->score);
    break;
  case PAUSED:
    strcpy(msg, "Paused. P:Resume Q:Quit");
    break;
  case GAME_OVER:
    sprintf(msg, "Game Over! Score:%ld R:Restart Q:Quit", ctx->score);
    break;
  default:
    break;
  }
  // 统一居中打印信息
  int max_y, max_x;
  getmaxyx(ctx->win_status, max_y, max_x);
  int x = (max_x - (int)strlen(msg)) / 2;
  mvwprintw(ctx->win_status, 1, x < 0 ? 0 : x, "%s", msg);

  // 刷新窗口
  wrefresh(ctx->win_status);

  // 绘制排行榜
  if(ctx->mp==0)
    draw_board(ctx);
  else 
    render_mp_rank(ctx); 
  return 1;
}

/*
 * 渲染逻辑_客户端
 * 根据更新后的ctx，渲染数据
 * 渲染失败返回0，成功返回1
 */
int gc_tick_render2(GameContext *ctx)
{
  // 渲染游戏区域
  renderContainer(ctx->win_game, ctx->newC, ctx->oldC, ctx->len, ctx->wid);
  // renderAllContainer(ctx->win_game, ctx->newC, ctx->len, ctx->wid);

  /* 渲染状态栏 */
  // 清空状态栏
  werase(ctx->win_status);
  // 绘制边框
  box(ctx->win_status, 0, 0);
  char msg[64] = "";
  switch (ctx->gstate2)
  {
  case READY:
    strcpy(msg, "Press SPACE to Start");
    break;
  case RUNNING:
    sprintf(msg, "Score: %ld  P:Pause Q:Quit", ctx->score2);
    break;
  case PAUSED:
    strcpy(msg, "Paused. P:Resume Q:Quit");
    break;
  case GAME_OVER:
    sprintf(msg, "Game Over! Score:%ld R:Restart Q:Quit", ctx->score2);
    break;
  }
  // 统一居中打印信息
  int max_y, max_x;
  getmaxyx(ctx->win_status, max_y, max_x);
  int x = (max_x - (int)strlen(msg)) / 2;
  mvwprintw(ctx->win_status, 1, x < 0 ? 0 : x, "%s", msg);

  // 刷新窗口
  wrefresh(ctx->win_status);

  // 绘制排行榜
  if(ctx->mp==0)
    draw_board(ctx);
  else 
    render_mp_rank(ctx); 
  return 1;
}

/*
 * 绘制排行榜
 * 绘制失败返回0，成功返回1
 */
int draw_board(GameContext *ctx)
{
  // 清空屏幕
  werase(ctx->win_board);
  // 设置边框
  box(ctx->win_board, 0, 0);
  // 写入标题
  mvwprintw(ctx->win_board, 1, 2, " LEADER BOARD ");
  // 加载排行榜数据
  ScoreEntry *arr;
  int n;
  if (!score_load(&arr, &n))
  {
    mvwprintw(ctx->win_board, 2, 2, "(empty)");
    wrefresh(ctx->win_board);
    return 1;
  }

  // char line[64];
  // 显示数据
  for (int i = 0; i < n && i < 10; i++)
  { /* 只显示前 10 */
    if(strcmp(ctx->player_name,arr[i].name)==0) wattron(ctx->win_board, COLOR_PAIR(1)); // 使用红色标记自己
      mvwprintw(ctx->win_board, 3 + i, 2, "No%2d: %-5s %3ld", i + 1, arr[i].name, arr[i].score);
    if(strcmp(ctx->player_name,arr[i].name)==0) wattron(ctx->win_board, COLOR_PAIR(1)); wattroff(ctx->win_board, COLOR_PAIR(1));
    // snprintf(line, sizeof(line), "NO%2d. %-12s %5ld",i + 1, arr[i].name, arr[i].score);
    // center_print(ctx->win_board, 3 + i, line);   /* 横向自动居中 */
  }

  free(arr);
  wrefresh(ctx->win_board);
  return 1;
}

/*
 * 居中打印函数
 * win为窗口
 * y为在第几行
 * text为要打印的文本内容
 */
void center_print(WINDOW *win, int y, const char *text)
{
  int max_y, max_x;
  getmaxyx(win, max_y, max_x);
  int x = (max_x - (int)strlen(text)) / 2;
  mvwprintw(win, y, x < 0 ? 0 : x, "%s", text);
}

// 打包要发送的数据
void gc_pack_state(GameContext *ctx, MPStatePkt *pkt)
{
    // 填充蛇的位置信息
    pack_snake(ctx->Snake, &pkt->s1);
    pack_snake(ctx->Snake2, &pkt->s2);
    // 填充食物位置
    pack_food(ctx->Food, pkt->food);
    // 填充障碍物位置
    pack_obstacle(ctx->Obstacle, pkt->obstacle);
    // 得分
    pkt->score = (uint16_t)ctx->score;
    pkt->score2 = (uint16_t)ctx->score2;
    // 游戏状态
    pkt->gstate = (uint8_t)ctx->gstate;
    pkt->gstate2 = (uint8_t)ctx->gstate2;
    pkt->status = (uint8_t)ctx->status;
    pkt->last_step_us = (uint16_t)ctx->last_step_us;
    // 在 gc_pack_state 函数中添加以下两行
    // pkt->dir = ctx->press_dir;    // 玩家1的当前方向
    // pkt->dir2 = ctx->press_dir2;  // 玩家2的当前方向
    // 打包时可能需捕获蛇的实时方向（而非 press_dir）
    pkt->dir = head_dir(ctx->Snake);    // 需实现 head_dir 函数
    pkt->dir2 = head_dir(ctx->Snake2);


    // 地图长宽
    pkt->wid = ctx->wid;
    pkt->len = ctx->len;

    // 玩家信息
    memset(pkt->player_name, 0, NAME_MAXLEN);  // 先清零
    memset(pkt->player_name2, 0, NAME_MAXLEN);
    strncpy(pkt->player_name, ctx->player_name, NAME_MAXLEN - 1);
    strncpy(pkt->player_name2, ctx->player_name2, NAME_MAXLEN - 1);
    pkt->player_name[NAME_MAXLEN-1] = '\0';
    pkt->player_name2[NAME_MAXLEN-1] = '\0';
}

// 辅助函数：将蛇的链表转换为MPSnake结构
static void pack_snake(NODE *snake, MPSnake *pkt_s)
{
  uint8_t len = 0;
  if (snake == NULL)
  {
    pkt_s->len = 0;
    return;
  }
  NODE *current = snake;
  do
  {
    if (len >= MP_MAX_BODY)
      break;
    pkt_s->body[len].x = (int8_t)current->data.i; 
    pkt_s->body[len].y = (int8_t)current->data.j; 
    len++;
    current = current->next;
  } while (current != snake); // 循环链表，遍历到头部结束
  pkt_s->len = len;
}

// 辅助函数：将食物链表转换为MPPos数组
static void pack_food(NODE *food, MPPos food_pkt[])
{
  int idx = 0;
  if (food == NULL)
    return;
  NODE *current = food;
  do
  {
    if (idx >= NUM_INIT_FOOD)
      break;
    food_pkt[idx].x = (int8_t)current->data.i;
    food_pkt[idx].y = (int8_t)current->data.j;
    idx++;
    current = current->next;
  } while (current != food);
}

// 辅助函数：将障碍物链表转换为MPPos数组
static void pack_obstacle(NODE *obstacle, MPPos obstacle_pkt[])
{
  int idx = 0;
  if (obstacle == NULL)
    return;
  NODE *current = obstacle;
  do
  {
    if (idx >= NUM_INIT_ONSTACLE)
      break;
    obstacle_pkt[idx].x = (int8_t)current->data.i;
    obstacle_pkt[idx].y = (int8_t)current->data.j;
    idx++;
    current = current->next;
  } while (current != obstacle);
}


/* ================== 解包蛇数据 ================== */
static void unpack_snake(NODE **snake_ptr, const MPSnake *pkt_snake)
{
  if (pkt_snake->len == 0 || pkt_snake->len > MP_MAX_BODY)
    return;

  // 构建新链表
  for (uint8_t i = 0; i < pkt_snake->len; ++i)
  {
    MPPos pos = pkt_snake->body[i];
    // 创建新节点并保持循环链表结构
    NODE *node = malloc(sizeof(NODE));
    node->data = (Coordinate){pos.x, pos.y}; 
    dclist_tail_add(snake_ptr, node);
  }
};


// 方向本质上应由 蛇头和第二节身体的位置关系 推算
static enum DIRECTIONS head_dir(const NODE *snake)
{
    if (!snake || snake->next == snake) return none;
    int di = snake->data.i - snake->next->data.i;
    int dj = snake->data.j - snake->next->data.j;
    return delta2dir(di, dj);          // 已在 snake.c 中实现
}

// 对接收到的数据包解包
// 解包网络数据包到游戏上下文
void gc_unpack_state(GameContext *ctx, MPStatePkt *pkt)
{
  if (!ctx || !pkt) return; // 空指针检查

  // 记录清空整个container
  Block oldC[LENTH_BOUNDARY][WIDTH_BOUNDARY];
  memcpy(oldC, ctx->oldC, sizeof(oldC));
  clear_container(ctx->newC, ctx->len, ctx->wid);
  
  // 解包主蛇和副蛇
  if (ctx->Snake!=NULL) {
    dclist_destroy(&ctx->Snake); // 先销毁旧蛇
  }
  if (ctx->Snake2) {
    dclist_destroy(&ctx->Snake2); // 先销毁旧蛇
  }
  unpack_snake(&ctx->Snake, &pkt->s1);
  unpack_snake(&ctx->Snake2, &pkt->s2);

  // 更新蛇的显示
  if(ctx->Snake) {
    paint_new_Snake(ctx->Snake, ctx->newC, head_dir(ctx->Snake), 0); // 设置 index 为 0
  }
  if(ctx->Snake2) {
    paint_new_Snake(ctx->Snake2, ctx->newC, head_dir(ctx->Snake2), 1); // 设置 index 为 1
  }

  /* ================== 解包食物数据 ================== */
  if (ctx->Food) {
    dclist_destroy(&ctx->Food); // 清空旧食物
  }
  for (int i = 0; i < NUM_INIT_FOOD && i < sizeof(pkt->food)/sizeof(pkt->food[0]); ++i)
  {
    if (pkt->food[i].x < 0 || pkt->food[i].x >= ctx->len ||
        pkt->food[i].y < 0 || pkt->food[i].y >= ctx->wid)
      continue;

    // 重建食物链表
    NODE *node = malloc(sizeof(NODE));
    if(!node) continue; // 内存分配失败
    
    node->data = (Coordinate){pkt->food[i].x, pkt->food[i].y}; // (i,j)
    if(dclist_tail_add(&ctx->Food, node) < 0) {
      free(node);
      continue; // 添加失败
    }

    // 更新block信息
    if (node->data.i >= 0 && node->data.i < ctx->len && 
        node->data.j >= 0 && node->data.j < ctx->wid) {
      Block *blk = &ctx->newC[node->data.i][node->data.j];
      blk->type = food;
      blk->type_index = rand() % NUM_FOOD; // 随机选择食物类型
      blk->dir = none; // 食物不需要方向
      blk->index = 0; // 设置 index 为 0
    }
  }

  /* ================== 解包障碍物数据 ================== */
  if (ctx->Obstacle) {
    dclist_destroy(&ctx->Obstacle); // 清空旧障碍物
  }
  for (int i = 0; i < NUM_INIT_ONSTACLE && i < sizeof(pkt->obstacle)/sizeof(pkt->obstacle[0]); ++i)
  {
    if (pkt->obstacle[i].x < 0 || pkt->obstacle[i].x >= ctx->len ||
        pkt->obstacle[i].y < 0 || pkt->obstacle[i].y >= ctx->wid)
      continue;

    // 重建障碍物链表
    NODE *node = malloc(sizeof(NODE));
    if(!node) continue; // 内存分配失败
    
    node->data = (Coordinate){pkt->obstacle[i].x, pkt->obstacle[i].y};
    if(dclist_tail_add(&ctx->Obstacle, node) < 0) {
      free(node);
      continue; // 添加失败
    }

    // 更新block信息
    if (node->data.i >= 0 && node->data.i < ctx->len && 
        node->data.j >= 0 && node->data.j < ctx->wid) {
      Block *blk = &ctx->newC[node->data.i][node->data.j];
      blk->type = obstacle;
      blk->type_index = rand() % NUM_OBSTACLE; // 随机选择障碍物类型
      blk->dir = none; // 障碍物不需要方向
      blk->index = 0; // 设置 index 为 0
    }
  }

  /* ================== 更新游戏状态 ================== */
    // 更新地图长宽
    ctx->len = pkt->len;
    ctx->wid = pkt->wid;
    // 初始方向
    ctx->press_dir = pkt->dir;
    ctx->press_dir2 = pkt->dir2;
    // 分数
    ctx->score = pkt->score;
    ctx->score2 = pkt->score2;
    // 状态
    ctx->gstate = (enum GAME_STATE)pkt->gstate;
    ctx->gstate2 = (enum GAME_STATE)pkt->gstate2;
    // 游戏结果
    ctx->status = pkt->status;
    // 时间帧
    ctx->last_step_us = pkt->last_step_us;
    
    // 更新玩家信息
    strncpy(ctx->player_name, pkt->player_name, NAME_MAXLEN - 1);
    ctx->player_name[NAME_MAXLEN - 1] = '\0';  // 确保字符串结束
    strncpy(ctx->player_name2, pkt->player_name2, NAME_MAXLEN - 1);
    ctx->player_name2[NAME_MAXLEN - 1] = '\0';  // 确保字符串结束
}

/* 渲染双人对战排行榜 */
void render_mp_rank(GameContext *ctx) {
    // 清空屏幕
    werase(ctx->win_board);
    // 设置边框
    box(ctx->win_board, 0, 0);
    mvwprintw(ctx->win_board, 1, 1, "Battle Rankings");
    mvwprintw(ctx->win_board, 2, 1, "----------------");
    
    // 显示玩家1信息
    mvwprintw(ctx->win_board, 3, 1, "1. %s: %ld", ctx->player_name, ctx->score);
    
    // 显示玩家2信息
    mvwprintw(ctx->win_board, 4, 1, "2. %s: %ld", ctx->player_name2, ctx->score2);
    
    // 显示游戏状态
    char* status_str;
    switch(ctx->status) {
        case -1:
            status_str = "Player 1 wins!";
            break;
        case -2:
            status_str = "Player 2 wins!";
            break;
        case 0:
            status_str = "Game Over Abnormally";
            break;
        default:
            status_str = "Game in Progress";
    }
    mvwprintw(ctx->win_board, 6, 1, "Status: ");
    mvwprintw(ctx->win_board, 7, 1, " %s", status_str);
    
    wrefresh(ctx->win_board);
}



