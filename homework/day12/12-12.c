/*************************************************************************
  > File Name:    12-12.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月23日 星期五 16时11分50秒
  12. 动态申请一个整型数组，并给每个元素赋值，要求删除第3个元素
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 10

int main(int argc,char *argv[])
{
    int *ptr=(int*)malloc(sizeof(int)*LEN);
    memset(ptr,0,sizeof(int)*LEN);
    int i=0,len=LEN;
    for(i=0;i<len;i++) scanf("%d",ptr+i);
    for(i=0;i<len;i++) printf("%d ",*(ptr+i));
    //删除第三个
    for(i=3;i<LEN;i++) *(ptr+i-1)=*(ptr+i);
    len--;
    printf("\n");
    for(i=0;i<len;i++) printf("%d ",*(ptr+i));
    return 0;
}
