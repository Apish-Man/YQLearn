#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "student_2.h"

// 全局变量定义
const char *COURSE_NAMES[NUM_SUB] = {"语文", "数学", "英语"};

// 3.函数定义
/*
 * 2.1 检查学号输入是否有效
 * 形参：学号：Student* stuArray 当前人数: int count 当前学号 char* p
 * 返回-1，不存在，-2，不符合规则，>=0，存在，且返回下标
 */
int isExist(Student *stuArray, int count, const char *input)
{
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        if (input[i] < '0' || input[i] > '9')
            return -2; // 输入不符合
    }
    for (int i = 0; i < count; i++)
    {
        if (strcmp(stuArray[i].id, input) == 0)
            return i; // 重复,返回下标
    }
    return -1;
}
/*
 * 2.1 添加学生信息
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
 * 返回值：int ,表示是否成功
 * 返回1，表示成功
 */
int addStudent(Student *stuArray, int *count, const char **couseName)
{
    int len = *count;

#ifdef DEBUG
    printf("添加学生信息，当前数组长度：%d\n", len);
#endif

    if (*count >= MAX_STUDENTS)
    {
        printf("当前内存空间已满，无法插入\n");
        return -1;
    }
    // 临时变量
    Student student;
    // 1.输入学号
    char tmp[LEN_ID];
    int flag = 0; // 输入是否有效
    while (getchar() != '\n')
        ; // 清理缓冲区，前面选择时输入的\n
    do
    {
        printf("请输入%d位学号(全部为数字)，若多于%d位，将被截断\n", LEN_ID - 1, LEN_ID - 1);
        // 1.读取输入
        if (fgets(tmp, LEN_ID, stdin) == NULL) // 最多读入LEN_ID-1个字符，最后一个会自动补0
        {
            printf("输入错误\n");
            continue;
        }
        // 2.去除换行符
        size_t len = strlen(tmp);
        if (len > 0 && tmp[len - 1] == '\n')
        {                      // 此时未读满
            tmp[--len] = '\0'; // 删除换行符,此时读入为abc\n\0
        }
        else if (len == sizeof(tmp) - 1)
        { // 此时读满了
            while (getchar() != '\n')
                ;
        }
        // 3.处理空输入
        if (len == 0)
        {
            printf("不能输入空字符串\n");
        }
        if (strlen(tmp) == LEN_ID - 1 && isExist(stuArray, *count, tmp) == -1)
            flag = 1;
        else
            printf("学号输入无效，请仔细检查后重新输入\n");
    } while (!flag);
    strcpy(student.id, tmp); // 录入学号

    // 2.输入姓名
    char tmp_name[LEN_NAME];
    flag = 0; // 输入是否有效
    do
    {
        printf("请输入最高%d位姓名，若多于%d位，将被截断\n", LEN_NAME - 1, LEN_NAME - 1);
        if (fgets(tmp_name, LEN_NAME, stdin) == NULL) // 此处最大LEN_NAME，但fgets会补\0，最多读入LEN_NAME-1
        {
            printf("输入错误\n");
            continue;
        }
        // 去除换行或清空缓冲区
        size_t len = strlen(tmp_name);
        if (len > 0 && tmp_name[len - 1] == '\n')
            tmp_name[--len] = '\0';
        else if (len == sizeof(tmp_name) - 1)
            while (getchar() != '\n')
                ;
        // 空字符串
        if (len == 0)
        {
            printf("不能输入空字符串\n");
        }
        if (strlen(tmp_name) < LEN_NAME - 1)
            flag = 1;
        else
            printf("姓名输入无效，请仔细检查后重新输入\n");
    } while (!flag);
    // 录入姓名
    strcpy(student.name, tmp_name);

    // 3.录入成绩
    for (int i = 0; i < NUM_SUB;)
    {
        double tmp = 0.0;
        printf("请输入%s成绩:\n", couseName[i]);
        int key = scanf("%lf", &tmp); // 使用key统计正确的输入，可过滤错误输入
        // 清除缓冲区
        while (getchar() != '\n')
            ;
        // 验证成绩有效性
        if (!key || tmp < 0 || tmp > 100)
        {
            printf("录入成绩不合要求，请重新输入\n");
            continue;
        }
        student.scores[i] = tmp; // 录入成绩
        i++;
    }
    stuArray[*count] = student;

#ifdef DEBUG
    printf("添加学生信息，当前学生信息：%s\t%s\t%.3lf\t%.3lf\t%.3lf\n", student.id, student.name, student.scores[0], student.scores[1], student.scores[2]);
#endif

    (*count)++;
    return 1;
}

/*
 * 2.2 显示所有学生信息
 * 形参：结构体数组：Student* stuArray 当前人数: int count
 * 返回值：void
 */
int showAllStudent(Student *stuArray, int count, const char **couseName)
{
    printf("学号：\t姓名：\t");
    for (int i = 0; i < NUM_SUB; i++)
    {
        printf("%s\t\t", couseName[i]);
    }
    for (int i = 0; i < count; i++)
    {
        printf("\n%s\t%s\t", stuArray[i].id, stuArray[i].name);
        for (int j = 0; j < NUM_SUB; j++)
        {
            printf("%.3lf\t\t", stuArray[i].scores[j]);
        }
    }
    printf("\n");
    return 0;
}

/*
 * 2.3 计算每个学生的平均分和总分
 * 形参：结构体数组：Student* stuArray 当前人数: int count
 * 返回值：void
 */
void caculateEveryStudentAverSum(Student *stuArray, int count)
{
    printf("学号：\t姓名：\t总分\t平均分\t\n");
    for (int i = 0; i < count; i++)
    {
        double sum = 0, aver = 0;
        for (int j = 0; j < NUM_SUB; j++)
        {
            sum += stuArray[i].scores[j];
        }
        aver = sum / NUM_SUB;
        printf("%s\t%s\t%.3lf\t%.3lf\n", stuArray[i].id, stuArray[i].name, sum, aver);
    }
}

/*
 * 2.5 查找学生信息
 * 形参：结构体数组：Student* stuArray  当前人数: int count 学号：char* target
 * 返回值：int ,表示是否成功
 * 返回1，表示成功
 */
int findStudent(Student *stuArray, int count, const char *target, const char **couseName)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(stuArray[i].id, target) == 0)
        {
            // 找到
            // 打印表头
            printf("学号\t姓名\t");
            for (int j = 0; j < NUM_SUB; j++)
                printf("%s\t\t", couseName[j]);
            printf("平均分\t成绩\t\n");
            // 打印数据
            double sum = 0, ave = 0;
            printf("%s\t%s\t", stuArray[i].id, stuArray[i].name);
            for (int j = 0; j < NUM_SUB; j++)
            {
                sum += stuArray[i].scores[j];
                printf("%.3lf\t\t", stuArray[i].scores[j]);
            }
            ave = sum / NUM_SUB;
            printf("%.3lf\t%.3lf\t\n", ave, sum);
            return 1;
        }
    }
    printf("查无此人\n");
    return 0;
}

/*
 * 2.4 根据某科成绩排序
 * 形参：结构体数组：Student* stuArray 当前人数: int count 交换科目：int indexSub 升序降序：1升0降 int flag
 * 返回值：int ,表示是否成功
 * 返回1，表示成功
 * 返回-1，比较索引出错
 * 返回-2，元素交换出错
 */
// 交换x,y
int swap(int x, int y, Student *stuArray)
{
    Student tmp = stuArray[x];
    stuArray[x] = stuArray[y];
    stuArray[y] = tmp;

    return 1;
}

int sortStudent(Student *stuArray, int count, int indexSub, int flag)
{
    if (indexSub < 0 || indexSub >= NUM_SUB)
        return -1; // 比较索引出错
    for (int i = 0; i < count - 1; i++)
    {
        // n-1趟冒泡，每次确定1个的位置，n-1个确定，最后一个也确定
        int is_swap = 0; // 标志是否交换
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (flag)
            {
                // 升序
                if (stuArray[j].scores[indexSub] > stuArray[j + 1].scores[indexSub])
                {
                    if (swap(j, j + 1, stuArray) == 1) // 交换成功，返回1
                        is_swap = 1;
                    else
                    {
                        printf("元素交换出错\n");
                        return -2; // 元素交换出错
                    }
                }
            }
            else
            {
                // 降序
                if (stuArray[j].scores[indexSub] < stuArray[j + 1].scores[indexSub])
                {
                    if (swap(j, j + 1, stuArray) == 1) // 交换成功，返回1
                        is_swap = 1;
                    else
                    {
                        printf("元素交换出错\n");
                        return -2; // 元素交换出错
                    }
                }
            }
        }
        if (!is_swap)
            return 1; // 结束，没有交换，已经有序
    }
    return 0;
}

/*
 * 2.6 删除学生信息
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学号：char* target
 * 返回值：int ,表示是否成功
 * 返回1，表示成功，-1，不存在该学号
 */
int delStudent(Student *stuArray, int *count, const char *target)
{
    // 验证学号有效性
    int index = isExist(stuArray, *count, target);

#ifdef DEBUG
    printf("删除学生信息，当前获取到的目标index:%d\n", index);
#endif

    int len = *count;
    if (index >= 0 && index < len)
    {
        // 删除
        for (int i = index; i < len-1; i++)
        {
            stuArray[i] = stuArray[i + 1]; // 前移
        }
        (*count)--;
        return 1;
    }else if(index==len){
        //删除最后一个元素
        (*count)--;
        return 1;
    }
    return -1; // 删除失败，学号不存在
}

/*
 * 2.7 计算班级平均分
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
 * 返回值：int ,表示是否成功
 * 返回1，表示成功，-1，表示计算失败
 */
int showAverageGrade(Student *stuArray, int count, const char **couseName)
{
    if(count==0) return -1;
    // 计算并展示平均分，各科目平均分，总平均分
    for (int i = 0; i < NUM_SUB; i++)
    {
        printf("%s平均分\t", couseName[i]);
    }
    printf("总分平均分\t\n");
    double sum = 0, aver[NUM_SUB] = {0};
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < NUM_SUB; j++)
        {
            aver[j]+=stuArray[i].scores[j];
            sum += stuArray[i].scores[j];
        }
    }
    sum=sum/count;
    for (int i = 0; i < NUM_SUB; i++) aver[i]/=count;
    for (int i = 0; i < NUM_SUB; i++) printf("%.3lf\t\t", aver[i]);
    printf("%.3lf\n",sum);
    return 1;
}

/*
 * 2.7 统计及格率
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
 * 返回值：int ,表示是否成功
 * 返回1，表示成功，-1，表示计算失败
 */
extern int showPassRate(Student *stuArray, int count, const char **couseName)
{
    // 计算并展示及格率，各科目及格率
    double passCount[NUM_SUB]={0};
    for (int i = 0; i < NUM_SUB; i++)
    {
        printf("%s及格率\t", couseName[i]);
    }
    printf("\n");
    //统计人数
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < NUM_SUB; j++)
        {
            if(IS_PASS(stuArray[i].scores[j])) passCount[j]++;
        }
    }
    for (int i = 0; i < NUM_SUB; i++) printf("%.3lf\t\t", passCount[i]/count);
    printf("\n");
    return 1;
}
