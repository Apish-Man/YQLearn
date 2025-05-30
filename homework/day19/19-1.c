/*************************************************************************
  > File Name:    19-1.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月29日 星期四 22时14分38秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define NUM_SCORES 3
#define MAX_STU 10

//定义student结构体
typedef struct Student{
    char name[20];
    int id;
    float scores[NUM_SCORES];
}Stu;

//定义函数计算平均数
double judgeAverage(Stu stu)
{
    double sum=0;
    for(int i=0;i<NUM_SCORES;i++)
    {
        sum+=stu.scores[i];
    }
    return sum/NUM_SCORES;
}

//定义结构体赋值函数
int addStu(Stu* stu)
{
    printf("请输入学生姓名:\n");
    scanf("%s",stu->name);
    printf("请输入学号:\n");
    scanf("%d",&stu->id);
    printf("请依次输入%d门课程成绩\n",NUM_SCORES);
    for(int i=0;i<NUM_SCORES;)
    {
        printf("请输入第%d门成绩:",i);
        double temp=0;
        scanf("%lf",&temp);
        if(temp<0||temp>100)
        {
            printf("成绩输入有误\n");
            continue;
        }
        stu->scores[i]=temp;
        i++;
    }
    return 1;
}

int main(int argc,char *argv[])
{
    Stu stuArray[MAX_STU];
    int len=0;//记录当前长度
    //两个成员变量
    printf("请输入第一个学生信息:\n");
    addStu(&stuArray[len]);
    len++;
    printf("该生平均成绩为：%.3lf\n",judgeAverage(stuArray[0]));

    printf("请输入第二个学生信息:\n");
    addStu(&stuArray[len]);
    len++;
    printf("该生平均成绩为：%.3lf\n",judgeAverage(stuArray[1]));
    
    return 0;
}






































































































