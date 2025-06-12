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
  if(!*arr) {
    fclose(fp);
    return 0;
  }

  char line[256]; // 用于读取每一行
  while(fgets(line, sizeof(line), fp)) {
    // 跳过空行和无效行
    if(line[0] == '\n' || line[0] == '\0') continue;
    
    // 移除行尾的换行符
    line[strcspn(line, "\n")] = 0;
    
    // 解析行数据
    char name[NAME_MAXLEN] = {0};
    long score = 0;
    time_t ts = 0;
    
    // 使用更安全的解析方式
    if(sscanf(line, "%15[^,],%ld,%ld", name, &score, &ts) != 3) {
      continue; // 跳过格式不正确的行
    }
    
    // 验证数据有效性
    if(score <= 0 || ts <= 0 || strlen(name) == 0) {
      continue; // 跳过无效数据
    }
    
    // 确保数组有足够空间
    if(n == cap) {
      cap *= 2;
      ScoreEntry *new_arr = realloc(*arr, cap * sizeof(ScoreEntry));
      if(!new_arr) {
        free(*arr);
        fclose(fp);
        return 0;
      }
      *arr = new_arr;
    }
    
    // 复制数据到数组
    strncpy((*arr)[n].name, name, NAME_MAXLEN-1);
    (*arr)[n].name[NAME_MAXLEN-1] = '\0';
    (*arr)[n].score = score;
    (*arr)[n].ts = ts;
    n++;
  }
  
  fclose(fp);
  *cnt = n;
  
  // 对数据进行排序
  if(n > 0) {
    score_sort(*arr, n);
  }
  return 1;
}

/*
* 向文件中写入新记录
* 成功返回1，失败返回0
*/
int score_append(const char *name,long score)
{
  if(!name || score <= 0) return 0; // 验证输入参数
  
  // 获取当前时间戳
  time_t now = time(NULL);
  if(now == -1) return 0; // 时间获取失败
  
  ScoreEntry *arr = NULL;
  int n = 0;
  
  // 加载数据，用于判断重名等
  if(!score_load(&arr, &n)) return 0;

  // 写入或者替换同名
  int replaced = 0;
  for(int i = 0; i < n; i++) {
    // 找到同名
    if(strcmp(arr[i].name, name) == 0) {
      replaced = 1;
      if(score > arr[i].score) {
        // 更新
        arr[i].score = score;
        arr[i].ts = now;
      }
      break;
    }
  }

  // 若是追加，判断数组是否已经满
  if(!replaced) {
    ScoreEntry *new_arr = realloc(arr, (n+1) * sizeof(ScoreEntry));
    if(!new_arr) {
      free(arr);
      return 0;
    }
    arr = new_arr;
    
    // 新记录追加
    strncpy(arr[n].name, name, NAME_MAXLEN-1);
    arr[n].name[NAME_MAXLEN-1] = '\0';
    arr[n].score = score;
    arr[n].ts = now;
    n++;
  }

  // 重新排序数据
  score_sort(arr, n);
  if(n > TOP_K) n = TOP_K;

  // 覆盖写回
  FILE *fp = fopen(SCORE_PATH, "w");
  if(!fp) {
    free(arr);
    return 0;
  }

  // 写入数据
  for(int i = 0; i < n; i++) {
    if(fprintf(fp, "%s,%ld,%ld\n", arr[i].name, arr[i].score, arr[i].ts) < 0) {
      fclose(fp);
      free(arr);
      return 0;
    }
  }
  
  fclose(fp);
  free(arr);
  return 1;
}

