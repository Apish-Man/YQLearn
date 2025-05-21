/*
*6. 通过键盘输入 10 个学员成绩：
*1）输出不及格学员的成绩和下标。
*2）求最高分的下标值
*3）求最低成绩的下标值
*4）求总成绩及平均成绩
*/
#include <stdio.h>

int main()
{
    double grade[10];
    int i=0;
    // 输入成绩
    for(i=0;i<10;i++) scanf("%lf",&grade[i]);
    // 寻找不及格，顺便记录总成绩，最高分下标，最低分下标
    int max=0,min=0;
    double sum=0,average=0;
    for(i=0;i<10;i++)
    {
        // 寻找不及格
        if(grade[i]<60) printf("成绩：%lf 下标：%d\n",grade[i],i);
        // 判断最高分下标
        if(grade[i]>grade[max]) max=i;
        // 判断最低分下标
        if(grade[i]<grade[min]) min=i;
        // 求和
        sum+=grade[i];
    }
    printf("最高分下标：%d\n",max);
    printf("最低分下标：%d\n",min);
    printf("总成绩:%lf\n",sum);
    printf("平均成绩:%lf\n",sum/10.0);
    return 0;
}