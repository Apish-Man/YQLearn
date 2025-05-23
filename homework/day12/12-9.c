/*************************************************************************
  > File Name:    12-9.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月23日 星期五 10时40分04秒
  9. 编写一个指针函数，返回二维数组中最大元素的地址。
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COL 10
#define ROW 10

int* get_max(int (*p)[COL],int row,int col)
{
    int *max=p[0];
    int i=0;
    for(i=0;i<row*col;i++)
    {
        if(*(p[0]+i)>*(max)) max=p[0]+i;
    }
    return max;
}

int main(int argc,char *argv[])
{
    srand(time(NULL));
    int arr[ROW][COL];
    int i=0,j=0;
    for(i=0;i<ROW;i++)
    {
        for(j=0;j<COL;j++)
        {
            arr[i][j]=rand()%100;
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
    int *max_add=get_max(arr,ROW,COL);
    printf("%p %d\n",max_add,*max_add);

    return 0;
}
