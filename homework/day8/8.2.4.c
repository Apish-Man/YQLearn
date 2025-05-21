/*
计算一个矩阵下三角元素的和
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 2

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
    long long ans=0;
    // 求下三角的话，第i号的前i个元素，0,1  1,2 2,3
    for(i=0;i<MAX;i++) 
    {
        for(j=0;j<i+1;j++)
            ans+=array[i][j];
    }
    printf("%lld\n",ans);

}