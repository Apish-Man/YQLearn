/*************************************************************************
  > File Name:    10-5.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 20时01分21秒
 ************************************************************************/

#include <stdio.h>

/*
 *@param x输入
 *return 函数值
 */
double fun(double x)
{
    double ans=0.0;
    if(x>5) ans=4*x+7;
    else ans=-2*x+3;
    return ans;
}

int main(int argc,char *argv[])
{
    double x;
    scanf("%lf",&x);
    printf("%0.3lf\n",fun(x));
    return 0;
}
