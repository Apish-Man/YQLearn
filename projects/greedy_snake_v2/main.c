/*************************************************************************
  > File Name:    main.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年06月03日 星期二 20时21分34秒
 ************************************************************************/

#include "block.h"
#include <ncurses.h>
#include <unistd.h>
#include "game.h"

#define INIT_X 0
#define INIT_Y 0


// 定义图标显示
// 蛇头，对应上0，下1，左2，右3
const char snake_head_icon[4][9] = {" @ o oOOO", "OOOo o @ ", " oO@ O oO", "Oo O @Oo "};
// 蛇中，对应上0，下1，左2，右3
const char snake_middle_icon[4][9] = {"|0||0||0|", "|0||0||0|", "---OOO---", "---OOO---"};
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
  srand(time(NULL));
  // 初始化ncurses函数
  WINDOW *stdscr = initscr(); // 初始化终端，返回标准窗口指针[5][4]
  cbreak();                   // 禁用行缓冲，直接读取输入[4]
  noecho();                   // 关闭输入字符回显[4]
  keypad(stdscr, TRUE);       // 启用功能键（如方向键）识别[4]
  nodelay(stdscr, TRUE);      // 启用非阻塞输入[4]
  start_color();              // 初始化颜色支持[4]
  curs_set(0);                // 0-隐藏，1-正常，2-高亮,关闭文本光标
  // 在初始化部分添加

  // 创建容器,旧容器，保存旧状态
  Block container[LENTH_BOUNDARY][WIDTH_BOUNDARY], oldcontainer[LENTH_BOUNDARY][WIDTH_BOUNDARY];

  // 初始化容器
  if (!initContainer(container, LENTH_BOUNDARY, WIDTH_BOUNDARY, INIT_X, INIT_Y))
  {
    printf("初始化失败\n");
    return -1;
  }
  if (!initEmptyContainer(oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY, INIT_X, INIT_Y))
  {
    printf("初始化失败\n");
    return -1;
  }

  // 创建窗口
  WINDOW *win_game = newwin(LENTH_BOUNDARY * BLOCK_SIZE, WIDTH_BOUNDARY * BLOCK_SIZE, INIT_X, INIT_Y); // 创建新窗口：10行20列，起始坐标(5,5)[4]
  // 在初始化部分创建调试窗口
  WINDOW *win_debug = newwin(4, WIDTH_BOUNDARY * BLOCK_SIZE, LENTH_BOUNDARY * BLOCK_SIZE + 1, 0); // 位于主窗口下方

  // 定义双向循环链表记录，蛇，食物，障碍物
  NODE *Snake = NULL;
  NODE *Food = NULL;
  NODE *Obstacle = NULL;

  // 初始化游戏
  init_game(win_game, container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY, &Snake, &Food, &Obstacle);

  // 初始化页面
  // renderContainer(win_game, container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY);

  int press_dir; // 按键按下方向

  int ch;
  while (1)
  {
    ch = getch();

    // 调试输出区域
    werase(win_debug);    // 清空调试窗口
    box(win_debug, 0, 0); // 给调试窗口加边框
    // mvwprintw(win_debug, 1, 1, "Last Key: %d (0x%X)", ch, ch); // 显示十进制和十六进制值
    // 添加特殊键识别
    const char *keyname = "Unknown";

    // 处理输入
    switch (ch)
    {
    case 'W':
    case 'w':
      press_dir = up;
      keyname = "up";
      mvwaddch(win_game, LENTH_BOUNDARY / 2, WIDTH_BOUNDARY / 2, ch); // 输出到窗口中心
      break;
    case 'S':
    case 's':
      press_dir = down;
      keyname = "down";
      mvwaddch(win_game, LENTH_BOUNDARY / 2, WIDTH_BOUNDARY / 2, ch); // 输出到窗口中心
      break;
    case 'A':
    case 'a':
      press_dir = left;
      keyname = "left";
      mvwaddch(win_game, LENTH_BOUNDARY / 2, WIDTH_BOUNDARY / 2, ch); // 输出到窗口中心
      break;
    case 'D':
    case 'd':
      press_dir = right;
      keyname = "right";
      mvwaddch(win_game, LENTH_BOUNDARY / 2, WIDTH_BOUNDARY / 2, ch); // 输出到窗口中心
      break;
    case 'q':
      delwin(win_game); // 销毁窗口[4]
      endwin();         // 恢复终端原始模式并释放资源[5]
      return 1;         // 退出游戏
    case ERR:
      keyname = "No Input";
      press_dir = none;
      break;
    default:
      press_dir = none;
      keyname = "others Input"; // 可打印字符直接显示
      break;
    }
    // 调试窗口
    // mvwprintw(win_debug, 2, 1, "Decode: %s", keyname);

    // 在调试窗口中显示链表状态
    dclist_showInWindow(win_debug, Snake, "Snake");
    wrefresh(win_debug); // 必须单独刷新调试窗口
    
    // 更新容器内容（业务逻辑）
    updateContainer(container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY, &Snake, &Food, &Obstacle, press_dir);
    // 渲染
    renderContainer(win_game, container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY);

    wrefresh(win_game); // 刷新游戏窗口

    // 清理缓冲区，避免输入过多卡顿
    // 在每次循环末尾清空输入缓冲区
    flushinp(); // 丢弃所有未处理的输入

    // 控制刷新率（单位：微秒）
    usleep(500000); // 100ms-100000us刷新一次,1秒（s）等于1000毫秒（ms）,usleep()函数的单位是微秒（μs），1ms=1000微秒
  }
  delwin(win_game); // 销毁窗口[4]

  // 在main.c的退出前添加
  dclist_destroy(&Snake);
  dclist_destroy(&Food);
  dclist_destroy(&Obstacle);
  endwin(); // 恢复终端原始模式并释放资源[5]
  return 0;
}
