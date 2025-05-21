/*************************************************************************
  > File Name:    10-6.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 20时07分20秒
 ************************************************************************/

#include <stdio.h>

/*
 *用数组存储多个数据，或者设计求2个数的，然后使用时循环调用
 *@param ans为前num-1个数的平均值,x为新数，num为当前录入第几个数
 *return 两个数的平均值
 */
double average(double ans,double x,int num)
{
    return (ans*(num-1)+x)/num;
}

int main(int argc,char *argv[])
{
    double x,ans=0;
    int num=0;//记录第几个数,以0结束
    int i=0;
    printf("输入数据个数: ");
    scanf("%d",&i);
    while(i--)
    {
        scanf("%lf",&x);
        num++;
        ans=average(ans,x,num);
    }
    printf("%lf\n",ans);
    return 0;
}
