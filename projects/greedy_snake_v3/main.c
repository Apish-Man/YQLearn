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

/*------------------------常量定义------------------------------*/
// 定义图标显示
// 蛇头，对应上0，下1，左2，右3
const char snake_head_icon[4][9] = {" @ o oOOO", "OOOo o @ ", " oO@ O oO", "Oo O @Oo "};
// 蛇中，对应上0，下1，左2，右3
const char snake_middle_icon[8][9] = {"|0||0||0|", "|0||0||0|", "---OOO---", "---OOO---","|0-|0o---", "---|oo|0|", "---o0|-0|", "|0|o0|---"};
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
  curs_set(0);                // 0-隐藏，1-正常，2-高亮,关闭文本光标

  /*------------------------GameContext初始化------------------------------*/
  // 新建游戏结构体
  GameContext ctx;
  // 游戏结构体初始化
  gc_init(&ctx, INIT_X, INIT_Y);
  
  while (1)
  {
    int ch = getch();
    // 根据按键更新ctx
    gc_handle_input(&ctx,ch);
    // 游戏逻辑推进（上面是更新了ctx的按键及状态，未根据按键处理链表和container）
    gc_tick_logic(&ctx);
    // 根据ctx渲染数据
    gc_tick_render(&ctx);

    /*------------------------本次循环结束收尾------------------------------*/
    // 清理缓冲区，避免输入过多卡顿
    // flushinp(); // 丢弃所有未处理的输入

    // 控制刷新率（单位：微秒）
    usleep(500000); // 100ms-100000us刷新一次,1秒（s）等于1000毫秒（ms）,usleep()函数的单位是微秒（μs），1ms=1000微秒
  }

  // 销毁游戏
  gc_destroy(&ctx);
  endwin(); // 恢复终端原始模式并释放资源[5]
  return 0;
}
