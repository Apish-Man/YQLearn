/*
求出一个矩阵的鞍点。鞍点的含义为行上最大同时列上也最大。
*/
#include <stdio.h>
#include <stdlib.h>

#define MAX 3

int main()
{
    int array[MAX][MAX];
    // 使用随机数函数生成初始值
    int i=0,j=0;
    for(i=0;i<MAX;i++)
    {
        for(j=0;j<MAX;j++)
        {
            array[i][j]=rand();
        }
    }
    // 输出总的数据
    for(i=0;i<MAX;i++)
    {
        for(j=0;j<MAX;j++)
        {
            printf("%d ",array[i][j]);
        }
        printf("\n");
    }
    // 找到每行的最大值，然后判断是否是列的最大值
    for(i=0;i<MAX;i++)
    {
        int max=0,src_i=i,src_j;
        // 找第i行最大值
        for(j=0;j<MAX;j++)
        {
            if(array[i][j]>max)
            {
                max=array[i][j];
                src_j=j;
            }
        }
        // 判断是否是列j的最大值
        int k=0,flag=1;
        for(k=0;k<MAX;k++)
        {
            if(array[k][j]>max)
            {
                flag=0;
                break;
            }
        }
        if(flag)
        {
            // 是鞍点
            printf("是鞍点，行下标%d,列下标%d,值%d\n",src_i,src_j,array[src_i][src_j]);
        }
    }
    return 0;
}