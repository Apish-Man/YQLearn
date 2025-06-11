/*************************************************************************
  > File Name:    score.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月08日 星期日 10时16分08秒
 ************************************************************************/

/* score.c */
#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* 成绩比较
* 传入qsort使用
*/
static int cmp(const void *a,const void *b){
    const ScoreEntry *x=a,*y=b;
    if(x->score!=y->score)   return y->score - x->score;        /* 高 → 低 */
    return (int)(y->ts - x->ts);                                /* 新 → 旧 */
}

/*
* 对得分进行排序成绩排序
*/
void score_sort(ScoreEntry *arr,int n)
{ 
  qsort(arr,n,sizeof(ScoreEntry),cmp); 
}

/*
* 从文件中加载排行榜，并且根据高低进行排序
* 成功返回1，失败返回0
*/
int score_load(ScoreEntry **arr,int *cnt)
{
  // 打开文件（追加模式）
  FILE *fp=fopen(SCORE_PATH,"a+");
  if(!fp) return 0;//文件打开失败

  // 读入全部记录
  int cap=32,n=0;
  *arr=malloc(cap*sizeof(ScoreEntry));
  while(!feof(fp))
  {
    ScoreEntry e;
    // 读入一行，代表一个记录
    if(fscanf(fp,"%15[^,],%ld,%ld\n",e.name,&e.score,&e.ts)==3){
      // 得到记录
      if(n==cap)
      {
        cap*=2;
        *arr=realloc(*arr,cap*sizeof(ScoreEntry));
      }
      (*arr)[n++]=e;
    }
  }
  // 关闭文件
  fclose(fp);
  *cnt=n;
  // 对数据进行排序
  score_sort(*arr,n);
  return 1;
}

/*
* 向文件中写入新记录
* 成功返回1，失败返回0
*/
int score_append(const char *name,long score)
{
  //  获取当前时间戳
  time_t now=time(NULL);
  ScoreEntry *arr;
  int n;
  // 加载数据，用于判断重名等
  if(!score_load(&arr,&n)) return 0;

  // 写入或者替换同名
  int replaced=0;
  for(int i=0;i<n;i++)
  {
    // 找到同名
    if(strcmp(arr[i].name,name)==0)
    {
      replaced=1;
      if(score>arr[i].score)
      {
        // 更新
        arr[i].score=score;
        arr[i].ts=now;
      }
      break;
    }
  }

  // 若是追加，判断数组释放已经满
  if(!replaced)
  {
    arr=realloc(arr,(n+1)*sizeof(ScoreEntry));
    if(!arr) return 0;
    // 新记录追加
    strncpy(arr[n].name,name,NAME_MAXLEN-1);
    arr[n].name[NAME_MAXLEN-1]=0;
    arr[n].score=score;
    arr[n].ts=now;
    n++;
  }

  // 重新排序数据
  score_sort(arr,n);
  if(n>TOP_K) n=TOP_K;

  //覆盖写回
  FILE *fp=fopen(SCORE_PATH,"w");
  if(!fp) return 0;

  for(int i=0;i<n;i++)
    fprintf(fp,"%s,%ld,%ld\n",arr[i].name,arr[i].score,arr[i].ts);
  
  fclose(fp);free(arr);
  return 1;
}

