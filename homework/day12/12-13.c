/*************************************************************************
  > File Name:    12-12.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月23日 星期五 16时11分50秒
  13. 动态申请一个整型数组，并给每个元素赋值，要求在第4个元素后插入100；
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 15

int main(int argc,char *argv[])
{
    int *ptr=(int*)malloc(sizeof(int)*LEN);
    memset(ptr,0,sizeof(int)*LEN);
    int i=0,len=10;
    for(i=0;i<len;i++) scanf("%d",ptr+i);
    for(i=0;i<len;i++) printf("%d ",*(ptr+i));
    //第5-len后移
    for(i=len-1;i>4;i--) *(ptr+i+1)=*(ptr+i);
    len++;
    //修改
    *(ptr+5)=100;
    printf("\n");
    for(i=0;i<len;i++) printf("%d ",*(ptr+i));
    return 0;
}
