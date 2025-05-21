/*************************************************************************
  > File Name:    10-1.c
  > Author:       WeiXinQiang
  > Description:  
  > Created Time: 2025年05月16日 星期五 18时46分21秒
 ************************************************************************/

/*
 *编写函数，输入球的半径，返回球的体积
 */
#include <stdio.h>

#define PI 3.14

/*
 * @param radius 输入半径
 * @return 返回体积
 */
double get_volume(double radius)
{
    return PI*radius*radius*radius*(3.0/4.0);
}

int main(int argc,char *argv[])
{
    double radius=0.0;
    scanf("%lf",&radius);
    printf("%lf\n",get_volume(radius));
    return 0;
}

