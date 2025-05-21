/*
1. 使用* 在控制台分别打印上直角三角形、下直角三角形，上等腰三角形、下等腰三角形、菱形、平行四边形
*/

#include <stdio.h>

// 上直角三角形
void printf1(int high)
{
    int i=0,j=0;
    for (i = 0; i < high; i++)
    {
        for(j=0;j<high-i;j++)
        {
            printf("*");
        }
        printf("\n");
    }
}

// 下直角三角形
void printf2(int high)
{
    int i=0,j=0;
    for (i = high-1; i >= 0; i--)
    {
        for(j=0;j<high-i;j++)
        {
            printf("*");
        }
        printf("\n");
    }
}

// 上等腰三角形,wid需要为奇数
void printf3(int wide)
{
    int i=0,j=0;
    for (i = 0; i < (wide+1)/2; i++)
    {
        for(j=0;j<i;j++) printf(" ");
        for(j=0;j<wide-i*2;j++) printf("*");
        printf("\n");
    }
}

// 下等腰三角形,wid需要为奇数
void printf4(int wide)
{
    int i=0,j=0;
    for (i = (wide+1)/2-1; i >= 0; i--)
    {
        for(j=0;j<i;j++) printf(" ");
        for(j=0;j<wide-i*2;j++) printf("*");
        printf("\n");
    }
}

// 棱形
void printf5(int wide)
{
    int i=0,j=0,k=0;
    for (i = 0; i < wide; i++)
    {
        for(j=0;j<k;j++) printf(" ");
        for(j=0;j<wide;j++) printf("*");
        k++;
        printf("\n");
    }
}

// 平行四边形
void printf6(int wide,int high)
{
    int i=0,j=0;
    for (i = high-1; i >= 0; i--)
    {
        for(j=0;j<i;j++) printf(" ");
        for(j=0;j<wide;j++) printf("*");
        printf("\n");
    }
}
int main()
{
    printf("上直角三角形:\n");printf1(5);
    printf("下直角三角形:\n");printf2(5);
    printf("上等腰三角形:\n");printf3(5);
    printf("下等腰三角形:\n");printf4(5);
    printf("棱形:\n");printf5(5);
    printf("平行四边形:\n");printf6(5,4);
    return 0;
}