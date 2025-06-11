/*************************************************************************
  > File Name:    score.h
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月08日 星期日 10时16分02秒
 ************************************************************************/

#ifndef _SCORE_H
#define _SCORE_H

#include <time.h>

#define SCORE_PATH   "scores.dat"   /* CSV：name,score,timestamp\n */
#define NAME_MAXLEN  16
#define TOP_K        20             /* 仅保留前 20 名 */

typedef struct {
    char   name[NAME_MAXLEN];
    long   score;
    time_t ts;          /* 备用：同分时按最新覆盖或保留最早 */
} ScoreEntry;

/*
* 从文件中加载排行榜，并且根据高低进行排序
* malloc 内存，由调用者 free 
* 成功返回1，失败返回0
*/
extern int  score_load(ScoreEntry **arr, int *cnt);                

/*
* 向文件中写入新记录
* 追加后自动截断、排序
* 成功返回1，失败返回0
*/
extern int  score_append(const char *name, long score);      

/*
* 对得分进行排序成绩排序 
* 纯降序 + 时间逆序 
*/
extern void score_sort(ScoreEntry *arr, int n);   

#endif //_SCORE_H
