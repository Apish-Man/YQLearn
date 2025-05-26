/*************************************************************************
  > File Name:    12-1.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 20时37分28秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 3
#define LEN_ID 10
#define LEN_NAME 10
#define NUM_SCORE 3

char id[MAX][LEN_ID];       // 学生ID二维数组
char name[MAX][LEN_NAME];   // 学生姓名二维数组
int score[MAX][NUM_SCORE];  // 成绩二维数组
int len;                    // 当前记录数

bool init()
{
    len = 0;  // 实际初始化操作
    return true;
}

void menu()
{
    printf("欢迎来到学生成绩管理系统\n");
    printf("请输入选项选择功能：\n");
    printf("0.退出系统\n1.添加学生信息\n2.显示所有学生信息\n3.计算学生平均分\n4.查找最高分科目\n");
}

int add_one_student()
{
    if(!(id&&name&&score)) return -1;//-1表示内存开辟失败
    if(len>=MAX) return -2;//-2表示存储空间满，添加失败
    printf("请依次输入以下信息：学号，姓名，语文成绩，数学成绩，英语成绩（使用空格隔开）\n");
    scanf("%s%s%d%d%d",id[len],name[len],&score[len][0],&score[len][1],&score[len][2]);
    len++;
    printf("添加成功\n");
    return len;//返回当前数据长度
}

void show_all_student()
{
    if(len==0) 
    {
        printf("当前数据为空\n");
        return;
    }
    int i=0;
    printf("学号\t姓名\t语文成绩\t数学成绩\t英语成绩\t\n");
    for(i=0;i<len;i++)
    {
        printf("%s\t%s\t%d\t\t%d\t\t%d\t\n",id[i],name[i],score[i][0],score[i][1],score[i][2]);
    }
    return ;
}

int exit_system()
{
    //数据是否等操作，如free
    printf("欢迎再次使用\n");
    return 1;
}

double caculate_average_score(char *arr,int grade_len)
{
    double ans=0;
    int key=0,i=0;
    // 找学生下标
    for(i=0;i<len;i++)
    {
        if(strcmp(arr,id[i])==0)
        {
            key=i;
            break;
        }
    }
    if(i==len) {
        printf("查无此人\n");
        return -1;//查无此人
    }
    for(i=0;i<grade_len;i++)
    {
        ans+=score[key][i];
    }
    return ans/grade_len;
}

double caculate_max_score_grade(char *arr,int grade_len)
{
    double max=0,ans=0;
    int key=0,i=0;
    // 找学生下标
    for(i=0;i<len;i++)
    {
        if(strcmp(arr,id[i])==0)
        {
            key=i;
            break;
        }
    }
    if(i==len) {
        printf("查无此人\n");
        return -1;//查无此人
    }
    for(i=0;i<grade_len;i++)
    {
        if(max<score[key][i]) {max=score[key][i];ans=i;}
    }
    return ans;
}

int main(int argc,char *argv[])
 {
    init();
    int choice=0,exit=0;
    while(!exit){
        menu();
        scanf("%d",&choice);
        switch(choice){
            case 0:
                exit=exit_system();
                break;
            case 1:
                add_one_student();
                // 清空缓冲区
                while(getchar()!='\n');
                printf("当前学生个数%d  最大容量%d\n",len,MAX);
                break;
            case 2:
                show_all_student();
                break;
            case 3:
                {
                    //计算学生平均分 
                    char student_id[LEN_ID];
                    printf("请输入学生id\n");
                    scanf("%s",student_id);
                    double aver=caculate_average_score(student_id,3);
                    printf("学号为%s的学生的平均分：%.3lf\n",student_id,aver);
                    break;
                }
            case 4:
                {
                    //查找学生最高分科目
                    char student_id[LEN_ID];
                    printf("请输入学生id\n");
                    scanf("%s",student_id);
                    int ans=caculate_max_score_grade(student_id,3);
                    if(ans==0)
                        printf("学号为%s的学生的最高分科目为：%s\n",student_id,"语文");
                    else if(ans==1)
                        printf("学号为%s的学生的最高分科目为：%s\n",student_id,"数学");
                    else if(ans==2)
                        printf("学号为%s的学生的最高分科目为：%s\n",student_id,"英语");
                    else printf("查找失败\n");
                    break;
                }
            default:
                printf("请输入正确选项!!\n");
                break;
        }
    }
    return 0;
 }
