/*************************************************************************
  > File Name:    obstacle.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月05日 星期四 16时31分47秒
 ************************************************************************/

#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include "block.h"
#include "dclist.h"
#include <time.h>


/*
* 初始化障碍物
* @param container 网格的当前状态
* @param int wid  边界宽度
* @param int len  边界长度
* @param int num  障碍物个数个数
* @return NODE*,返回初始化障碍物，return NULL表示失败
*/
extern NODE* init_obstacle(Block (*newcontainer)[WIDTH_BOUNDARY],Block (*oldcontainer)[WIDTH_BOUNDARY],int len,int wid,int num);

//创建节点
extern NODE* createObstacleNode(Block (*newcontainer)[WIDTH_BOUNDARY],int len,int wid);


#endif //_OBSTACLE_H
