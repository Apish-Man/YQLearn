/*
作业1：输入你的身高和体重，测试你的健康状况。
计算bmi的值， bmi = （体重（kg）/身高（m）的平方)
如果bmi 小于18.5，则显示“偏瘦，注意加强营养”
如果bmi 在18.5和23.9之间，则显示“体重指数良好，注意保持”
如果bmi值大于23.9 ，则显示“你有点偏胖，注意锻炼”
*/

#include <stdio.h>

int main()
{
    double bmi,weight,hight;
    scanf("%lf%lf",&weight,&hight);
    bmi=weight/hight;
    if(bmi<18.5) printf("偏瘦，注意加强营养\n");
    else if(bmi>23.9) printf("你有点偏胖，注意锻炼\n");
    else printf("体重指数良好，注意保持\n");
    return 0;
}