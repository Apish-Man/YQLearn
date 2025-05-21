/*
作业6：通过键入输入一个年份和月份，输出显示该月有多少天。（考虑润年情况）
闰年计算规则：(year % 400==0) || (year % 4 == 0 && year % 100 != 0)
*/
#include <stdio.h>

//0为正常，1为闰年
int month_days[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};

int main()
{
    int year,month,days;
    scanf("%d%d",&year,&month);
    if((year % 400==0) || (year % 4 == 0 && year % 100 != 0)) days=month_days[1][month-1];
    else days=month_days[0][month-1];
    printf("天数:%d\n",days);
    return 0;
}