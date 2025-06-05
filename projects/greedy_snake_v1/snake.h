/*************************************************************************
  > File Name:    snake.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月03日 星期二 20时21分52秒
 ************************************************************************/

#ifndef _SNAKE_H
#define _SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define WIDTH_BOUNDARY  40  //边界宽度
#define LENTH_BOUNDARY  40  //边界长度

//定义食物结构体
typedef struct Food{
    int weight;//食物权重，权重大表示生长多
    char* icon;//食物图标
}Food;

//定义蛇节点结构体：
typedef struct SnakeNode{
    int icon;//1,2,3  1--头，2---中间,3----尾巴
    int x,y;//记录当前节点
    int dir;//表示当前节点方向，0,1,2,3和对应的方向一一对应
    struct SnakeNode *next,*prev;//下一个蛇节点,上一个蛇节点
}SnakeNode,*Snake;

/*
* 初始化食物函数，要求weight和icon数组长度一致且对应
* @param int *weight 食物权重
* @param char *icon  食物对应图标
* @param int len  数组长度
* @return Food *foodArray,返回长度为len的食物列表，return NULL表示失败
*/
extern Food* init_food(int *weight,char **icon,int len);

/*
* 初始化蛇函数，创建一个蛇头一个蛇尾，同时会随机一个位置
* @param int *vis 网格的当前状态，在随机位置时进行标记
* @param int wid  边界宽度
* @param int len  边界长度
* @return Snake,返回初始化蛇，return NULL表示失败
*/
extern Snake init_snake(int (*vis)[WIDTH_BOUNDARY],int wid,int len);

/*
* 初始化函数,主要是vis函数赋初值（边界，小蛇，食物）
* @param Food* foodArray 待分配位置的食物
* @param int food_len  食物种类数
* @param Snake snake 待分配位置的小蛇（创建时已分配位置，此处备用）
* @param int *vis 网格的当前状态，在随机位置时进行标记
* @param int food_num  生成食物个数
* @param int wid  边界宽度
* @param int len  边界长度
* @param int num_obstacle 障碍物个数
* @return 返回1表示初始化成功，返回0表示失败
*/
extern int init_game(const Food* foodArray,int food_len,Snake snake,int (*vis)[WIDTH_BOUNDARY],int food_num,int wid,int len,int num_obstacle);

/*
* 打印界面函数,按照行遍历vis，根据其值打印对应图标
* @param int *vis 网格的当前状态，在随机位置时进行标记
* @param int wid  边界宽度
* @param int len  边界长度
* @param char *food_icon  食物对应图标
* @param int len_ficon  食物图标长度
* @param char *snake_icon  小蛇对应图标
* @param int len_sicon  小蛇图标长度
* @param char *boundary_icon  边界对应图标,第一个元素为横着的边界，第二个元素为竖着的边界，第三个元素为障碍物
* @param int len_bicon  边界图标宽度（默认2）
* @return 返回1表示初始化成功，返回0表示失败
*/
extern int printf_game(int (*vis)[WIDTH_BOUNDARY],int wid,int len,char **food_icon,int lficon,char **snake_icon,int lsicon,char **boundary_icon,int lbicon);

#endif //_SNAKE_H
