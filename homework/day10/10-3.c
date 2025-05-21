/*************************************************************************
  > File Name:    10-3.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 19时18分36秒
 ************************************************************************/

#include <stdio.h>
#include <math.h>

/*
 *@param 4个参数，代表两个坐标
 *返回两点距离
 */
double get_distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
int main(int argc,char *argv[])
{
    double x1,x2,y1,y2;
    printf("请输入x1,y1,x2,y2:\n");
    scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
    printf("%0.5lf\n",get_distance(x1,y1,x2,y2));
    return 0;
}
