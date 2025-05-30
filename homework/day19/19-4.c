/*************************************************************************
  > File Name:    19-4.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月30日 星期五 16时01分51秒
 ************************************************************************/

#include <stdio.h>

#define MAX 3

enum Grade{A,B,C,D,E};
union ExtraInfo{
    int is_monitor;//0,1
    enum Grade scholarship;//奖学金等级
};
int info_type=0;//0表示union存班长信息，1表示存奖学金等级
typedef struct Student{
    int id;
    char name[20];
    float scores[3];
    union ExtraInfo extra;
}Student;

//添加学生
int addStudent(Student *array,int* len)
{
    Student temp;
    printf("请输入学号：\n");
    scanf("%d",&temp.id);
    printf("请输入姓名:\n");
    scanf("%s",temp.name);
    //输入成绩
    for(int i=0;i<3;)
    {
        float s;
        printf("请输入第%d门成绩：\n",i);
        scanf("%f",&s);
        if(s<0||s>100){
            printf("输入有误\n");
            continue;
        }
        temp.scores[i]=s;
        i++;
    }
    printf("请输入附加信息（是否班长(0,1)）:\n");
    //此处存班长
    info_type=0;
    scanf("%d",&temp.extra.is_monitor);
    //记录
    array[*len]=temp;
    (*len)++;
    return 1;
}

//计算平均分并且赋值等级
int accelate(Student* array,int len)
{
    for(int i=0;i<len;i++)
    {
        float average=0;
        for(int j=0;j<3;j++) average+=array[i].scores[j];
        //此时union存等级
        info_type=1;
        //判断等级
        average/=3;
        if(average>=90) array[i].extra.scholarship='A';
        else if(average>=80) array[i].extra.scholarship='B';
        else if(average>=70) array[i].extra.scholarship='C';
        else if(average>=60) array[i].extra.scholarship='D';
        else array[i].extra.scholarship='E';
    }
    return 1;
}

//打印信息
void printfStu(const Student* array,int len)
{
    printf("学号\t姓名\t成绩1\t成绩2\t成绩3\t附加信息\n");
    for(int i=0;i<len;i++)
    {
        if(!info_type) //班长
            printf("%d\t%s\t%.3f\t%.3f\t%.3f\t%s\n",
                    array[i].id,array[i].name,array[i].scores[0],array[i].scores[1],array[i].scores[2],array[i].extra.is_monitor==0?"普通学生":"班长");  //班长
        else //等级
            printf("%d\t%s\t%.3f\t%.3f\t%.3f\t%c\n",
                    array[i].id,array[i].name,array[i].scores[0],array[i].scores[1],array[i].scores[2],array[i].extra.scholarship);  //等级
    }
    return ;
}

int main(int argc,char *argv[])
{
    Student stuArray[MAX];
    int len=0;
    //添加学生
    for(int i=0;i<MAX;i++) addStudent(stuArray,&len);
    //打印信息
    printfStu(stuArray,len);

    printf("--------------------------存等级-------------------\n");
    //计算
    accelate(stuArray,len);
    //打印信息
    printfStu(stuArray,len);
    return 0;
}
