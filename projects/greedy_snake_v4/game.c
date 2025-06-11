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
#include "net.h"

// 创建单机游戏
void create_single_player_game(GameContext *ctx, const char *name)
{
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
WINDOW* game_init_ncurses()
{
  stdscr = initscr(); // 初始化终端，返回标准窗口指针[5][4]
  cbreak();                   // 禁用行缓冲，直接读取输入[4]
  noecho();                   // 关闭输入字符回显[4]
  keypad(stdscr, TRUE);       // 启用功能键（如方向键）识别[4]
  nodelay(stdscr, TRUE);      // 启用非阻塞输入[4]
  start_color();              // 初始化颜色支持[4]
  // 调整颜色
  init_pair(1, COLOR_YELLOW, COLOR_BLACK); // 菜单标题
  init_pair(2, COLOR_WHITE, COLOR_BLACK);  // 菜单选项
  init_pair(3, COLOR_CYAN, COLOR_BLACK);   // 输入提示
  curs_set(0);                             // 0-隐藏，1-正常，2-高亮,关闭文本光标
  return stdscr;
}

// 打印菜单
void game_printf_menu(WINDOW *menu_win,int win_h,int win_w,int margin,int left)
{
  int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int win_y = (rows - win_h) / 2;
    int win_x = (cols - win_w) / 2;
    menu_win = newwin(win_h, win_w, win_y, win_x);
    box(menu_win, 0, 0);

    /* —— 标题 —— */
    wattron(menu_win, COLOR_PAIR(1) | A_BOLD);
    center_print(menu_win, 1, "=== Snake Game Menu ===");
    wattroff(menu_win, COLOR_PAIR(1) | A_BOLD);

    /* —— 菜单项 —— */
    mvwprintw(menu_win, left + 0, margin, "1. single-player game");
    mvwprintw(menu_win, left + 1, margin, "2. Create a room");
    mvwprintw(menu_win, left + 2, margin, "3. Join a room");
    mvwprintw(menu_win, left + 1, margin, "4. View the Leaderboard");
    mvwprintw(menu_win, left + 2, margin, "5. Quit");

    /* —— “Select please:” 提示 —— */
    wattron(menu_win, COLOR_PAIR(2));
    mvwprintw(menu_win, left + 6, margin, "Select please:");
    wattroff(menu_win, COLOR_PAIR(2));

    wrefresh(menu_win);
}

// 输入昵称
void game_get_nickname(WINDOW *stdscr,WINDOW *menu_win,int win_h,int win_w,int margin,int left,char *name)
{
      echo();                 /* 开回显 */
      curs_set(1);            /* 显示光标 */
      nodelay(stdscr, FALSE); /* 关键！改回阻塞模式 */

      mvwprintw(menu_win, win_h - 3, margin, "please input your nickname!");
      mvwprintw(menu_win, win_h - 2, margin, "Nickname:");

      int input_row = win_h - 2;
      int input_col = margin + (int)strlen("Nickname:") + 1;

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

// 创建房间
void game_create_room(GameContext *ctx, const char *name)
{
    gc_init(ctx,INIT_X,INIT_Y);
    // mp为1表示联机，
     ctx->mp=1; ctx->isServer=1;

     int listen_fd, cli_fd;
     if(net_init_server(&listen_fd,&cli_fd)<0){ perror("net_init_server"); return;  }
    //  接收到的客户端 fcli_fd
     ctx->cli_fd = cli_fd;
     net_set_nonblock(cli_fd);

     while(1){
         /* 1. 处理本机输入（玩家1）*/
         int c=getch(); gc_handle_input(ctx,c);

         /* 2. 读远端按键 */
         MPKeyPkt k;
         if(net_recv(cli_fd,&k,sizeof(k))==sizeof(k))
             gc_handle_remote_input(ctx,k.key);

         /* 3. 推进两条蛇 */
         gc_tick_logic(ctx);

         /* 4. 发最新状态 */
         MPStatePkt pkt; gc_pack_state(ctx,&pkt);
         net_send(cli_fd,&pkt,sizeof(pkt));

         /* 5. 渲染 + 帧限速 */
         gc_tick_render(ctx); net_sleep_frame();

         if(ctx->gstate==GAME_OVER) break;
     }
     gc_destroy(ctx);
     close(cli_fd); close(listen_fd);
}

// 加入房间
void game_join_room(GameContext *ctx, const char *name)
{
     char ip[32]="";  /* 提示用户输入 Host IP，略 */
     int sock_fd; 
     if(net_init_client(&sock_fd,ip)<0)
     { 
      // 初始化失败
      return;
     }
     net_set_nonblock(sock_fd);

     gc_init(ctx,INIT_X,INIT_Y);
     ctx->mp=1; ctx->isServer=0; ctx->sock_fd=sock_fd;

     while(1){
         /* 1. 把按键发给服务器 */
         int c=getch();
         if(c!=ERR){ MPKeyPkt k={c}; net_send(sock_fd,&k,sizeof(k)); }

         /* 2. 尝试接收状态包 */
         MPStatePkt pkt;
         if(net_recv(sock_fd,&pkt,sizeof(pkt))==sizeof(pkt)){
             gc_unpack_state(ctx,&pkt);
         }

         /* 3. 渲染本地画面 */
         gc_tick_render(ctx); net_sleep_frame();

         if(ctx->gstate==GAME_OVER) break;
     }
     gc_destroy(&ctx); close(sock_fd);
}

