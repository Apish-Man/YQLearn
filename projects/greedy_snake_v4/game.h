/*************************************************************************
  > File Name:    game.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月10日 星期二 19时28分10秒
 ************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <unistd.h>
#include "game_context.h"
#include "mynet.h"
#include <sys/time.h>

// 创建单机游戏
extern void create_single_player_game(GameContext *ctx, const char *player_name);

// 浏览排行榜
extern void game_view_board();

// 初始化ncurses
extern WINDOW* game_init_ncurses();

// 打印菜单
extern WINDOW* game_printf_menu(int win_h,int win_w,int margin,int left);

// 输入昵称
extern void game_get_nickname(WINDOW *stdscr,WINDOW *menu_win,int win_h,int win_w,int margin,int left,char *name);

// 创建房间
extern void game_create_room(GameContext *ctx, const char *name);

// 加入房间
extern void game_join_room(GameContext *ctx, const char *name,const char *ip);

// 输入IP地址并验证格式
extern void game_get_ip(WINDOW *stdscr, WINDOW *menu_win, int win_h, int win_w, int margin, int left, char *ip);

#endif /* GAME_H */

