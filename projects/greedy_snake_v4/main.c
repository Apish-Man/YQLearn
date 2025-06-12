/*************************************************************************
  > File Name:    main.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月03日 星期二 20时21分34秒
 ************************************************************************/

#include "block.h"
#include <ncurses.h>
// #include <./ncurses/ncurses.h>
#include <unistd.h>
#include "game.h"
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
const char food_icon[4][9] = {"****1****", "****2****", "****3****", "****4****"};
// 障碍物，不定义方向
const char obstacle_icon[2][9] = {"---|!|---", "---|^|---"};
// 边界，对应上下0，左右1
const char boundary_icon[2][9] = {"=========", "|||||||||"};

int main(int argc, char *argv[])
{
  WINDOW *stdscr;
  /*------------------------ncurses初始化------------------------------*/
  srand(time(NULL));
  // 初始化ncurses函数
  stdscr=game_init_ncurses();

  while (1)
  {
    /* —— 计算并创建一个居中的菜单窗口 —— */
    WINDOW *menu_win;
    const int win_h = 14, win_w = 38; /* 可以根据喜好调整尺寸 */
    const int margin = 2; /* 靠左缩进 2 格，看着不贴边 */
    int left = 3;         /* 行起始（列用 margin） */

    menu_win=game_printf_menu(win_h,win_w,margin,left);

    int ch = getch();
    switch (ch)
    {
    case '1':
    {
      /* 读取昵称 */
      char name[NAME_MAXLEN] = {0};

      // 输入昵称
      game_get_nickname(stdscr,menu_win,win_h,win_w,margin,left,name);

      /* 初始化 GameContext */
      GameContext ctx;
      create_single_player_game(&ctx, name);
    }
    break;
    case '2': /* TODO 创建房间 */
    {
      /* 读取昵称 */
      char name0[NAME_MAXLEN] = {0};

      // 输入昵称
      game_get_nickname(stdscr,menu_win,win_h,win_w,margin,left,name0);

      // 双人游戏GameContext
      GameContext ctx;
      ctx.win_menu=menu_win;
      game_create_room(&ctx,name0);
    }
    break;
    case '3': /* TODO 加入房间 */
    {
      /* 读取昵称 */
      char name2[NAME_MAXLEN] = {0};

      // 输入昵称
      game_get_nickname(stdscr,menu_win,win_h,win_w,margin,left,name2);

      /* 读取op */
      char ip[32] = {0};

      // 输入昵称
      game_get_ip(stdscr,menu_win,win_h,win_w,margin,left,ip);

      // 双人游戏GameContext
      GameContext ctx;
      ctx.win_menu=menu_win;
      game_join_room(&ctx,name2,ip);
    }
      break;
    case '4':
    { 
      game_view_board();
      while ((ch = getch()) != 'q'); /* 等待返回 */
      clear();//清空界面
    }
    break;
    case '5':
      endwin();
      exit(0);
    // default:
    //   break;
    }
    delwin(menu_win); /* 把 menu_win 释放掉，避免内存泄漏 */
  }
  endwin(); // 恢复终端原始模式并释放资源[5]
  return 0;
}
