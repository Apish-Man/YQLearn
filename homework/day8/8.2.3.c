/*
在行列相等数组计算主对角线元素的和
*/
#include <stdio.h>
#include <stdlib.h>

#define MAX 10

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
    long long ans=0;
    for(i=0;i<MAX;i++) ans+=array[i][i];
    printf("%lld\n",ans);

}