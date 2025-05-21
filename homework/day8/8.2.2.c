/*
二维整型数组，求所有元素平均值，求每行最大值，求每列最小值。
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
    int max_array[MAX];
    int min_array[MAX];
    // 求所有元素平均值,最大，最小值
    long long sum=0;
    for(i=0;i<MAX;i++)
    {
        max_array[i]=array[i][0];//假设每行最大值为每行第一个数
        min_array[i]=array[0][i];//假设每列最小值为每行第一个数
        for(j=0;j<MAX;j++)
        {
            sum+=array[i][j];
            // 求行最大值时，i为行，j为列
            if(array[i][j]>max_array[i]) max_array[i]=array[i][j];
            // 求列最小值时，i为列，j为行
            if(array[j][i]<min_array[i]) min_array[i]=array[j][i];
        }
    }
    double average=sum/100;
    printf("平均值：%lf\n",average);
    // 输出总的数据
    for(i=0;i<MAX;i++)
    {
        for(j=0;j<MAX;j++)
        {
            printf("%d ",array[i][j]);
        }
        printf("\n");
    }
    // 输出每行最大值，最小值
    for(i=0;i<MAX;i++)
    {
        printf("第%d行最大值:%d  第%d列最小值:%d\n",i,max_array[i],i,min_array[i]);
    }
    return 0;
}