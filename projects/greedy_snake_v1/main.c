/*************************************************************************
  > File Name:    main.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月03日 星期二 20时21分34秒
 ************************************************************************/

#include <stdio.h>
#include "snake.h"


#define LEN_FOOD 3 //食物种类
#define NUM_INIT_FOOD 10 //初始化食物数量

#define NUM_OBSTACLE 20//初始化障碍物个数

int directions[4][2]={{0,-1},{-1,0},{0,1},{1,0}};//上，左，下，右，逆时针转
int vis[LENTH_BOUNDARY][WIDTH_BOUNDARY]={0};//表示当前位置的状态，-3--边界左右'|'，-2--边界上下'-'，-1--障碍物，0-空白，1,2，3--蛇头，中间，尾巴，后面全部表示食物i-food[i-4]

int main(int argc,char *argv[])
{
  int weight[LEN_FOOD]={1,2,3};
  char *food_icon[LEN_FOOD]={"🥩","🥥","🥮"};
  // char *food_icon[LEN_FOOD]={"#","*","$"};
  char *snake_icon[3]={"🐲","🌫️", "💨"};
  // char *snake_icon[3]={"w","s", "q"};
  //第一个元素为障碍物，第二个元素为横着的边界，第三个元素为竖着的边界
  char *boundary_icon[3]={"⚠️","-","|"};
  // char *boundary_icon[3]={"@","-","|"};//0,1,2对应于其在vis中值的取反-1
  //初始化食物
  Food *foodArray=init_food(weight,food_icon,LEN_FOOD);
  if(!foodArray){
    printf("初始化食物失败\n");
    return -1;
  }
  //初始化小蛇
  Snake snake = init_snake(vis,WIDTH_BOUNDARY,LENTH_BOUNDARY);
  if(!snake){
    printf("初始化小蛇失败\n");
    return -1;
  }
  // 初始号游戏
  if(!init_game(foodArray,LEN_FOOD,snake,vis,NUM_INIT_FOOD,WIDTH_BOUNDARY,LENTH_BOUNDARY,NUM_OBSTACLE))
  {
    printf("初始化游戏失败\n");
    return -1;
  }

  // 打印游戏界面
  while(1)
  {
    system("clear");
    printf_game(vis,WIDTH_BOUNDARY,LENTH_BOUNDARY,food_icon,LEN_FOOD,snake_icon,3,boundary_icon,3);
  }

  return 0;
}
