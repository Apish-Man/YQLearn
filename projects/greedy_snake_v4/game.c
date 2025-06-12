/*************************************************************************
  > File Name:    game.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月10日 星期二 19时28分01秒
 ************************************************************************/

#include "game.h"
#include <ctype.h> /* isspace */
#include <string.h>
#include <ncurses.h>

// 创建单机游戏
void create_single_player_game(GameContext *ctx, const char *name)
{
  ctx->mp = 0;
  gc_init(ctx, INIT_X, INIT_Y);
  strncpy(ctx->player_name, name, NAME_MAXLEN - 1);

  /* -------- 单人游戏主循环 -------- */
  while (1)
  {
    int c = getch();         /* 非阻塞读键 */
    gc_handle_input(ctx, c); /* 所有按键先交给 ctx 处理 */

    /* 不管是什么状态，点击q都会返回主菜单 */
    if (c == 'q' || c == 'Q')
    {
      break; /* 跳出 while，回到主菜单 */
    }

    /* -------- 游戏已结束？ -------- */
    if (ctx->gstate == GAME_OVER)
    {
      /* r/R 已在 gc_handle_input 里做了 gc_reset()，直接继续 */
      if (c == 'r' || c == 'R')
        continue;

      /* q 返回主菜单 */
      if (c == 'q' || c == 'Q')
      {
        break; /* 跳出 while，回到主菜单 */
      }

      /* 没敲键就留在 Game-Over 画面 */
      gc_tick_render(ctx);
      usleep(50000);
      continue;
    }

    /* -------- 正常推进游戏 -------- */
    if (ctx->gstate == RUNNING || ctx->gstate == READY)
      gc_tick_logic(ctx);
    gc_tick_render(ctx);
    usleep(500000); /* 50 ms 帧间隔≈20 fps */
  }

  /* 记录成绩并销毁 */
  score_append(ctx->player_name, ctx->score); /* 记分 */
  gc_destroy(ctx);                            /* 关闭窗口/链表 */
  clear();                                    /* 清掉残余画面 */
  refresh();                                  /* 避免菜单与旧界面重叠 */
}

// 浏览排行榜
void game_view_board()
{
  /* 只浏览排行榜 */
  ScoreEntry *arr;
  int n;
  score_load(&arr, &n);
  clear();

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  box(stdscr, 0, 0);
  center_print(stdscr, 1, "Leaderboard (q returned)");

  int col_w = 22; // 一行宽度
  int start_x = (max_x - col_w) / 2;
  char line[64];

  for (int i = 0; i < n && i < 20; i++)
  {
    snprintf(line, sizeof(line), "NO%2d. %-12s %5ld", i + 1, arr[i].name, arr[i].score);
    center_print(stdscr, 3 + i, line); /* 横向自动居中 */
  }
  refresh();
  free(arr);
}

// 初始化ncurses
WINDOW *game_init_ncurses()
{
  WINDOW *stdscr = initscr(); // 初始化终端，返回标准窗口指针[5][4]
  cbreak();                   // 禁用行缓冲，直接读取输入[4]
  noecho();                   // 关闭输入字符回显[4]
  keypad(stdscr, TRUE);       // 启用功能键（如方向键）识别[4]
  nodelay(stdscr, TRUE);      // 启用非阻塞输入[4]
  start_color();              // 初始化颜色支持[4]

  // 初始化颜色对
  init_pair(1, COLOR_RED, COLOR_BLACK);    // 第一条蛇 - 鲜红色
  init_pair(2, COLOR_CYAN, COLOR_BLACK);   // 第二条蛇 - 青色
  init_pair(3, COLOR_YELLOW, COLOR_BLACK); // 边界 - 黄色
  init_pair(4, COLOR_GREEN, COLOR_BLACK);  // 食物 - 绿色
  init_pair(5, COLOR_RED, COLOR_BLACK);    // 障碍物 - 红色
  init_pair(6, COLOR_CYAN, COLOR_BLACK);   // 菜单标题
  init_pair(7, COLOR_WHITE, COLOR_BLACK);  // 菜单选项
  init_pair(8, COLOR_CYAN, COLOR_BLACK);   // 输入提示

  curs_set(0); // 0-隐藏，1-正常，2-高亮,关闭文本光标
  return stdscr;
}

// 打印菜单
WINDOW *game_printf_menu(int win_h, int win_w, int margin, int left)
{
  int rows, cols;
  getmaxyx(stdscr, rows, cols);

  // 确保窗口大小有效
  if (rows <= 0 || cols <= 0)
  {
    rows = 14; // 默认值
    cols = 38; // 默认值
  }

  int win_y = (rows - win_h) / 2;
  int win_x = (cols - win_w) / 2;

  // 确保窗口位置有效
  if (win_y < 0)
    win_y = 0;
  if (win_x < 0)
    win_x = 0;

  WINDOW *menu_win = newwin(win_h, win_w, win_y, win_x);
  box(menu_win, 0, 0);

  /* —— 标题 —— */
  wattron(menu_win, COLOR_PAIR(6) | A_BOLD);
  center_print(menu_win, 1, "=== Snake Game Menu ===");
  wattroff(menu_win, COLOR_PAIR(6) | A_BOLD);

  /* —— 菜单项 —— */
  wattron(menu_win, COLOR_PAIR(7));
  mvwprintw(menu_win, left + 0, margin, "1. single-player game");
  mvwprintw(menu_win, left + 1, margin, "2. Create a room");
  mvwprintw(menu_win, left + 2, margin, "3. Join a room");
  mvwprintw(menu_win, left + 3, margin, "4. View the Leaderboard");
  mvwprintw(menu_win, left + 4, margin, "5. Quit");
  wattroff(menu_win, COLOR_PAIR(7));

  /* —— "Select please:" 提示 —— */
  wattron(menu_win, COLOR_PAIR(8));
  mvwprintw(menu_win, left + 6, margin, "Select please:");
  wattroff(menu_win, COLOR_PAIR(8));

  wrefresh(menu_win);
  return menu_win;
}

// 输入昵称
void game_get_nickname(WINDOW *stdscr, WINDOW *menu_win, int win_h, int win_w, int margin, int left, char *name)
{
  echo();                 /* 开回显 */
  curs_set(1);            /* 显示光标 */
  nodelay(stdscr, FALSE); /* 关键！改回阻塞模式 */

  mvwprintw(menu_win, win_h - 3, margin, "please input your nickname!");
  mvwprintw(menu_win, win_h - 2, margin, "Nickname:");

  int input_row = win_h - 2;
  int input_col = margin + (int)strlen("Nickname:") + 1;

  flushinp();
  // 阻塞输入正确的nickname
  while (1)
  {
    /* 清掉旧输入区域，光标移到输入位置 */
    mvwprintw(menu_win, input_row, input_col, "%-*s", NAME_MAXLEN - 1, "");
    wmove(menu_win, input_row, input_col);
    wrefresh(menu_win);

    /* 阻塞读取 */
    if (wgetnstr(menu_win, name, NAME_MAXLEN - 1) == ERR)
      continue; /* 读取失败就再来一次 */

    /* ——— 去掉前后空白，判断是否为空串 ——— */
    char *p = name;
    while (*p && isspace((unsigned char)*p))
      p++;                           /* 去前导空白 */
    memmove(name, p, strlen(p) + 1); /* 左移到首部 */

    size_t len = strlen(name);
    while (len && isspace((unsigned char)name[len - 1]))
      name[--len] = '\0'; /* 去尾部空白 */

    if (len == 0)
    { /* 还是空？再提示 */
      wattron(menu_win, COLOR_PAIR(2));
      mvwprintw(menu_win, input_row - 1, margin, "Nickname cannot be empty!");
      wattroff(menu_win, COLOR_PAIR(2));
      wrefresh(menu_win);
      napms(900);                                                                /* 留 0.9 秒给用户看提示 */
      mvwprintw(menu_win, input_row - 1, margin, "%*s", win_w - 2 * margin, ""); /* 清提示 */
      continue;                                                                  /* 回到 while 顶部重新输 */
    }
    break; /* 输入有效，退出循环 */
  }

  // 恢复状态
  nodelay(stdscr, TRUE); /* 继续非阻塞 */
  noecho();
  curs_set(0);
}

/* 辅助函数：验证IPv4格式 */
int is_valid_ip(const char *str)
{
  unsigned char octet[4];
  return inet_pton(AF_INET, str, octet) == 1;
};

// 输入IP地址并验证格式
void game_get_ip(WINDOW *stdscr, WINDOW *menu_win, int win_h, int win_w, int margin, int left, char *ip)
{
  echo();                 /* 开回显 */
  curs_set(1);            /* 显示光标 */
  nodelay(stdscr, FALSE); /* 改为阻塞模式 */

  mvwprintw(menu_win, win_h - 3, margin, "Please input server IP:     ");
  mvwprintw(menu_win, win_h - 2, margin, "IP:      ");

  const int input_row = win_h - 2;
  const int input_col = margin + (int)strlen("IP:      ") + 1;
  const int max_ip_length = 15; // IPv4最大长度: 15字节（包含结尾空字符）

  while (1)
  {
    /* 清空旧输入 */
    mvwprintw(menu_win, input_row, input_col, "%-*s", max_ip_length, "");
    wmove(menu_win, input_row, input_col);
    wrefresh(menu_win);

    /* 阻塞读取输入 */
    int ret = wgetnstr(menu_win, ip, max_ip_length - 1); // 预留1字节给空字符
    if (ret == ERR)
      continue;

    /* 清理输入 */
    char *p = ip;
    // 跳过前导空白
    while (*p && isspace((unsigned char)*p))
      p++;
    memmove(ip, p, strlen(p) + 1);
    // 去除尾部空白
    size_t len = strlen(ip);
    while (len > 0 && isspace((unsigned char)ip[len - 1]))
      ip[--len] = '\0';

    /* 验证逻辑 */
    if (len == 0)
    {
      mvwprintw(menu_win, input_row - 1, margin, "IP cannot be empty!");
    }
    else if (len > 15)
    {
      mvwprintw(menu_win, input_row - 1, margin, "IP too long! (max 15 chars)");
    }
    else if (!is_valid_ip(ip))
    {
      mvwprintw(menu_win, input_row - 1, margin, "Invalid IPv4 format!");
    }
    else
    {
      break; // 验证通过
    }

    // 显示错误提示
    wattr_on(menu_win, COLOR_PAIR(3), NULL);
    wrefresh(menu_win);
    napms(1200); // 显示1.2秒

    // 清除错误信息
    mvwprintw(menu_win, input_row - 1, margin, "%*s", win_w - 2 * margin, "");
  }

  /* 恢复状态 */
  nodelay(stdscr, TRUE); /* 继续非阻塞 */
  noecho();
  curs_set(0);
}

/* 头部工具函数 —— 获取当前时间（微秒） */
static inline long long now_us(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

// 创建房间
void game_create_room(GameContext *ctx, const char *name)
{
  // 1.昵称
  strncpy(ctx->player_name, name, NAME_MAXLEN - 1);
  // mp为1表示联机，isServer为真表示服务器
  ctx->mp=1; ctx->isServer=1;
  gc_init(ctx, INIT_X, INIT_Y);
  strncpy(ctx->player_name, name, NAME_MAXLEN - 1);
  ctx->last_step_us=now_us();

  // 2.socket,bind
  // listen_fd为监听socket，cli_fd为客户端socket
  int listen_fd, cli_fd;
  // 3.阻塞状态下等待连接
  if(net_init_server(&listen_fd,&cli_fd)<0)
  { 
    // 连接失败
    perror("net_init_server"); 
    return;  
  }

  //  接收到的客户端 fcli_fd
  ctx->cli_fd = cli_fd;
  net_set_nonblock(cli_fd);

  // 接收客户端昵称
  MPNamePkt npkt;
  if (net_recv(cli_fd, &npkt, sizeof(npkt)) != sizeof(npkt))
  {
    perror("Failed to receive client nickname");
    return;
  }
  strncpy(ctx->player_name2, npkt.player_name, NAME_MAXLEN - 1);
  ctx->player_name2[NAME_MAXLEN - 1] = '\0';

  // 3.连接成功后发送初始数据，尝试10次
  int retry = 0;
  while(retry < 10) {
    MPStatePkt pkt;
    gc_pack_state(ctx,&pkt);
    if(net_send(cli_fd,&pkt,sizeof(pkt)) == sizeof(pkt)) {
      // 发送成功
      break;
    }
    retry++;
  }
  if(retry == 10) {
    perror("Failed to send initial state to server");
    return;
  }

  // 进入前先清屏
  werase(ctx->win_menu); wrefresh(ctx->win_menu);delwin(ctx->win_menu);clear(); refresh();

  bool sent_final_pkt = false; /* 只在 Game-Over 第一次进入时发送 */
  // 4.游戏主循环
  while(1){
        /* —— ① 采集本地输入 —— */
        int c = getch();
        if (c=='q'||c=='Q') break;
        gc_handle_input(ctx, c);           /* 把本地按键写进 ctx->press_dir */

        /* —— ② 收集远端输入（非阻塞） —— */
        MPKeyPkt k;
        if (net_recv(cli_fd,&k,sizeof(k)) == sizeof(k))
            gc_handle_remote_input(ctx, k.key);

        // 只在游戏未结束或者，未发送最后一帧的情况下推进游戏逻辑
        if(ctx->status==1||!sent_final_pkt){
            /* —— ③ 推进游戏逻辑（立即使用刚收到的两个方向） —— */
            // gc_tick_logic_double(ctx);
            long long t = now_us();
            if (t - ctx->last_step_us >= SNAKE_STEP_MSEC * 1000) {
                gc_tick_logic_double(ctx);     /* 真正移动蛇 */
                ctx->last_step_us = t;
            }
        }

        /* —— ④ 将最新状态广播给客户端 —— */
        MPStatePkt pkt;
        gc_pack_state(ctx,&pkt);
        net_send(cli_fd,&pkt,sizeof(pkt));     /* 失败也无所谓，下一帧重发 */


        /* —— ⑤ 渲染（最后才画，这一帧就能看到最新结果） —— */
        gc_tick_render(ctx);

        /* —— ⑥ 固定帧率 —— */
        net_sleep_frame();                     /* 相当于 usleep(FRAME_USEC) */
  }
  // 销毁game_context
  gc_destroy(ctx);
  // 关闭socket
  close(cli_fd); close(listen_fd);
}


// 加入房间
void game_join_room(GameContext *ctx, const char *name,const char *ip)
{
     // 1.昵称
     strncpy(ctx->player_name, name, NAME_MAXLEN - 1);
     // mp为1表示联机，isServer为假表示客户端
     ctx->mp=1; ctx->isServer=0;
     
     // 2.连接服务器
     int sock_fd; 
     if(net_init_client(&sock_fd,ip)<0)
     { 
      // 初始化失败
      perror("net_init_client");
      return;
     }
     net_set_nonblock(sock_fd);
     ctx->sock_fd = sock_fd;

    //  发送昵称给服务器
    MPNamePkt mpkt;
    strncpy(mpkt.player_name, name, NAME_MAXLEN - 1);
    mpkt.player_name[NAME_MAXLEN-1] = '\0';
    if (net_send(sock_fd, &mpkt, sizeof(mpkt)) != sizeof(mpkt))
    {
      perror("Failed to send nickname to server");
      return;
    }

     // 3.等待服务器的初始状态，不再需要首次数据传输
     gc_init_client2(ctx,INIT_X,INIT_Y);
     ctx->last_step_us = now_us();  // 初始化时间戳

     // 进行一次初始化全渲染
     renderAllContainer(ctx->win_game,ctx->newC,ctx->len,ctx->wid);

     // 进入前先清屏
     werase(ctx->win_menu); wrefresh(ctx->win_menu);delwin(ctx->win_menu);clear(); refresh();

     // 5.游戏主循环
     bool in_game_over = false;//标志游戏是否结束
     while(1){
        /* ① 接收最新状态包（非阻塞；若没收到就用上一帧状态） */
        MPStatePkt pkt;
        if (net_recv(sock_fd,&pkt,sizeof(pkt)) == sizeof(pkt))
            gc_unpack_state(ctx,&pkt);

        /* ② 渲染 — 现在看到的一定是服务器刚算完的那一帧 */
        gc_tick_render(ctx);

        /* ③ 判断是否读取本地按键并发给服务器 */
        long long t = now_us();
        if (t - ctx->last_step_us >= SNAKE_STEP_MSEC * 1000) {
            int c = getch();
            if (c=='q'||c=='Q') break;
            MPKeyPkt k = { c };
            net_send(sock_fd,&k,sizeof(k));
        }

        /* ④ 定帧 */
        net_sleep_frame();
     }
     gc_destroy(ctx); 
     close(sock_fd);
}
