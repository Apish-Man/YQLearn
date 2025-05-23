/*************************************************************************
  > File Name:    12-11.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月23日 星期五 12时59分48秒
  11. 动态申请一个具有10个float类型元素的内存空间，从一个已有的数组中拷贝数据，并找出第一
次出现 12.35 的下标位置，并输出。
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 1e-6

int main(int argc,char *argv[])
{
    float *ptr=(float*)malloc(sizeof(float)*10);
    if(ptr==NULL)
    {
        printf("内存分配失败\n");
        return -1;
    }
    memset(ptr,0,sizeof(float)*10);
    float arr[10]={1.1,1.2,12.35,1.4,1.5,1.6,1.7,1.8,12.35,2.0};
    int i=0;
    for(i=0;i<10;i++) *(ptr+i)=arr[i];
    //for(i=0;i<10;i++) printf("%lf ",*(ptr+i));
    for(i=0;i<10;i++) if(fabs(*(ptr+i)-12.35)<EPSILON) {printf("%d\n",i);break;}
    return 0;
}
