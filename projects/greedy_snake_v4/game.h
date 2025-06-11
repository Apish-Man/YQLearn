/*************************************************************************
  > File Name:    game.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月10日 星期二 19时28分10秒
 ************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "game_context.h"
#include <unistd.h>

// 创建单机游戏
void create_single_player_game(GameContext *ctx, const char *player_name);

// 浏览排行榜
void game_view_board();

// 初始化ncurses
WINDOW* game_init_ncurses();

// 打印菜单
void game_printf_menu(WINDOW *menu_win,int win_h,int win_w,int margin,int left);

// 输入昵称
void game_get_nickname(WINDOW *stdscr,WINDOW *menu_win,int win_h,int win_w,int margin,int left,char *name);

// 创建房间
void game_create_room(GameContext *ctx, const char *name);

// 加入房间
void game_join_room(GameContext *ctx, const char *name);
#endif /* GAME_H */

