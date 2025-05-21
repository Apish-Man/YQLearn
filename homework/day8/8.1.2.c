/*
*2. 从键盘输入年、月、日，计算并输出该日是该年第几天
*/
#include <stdbool.h>
#include <stdio.h>

// 1,3,5,7,8,10,12--这几个月每月31天；2月闰年29天，非闰年28天；其他月30天
const int Days[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};//第一个数组为普通年，第二个数组为闰年

// 判断是否是闰年
// 能被4整除且不能被100整除的年份，或者能被400整除的年份
int judge(int year)
{
    if(!(year%400)) return 1;
    if(!(year%4)&&(year%100)) return 1;
    return 0;
}

int main()
{
    int year,month,day;
    while(scanf("%d%d%d",&year,&month,&day)!=EOF)
    {
        int days=0,index=judge(year);
        int i=0;
        for(i=0;i<month-1;i++) days+=Days[index][i];
        days+=day;
        printf("%d\n",days);
    }
    return 0;
}