/*************************************************************************
  > File Name:    main.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月03日 星期二 20时21分34秒
 ************************************************************************/

#include "block.h"
#include <ncurses.h>
#include <unistd.h>
#include "game_context.h"
#include <ctype.h> //判断是否空白

/*------------------------常量定义------------------------------*/
// 定义图标显示
// 蛇头，对应上0，下1，左2，右3
const char snake_head_icon[4][9] = {" @ o oOOO", "OOOo o @ ", " oO@ O oO", "Oo O @Oo "};
// 蛇中，对应上0，下1，左2，右3
const char snake_middle_icon[8][9] = {"|0||0||0|", "|0||0||0|", "---OOO---", "---OOO---", "|0-|0o---", "---|oo|0|", "---o0|-0|", "|0|o0|---"};
// 蛇尾，对应上0，下1，左2，右3
const char snake_tail_icon[4][9] = {"$ $$ $ $ ", " $ $ $$ $", "$$   $$$ ", " $$$   $$"};
// 食物，对应上0，下1，左2，右3
const char food_icon[NUM_FOOD][9] = {"****1****", "****2****", "****3****", "****4****"};
// 障碍物，不定义方向
const char obstacle_icon[NUM_OBSTACLE][9] = {"---|!|---", "---|^|---"};
// 边界，对应上下0，左右1
const char boundary_icon[2][9] = {"=========", "|||||||||"};

int main(int argc, char *argv[])
{
  /*------------------------ncurses初始化------------------------------*/
  srand(time(NULL));
  // 初始化ncurses函数
  WINDOW *stdscr = initscr(); // 初始化终端，返回标准窗口指针[5][4]
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

  while (1)
  {
    /* —— 计算并创建一个居中的菜单窗口 —— */
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    const int win_h = 14, win_w = 38; /* 可以根据喜好调整尺寸 */
    int win_y = (rows - win_h) / 2;
    int win_x = (cols - win_w) / 2;
    WINDOW *menu_win = newwin(win_h, win_w, win_y, win_x);
    box(menu_win, 0, 0);

    /* —— 标题 —— */
    wattron(menu_win, COLOR_PAIR(1) | A_BOLD);
    center_print(menu_win, 1, "=== Snake Game Menu ===");
    wattroff(menu_win, COLOR_PAIR(1) | A_BOLD);

    /* —— 菜单项 —— */
    int left = 3;         /* 行起始（列用 margin） */
    const int margin = 2; /* 靠左缩进 2 格，看着不贴边 */
    mvwprintw(menu_win, left + 0, margin, "1. single-player game");
    mvwprintw(menu_win, left + 1, margin, "2. Create a room");
    mvwprintw(menu_win, left + 2, margin, "3. Join a room");
    mvwprintw(menu_win, left + 3, margin, "4. View the Leaderboard");
    mvwprintw(menu_win, left + 4, margin, "5. Quit");

    /* —— “Select please:” 提示 —— */
    wattron(menu_win, COLOR_PAIR(2));
    mvwprintw(menu_win, left + 6, margin, "Select please:");
    wattroff(menu_win, COLOR_PAIR(2));

    wrefresh(menu_win);

    int ch = getch();
    switch (ch)
    {
    case '1':
    {
      /* 读取昵称 */
      echo();                 /* 开回显 */
      curs_set(1);            /* 显示光标 */
      nodelay(stdscr, FALSE); /* 关键！改回阻塞模式 */

      char name[NAME_MAXLEN] = {0};
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

      /* 初始化 GameContext */
      GameContext ctx;
      gc_init(&ctx, INIT_X, INIT_Y);
      strncpy(ctx.player_name, name, NAME_MAXLEN - 1);

      /* -------- 单人游戏主循环 -------- */
      while (1)
      {
        int c = getch();          /* 非阻塞读键 */
        gc_handle_input(&ctx, c); /* 所有按键先交给 ctx 处理 */

        /* 不管是什么状态，点击q都会返回主菜单 */
        if (c == 'q' || c == 'Q')
        {
          break; /* 跳出 while，回到主菜单 */
        }

        /* -------- 游戏已结束？ -------- */
        if (ctx.gstate == GAME_OVER)
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
          gc_tick_render(&ctx);
          usleep(50000);
          continue;
        }

        /* -------- 正常推进游戏 -------- */
        if (ctx.gstate == RUNNING || ctx.gstate == READY)
          gc_tick_logic(&ctx);
        gc_tick_render(&ctx);
        usleep(500000); /* 50 ms 帧间隔≈20 fps */
      }

      /* 记录成绩并销毁 */
      score_append(ctx.player_name, ctx.score); /* 记分 */
      gc_destroy(&ctx);                         /* 关闭窗口/链表 */
      clear();                                  /* 清掉残余画面 */
      refresh();                                /* 避免菜单与旧界面重叠 */
    }
    break;
    case '2': /* TODO 创建房间 */
      break;
    case '3': /* TODO 加入房间 */
      break;
    case '4':
    { /* 只浏览排行榜 */
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
      while ((ch = getch()) != 'q')
        ; /* 等待返回 */
      clear();//清空界面
    }
    break;

    case '5':
      endwin();
      exit(0);
    default:
      break;
    }
    delwin(menu_win); /* 把 menu_win 释放掉，避免内存泄漏 */
  }
  endwin(); // 恢复终端原始模式并释放资源[5]
  return 0;
}
