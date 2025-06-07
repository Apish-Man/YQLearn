/*************************************************************************
  > File Name:    snake.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月03日 星期二 20时22分00秒
 ************************************************************************/

#ifndef _DCLIST_H
#define _DCLIST_H

// 引入相关的头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

typedef struct 
{
  int i;
  int j;
}Coordinate;


// 定义数据类型
typedef Coordinate DATA;

// 定义双向链表节点结构体
typedef struct node
{
  DATA         data;    // 数据域：节点数据
  struct node *prev;    // 指针域：前驱指针
  struct node *next;    // 指针域：后继指针
} NODE;

/**
 * 向链表添加节点数据
 * @param head：待操作的链表
 * @param data：待插入的数据
 * @return 成功返回0，失败返回-1
 */
extern int dclist_add(NODE **head, DATA data);

/**
 * 向链表添加节点数据,尾插法
 * @param head：待操作的链表
 * @param node：待插入的节点
 * @return 成功返回0，失败返回-1
 */
extern int dclist_tail_add(NODE **head, NODE *node);

/**
 * 向链表添加节点数据,头插法
 * @param head：待操作的链表
 * @param node：待插入的节点
 * @return 成功返回0，失败返回-1
 */
extern int dclist_head_add(NODE **head, NODE *node);

/**
 * 在 ncurses 窗口中调试打印双向循环链表
 * @param win: ncurses 窗口指针
 * @param head: 待操作的链表
 * @param title: 调试信息的标题
 */
extern void dclist_showInWindow(WINDOW *win, const NODE *head, const char *title);

/**
 * 遍历链表数据
 * @param head：待操作的链表
 */
extern void dclist_showAll(const NODE *head);

/**
 * 获取链表长度
 * @param head：待操作的链表
 */
extern int dclist_len(const NODE *head);

/**
 * 根据old修改对应节点数据为newdata
 * @param head：待操作的链表
 * @param old：待修改的目标节点数据
 * @param newdata：修改的数据
 * @return 成功返回0，失败返回-1
 */
extern int dclist_update(const NODE *head, DATA old, DATA newdata);

/**
 * 删除链表节点
 * @param head：待操作的链表
 * @param data：待删除的节点数据
 * @return 成功返回0，失败返回-1
 */
extern int dclist_delete(NODE **head, DATA data);

/**
 * 删除链表尾节点节点
 * @param head：待操作的链表
 * @return 成功返回0，失败返回-1
 */
extern int dclist_tail_delete(NODE **head);

/**
 * 销毁链表
 * @param head：待销毁的链表
 */
extern void dclist_destroy(NODE **head);

#endif //_DCLIST_H
