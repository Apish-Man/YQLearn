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
int  gc_init  (GameContext *ctx, int origin_x, int origin_y)
{
  // 初始化长宽
    ctx->len = LENTH_BOUNDARY;
    ctx->wid = WIDTH_BOUNDARY;
    ctx->win_game = newwin(ctx->len*BLOCK_SIZE, ctx->wid*BLOCK_SIZE, origin_y, origin_x);
    ctx->win_status = newwin(3, ctx->wid*BLOCK_SIZE, origin_x + ctx->len*BLOCK_SIZE, origin_y);
    ctx->win_debug = newwin(3, ctx->wid*BLOCK_SIZE, origin_x + ctx->len*BLOCK_SIZE + 4, origin_y);
    ctx->origin_x=origin_x;
    ctx->origin_y=origin_y;

    int board_w = 23;                    /* 排行榜栏固定宽度 */
    ctx->win_board  = newwin(ctx->len*BLOCK_SIZE,board_w,origin_y,origin_x + ctx->wid*BLOCK_SIZE + 1);


    ctx->press_dir = none;
    ctx->press_dir2=none;
    ctx->score = 0;
    ctx->score2 = 0;
    ctx->last_saved_score=-1;
    ctx->status=INIT;
    ctx->player_name[0]='\0';

    ctx->Snake=NULL;
    ctx->Snake2=NULL;
    ctx->Food=NULL;
    ctx->Obstacle=NULL;

    gc_reset(ctx);
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
    initContainer(ctx->newC, ctx->len, ctx->wid,ctx->origin_x,ctx->origin_y);
    if(ctx->status==INIT)
      initEmptyContainer(ctx->oldC, ctx->len, ctx->wid,ctx->origin_x,ctx->origin_y);

    // 初始化链表
    if(ctx->status!=INIT)
    {
      // 释放旧链表
      dclist_destroy(&ctx->Snake);
      dclist_destroy(&ctx->Food);
      dclist_destroy(&ctx->Obstacle);
    }
    ctx->Snake = init_snake(ctx->newC,ctx->oldC, ctx->len, ctx->wid);
    ctx->Food = init_food(ctx->newC,ctx->oldC, ctx->len, ctx->wid, NUM_INIT_FOOD);
    ctx->Obstacle = init_obstacle(ctx->newC,ctx->oldC, ctx->len, ctx->wid, NUM_INIT_ONSTACLE);

    if(!ctx->Snake||!ctx->Food||!ctx->Obstacle) return 0;

    ctx->gstate = READY;
    ctx->status = 1;
    ctx->score = dclist_len(ctx->Snake);
    ctx->press_dir=none;

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
    switch (ch) {
        // case 'q': case 'Q': endwin(); exit(0);
        case ' ': if (ctx->gstate == READY) ctx->gstate = RUNNING; break;
        case 'p': case 'P':
            if (ctx->gstate == RUNNING) ctx->gstate = PAUSED;
            else if (ctx->gstate == PAUSED) ctx->gstate = RUNNING;
            break;
        case 'r': case 'R':
            if (ctx->gstate == GAME_OVER) 
            {
              score_append(ctx->player_name, ctx->score);
              gc_reset(ctx);
            }
            break;
        default:
            if (ctx->gstate == RUNNING) {
                // 忽略与当前方向相同的按键，不允许180°反向
                switch(ch) {
                    case 'w':case 'W': ctx->press_dir = up; break;
                    case 's':case 'S': ctx->press_dir = down; break;
                    case 'a':case 'A': ctx->press_dir = left; break;
                    case 'd':case 'D': ctx->press_dir = right; break;
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
    if (ctx->gstate != RUNNING) return 1;

    // 沿给定方向或者默认方向前进，并且更新状态
    // 此处需要屏蔽180°反转
    int i=ctx->Snake->data.i,j=ctx->Snake->data.j;
    int last_dir=ctx->newC[i][j].dir,next_dir=ctx->press_dir;
    if((last_dir==1&&next_dir==2)||(last_dir==2&&next_dir==1)||(last_dir==3&&next_dir==4)||(last_dir==4&&next_dir==3))  ctx->press_dir=last_dir;

    ctx->status = go_next_one(ctx);

    if (ctx->status != 1) {
        ctx->gstate = GAME_OVER;
    } else {
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
 * 沿给定方向向前走，走的步数为1步
 * 更新container和三个链表的状态，若吃到东西，撞到障碍物，碰到自身，走到空格
 * 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身，如果未结束，返回1，如果运行出错，返回0
 */
int go_next_one(GameContext *ctx)
{
  if (ctx->Snake== NULL)
  {
    return 0;
  }
  // 0.获取头元素坐标
  int head_i = (ctx->Snake)->data.i, head_j = (ctx->Snake)->data.j;
  // 1.获取下一位置坐标
  int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上0，下1，左2，右3,dir对应1-4，对应于此的0-3
  int dir;
  if(ctx->press_dir==0)
  {
    // 直行
    dir = ctx->newC[head_i][head_j].dir;
  }else{
    // 带方向
    dir=ctx->press_dir;
  }
  int next_i = head_i + directions[dir - 1][0];
  int next_j = head_j + directions[dir - 1][1];
  if (next_i >= LENTH_BOUNDARY || next_j >= WIDTH_BOUNDARY || next_i < 0 || next_j < 0)
    return 0;
  // 2.判断下一位置状态
  if (ctx->newC[next_i][next_j].type == obstacle)
  {
    // 2.1障碍物，结束
    return -1;
  }
  else if (ctx->newC[next_i][next_j].type == boundary)
  {
    // 2.2边界，结束
    return -2;
  }
  else if (ctx->newC[next_i][next_j].type == snake)
  {
    // 2.3咬到自身，结束
    return -3;
  }
  else if (ctx->newC[next_i][next_j].type == food)
  {
    // 2.4食物，向前走，并且添加新节点
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int  snapshotCnt = 0;
    snapshot_old_snake(ctx->Snake,snapshotPos,&snapshotCnt);
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
    dclist_delete(&ctx->Food,(Coordinate){next_i,next_j});
    // c.2.生成新的食物节点到Food并且更新container（因为Snake和Food不一样，Food不用考虑状态什么）
    add_one_food_and_update_container(&ctx->Food,ctx->newC,ctx->oldC,ctx->len,ctx->wid);

    // d.清空旧container
    clear_old_Snake(snapshotPos,snapshotCnt,ctx->newC,ctx->oldC);

    // e.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(ctx->Snake,ctx->newC,dir);
    return 1;
  }
  else
  {
    // 2.5 遇到空格，安全，向前走
    // a.拍快照，记录旧蛇的坐标，在更新container时传入，便于清理旧container
    Coordinate snapshotPos[MAX_SNAKE];
    int  snapshotCnt = 0;
    snapshot_old_snake(ctx->Snake,snapshotPos,&snapshotCnt);
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
    clear_old_Snake(snapshotPos,snapshotCnt,ctx->newC,ctx->oldC);

    // d.重新绘制Snake(此时旧蛇已经清空，只需要根据Snake链表重新绘画蛇即可)
    // 此处的dir是计算好的头节点的方向
    paint_new_Snake(ctx->Snake,ctx->newC,dir);
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
    renderContainer(ctx->win_game, ctx->newC, ctx->oldC, ctx->len, ctx->wid);
    // renderAllContainer(ctx->win_game, ctx->newC, ctx->len, ctx->wid);
    
    /* 渲染状态栏 */
    // 清空状态栏
    werase(ctx->win_status);
    // 绘制边框
    box(ctx->win_status, 0, 0);
    char msg[64]="";
    switch(ctx->gstate) {
        case READY:
            strcpy(msg,"Press SPACE to Start");
            break;
        case RUNNING:
            sprintf(msg,"Score: %ld  P:Pause Q:Quit", ctx->score);
            break;
        case PAUSED:
            strcpy(msg,"Paused. P:Resume Q:Quit");
            break;
        case GAME_OVER:
            sprintf(msg,"Game Over! Score:%ld R:Restart Q:Quit", ctx->score);
            break;
    }
    // 统一居中打印信息
    int max_y,max_x;
    getmaxyx(ctx->win_status,max_y,max_x);
    int x=(max_x-(int)strlen(msg))/2;
    mvwprintw(ctx->win_status,1,x<0?0:x,"%s",msg);

    // 刷新窗口
    wrefresh(ctx->win_status);

    // 绘制排行榜
    draw_board(ctx);
    return 1;
}

/*
 * 绘制排行榜
 * 绘制失败返回0，成功返回1
 */
int draw_board(GameContext *ctx){
    // 清空屏幕
    werase(ctx->win_board); 
    // 设置边框
    box(ctx->win_board,0,0);
    // 写入标题
    mvwprintw(ctx->win_board,1,2," LEADER BOARD ");
    // 加载排行榜数据
    ScoreEntry *arr; int n;
    if(!score_load(&arr,&n))
    { 
      mvwprintw(ctx->win_board,2,2,"(empty)"); 
      wrefresh(ctx->win_board); return 1; 
    }

    // char line[64]; 
    // 显示数据
    for(int i=0;i<n && i<10;i++){             /* 只显示前 10 */
        mvwprintw(ctx->win_board,3+i,2,"No%2d: %-5s %3ld",i+1,arr[i].name,arr[i].score);
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
