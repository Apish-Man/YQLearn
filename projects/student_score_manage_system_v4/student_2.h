// #ifdef __cplusplus
// extern "C" {
// #endif

#ifndef _STUDENT_H
#define _STUDENT_H

// 最大学生数量
#define MAX_STUDENTS 10
// 学科数目
#define NUM_SUB 3
// 学号长度
#define LEN_ID 5
// 姓名长度
#define LEN_NAME 10

// 及格分数线
#define PASS_SCORE 60
// A级分数线
#define GRADE_A_SCORE 90

// 成绩及格宏,1 pass 0 unpass
#define IS_PASS(score) score >= PASS_SCORE ? 1 : 0

// 定义调试模式
#define DEBUG

// 定义结构体
typedef struct
{
    char id[LEN_ID];        // 学号
    char name[LEN_NAME];    // 姓名
    double scores[NUM_SUB]; // 成绩
} Student;

// 声明全局数组（不分配内存）
extern const char *COURSE_NAMES[NUM_SUB];

// 2.函数声明
/*
 * 2.1 添加学生信息
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
 * 返回值：int ,表示是否成功
 * 返回1，表示成功
 */
extern int addStudent(Student *stuArray, int *count, const char **couseName);

/*
 * 2.2 显示所有学生信息
 * 形参：结构体数组：Student* stuArray 当前人数: int count
 * 返回值：void
 */
extern int showAllStudent(Student *stuArray, int count, const char **couseName);

/*
 * 2.3 计算每个学生的平均分和总分
 * 形参：结构体数组：Student* stuArray 当前人数: int count
 * 返回值：void
 */
extern void caculateEveryStudentAverSum(Student *stuArray, int count);

/*
 * 2.4 根据某科成绩排序
 * 形参：结构体数组：Student* stuArray 当前人数: int count 交换科目：int indexSub 升序降序：1升0降 int flag
 * 返回值：int ,表示是否成功
 * 返回1，表示成功
 * 返回-1，比较索引出错
 * 返回-2，元素交换出错
 */
extern int sortStudent(Student *stuArray, int count, int indexSub, int flag);

/*
 * 2.5 查找学生信息
 * 形参：结构体数组：Student* stuArray  当前人数: int count 学号：char* target
 * 返回值：int ,表示是否成功
 * 返回1，表示成功
 */
extern int findStudent(Student *stuArray, int count, const char *target, const char **couseName);

/*
 * 2.6 删除学生信息
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学号：char* target
 * 返回值：int ,表示是否成功
 * 返回1，表示成功，-1，不存在该学号
 */
extern int delStudent(Student *stuArray, int *count, const char *target);

/*
 * 2.7 计算班级平均分
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
 * 返回值：int ,表示是否成功
 * 返回1，表示成功，-1，表示计算失败
 */
extern int showAverageGrade(Student *stuArray, int count, const char **couseName);

/*
 * 2.7 统计及格率
 * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
 * 返回值：int ,表示是否成功
 * 返回1，表示成功，-1，表示计算失败
 */
extern int showPassRate(Student *stuArray, int count, const char **couseName);
#endif

// #ifdef __cplusplus
// }
// #endif