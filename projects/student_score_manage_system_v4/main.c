/*************************************************************************
  > File Name:    12-1.c
  > Author:       魏新强
  > Description:
  > Created Time: 2025年05月22日 星期四 20时37分28秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "student.h"

int main()
{

#ifdef DEBUG
    printf("----------以调试模式运行------------\n");
#else
    printf("----------以生产模式运行------------\n");
#endif
    // 1.定义初始变量
    Student stuArray[MAX_STUDENTS];
    // 当前长度
    int stu_count = 0;
    // 定义课程名称
    // const char* COURSE_NAMES[NUM_SUB]={"语文","数学","英语"};

    // 主函数
    int choice;
    do
    {
        printf("\t\t欢迎来到学生管理系统v2.0\n\n");
        printf("\t1. 添加学生信息\t\t2. 显示学生信息\n");
        printf("\t3. 计算学生平均分和总分\t4. 根据某科成绩排序\n");
        printf("\t5. 查找学生信息\t\t6. 删除学生信息\n");
        printf("\t7. 计算班级平均分\t8. 统计及格率\n");
        printf("\t9. 退出程序\n\n");

        scanf("%d", &choice);
        switch (choice)
        {
        // 添加学生信息
        case 1:
            addStudent(stuArray, &stu_count, COURSE_NAMES);
            break;
        // 显示所有学生信息
        case 2:
            showAllStudent(stuArray, stu_count, COURSE_NAMES);
            break;
        // 计算每个学生的平均分和总分
        case 3:
            caculateEveryStudentAverSum(stuArray, stu_count);
            break;
        // 根据某科成绩排序
        case 4:
        {
            // 1.选择排序学科，2.选择升序，降序
            int indexSub = -1, flag = -1;
            int valid = 0;
            do
            {
                printf("请选择排序科目:\n");
                for (int i = 0; i < NUM_SUB; i++)
                    printf("%d:%s\t", i, COURSE_NAMES[i]);
                printf("\n");
                scanf("%d", &indexSub);
                if (indexSub >= 0 && indexSub < NUM_SUB)
                    valid = 1;
                else
                    printf("输入错误\n");
            } while (!valid);
            valid = 0;
            do
            {
                printf("请选择排序方式:\n0:降序\n1:升序\n");
                scanf("%d", &flag);
                if (flag == 0 || flag == 1)
                    valid = 1;
                else
                    printf("输入错误\n");
            } while (!valid);
            sortStudent(stuArray, stu_count, indexSub, flag);
            showAllStudent(stuArray, stu_count, COURSE_NAMES);
            break;
        }
        // 查找学生信息
        case 5:
        {
            char target[LEN_ID] = {0};
            // 去掉\n
            getchar();
            printf("请输入要查找的学号（学号长度为%d）：\n", LEN_ID - 1);
            int flag = 0;
            do
            {
                // 1. 读取输入
                if (fgets(target, LEN_ID, stdin) == NULL)
                {
                    printf("输入错误\n");
                }
                // 2. 去除换行符
                size_t len = strlen(target);
                if (len > 0 && target[len - 1] == '\n')
                {
                    target[--len] = '\0'; // 安全替换换行符
                }
                else if (len == sizeof(target) - 1)
                {
                    // 输入缓冲区未清空时可能需要清理
                    while (getchar() != '\n')
                        ;
                }
                // 3. 处理空输入
                if (len == 0)
                {
                    printf("不能输入空字符串\n");
                }
                if (len == LEN_ID - 1)
                    flag = 1;
                else
                {
                    printf("学号输入有误，重新输入\n");
                }
            } while (!flag);
            findStudent(stuArray, stu_count, target, COURSE_NAMES);
            break;
        }
        // 删除学生信息
        case 6:
        {
            char target[LEN_ID] = {0};
            // 去掉\n
            getchar();
            printf("请输入要删除的学号（学号长度为%d）：\n", LEN_ID - 1);
            int flag = 0;
            do
            {
                // 1. 读取输入
                if (fgets(target, LEN_ID, stdin) == NULL)
                {
                    printf("输入错误\n");
                }
                // 2. 去除换行符
                size_t len = strlen(target);
                if (len > 0 && target[len - 1] == '\n')
                {
                    target[--len] = '\0'; // 安全替换换行符
                }
                else if (len == sizeof(target) - 1)
                {
                    // 输入缓冲区未清空时可能需要清理
                    while (getchar() != '\n')
                        ;
                }
                // 3. 处理空输入
                if (len == 0)
                {
                    printf("不能输入空字符串\n");
                }
                if (len == LEN_ID - 1)
                    flag = 1;
                else
                {
                    printf("学号输入有误，重新输入\n");
                }
            } while (!flag);
            int ans = delStudent(stuArray, &stu_count, target);
            if (ans == 1)
                printf("删除成功\n");
            else
                printf("删除失败,请检查学号\n");
            break;
        }
        // 计算班级平均分
        case 7:
            showAverageGrade(stuArray, stu_count, COURSE_NAMES);
            break;
        // 统计及格率
        case 8:
            showPassRate(stuArray, stu_count, COURSE_NAMES);
            break;
        // 退出程序
        case 9:
            printf("欢迎下次使用👏\n");
            return 0;
            break;
        default:
            printf("输入选项无效!\n");
            break;
        }
    } while (choice != 9);

    return 0;
}