/*
5. 输出半径为1~10的圆面积，面积大于100时停止
*/
#include <stdio.h>

#define PI 3.14

double circle_area(int n)
{
    return PI*n*n;
}

int main()
{
    int r=1;
    for(r=1;r<=10;r++)
    {
        double area=circle_area(r);
        if(area>100) return 0;
        printf("r:%d area:%lf\n",r,area);
    }
    return 0;
}