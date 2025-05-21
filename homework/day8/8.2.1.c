/*
    一个二维数组赋了初值，用户输入一个数，在该二维数组中查找。找到则返回行列位置，没找
    到则提示。
*/
#include <stdio.h>

#define MAX 10
int array[MAX][MAX];

int main()
{
    int i=0,j=0,num=0;
    // 初始化二维数组
    for(i=0;i<MAX;i++)
        for(j=0;j<MAX;j++)
            array[i][j]=num++;
    // 输入数据，查找
    int flag=0;
    scanf("%d",&num);
    for(i=0;i<MAX;i++)
        for(j=0;j<MAX;j++)
        {
            if(array[i][j]==num)
            {
                printf("行：%d 列：%d\n",i,j);
                flag=1;
            }
        }
    if(flag==0) printf("未找到\n");
    return 0;
}