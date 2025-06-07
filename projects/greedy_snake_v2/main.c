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
  // 在初始化部分添加
  // 创建容器,旧容器，保存旧状态
  Block container[LENTH_BOUNDARY][WIDTH_BOUNDARY], oldcontainer[LENTH_BOUNDARY][WIDTH_BOUNDARY];
  // 定义双向循环链表记录，蛇，食物，障碍物
  NODE *Snake = NULL;
  NODE *Food = NULL;
  NODE *Obstacle = NULL;
  // 游戏初始状态
  enum GAME_STATE gstate = READY;
  int status = 1;          // 保存 updateGame 返回值
  long score = 0;          // 简单用蛇长度计分

  /*------------------------容器初始化------------------------------*/
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

  /*------------------------窗口初始化------------------------------*/
  // 创建窗口
  WINDOW *win_game = newwin(LENTH_BOUNDARY * BLOCK_SIZE, WIDTH_BOUNDARY * BLOCK_SIZE, INIT_X, INIT_Y); // 创建新窗口：10行20列，起始坐标(5,5)[4]
  // 在初始化部分创建调试窗口
  WINDOW *win_debug = newwin(4, WIDTH_BOUNDARY * BLOCK_SIZE, LENTH_BOUNDARY * BLOCK_SIZE + 1, 0); // 位于主窗口下方
  // 游戏状态窗口
  WINDOW *win_status = newwin(3, WIDTH_BOUNDARY*BLOCK_SIZE, LENTH_BOUNDARY*BLOCK_SIZE+1, 0); 

  // 初始化游戏
  init_game(win_game, container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY, &Snake, &Food, &Obstacle);

  int ch;
  int press_dir; // 按键按下方向
  while (1)
  {
    /*------------------------初始化------------------------------*/
    ch = getch();
    // 调试输出区域
    werase(win_debug);    // 清空调试窗口
    box(win_debug, 0, 0); // 给调试窗口加边框

    // 解析按键并切换状态
    handle_input(ch, &gstate,&press_dir);

    /*------------------------调试------------------------------*/
    // 在调试窗口中显示链表状态
    // dclist_showInWindow(win_debug, Snake, "Snake");
    // dclist_showInWindow(win_debug, Food, "Food");
    // wrefresh(win_debug); // 必须单独刷新调试窗口
    
    /*------------------------业务逻辑------------------------------*/
    // 更新下一步游戏状态，同步更新容器内容
    if (gstate == RUNNING) {          // 仅在运行态更新逻辑
        status = updateGame(container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY, &Snake, &Food, &Obstacle, press_dir);
        // 如果游戏结束，返回-1障碍物，-2边界，-3咬到自身，如果未结束，返回1，如果运行出错，返回0
        if (status != 1) gstate = GAME_OVER;
        else score = dclist_len(Snake);   // 你可以直接用链表长度
    }
    
    /*------------------------渲染和更新窗口------------------------------*/
    renderContainer(win_game, container, oldcontainer, LENTH_BOUNDARY, WIDTH_BOUNDARY);
    wrefresh(win_game); // 刷新游戏窗口

    // 显示提示 / 分数
    show_ui(win_status, gstate, status, score); 

    /*------------------------本次循环结束收尾------------------------------*/
    // 清理缓冲区，避免输入过多卡顿
    flushinp(); // 丢弃所有未处理的输入

    // 控制刷新率（单位：微秒）
    usleep(500000); // 100ms-100000us刷新一次,1秒（s）等于1000毫秒（ms）,usleep()函数的单位是微秒（μs），1ms=1000微秒
  }

  // 销毁游戏
  destory_game(&Snake,&Food,&Obstacle);
  delwin(win_game); // 销毁窗口[4]
  endwin(); // 恢复终端原始模式并释放资源[5]
  return 0;
}
