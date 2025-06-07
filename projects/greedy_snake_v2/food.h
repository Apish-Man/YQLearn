/*************************************************************************
  > File Name:    food.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月05日 星期四 09时19分49秒
 ************************************************************************/

#ifndef _FOOD_H
#define _FOOD_H

#include "block.h"
#include "dclist.h"
#include <time.h>


/*
* 初始化食物
* @param container 网格的当前状态
* @param int wid  边界宽度
* @param int len  边界长度
* @param int num  食物个数
* @return NODE*,返回初始化食物，return NULL表示失败
*/
extern NODE* init_food(Block (*newcontainer)[WIDTH_BOUNDARY],Block (*oldcontainer)[WIDTH_BOUNDARY],int len,int wid,int num);

//创建节点
extern NODE* createFoodNode(Block (*newcontainer)[WIDTH_BOUNDARY],int len,int wid);

/*
* 根据container的状态，生成一个随机且合适的位置放置food,并且更新container
* @param Food,新的食物链表
* @param newContainer,当前状态
* @param oldContainer,旧状态
* @return int 1为成功，0为失败
*/
extern int add_one_food_and_update_container(NODE** Food,Block (*newcontainer)[WIDTH_BOUNDARY],Block (*oldcontainer)[WIDTH_BOUNDARY],int len,int wid);

#endif //_FOOD_H
